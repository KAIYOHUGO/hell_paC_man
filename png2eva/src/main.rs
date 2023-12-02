use anyhow::{anyhow, Result};
use clap::Parser;
use std::fs::File;
use std::io::Write;

#[derive(Debug, Parser)]
struct Cli {
    input: String,

    #[arg(short, long, default_value = "out.eva")]
    output: String,
}

fn main() -> Result<()> {
    let cli = Cli::parse();

    let img = image::open(cli.input)?;
    let mut output = File::create(cli.output)?;
    let rgba_img = img
        .as_rgba8()
        .ok_or_else(|| anyhow!("Cannot parse as rgba"))?;
    let height = rgba_img.height();
    let width = rgba_img.width();
    output.write(&[height as u8, width as u8])?;
    for y in 0..height {
        for x in 0..width {
            let rgba = rgba_img[(x, y)].0;
            output.write(&rgba)?;
        }
    }

    Ok(())
}
