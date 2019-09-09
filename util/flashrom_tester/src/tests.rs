//
// Copyright 2019, Google Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//    * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//    * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Alternatively, this software may be distributed under the terms of the
// GNU General Public License ("GPL") version 2 as published by the Free
// Software Foundation.
//

use super::cmd;
use super::flashrom::{self, Flashrom, FlashromError};
use super::mosys;
use super::rand_util;
use super::tester::{self, TestResult};
use super::types;
use super::utils;
use std::fs::File;
use std::io::Write;

const LAYOUT_FILE: &'static str = "/tmp/layout.file";

/// Run tests.
///
/// Only returns an Error if there was an internal error; test failures are Ok.
pub fn generic(
    path: &str,
    fc: types::FlashChip,
    print_layout: bool,
) -> Result<(), Box<dyn std::error::Error>> {
    let p = path.to_string();
    let cmd = cmd::FlashromCmd { path: p, fc };

    let chip_name = flashrom::name(&cmd)?;
    info!("Found chip name: {}.", chip_name);

    info!("Calculate ROM partition sizes & Create the layout file.");
    let rom_sz: i64 = cmd.get_size()?;
    let layout_sizes = utils::get_layout_sizes(rom_sz)?;
    {
        let mut f = File::create(LAYOUT_FILE)?;
        let mut buf: Vec<u8> = vec![];
        utils::construct_layout_file(&mut buf, &layout_sizes)?;

        f.write_all(&buf)?;
        if print_layout {
            info!(
                "Dumping layout file as requested:\n{}",
                String::from_utf8_lossy(&buf)
            );
        }
    }

    info!("Create a Binary with random contents.");
    rand_util::gen_rand_testdata("/tmp/random_content.bin", rom_sz as usize)?;

    info!(
        "Record crossystem information.\n{}",
        utils::collect_crosssystem()?
    );

    // run specialization tests:
    //  ================================================
    let default_test_params = match fc {
        types::FlashChip::EC => ec(&cmd),
        types::FlashChip::HOST => host(&cmd),
        types::FlashChip::SERVO => servo(&cmd),
        types::FlashChip::DEDIPROG => dediprog(&cmd),
    };

    //  ================================================
    //
    let wp_test_fn = |param: &tester::TestParams| {
        // TODO(quasisec): Should this be in generic() ?
        let wpen = if param.fc != types::FlashChip::SERVO && param.fc != types::FlashChip::DEDIPROG
        {
            let wp = utils::get_hardware_wp()?;
            let state = if wp { "EN" } else { "DIS" };
            info!("Hardware write protect is {}ABLED", state);
            wp
        } else {
            true
        };

        // NOTE: This is not strictly a 'test' as it is allowed to fail on some platforms.
        //       However, we will warn when it does fail.
        match flashrom::wp_list(&param.cmd) {
            Ok(list_str) => info!("\n{}", list_str),
            Err(e) => warn!("{:?}", e),
        };

        if !wpen && flashrom::wp_status(&param.cmd, true)? {
            warn!("ROM is write protected.  Attempting to disable..");
            flashrom::wp_toggle(&param.cmd, false)?;
        }
        if wpen && flashrom::wp_status(&param.cmd, true)? {
            warn!(
                "Hardware write protect seems to be asserted, attempt to get user to de-assert it."
            );
            utils::toggle_hw_wp(true)?;
            warn!("ROM is write protected.  Attempting to disable..");
            flashrom::wp_toggle(&param.cmd, false)?;
        }
        if flashrom::wp_status(&param.cmd, true)? {
            // TODO(quasisec): Should fail the whole test suite here?
            return Err("Cannot disable write protect.  Cannot continue.".into());
        }

        info!("Successfully disable Write-protect");
        Ok(())
    };
    let wp_test = tester::TestCase {
        name: "Toggle WP",
        params: &default_test_params,
        test_fn: wp_test_fn,
        conclusion: tester::TestConclusion::Pass,
    };

    //  ================================================
    //
    let read_test_fn = |param: &tester::TestParams| {
        flashrom::read(&param.cmd, "/tmp/flashrom_tester_read.dat")?;
        flashrom::verify(&param.cmd, "/tmp/flashrom_tester_read.dat")?;
        Ok(())
    };
    let read_test = tester::TestCase {
        name: "Read",
        params: &default_test_params,
        test_fn: read_test_fn,
        conclusion: tester::TestConclusion::Pass,
    };

    //  ================================================
    //
    let erase_write_test_fn = |param: &tester::TestParams| {
        flashrom::read(&param.cmd, "/tmp/flashrom_tester_read.dat")?;
        flashrom::verify(&param.cmd, "/tmp/flashrom_tester_read.dat")?;

        flashrom::wp_toggle(&param.cmd, true)?;
        println!("Replace battery to assert hardware write-protect.");
        utils::toggle_hw_wp(false)?;
        if flashrom::erase(&param.cmd).is_ok() {
            warn!("flash image in an inconsistent state! Attempting to restore..");
            flashrom::write(&param.cmd, "/tmp/flashrom_tester_read.dat")?;
            flashrom::verify(&param.cmd, "/tmp/flashrom_tester_read.dat")?;
            return Err("Hardware write protect asserted however can still erase!".into());
        }
        println!("Remove battery to de-assert hardware write-protect.");
        utils::toggle_hw_wp(true)?;
        flashrom::wp_toggle(&param.cmd, false)?;

        flashrom::erase(&param.cmd)?;
        flashrom::write(&param.cmd, "/tmp/flashrom_tester_read.dat")?;

        flashrom::wp_toggle(&param.cmd, true)?;
        println!("Replace battery to assert hardware write-protect.");
        utils::toggle_hw_wp(false)?;
        Ok(())
    };
    let erase_write_test = tester::TestCase {
        name: "Erase/Write",
        params: &default_test_params,
        test_fn: erase_write_test_fn,
        conclusion: tester::TestConclusion::Pass,
    };

    //  ================================================
    //
    let verify_fail_test_fn =
        |param: &tester::TestParams| Ok(flashrom::verify(&param.cmd, "/tmp/random_content.bin")?);
    let verify_fail_test = tester::TestCase {
        name: "Fail to verify",
        params: &default_test_params,
        test_fn: verify_fail_test_fn,
        conclusion: tester::TestConclusion::Fail,
    };

    //  ================================================
    //
    fn lock_test_fn(param: &tester::TestParams) -> TestResult {
        println!("Remove battery to de-assert hardware write-protect.");
        utils::toggle_hw_wp(true)?;

        // Don't assume soft write-protect state, and so toggle back on.
        flashrom::wp_toggle(&param.cmd, true)?;

        // TODO(quasisec): Should this be in generic() ?
        let wpen = if param.fc != types::FlashChip::SERVO && param.fc != types::FlashChip::DEDIPROG
        {
            let wp = utils::get_hardware_wp()?;
            let state = if wp { "EN" } else { "DIS" };
            info!("Hardware write protect is {}ABLED", state);
            wp
        } else {
            true
        };

        if !wpen && flashrom::wp_status(&param.cmd, true)? {
            info!("WP should unlock since hardware WP is de-asserted.  Attempting to disable..");
            flashrom::wp_toggle(&param.cmd, false)?;
        } else {
            return Err("Hardware write protect still asserted!".into());
        }

        // Validate we successfully disabled soft write-protect when hardware write-protect was
        // de-asserted.
        if flashrom::wp_status(&param.cmd, true)? {
            return Err("Cannot disable write protect.  Cannot continue.".into());
        }

        // Toggle soft write-protect back on after we are done.
        flashrom::wp_toggle(&param.cmd, true)?;

        // --- deal with the other case of hardware wp being asserted. //

        println!("Replace battery to assert hardware write-protect.");
        utils::toggle_hw_wp(false)?;

        // TODO(quasisec): Should this be in generic() ?
        let wpen = if param.fc != types::FlashChip::SERVO && param.fc != types::FlashChip::DEDIPROG
        {
            let wp = utils::get_hardware_wp()?;
            let state = if wp { "EN" } else { "DIS" };
            info!("Hardware write protect is {}ABLED", state);
            wp
        } else {
            true
        };

        if wpen && flashrom::wp_status(&param.cmd, true)? {
            info!("WP should stay locked since hardware WP is asserted.  Attempting to disable..");
            if flashrom::wp_toggle(&param.cmd, false).is_ok() {
                return Err(
                    "Soft write-protect didn't stay locked however hardware WP was asserted."
                        .into(),
                );
            }
        } else {
            return Err("Hardware write protect was not asserted!".into());
        }

        // Validate we successfully disabled soft write-protect when hardware write-protect was
        // de-asserted.
        if flashrom::wp_status(&param.cmd, false)? {
            return Err("Soft write protect wasn't enabled.".into());
        }
        Ok(())
    };
    let lock_test = tester::TestCase {
        name: "Lock",
        params: &default_test_params,
        test_fn: lock_test_fn,
        conclusion: tester::TestConclusion::Pass,
    };

    //  ================================================
    //
    let lock_top_quad_test_fn = |param: &tester::TestParams| {
        let rom_sz: i64 = param.cmd.get_size()?;
        let layout_sizes = utils::get_layout_sizes(rom_sz)?;

        let topq_sec = utils::layout_section(&layout_sizes, utils::LayoutNames::TopQuad);
        test_section(&param.cmd, topq_sec)?;
        Ok(())
    };
    let lock_top_quad_test = tester::TestCase {
        name: "Lock top quad",
        params: &default_test_params,
        test_fn: lock_top_quad_test_fn,
        conclusion: tester::TestConclusion::Pass,
    };

    //  ================================================
    //
    let lock_top_half_test_fn = |param: &tester::TestParams| {
        let rom_sz: i64 = param.cmd.get_size()?;
        let layout_sizes = utils::get_layout_sizes(rom_sz)?;

        let toph_sec = utils::layout_section(&layout_sizes, utils::LayoutNames::TopHalf);
        test_section(&param.cmd, toph_sec)?;
        Ok(())
    };
    let lock_top_half_test = tester::TestCase {
        name: "Lock top half",
        params: &default_test_params,
        test_fn: lock_top_half_test_fn,
        conclusion: tester::TestConclusion::Pass,
    };

    //  ================================================
    //
    let lock_bottom_half_test_fn = |param: &tester::TestParams| {
        let rom_sz: i64 = param.cmd.get_size()?;
        let layout_sizes = utils::get_layout_sizes(rom_sz)?;

        let both_sec = utils::layout_section(&layout_sizes, utils::LayoutNames::BottomHalf);
        test_section(&param.cmd, both_sec)?;
        Ok(())
    };
    let lock_bottom_half_test = tester::TestCase {
        name: "Lock bottom half",
        params: &default_test_params,
        test_fn: lock_bottom_half_test_fn,
        conclusion: tester::TestConclusion::Pass,
    };

    //  ================================================
    //
    let lock_bottom_quad_test_fn = |param: &tester::TestParams| {
        let rom_sz: i64 = param.cmd.get_size()?;
        let layout_sizes = utils::get_layout_sizes(rom_sz)?;

        let botq_sec = utils::layout_section(&layout_sizes, utils::LayoutNames::BottomQuad);
        test_section(&param.cmd, botq_sec)?;
        Ok(())
    };
    let lock_bottom_quad_test = tester::TestCase {
        name: "Lock bottom quad",
        params: &default_test_params,
        test_fn: lock_bottom_quad_test_fn,
        conclusion: tester::TestConclusion::Pass,
    };

    //  ================================================
    //
    let coreboot_elog_sanity_test = tester::TestCase {
        name: "Coreboot ELOG sanity",
        params: &default_test_params,
        test_fn: |params| {
            // Check that the elog contains *something*, as an indication that Coreboot
            // is actually able to write to the Flash. Because this invokes mosys on the
            // host, it doesn't make sense to run for other chips.
            if params.fc != types::FlashChip::HOST {
                info!("Skipping ELOG sanity check for non-host chip");
                return Ok(());
            }
            // Output is one event per line, drop empty lines in the interest of being defensive.
            let event_count = mosys::eventlog_list()?
                .lines()
                .filter(|l| !l.is_empty())
                .count();

            if event_count == 0 {
                Err("ELOG contained no events".into())
            } else {
                Ok(())
            }
        },
        conclusion: tester::TestConclusion::Pass,
    };

    //  ================================================
    //
    let consistent_exit_test_fn =
        |param: &tester::TestParams| Ok(consistent_flash_checks(&param.cmd)?);
    let consistent_exit_test = tester::TestCase {
        name: "Flash image consistency check at end of tests",
        params: &default_test_params,
        test_fn: consistent_exit_test_fn,
        conclusion: tester::TestConclusion::Pass,
    };

    // run generic tests:
    //  ================================================

    // Register tests to run:
    let tests = [
        &wp_test,
        &read_test,
        &erase_write_test,
        &verify_fail_test,
        &lock_test,
        &lock_top_quad_test,
        &lock_bottom_quad_test,
        &lock_bottom_half_test,
        &lock_top_half_test,
        &coreboot_elog_sanity_test,
        &consistent_exit_test,
    ];
    // ------------------------.
    // Run all the tests and collate the findings:
    let results = tester::run_all_tests(&tests);

    let os_rel = sys_info::os_release().unwrap_or("<Unknown OS>".to_string());
    let system_info = mosys::system_info().unwrap_or("<Unknown System>".to_string());
    let bios_info = mosys::bios_info().unwrap_or("<Unknown BIOS>".to_string());
    let meta_data = tester::ReportMetaData {
        chip_name: chip_name,
        os_release: os_rel,
        system_info: system_info,
        bios_info: bios_info,
    };
    tester::collate_all_test_runs(&results, meta_data);
    Ok(())
}

