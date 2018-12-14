/**
 * @file Headers for load.c, used to load audio files.
 *
 * Copyright (c) Jack MacDougall 2018
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef SIMPLE_AUDIO_C99_LOAD_H
#define SIMPLE_AUDIO_C99_LOAD_H

#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

/**
 * Audio file structure.
 */
typedef struct AudioFile {
    char *path;
    AVFormatContext *formatContext;
    AVCodec *codec;
    AVCodecContext *codecContext;
} AudioFile;

/**
 * Error code set when loadAudio fails.
 */
int loadAudioError;

/**
 * Load an audio file from the disk.
 * Returns NULL and sets loadAudioError when an error occurs.
 * @param path Audio file path.
 * @return AudioFile structure.
 */
AudioFile *loadAudio(char *);

/**
 * Clean up after an audio file.
 * @param audioFile Pointer to audio file.
 */
void closeAudio(AudioFile *);

#endif //SIMPLE_AUDIO_C99_LOAD_H