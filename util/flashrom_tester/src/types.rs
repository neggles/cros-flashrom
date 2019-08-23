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

#[derive(Copy, Clone, PartialEq, Debug)]
pub enum FlashChip {
    EC,
    HOST,
    SERVOv2,
    DEDIPROG,
}

impl FlashChip {
    pub fn from(s: &str) -> Result<FlashChip, &str> {
        let r = match s {
            "ec" => Ok(FlashChip::EC),
            "host" => Ok(FlashChip::HOST),
            "servo-v2" => Ok(FlashChip::SERVOv2),
            "dediprog" => Ok(FlashChip::DEDIPROG),
            _ => Err("cannot convert str to enum"),
        };
        return r;
    }
    pub fn to(fc: FlashChip) -> &'static str {
        let r = match fc {
            FlashChip::EC => "ec",
            FlashChip::HOST => "host",
            FlashChip::SERVOv2 => "ft2232_spi:type=servo-v2",
            FlashChip::DEDIPROG => "dediprog",
        };
        return r;
    }
}

pub const BOLD: &str = "\x1b[1m";

pub const RESET: &str = "\x1b[0m";
pub const BLACK: &str = "\x1b[30m";
pub const MAGENTA: &str = "\x1b[35m";
pub const YELLOW: &str = "\x1b[33m";
pub const GREEN: &str = "\x1b[92m";
pub const RED: &str = "\x1b[31m";

macro_rules! style_dbg {
    ($s: expr, $c: expr) => {
        format!("{}{:?}{}", $c, $s, types::RESET)
    };
}
macro_rules! style {
    ($s: expr, $c: expr) => {
        format!("{}{}{}", $c, $s, types::RESET)
    };
}
