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
use super::types;
use super::cmd;

// type-signature comes from the return type of flashrom.rs workers.
type TestResult = Result<(), std::io::Error>;

type TestFunction = fn(&TestParams) -> TestResult;
type PreFunction = fn(&TestParams) -> ();
type PostFunction = fn(&TestParams) -> ();

pub struct TestParams<'a> {
    pub cmd: &'a cmd::FlashromCmd,
    pub fc: types::FlashChip,
    pub log_text: Option<&'a str>,
    pub pre_fn: Option<PreFunction>,
    pub post_fn: Option<PostFunction>,
}

#[derive(Copy, Clone, PartialEq, Debug)]
pub enum TestConclusion {
    Pass, Fail, UnexpectedPass, UnexpectedFail
}

pub struct TestCase<'a> {
    pub name: &'a str,
    pub test_fn: TestFunction,
    pub params: &'a TestParams<'a>,
    pub conclusion: TestConclusion,
}

pub struct ReportMetaData {
    pub chip_name: std::string::String,
    pub os_release: std::string::String,
    pub system_info: std::string::String,
    pub bios_info: std::string::String,
}

fn decode_test_result(res: TestResult, con: TestConclusion) -> (TestConclusion, Option<std::io::Error>) {
    if res.is_ok() && con == TestConclusion::Fail {
        return (TestConclusion::UnexpectedPass, None);
    }
    if res.is_err() && con == TestConclusion::Pass {
        return (TestConclusion::UnexpectedFail, res.err());
    }

    (TestConclusion::Pass, None)
}

fn run_test(t: &TestCase) -> (TestConclusion, Option<std::io::Error>) {
    let params = &t.params;

    if params.log_text.is_some() {
        println!("{:?}", params.log_text);
    }

    if params.pre_fn.is_some() {
        params.pre_fn.unwrap()(params);
    }

    let res = (t.test_fn)(params);

    if params.post_fn.is_some() {
        params.post_fn.unwrap()(params);
    }

    decode_test_result(res, t.conclusion)
}

pub fn run_all_tests<'a>(ts: &Vec<&TestCase<'a>>) -> Vec<(&'a str, (TestConclusion, Option<std::io::Error>))> {
    let mut results = Vec::new();
    for t in ts.iter() {
        results.push( (t.name, run_test(t)) );
    }
    results
}

// not getting exported from types.rs due to ordering???
macro_rules! style {
    ($s: expr, $c: expr) => { format!("{}{:?}{}", $c, $s, types::RESET) }
}
macro_rules! style_ {
    ($s: expr, $c: expr) => { format!("{}{}{}", $c, $s, types::RESET) }
}
pub fn collate_all_test_runs<'a>(truns: Vec<(&'a str, (TestConclusion, Option<std::io::Error>))>, meta_data: ReportMetaData) -> Result<(), std::io::Error> {
    println!("");
    println!("  =============================");
    println!("  =====  AVL qual RESULTS  ====");
    println!("  =============================");
    println!("");
    println!("  %---------------------------%");
    println!("   os release: {}", meta_data.os_release);
    println!("   chip name: {}", meta_data.chip_name);
    println!("   system info: \n{}", meta_data.system_info);
    println!("   bios info: \n{}", meta_data.bios_info);
    println!("  %---------------------------%");
    println!("");
    for trun in truns.iter() {
        let (name, (result, error)) = trun;
        if *result != TestConclusion::Pass {
            println!(" {} {}", style_!(format!(" <+> {} test:", name), types::BOLD), style!(result, types::RED));
            match error {
                None => {},
                Some(e) => info!(" - {} failure details:\n{}", name, e.to_string()),
            };
        } else {
            println!(" {} {}", style_!(format!(" <+> {} test:", name), types::BOLD), style!(result, types::GREEN));
        }
    }
    println!("");
    Ok(())
}
