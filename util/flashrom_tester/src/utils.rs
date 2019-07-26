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

//#![feature(drain_filter)]

use std::io::prelude::*;
use std::fs::File;
use std::process::Command;
use std::io::{Error, ErrorKind};

//TODO(quasisec): Use drain_filter feature when we can.
//pub fn vgrep<'a>(s: &'a str, m: &str) -> &'a str {
//    let v: Vec<&str> = s.split('\n').collect();
//    v.drain_filter(|&x| x == m).collect::<Vec<_>>();
//    return drained.iter().fold("".to_string(), |x, xs| x + xs);
//}
pub fn vgrep(s: &str, m: &str) -> String {
    let v: Vec<&str> = s.split('\n').collect();
    let (drained, _v): (Vec<_>, Vec<_>) = v.into_iter().partition(|&x| x.find(m).is_none());

    return drained.iter().fold("".to_string(), |x, xs| x + xs);
}

pub fn qgrep(s: &str, m: &str) -> bool {
    // return true for any substring match
    return s.find(m).is_some();
}

pub fn last_line(s: &str) -> &str {
    s.split('\n').collect::<Vec<&str>>().last().unwrap_or(&"")
}

pub fn hex_string(v: i64) -> String {
    format!("0x{:06X}", v).to_string()
}

#[derive(Debug)]
pub enum LayoutNames {
    TopQuad, TopHalf, BottomHalf, BottomQuad
}

pub struct LayoutSizes {
    half_sz: i64,
    quad_sz: i64,
    rom_top: i64,
    bottom_half_top: i64,
    bottom_quad_top: i64,
    top_quad_bottom: i64,
}

pub fn get_layout_sizes(rom_sz: i64) -> Result<LayoutSizes, std::io::Error> {
    if rom_sz <= 0 {
        return Err(Error::new(ErrorKind::Other, "invalid rom size provided"));
    }
    if rom_sz % 2 != 0 {
        return Err(Error::new(ErrorKind::Other, "invalid rom size, not a power of 2"));
    }
    Ok(LayoutSizes{
        half_sz: rom_sz / 2,
        quad_sz: rom_sz / 4,
        rom_top: rom_sz - 1,
        bottom_half_top: (rom_sz / 2) - 1,
        bottom_quad_top: (rom_sz / 4) - 1,
        top_quad_bottom: (rom_sz / 4) * 3,
    })
}

pub fn layout_section(ls: &LayoutSizes, ln: LayoutNames) -> (&'static str, i64, i64) {
    match ln {
        LayoutNames::TopQuad => ("TOP_QUAD", ls.top_quad_bottom, ls.quad_sz),
        LayoutNames::TopHalf => ("TOP_HALF", ls.half_sz, ls.half_sz),
        LayoutNames::BottomHalf => ("BOTTOM_HALF", 0, ls.half_sz),
        LayoutNames::BottomQuad => ("BOTTOM_QUAD", 0, ls.quad_sz),
    }
}

pub fn construct_layout_file(path: &str, ls: &LayoutSizes) -> std::io::Result<()> {
    let mut buf = File::create(path)?;

    writeln!(buf, "000000:{:x} BOTTOM_QUAD", ls.bottom_quad_top)?;
    writeln!(buf, "000000:{:x} BOTTOM_HALF", ls.bottom_half_top)?;
    writeln!(buf, "{:x}:{:x} TOP_HALF", ls.half_sz, ls.rom_top)?;
    writeln!(buf, "{:x}:{:x} TOP_QUAD", ls.top_quad_bottom, ls.rom_top)?;

    buf.flush()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn _vgrep() {
        let s = "XXXqcYYY";
        let m = "qc";
        let n = "zz";

        assert_eq!("", vgrep(s, s));
        assert_eq!("", vgrep(s, m));
        assert_eq!(s, vgrep(s, n));

        let x = "idk\nLALLA\nZZZ\nQQ";
        let xs = "LA";

        assert_eq!("idkZZZQQ", vgrep(x, xs));
    }

    #[test]
    fn _qgrep() {
        let s = "XXXqcYYY";
        let m = "qc";
        let n = "zz";

        assert_eq!(true, qgrep(s, s));
        assert_eq!(true, qgrep(s, m));
        assert_eq!(false, qgrep(s, n));
    }

    #[test]
    fn _last_line() {
        let s = "XXX\nYYY\nZZZ";

        assert_eq!("ZZZ", last_line(s));
        assert_ne!("XXX", last_line(s));
    }
}

pub fn toggle_hw_wp(dis: bool) {
    // The easist way to toggle the harware write-protect is
    // to {dis}connect the battery.
    let s = if dis { "dis" } else { "" };
    info!(" > {}connect the battery", s);
    pause();
}

fn pause() {
    let mut stdout = std::io::stdout();
    // We want the cursor to stay at the end of the line, so we print without a newline
    // and flush manually.
    stdout.write(b"Press any key to continue...").unwrap();
    stdout.flush().unwrap();
    std::io::stdin().read(&mut [0]).unwrap();
}

pub fn gather_system_info() -> std::result::Result<(bool), std::io::Error> {
    info!("Gathering system information for the log file.");
    let cmd = Command::new("crossystem")
        .output()?;

    if !cmd.status.success() {
        // There is two cases on failure;
        //  i. ) A bad exit code,
        //  ii.) A SIG killed us.
        match cmd.status.code() {
            Some(code) => {
                let e = format!("Exited with error code: {}", code);
                return Err(Error::new(ErrorKind::Other, e));
            },
            None => return Err(Error::new(ErrorKind::Other, "Process terminated by a signal")),
        }
    };

    let stdout = String::from_utf8_lossy(&cmd.stdout);
    match parse_crosssystem(&stdout) {
        Ok((sysinfo, wp)) => {
            info!("{:#?}", sysinfo);
            return Ok(wp);
        },
        Err(_) => return Err(Error::new(ErrorKind::Other, "Hardware write protect is Unknown")),
    }
}

fn parse_crosssystem(s: &str) -> Result<(Vec<&str>, bool), &'static str> {
    // grep -v 'fwid +=' | grep -v 'hwid +='
    let sysinfo = s.split_terminator("\n").filter(|s| !s.contains("fwid +=") && !s.contains("hwid +="));
    let wp_s = sysinfo.clone().filter(|s| s.contains("wpsw_cur"));

    let wp_s_val = wp_s.collect::<Vec<_>>().first().unwrap().trim_start_matches("wpsw_cur")
        .trim_start_matches(' ')
        .trim_start_matches('=')
        .trim_start_matches(' ')
        .get(..1).unwrap()
        .parse::<u32>();

    match wp_s_val {
        Ok(v) => {
           if v == 1 {
               return Ok( (sysinfo.collect(), true) );
           } else if v == 0 {
               return Ok( (sysinfo.collect(), false) );
           } else {
               return Err("Unknown state value");
           }
        },
        Err(_) => return Err("Cannot parse state value"),
    }
}