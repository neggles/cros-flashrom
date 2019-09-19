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
use super::flashrom::{self, Flashrom};
use super::mosys;
use super::tester::{self, NewTestCase, OutputFormat, TestEnv, TestResult};
use super::types::{self, FlashChip};
use super::utils::{self, LayoutNames};
use std::collections::HashMap;
use std::fs::File;
use std::io::{BufRead, Write};

const LAYOUT_FILE: &'static str = "/tmp/layout.file";

/// Run tests.
///
/// Only returns an Error if there was an internal error; test failures are Ok.
pub fn generic(
    path: &str,
    fc: types::FlashChip,
    print_layout: bool,
    output_format: OutputFormat,
) -> Result<(), Box<dyn std::error::Error>> {
    let p = path.to_string();
    let cmd = cmd::FlashromCmd { path: p, fc };

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

    // Register tests to run:
    let tests: &[&dyn NewTestCase] = &[
        &tester::TestCase {
            name: "Get device name",
            params: &default_test_params,
            test_fn: |params| {
                // Success means we got something back, which is good enough.
                flashrom::name(&params.cmd)?;
                Ok(())
            },
            conclusion: tester::TestConclusion::Pass,
        },
        &("Toggle WP", wp_toggle_test),
        &("Erase/Write", erase_write_test),
        &("Fail to verify", verify_fail_test),
        &("Lock", lock_test),
        &("Lock top quad", partial_lock_test(LayoutNames::TopQuad)),
        &(
            "Lock bottom quad",
            partial_lock_test(LayoutNames::BottomQuad),
        ),
        &(
            "Lock bottom half",
            partial_lock_test(LayoutNames::BottomHalf),
        ),
        &("Lock top half", partial_lock_test(LayoutNames::TopHalf)),
        &("Coreboot ELOG sanity", elog_sanity_test),
        &("Host is ChromeOS", host_is_chrome_test),
    ];
    // ------------------------.
    // Run all the tests and collate the findings:
    let results = tester::run_all_tests(fc, &cmd, tests);

    let chip_name = flashrom::name(&cmd)
        .map(|x| format!("vendor=\"{}\" name=\"{}\"", x.0, x.1))
        .unwrap_or("<Unknown chip>".into());
    let os_rel = sys_info::os_release().unwrap_or("<Unknown OS>".to_string());
    let system_info = mosys::system_info().unwrap_or("<Unknown System>".to_string());
    let bios_info = mosys::bios_info().unwrap_or("<Unknown BIOS>".to_string());

    let meta_data = tester::ReportMetaData {
        chip_name: chip_name,
        os_release: os_rel,
        system_info: system_info,
        bios_info: bios_info,
    };
    tester::collate_all_test_runs(&results, meta_data, output_format);
    Ok(())
}

fn wp_toggle_test(env: &mut TestEnv) -> TestResult {
    // Fails if unable to set either one
    env.wp.set_hw(false)?;
    env.wp.set_sw(false)?;
    Ok(())
}

fn erase_write_test(env: &mut TestEnv) -> TestResult {
    if !env.is_golden() {
        info!("Memory has been modified; reflashing to ensure erasure can be detected");
        env.ensure_golden()?;
    }

    // With write protect enabled erase should fail.
    env.wp.set_sw(true)?.set_hw(true)?;
    if env.erase().is_ok() {
        info!("Flashrom returned Ok but this may be incorrect; verifying");
        if !env.is_golden() {
            return Err("Hardware write protect asserted however can still erase!".into());
        }
        info!("Erase claimed to succeed but verify is Ok; assume erase failed");
    }

    // With write protect disabled erase should succeed.
    env.wp.set_hw(false)?.set_sw(false)?;
    env.erase()?;
    if env.is_golden() {
        return Err("Successful erase didn't modify memory".into());
    }

    Ok(())
}

fn lock_test(env: &mut TestEnv) -> TestResult {
    if !env.wp.can_control_hw_wp() {
        return Err("Lock test requires ability to control hardware write protect".into());
    }

    env.wp.set_hw(false)?;
    // Toggling software WP off should work when hardware is off.
    // Then enable again for another go.
    env.wp.push().set_sw(false)?;

    env.wp.set_hw(true)?;
    // Clearing should fail when hardware is enabled
    if env.wp.set_sw(false).is_ok() {
        return Err("Software WP was reset despite hardware WP being enabled".into());
    }
    Ok(())
}

