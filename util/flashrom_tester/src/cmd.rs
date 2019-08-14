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

use super::flashrom;
use super::types;
use super::utils;

use std::process::Command;
use std::io::{Error, ErrorKind};

pub struct FlashromCmd {
    pub path: String,
    pub fc:   types::FlashChip,
}

impl flashrom::Flashrom for FlashromCmd {
    fn get_size(&self) -> Result<i64, std::io::Error> {
        let (stdout, _) = flashrom_dispatch(self.path.as_str(), &["--get-size"], self.fc)?;
        let sz = String::from_utf8_lossy(&stdout);

        match utils::vgrep(&sz, "coreboot").trim_end().parse::<i64>() {
            Ok(s) => Ok(s),
            Err(_e) => Err(Error::new(ErrorKind::Other, "'flashrom --get-size' output did not parse as int correctly")),
        }
    }

    // do I need this?
    fn new(path: String, fc: types::FlashChip) -> Self {
        FlashromCmd{path, fc}
    }

    fn dispatch(&self, fropt: flashrom::FlashromOpt)
        -> Result<(Vec<u8>, Vec<u8>), std::io::Error> {
        let params = flashrom_decode_opts(fropt);
        flashrom_dispatch(self.path.as_str(), &params, self.fc)
    }
}

fn flashrom_decode_opts(opts: flashrom::FlashromOpt) -> Vec<String> {
    let mut params = Vec::<String>::new();

    // ------------ WARNING !!! ------------
    // each param must NOT contain spaces!
    // -------------------------------------

    // wp_opt
   if opts.wp_opt.range.is_some() {
       let (x0, x1) = opts.wp_opt.range.unwrap();
       params.push("--wp-range".to_string());
       params.push(utils::hex_string(x0));
       params.push(utils::hex_string(x1));
    }
    if opts.wp_opt.status {
        params.push("--wp-status".to_string());
    } else if opts.wp_opt.list {
        params.push("--wp-list".to_string());
    } else if opts.wp_opt.enable {
        params.push("--wp-enable".to_string());
    } else if opts.wp_opt.disable {
        params.push("--wp-disable".to_string());
    }

    // io_opt
    if opts.io_opt.read.is_some() {
        params.push("-r".to_string());
        params.push(opts.io_opt.read.unwrap().to_string());
    } else if opts.io_opt.write.is_some() {
        params.push("-w".to_string());
        params.push(opts.io_opt.write.unwrap().to_string());
    } else if opts.io_opt.verify.is_some() {
        params.push("-v".to_string());
        params.push(opts.io_opt.verify.unwrap().to_string());
    } else if opts.io_opt.erase {
        params.push("-E".to_string());
    }

    // misc_opt
    if opts.layout.is_some() {
        params.push("-l".to_string());
        params.push(opts.layout.unwrap().to_string());
    }
    if opts.image.is_some() {
        params.push("-i".to_string());
        params.push(opts.image.unwrap().to_string());
    }

    if opts.flash_name {
        params.push("--flash-name".to_string());
    }
    if opts.ignore_fmap {
        params.push("--ignore-fmap".to_string());
    }
    if opts.verbose {
        params.push("-V".to_string());
    }

    params
}

fn flashrom_dispatch<S: AsRef<str>>(path: &str, params: &[S], fc: types::FlashChip)
    -> Result<(Vec<u8>, Vec<u8>), std::io::Error> {
    // from man page:
    //  ' -p, --programmer <name>[:parameter[,parameter[,parameter]]] '
    let mut args: Vec<&str> = vec!["-p", types::FlashChip::to(fc)];
    args.extend(params.iter().map(S::as_ref));

    info!("flashrom_dispatch() running: {} {:?}", path, args);

    let output = Command::new(path)
        .args(&args)
        .output()?;
    if !output.status.success() {
        // There is two cases on failure;
        //  i. ) A bad exit code,
        //  ii.) A SIG killed us.
        match output.status.code() {
            Some(code) => {
                let e = format!("{}\nExited with error code: {}", String::from_utf8_lossy(&output.stderr), code);
                return Err(Error::new(ErrorKind::Other, e));
            },
            None => return Err(Error::new(ErrorKind::Other, "Process terminated by a signal".to_string()))
        }
    }

    Ok((output.stdout, output.stderr))
}

pub fn dut_ctrl_toggle_wp(en: bool)
    -> Result<(Vec<u8>, Vec<u8>), std::io::Error> {

    let args = if en {
        ["fw_wp_en:off", "fw_wp:on"]
    } else {
        ["fw_wp_en:on", "fw_wp:off"]
    };

    let output = Command::new("dut-control")
        .args(&args)
        .output()?;
    if !output.status.success() {
        // There is two cases on failure;
        //  i. ) A bad exit code,
        //  ii.) A SIG killed us.
        match output.status.code() {
            Some(code) => {
                let e = format!("Exited with error code: {}", code);
                return Err(Error::new(ErrorKind::Other, e));
            },
            None => return Err(Error::new(ErrorKind::Other, "Process terminated by a signal".to_string()))
        }
    }

    Ok((output.stdout, output.stderr))
}
