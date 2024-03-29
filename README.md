# Simple Audio Player C99
Simple audio file player, written in C99 using libav/FFMpeg and libao.
Written in 6~ hours as a C99 programming exercise.
Designed to work with Linux.

## Features
 * Should work on Windows, Mac OS and Linux.
 * Compatable with all FFmpeg supported file formats.
 * Simple files with many comments written entirely in C99.
 * Easy to use CLI interface.

## Installation
This program requires libao and either libav or FFMpeg to be installed on your machine.
Installation instructions for these dependencies can be found on their respective websites.
On Ubuntu this step is simple:  
`$ sudo apt install libao-dev libavutil-dev libavcodec-dev libavformat-dev libavresample-dev`

## Usage
`$ ./simple-audio-c99 <file>`

Test with the included WAV file:  
`$ ./simple-audio-c99 windows98start.wav`

## Authors
Jack MacDougall ([lua.wtf](https://lua.wtf/))

## License
This project is licensed under [MIT](LICENSE).
More info in the [LICENSE](LICENSE) file.
