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

#[macro_use]
mod types;

mod cmd;
mod flashrom;
mod mosys;
mod rand_util;
mod tester;
mod tests;
mod utils;

use chrono::Local;
use clap::{App, Arg};
use env_logger::Builder;
use log::LevelFilter;
use std::env;
use std::io::Write;

pub mod built_info {
    include!(concat!(env!("OUT_DIR"), "/built.rs"));
}

fn main() {
    Builder::new()
        .format(|buf, record| {
            writeln!(
                buf,
                "{} [ {} ] - {}",
                style!(Local::now().format("%Y-%m-%dT%H:%M:%S"), types::MAGENTA),
                style!(record.level(), types::YELLOW),
                record.args()
            )
        })
        .filter(None, LevelFilter::Info)
        .parse_filters(&env::var("FLASHROM_TESTER_LOG").unwrap_or_default())
        .init();

    let matches = App::new("flashrom_tester")
        .long_version(&*format!(
            "{}-{}\n\
             Target: {}\n\
             Profile: {}\n\
             Features: {:?}\n\
             Build time: {}\n\
             Compiler: {}",
            built_info::PKG_VERSION,
            option_env!("VCSID").unwrap_or("<unknown>"),
            built_info::TARGET,
            built_info::PROFILE,
            built_info::FEATURES,
            built_info::BUILT_TIME_UTC,
            built_info::RUSTC_VERSION,
        ))
        .arg(Arg::with_name("flashrom_binary").required(true))
        .arg(
            Arg::with_name("ccd_target_type")
                .required(true)
                .possible_values(&["host", "ec", "servo"]),
        )
        .arg(
            Arg::with_name("print-layout")
                .long("print-layout")
                .help("Print the layout file's contents before running tests"),
        )
        .get_matches();

    let flashrom_path = matches
        .value_of("flashrom_binary")
        .expect("flashrom_binary should be required");
    let ccd_type = types::FlashChip::from(
        matches
            .value_of("ccd_target_type")
            .expect("ccd_target_type should be required"),
    )
    .expect("ccd_target_type should admit only known types");

    let print_layout = matches.is_present("print-layout");

    if let Err(e) = tests::generic(flashrom_path, ccd_type, print_layout) {
        eprintln!("Failed to run tests: {:?}", e);
    }
}
