/**
 * @file Used to output errors.
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

#include "err.h"

#include <stdio.h>
#include <stdlib.h>

#include <libavutil/avutil.h>


/*
 * Error output functions.
 */

//Function to convert errors into strings.
char *avErrToString(int err) {
    //Allocate buffer.
    char *buffer = (char *) malloc(ERR_AVERRTOSTRING_BUFFER);
    //Fill buffer.
    av_strerror(err, buffer, ERR_AVERRTOSTRING_BUFFER);
    //Return the string.
    return buffer;
}

//Function to print errors in a specific style.
void printErr(char *err) {
    fprintf(stderr, "Failed to play file. %s.\n", err);
}

//Function to quickly print FFMpeg errors.
void printAVErr(int err) {
    if (err != 0) {
        //Get error.
        char *error = avErrToString(err);
        //Print.
        printErr(error);
        //Free memory.
        free(error);
    }
}