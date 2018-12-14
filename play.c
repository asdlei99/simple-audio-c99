/**
 * @file Used to play audio data.
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

#include "play.h"

#include <stdio.h>
#include <stdlib.h>

#include <libavutil/avutil.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavresample/avresample.h>
#include <libavutil/opt.h>

#include <ao/ao.h>

#include "err.h"
#include "load.h"


/*
 * Audio playback functions.
 */

//Play audio.
int playAudio(AudioFile *audioFile) {
    /*
     * Utilities.
     */

    //Error number container.
    int err = 0;
    //Iterator.
    int i = 0;


    /*
     * Set up libao output.
     */

    //Run initializer.
    ao_initialize();

    //Get default driver.
    int defaultDriver = ao_default_driver_id();
    //Get driver info.
    ao_info *info = ao_driver_info(defaultDriver);

    //Create output format.
    ao_sample_format *sampleFormat = malloc(sizeof(ao_sample_format));
    //Set options.
    sampleFormat->bits = OUTPUT_BITS;
    sampleFormat->channels = OUTPUT_CHANNELS;
    sampleFormat->rate = OUTPUT_RATE;
    sampleFormat->byte_format = info->preferred_byte_format;
    sampleFormat->matrix = 0;

    //Open audio output.
    ao_device *device = ao_open_live(defaultDriver, sampleFormat, NULL);
    //Check for errors.
    if (device == NULL) {
        printErr("Failed to open output");
        printf("%i", errno);
        free(sampleFormat);
        ao_close(device);
        ao_shutdown();
        return 8;
    }


    /*
     * Resample audio.
     */

    //Allocate resampler context.
    AVAudioResampleContext *resampleContext = avresample_alloc_context();

    //Set resampler properties.

    //File channels.
    av_opt_set_int(resampleContext, "in_channel_layout",
                   av_get_default_channel_layout(audioFile->codecContext->channels), 0);
    //Device channels.
    av_opt_set_int(resampleContext, "out_channel_layout",
                   av_get_default_channel_layout(sampleFormat->channels), 0);

    //File sample rate.
    av_opt_set_int(resampleContext, "in_sample_rate",
                   audioFile->codecContext->sample_rate, 0);
    //Device sample rate.
    av_opt_set_int(resampleContext, "out_sample_rate",
                   sampleFormat->rate, 0);

    //File bit depth.
    av_opt_set_int(resampleContext, "in_sample_fmt",
                   audioFile->codecContext->sample_fmt, 0);

    //Device bit depth.
    //Depends on OUTPUT_BITS!!
    av_opt_set_int(resampleContext, "out_sample_fmt",
                   AV_SAMPLE_FMT_S16, 0);

    //Attempt to open the resampler.
    err = avresample_open(resampleContext);
    //Check for errors.
    if (err != 0) {
        printAVErr(err);
        free(sampleFormat);
        ao_close(device);
        ao_shutdown();
        avresample_free(&resampleContext);
        return 9;
    }


    /*
     * Playback!
     */

    //Start at beginning.
    avformat_seek_file(audioFile->formatContext, 0, 0, 0, 0, 0);

    //Get sample format.
    int64_t outputSampleFormat;
    av_opt_get_int(resampleContext, "out_sample_fmt", 0, &outputSampleFormat);

    //Init the packet.
    AVPacket packet = {0};
    av_init_packet(&packet);

    //Allocate a new frame.
    AVFrame *frame = av_frame_alloc();

    //Variables for the loop.
    int gotFrame = 0; //Did we get a frame?
    int count = 0; //Bytes consumed.
    int outputSampleCount = 0; //Samples to play after resampling.
    uint8_t *output; //Audio data buffer.
    int outputLineSize; //Magic for libav, probably.

    //Read the file!
    for (;;) {

        //Fill packets, quit if no more data.
        if (av_read_frame(audioFile->formatContext, &packet)) {
            //No more data.
            break;
        }

        //Did we get a frame?
        gotFrame = 0;

        //Decode audio, count is number of bytes consumed from the input.
        count = avcodec_decode_audio4(audioFile->codecContext, frame, &gotFrame, &packet);

        //Can we play audio?
        if (gotFrame != 0) {
            //Calculate number of samples after resampling.
            outputSampleCount = avresample_get_out_samples(resampleContext, frame->nb_samples);

            //Allocate buffer for samples.
            //TODO: May not work! (cast outputSampleFormat to AVSampleFormat)
            av_samples_alloc(&output, &outputLineSize, sampleFormat->channels, outputSampleCount,
                             outputSampleFormat, 0);

            //Resample!
            outputSampleCount = avresample_convert(resampleContext, &output, outputLineSize, outputSampleCount,
                                               frame->extended_data, frame->linesize[0], frame->nb_samples);

            //Play raw data with libao.
            ao_play(device, (char*)output, outputSampleCount * 4);
        }

        //Free the output buffer.
        if (output != NULL) {
            free(output);
            output = NULL;
        }
    }


    /*
     * Clean up!
     */

    //Clear sample format.
    free(sampleFormat);

    //Close audio output.
    ao_close(device);
    ao_shutdown();

    //Clean up context.
    avresample_free(&resampleContext);

    //Free the output buffer.
    if (output != NULL) {
        free(output);
    }

    //Free frame and packet.
    av_frame_free(&frame);
    av_free_packet(&packet);

    //Return!
    return 0;
}