fn consistent_flash_checks(cmd: &cmd::FlashromCmd) -> Result<(), FlashromError> {
    if flashrom::verify(&cmd, "/tmp/flashrom_tester_read.dat").is_ok() {
        return Ok(());
    }
    warn!("flash image in an inconsistent state! Attempting to restore..");
    flashrom::write(&cmd, "/tmp/flashrom_tester_read.dat")?;
    flashrom::verify(&cmd, "/tmp/flashrom_tester_read.dat")
}

fn test_section(
    cmd: &cmd::FlashromCmd,
    section: (&'static str, i64, i64),
) -> Result<(), Box<dyn std::error::Error>> {
    let (name, start, len) = section;

    debug!("test_section() :: name = '{}' ..", name);
    debug!("-------------------------------------------");

    consistent_flash_checks(&cmd)?;

    let rws = flashrom::ROMWriteSpecifics {
        layout_file: Some(LAYOUT_FILE),
        write_file: Some("/tmp/random_content.bin"),
        name_file: Some(name),
    };

    utils::toggle_hw_wp(true)?; // disconnect battery.
    flashrom::wp_toggle(&cmd, false)?;
    utils::toggle_hw_wp(false)?; // connect battery.
    flashrom::wp_status(&cmd, false)?;

    flashrom::wp_range(&cmd, (start, len), true)?;
    flashrom::wp_status(&cmd, false)?;

    if flashrom::write_file_with_layout(&cmd, &rws).is_ok() {
        return Err(
            "Section should be locked, should not have been overwritable with random data".into(),
        );
    }

    if flashrom::verify(&cmd, "/tmp/flashrom_tester_read.dat").is_err() {
        return Err("Section didn't locked, has been overwritable with random data!".into());
    }
    Ok(())
}

// ================================================
// Specialization tests:
// ================================================

fn host(cmd: &cmd::FlashromCmd) -> tester::TestParams {
    return tester::TestParams {
        cmd: &cmd,
        fc: types::FlashChip::HOST,
        log_text: None,
        pre_fn: None,
        post_fn: None,
    };
}

fn ec(_cmd: &cmd::FlashromCmd) -> tester::TestParams {
    panic!("Error: Unimplemented in this version, Please use 'host' parameter.");
}

fn servo(cmd: &cmd::FlashromCmd) -> tester::TestParams {
    let pre_fn = |_: &tester::TestParams| {
        if cmd::dut_ctrl_toggle_wp(false).is_err() {
            error!("failed to dispatch dut_ctrl_toggle_wp()!");
        }
    };
    let post_fn = |_: &tester::TestParams| {
        if cmd::dut_ctrl_toggle_wp(true).is_err() {
            error!("failed to dispatch dut_ctrl_toggle_wp()!");
        }
    };
    return tester::TestParams {
        cmd: &cmd,
        fc: types::FlashChip::SERVO,
        log_text: None,
        pre_fn: Some(pre_fn),
        post_fn: Some(post_fn),
    };
}

fn dediprog(cmd: &cmd::FlashromCmd) -> tester::TestParams {
    return tester::TestParams {
        cmd: &cmd,
        fc: types::FlashChip::DEDIPROG,
        log_text: None,
        pre_fn: None,
        post_fn: None,
    };
}
