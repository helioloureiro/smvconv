/*
 * Copyright (c) 2007  Gregor Richards
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void printInt(int v)
{
    printf("%c", v % 256);
    v >>= 8;
    printf("%c", v % 256);
    v >>= 8;
    printf("%c", v % 256);
}

int main(int argc, char **argv)
{
    int w, h, fr, fc, fpj, js, c;
    FILE *fhnd;
    char buf[10240];
    struct stat sbuf;

    if (argc != 8) {
        fprintf(stderr,
"Use: smvcombine <width> <height> <framerate> <# frames> <frames per jpeg>\n"
"                <jpeg size> <.wav file>\n"
"Input the list of jpeg files.\n");
        return 1;
    }

    w = atoi(argv[1]);
    h = atoi(argv[2]);
    fr = atoi(argv[3]);
    fc = atoi(argv[4]);
    fpj = atoi(argv[5]);
    js = atoi(argv[6]);

    // first just cat in the .wav file
    fhnd = fopen(argv[7], "r");
    if (fhnd == NULL) {
        perror(argv[7]);
        return 1;
    }
    while ((c = fgetc(fhnd)) != EOF) {
        putchar(c);
    }
    fclose(fhnd);

    // basic header
    fwrite("SMV002000", 1, 9, stdout);

    // width
    printInt(h);

    // height
    printInt(w);

    // unidentified
    fwrite("\x10\x00\x00\x01\x00\x00", 1, 6, stdout);

    // jpeg size
    printInt(js+3);

    // frame rate
    printInt(fr);

    // # frames
    printInt(fc);
    
    // unidentified
    fwrite("\x01\x00\x00\x00\x00\x00", 1, 6, stdout);
    
    // frames per jpeg
    printInt(fpj);
    
    // unidentified
    fwrite("\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01", 1, 12, stdout);

    // the jpeg files
    while (fgets(buf, 10240, stdin) != NULL) {
        c = strlen(buf) - 1;

        // get rid of cruft
        while (c > 0 &&
               (buf[c] == '\n' ||
                buf[c] == '\r')) {
            buf[c] = '\0';
            c--;
        }

        // get the file size
        if (stat(buf, &sbuf) == -1) {
            perror(buf);
            return 1;
        }

        // write it out
        printInt(sbuf.st_size);
        fhnd = fopen(buf, "r");
        if (fhnd == NULL) {
            perror(buf);
            return 1;
        }
        while ((c = fgetc(fhnd)) != EOF) {
            putchar(c);
        }
        fclose(fhnd);

        // and write out the buffer
        for (c = sbuf.st_size; c < js; c++) {
            putchar(0);
        }
    }

    return 0;
}
