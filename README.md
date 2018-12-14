# Simple Audio Player C99
Simple audio file player, written in C99 using libav/FFMpeg and libao.
Written in 6~ hours as a C99 programming exercise.
Designed to work with Linux.

## Features
 * Should work on Windows, Mac OS and Linux.
 * Compatable with all FFmpeg supported file formats.
 * Single file written completely in C99.
 * Easy to use CLI interface.

## Installation
This program requires libao and either libav or FFMpeg to be installed on your machine.
Installation instructions for these dependencies can be found on their respective websites.
On Ubuntu this step is simple:  
`$ sudo apt install libavutil libavcodec libavformat libavresample`

## Usage
`$ ./simple-audio-c99 <file>`

Test with the included WAV file:  
`$ ./simple-audio-c99 windows98start.wav`

## Authors
Jack MacDougall ([jackmacdougall.com](https://jackmacdougall.com/))

## License
This project is licensed under [MIT](LICENSE).
More info in the [LICENSE](LICENSE) file.