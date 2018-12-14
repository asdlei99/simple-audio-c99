/**
 * @file C99 version of my simple-audio-player project, originally written in Python 2.7.
 *
 * Copyright (c) Jack MacDougall 2018
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * Include libraries.
 */

#include <stdio.h>
#include <stdlib.h>

#include "err.h"
#include "load.h"
#include "play.h"


/*
 * Main code.
 */

//Main function runs when the program is started.
int main(int argc, char *argv[]) {
    /*
     * Simple argument parsing.
     * TODO: Replace with better system in future.
     */

    //Make sure a file argument is present.
    if (argc < 2) {
        //Missing argument.
        printErr("Missing file argument");
        return 1;
    }
    //Get file string.
    char *file = argv[1];
    //Say what file we are loading.
    printf("Loading file '%s'.\n", file);


    /*
     * Load audio file.
     */

    //Load the audio.
    AudioFile *audioFile = loadAudio(file);
    //Check for errors.
    if (loadAudioError > 0) {
        return loadAudioError;
    }


    /*
     * Play the audio file.
     */

    //Determine duration.
    int64_t duration = audioFile->formatContext->duration / AV_TIME_BASE;
    int64_t s = duration % 60;
    duration /= 60;
    int64_t m = duration % 60;
    duration /= 60;
    int64_t h = duration;
    //Log.
    printf("File '%s' has a a duration of %lu hours, %lu minutes and %lu seconds, playing with codec '%s'.\n",
           audioFile->formatContext->filename, h, m, s, audioFile->codec->long_name);

    //Play audio.
    int err = playAudio(audioFile);
    //Check for errors.
    if (err != 0) {
        return err;
    }

    /*
     * Clean up.
     */

    //Free audio file.
    closeAudio(audioFile);
    //Return.
    return 0;
}