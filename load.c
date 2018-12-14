/**
 * @file Used to load audio files.
 *
 * Copyright (c) Jack MacDougall 2018
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


/*
 * Includes.
 */

#include "load.h"

#include <stdio.h>
#include <stdlib.h>

#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#include "err.h"


/*
 * Audio file management functions.
 */

//Have we called av_register_all?
int av_register_all_called = 0;

//Load an audio file from disk.
AudioFile *loadAudio(char *path) {
    //Set error code.
    loadAudioError = 0;

    //Error number container.
    int err = 0;
    //Iterator.
    int i = 0;

    //Create a new AudioFile.
    AudioFile *audioFile = malloc(sizeof(AudioFile));
    //Set the path.
    audioFile->path = path;

    //Init FFMpeg if needed.
    if (av_register_all_called == 0) {
        av_register_all();
        av_register_all_called = 1;
    }

    //Define our format context.
    audioFile->formatContext = NULL;
    //Attempt to load the file.
    err = avformat_open_input(&audioFile->formatContext, path, NULL, NULL);
    //Check for errors.
    if (err != 0) {
        //Failed.
        printAVErr(err);
        loadAudioError = 2;
        avformat_close_input(&audioFile->formatContext);
        return NULL;
    }

    //Get stream information.
    err = avformat_find_stream_info(audioFile->formatContext, NULL);
    //Check for errors.
    if (err != 0) {
        //Failed.
        printAVErr(err);
        loadAudioError = 3;
        avformat_close_input(&audioFile->formatContext);
        return NULL;
    }

    //Find a valid audio stream.
    int streamIndex = -1;
    for (i = 0; i < audioFile->formatContext->nb_streams; i++) {
        //Check if this is an audio stream.
        if (audioFile->formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            //Found an audio stream! Set stream index.
            streamIndex = i;
            break;
        }
    }
    //Make sure we found a stream.
    if (streamIndex == -1) {
        //No stream.
        printErr("No audio stream found");
        loadAudioError = 4;
        avformat_close_input(&audioFile->formatContext);
        return NULL;
    }

    //Find a codec decoder.
    audioFile->codec = avcodec_find_decoder(audioFile->formatContext->streams[i]->codecpar->codec_id);
    //Make sure we have a valid decoder.
    if (audioFile->codec == NULL) {
        //No decoder.
        printErr("Unsupported audio codec");
        loadAudioError = 5;
        avformat_close_input(&audioFile->formatContext);
        return NULL;
    }

    //Create our codec context.
    audioFile->codecContext = avcodec_alloc_context3(audioFile->codec);

    //Load the context.
    err = avcodec_parameters_to_context(audioFile->codecContext, audioFile->formatContext->streams[i]->codecpar);
    //Check for errors.
    if (err != 0) {
        //Failed.
        printAVErr(err);
        loadAudioError = 6;
        avformat_close_input(&audioFile->formatContext);
        avcodec_free_context(&audioFile->codecContext);
        return NULL;
    }

    //Open the codec.
    err = avcodec_open2(audioFile->codecContext, audioFile->codec, NULL);
    //Check for errors.
    if (err != 0) {
        //Failed.
        printAVErr(err);
        loadAudioError = 7;
        avformat_close_input(&audioFile->formatContext);
        avcodec_free_context(&audioFile->codecContext);
        return NULL;
    }

    //Return our audio file.
    return audioFile;
}

//Clean up an audio file.
void closeAudio(AudioFile *audioFile) {
    //Free context.
    avcodec_free_context(&audioFile->codecContext);
    //Close input.
    avformat_close_input(&audioFile->formatContext);
    //Free the structure.
    free(audioFile);
}