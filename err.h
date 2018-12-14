/**
 * @file Headers for err.c, used to output errors.
 *
 * Copyright (c) Jack MacDougall 2018
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef SIMPLE_AUDIO_C99_ERR_H
#define SIMPLE_AUDIO_C99_ERR_H

/**
 * Buffer size for avErrToString.
 */
#define ERR_AVERRTOSTRING_BUFFER 32

/**
 * Convert FFMpeg errors into strings.
 * @param err AVError.
 * @return Human readable error.
 */
char *avErrToString(int);

/**
 * Print errors in this program's style.
 * @param err String of error to print.
 */
void printErr(char *);

/**
 * Quickly print AVErrors.
 * @param err AVError.
 */
void printAVErr(int);

#endif //SIMPLE_AUDIO_C99_ERR_H