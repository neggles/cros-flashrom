use super::types;
use std::io::Write;
use std::path::PathBuf;
use std::sync::Mutex;

struct Logger<W: Write + Send> {
    level: log::LevelFilter,
    target: LogTarget<W>,
}

enum LogTarget<W>
where
    W: Write,
{
    Terminal,
    Write(Mutex<W>),
}

impl<W: Write + Send> log::Log for Logger<W> {
    fn enabled(&self, metadata: &log::Metadata) -> bool {
        metadata.level() <= self.level
    }

    fn log(&self, record: &log::Record) {
        fn log_internal<W: Write>(mut w: W, record: &log::Record) -> std::io::Result<()> {
            let now = chrono::Local::now();
            write!(w, "{}{} ", types::MAGENTA, now.format("%Y-%m-%dT%H:%M:%S"))?;
            write!(
                w,
                "{}[ {} ]{} ",
                types::YELLOW,
                record.level(),
                types::RESET
            )?;
            writeln!(w, "{}", record.args())
        }

        // Write errors deliberately ignored
        let _ = match self.target {
            LogTarget::Terminal => {
                let stdout = std::io::stdout();
                let mut lock = stdout.lock();
                log_internal(&mut lock, record)
            }
            LogTarget::Write(ref mutex) => {
                let mut lock = mutex.lock().unwrap();
                log_internal(&mut *lock, record)
            }
        };
    }

    fn flush(&self) {
        // Flush errors deliberately ignored
        let _ = match self.target {
            LogTarget::Terminal => std::io::stdout().flush(),
            LogTarget::Write(ref w) => w.lock().unwrap().flush(),
        };
    }
}

pub fn init(to_file: Option<PathBuf>, debug: bool) {
    let mut logger = Logger {
        level: log::LevelFilter::Info,
        target: LogTarget::Terminal,
    };

    if debug {
        logger.level = log::LevelFilter::Debug;
    }
    if let Some(path) = to_file {
        logger.target = LogTarget::Write(Mutex::new(
            std::fs::File::create(path).expect("Unable to open log file for writing"),
        ));
    }

    log::set_max_level(logger.level);
    log::set_boxed_logger(Box::new(logger)).unwrap();
}

#[cfg(test)]
mod tests {
    use super::{LogTarget, Logger};
    use log::{Level, LevelFilter, Log, Record};
    use std::sync::Mutex;

    fn run_records(records: &[Record]) -> String {
        let mut buf = Vec::<u8>::new();
        {
            let lock = Mutex::new(&mut buf);
            let logger = Logger {
                level: LevelFilter::Info,
                target: LogTarget::Write(lock),
            };

            for record in records {
                if logger.enabled(record.metadata()) {
                    logger.log(&record);
                }
            }
        }
        String::from_utf8(buf).unwrap()
    }

    /// Log messages have the expected format
    #[test]
    fn format() {
        let buf = run_records(&[Record::builder()
            .args(format_args!("Test message at INFO"))
            .level(Level::Info)
            .build()]);

        assert_eq!(&buf[..5], "\x1b[35m");
        // Time is difficult to test, assume it's formatted okay
        assert_eq!(
            &buf[24..],
            " \x1b[33m[ INFO ]\x1b[0m Test message at INFO\n"
        );
    }

    #[test]
    fn level_filter() {
        let buf = run_records(&[
            Record::builder()
                .args(format_args!("Test message at DEBUG"))
                .level(Level::Debug)
                .build(),
            Record::builder()
                .args(format_args!("Hello, world!"))
                .level(Level::Error)
                .build(),
        ]);

        // There is one line because the Debug record wasn't written.
        println!("{}", buf);
        assert_eq!(buf.lines().count(), 1);
    }
}
