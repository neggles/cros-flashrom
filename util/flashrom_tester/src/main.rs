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

#[macro_use]
extern crate log;
extern crate chrono;
extern crate env_logger;

mod tests;
mod tester;
mod types;
mod cmd;
mod rand;
mod utils;
mod flashrom;
mod mosys;

use std::env;
use std::io::Write;
use chrono::Local;
use env_logger::Builder;
use log::LevelFilter;

fn dispatch_args(args: &[String]) -> Result<(), Box<dyn std::error::Error>> {
    match args {
        [_, ref path, ref flashchip] => {
            if path.len() > 0 {
                    debug!("got flashrom path = '{}'.", path);
            } else {
                    return Err("Missing flashrom path".into())
            }

            let fc: Result<types::FlashChip, &str> = types::FlashChip::from(&flashchip[..]);
            if fc.is_err() {
                return Err("Missing flashchip type, should be either 'ec', 'host', or 'servo-v2'.".into());
            }
            tests::generic(path, fc.unwrap())
        },
        _ => {
            Ok(())
        }
    }
}

fn help(s: Option<&str>) {
    eprintln!("");
    if s.is_some() {
        eprintln!("{}", s.unwrap());
    }
    eprintln!("Usage:
    flashrom_tester flashrom.bin <ec|host|servo-v2>");

}

fn parse_args(args: Vec<String>) -> Result<(), Box<dyn std::error::Error>> {
    match args.len() {
        1 => return Err("No arguments passed!".into()),
        3 => {
            match dispatch_args(args.as_slice()) {
                Ok(_) => Ok(()),
                Err(e) => {
                    error!("flashrom_tester failed to run due to an internal error with: {}.", e.to_string());
                    Err("Please verify that both 'dut-control' and 'crossytem' are in your PATH!".into())
                },
            }
        }
        _ => Err("Incorrect number of arguments passed, expected 3.".into()),
    }
}

pub mod built_info {
    include!(concat!(env!("OUT_DIR"), "/built.rs"));
}

fn compiletime_info() {
    info!("This is version {}, built for {} by {}.",
          built_info::PKG_VERSION, built_info::TARGET, built_info::RUSTC_VERSION);

    trace!("I was built with profile \"{}\", features \"{}\" on {}.",
           built_info::PROFILE, built_info::FEATURES_STR,
           built_info::BUILT_TIME_UTC);
}

fn main() {
    Builder::new()
        .format(|buf, record| {
            writeln!(buf,
                     "{} [ {} ] - {}",
                     colour!(Local::now().format("%Y-%m-%dT%H:%M:%S"), types::MAGENTA),
                     colour!(record.level(), types::YELLOW), record.args()
            )
        })
    .filter(None, LevelFilter::Info)
    .parse_filters(&env::var("FLASHROM_TESTER_LOG").unwrap_or_default())
    .init();

    compiletime_info();

    let args: Vec<String> = env::args().collect();
    match parse_args(args) {
        Ok(_) => return,
        Err(e) => help(Some(&e.to_string())),
    };
}