fn elog_sanity_test(env: &mut TestEnv) -> TestResult {
    // Check that the elog contains *something*, as an indication that Coreboot
    // is actually able to write to the Flash. Because this invokes mosys on the
    // host, it doesn't make sense to run for other chips.
    if env.chip_type() != FlashChip::HOST {
        info!("Skipping ELOG sanity check for non-host chip");
        return Ok(());
    }
    // mosys reads the flash, it should be back in the golden state
    env.ensure_golden()?;
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
}

fn host_is_chrome_test(_env: &mut TestEnv) -> TestResult {
    let release_info = if let Ok(f) = File::open("/etc/os-release") {
        let buf = std::io::BufReader::new(f);
        parse_os_release(buf.lines().flatten())
    } else {
        info!("Unable to read /etc/os-release to probe system information");
        HashMap::new()
    };

    match release_info.get("ID") {
        Some(id) if id == "chromeos" || id == "chromiumos" => Ok(()),
        oid => {
            let id = match oid {
                Some(s) => s,
                None => "UNKNOWN",
            };
            Err(format!(
                "Test host os-release \"{}\" should be but is not chromeos",
                id
            )
            .into())
        }
    }
}

fn partial_lock_test(section: LayoutNames) -> impl Fn(&mut TestEnv) -> TestResult {
    move |env: &mut TestEnv| {
        // Need a clean image for verification
        env.ensure_golden()?;

        let (name, start, len) = utils::layout_section(env.layout(), section);
        // Disable software WP so we can do range protection, but hardware WP
        // must remain enabled for (most) range protection to do anything.
        env.wp.set_hw(false)?.set_sw(false)?.set_hw(true)?;
        flashrom::wp_range(env.cmd, (start, len), true)?;

        let rws = flashrom::ROMWriteSpecifics {
            layout_file: Some(LAYOUT_FILE),
            write_file: Some(env.random_data_file()),
            name_file: Some(name),
        };
        if flashrom::write_file_with_layout(env.cmd, &rws).is_ok() {
            return Err(
                "Section should be locked, should not have been overwritable with random data"
                    .into(),
            );
        }
        if !env.is_golden() {
            return Err("Section didn't lock, has been overwritten with random data!".into());
        }
        Ok(())
    }
}

fn verify_fail_test(env: &mut TestEnv) -> TestResult {
    // Comparing the flash contents to random data says they're not the same.
    match env.verify(env.random_data_file()) {
        Ok(_) => Err("Verification says flash is full of random data".into()),
        Err(_) => Ok(()),
    }
}

/// Ad-hoc parsing of os-release(5); mostly according to the spec,
/// but ignores quotes and escaping.
fn parse_os_release<I: IntoIterator<Item = String>>(lines: I) -> HashMap<String, String> {
    fn parse_line(line: String) -> Option<(String, String)> {
        if line.is_empty() || line.starts_with('#') {
            return None;
        }

        let delimiter = match line.find('=') {
            Some(idx) => idx,
            None => {
                warn!("os-release entry seems malformed: {:?}", line);
                return None;
            }
        };
        Some((
            line[..delimiter].to_owned(),
            line[delimiter + 1..].to_owned(),
        ))
    }

    lines.into_iter().filter_map(parse_line).collect()
}

#[test]
fn test_parse_os_release() {
    let lines = [
        "BUILD_ID=12516.0.0",
        "# this line is a comment followed by an empty line",
        "",
        "ID_LIKE=chromiumos",
        "ID=chromeos",
        "VERSION=79",
        "EMPTY_VALUE=",
    ];
    let map = parse_os_release(lines.iter().map(|&s| s.to_owned()));

    fn get<'a, 'b>(m: &'a HashMap<String, String>, k: &'b str) -> Option<&'a str> {
        m.get(k).map(|s| s.as_ref())
    }

    assert_eq!(get(&map, "ID"), Some("chromeos"));
    assert_eq!(get(&map, "BUILD_ID"), Some("12516.0.0"));
    assert_eq!(get(&map, "EMPTY_VALUE"), Some(""));
    assert_eq!(get(&map, ""), None);
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
