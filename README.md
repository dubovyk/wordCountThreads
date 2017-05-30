# WordCounter part 1

## Intro

This is a simple tool to count number of words in the given file. It ignores punctuation and upper/lower case, that makes it more precise on real texts. Also, it uses multi-threading a lot that makes it quite fast.

## Compilation

To build this application simply import it into you CLion IDE and build it or just use CMake to do the same thing.

## Usage

To run this application you should create conf.ini file in the folder with the executable. The structure of the conf.ini file is following (each entry with a new line, no extra symbols):
- input file name
- output file name (sorted alphabetically)
- output file name (sorted by the number of occurrences of the word)
- number of threads
- block size (number of lines processes by counter threads as a single piece of data)

Example of such file can be found as conf.ini here.

## License

Copyright 2017 Sergey Dubovyk & Bondar Nina

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.