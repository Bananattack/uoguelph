// Andrew Crowell
// 0545826
// acrowell@uoguelph.ca
//
// thumbpeg.h
//
// Common header for the thumbpeg program.
// Defines global PILOT data structures with external linkage.
// Provides a helper 'image' struct (public class).
// Gives forward definitions for the D[i] (thumnailer) and C (compressor) processes' execution sections.
#ifndef CIS4450_A1_H
#define CIS4450_A1_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <glob.h>

#include <string>
// Pilot and libjpeg are C libraries,
// include them properly.
extern "C"
{
    #include <jpeglib.h>
    #include "pilot.h"
}

// Worker processes that each deal with the decompression and thumbnailing of some jpeg source passed.
extern PI_PROCESS** thumbnailProcesses;
// A single worker process responsible for compressing thumbnails supplied.
extern PI_PROCESS* compressProcess;

// PI_MAIN -> thumbnailProcesses[i] to tell a worker to start working with their data.
// Send length, byte data.
// If length = 0, shut down that process.
extern PI_CHANNEL** thumbnailRequests;
// thumbnailProcesses[i] -> compressProcess to tell the compressor to compress a thumbnailed image.
extern PI_CHANNEL** compressRequests;
// thumbnailProcesses[i] -> PI_MAIN to ask for another stream to decompress.
extern PI_CHANNEL** readyRequests; 
// PI_MAIN -> compressProcess to give number of thumbnails expected to compress.
// When the compressor gets its remaining thumbnails to compress to 0, it can shut down.
extern PI_CHANNEL* sizeRequest;
// compressProcess -> PI_MAIN to ask to write a file.
extern PI_CHANNEL* writeRequest;

// A selector of compressRequests[i], used by the compressor to acknowledge first task supplied.
extern PI_BUNDLE* compressBundle;

// Helpful definition of a 'byte', might need redefinition as something more meaningful
// on certain architectures.
typedef unsigned char u8;

// An image datatype, which consists of its dimensions,
// and a contiguous byte stream of 24bpp RGB values.
// Reads and writes from PI_Channels for convenience
// when transferring from D[i] -> c.
struct Image
{
    int width;
    int height;
    u8* data;

    Image(int w, int h)
    {
        width = w;
        height = h;
        data = new u8[w * h * 3];
    }

    Image(PI_CHANNEL* chan)
    {
        PI_Read(chan, "%d", &width);
        PI_Read(chan, "%d", &height);

        data = new u8[width * height * 3];

        PI_Read(chan, "%*b", width * height * 3, data);
    }

    ~Image()
    {
        delete[] data;
    }

    void writeToChannel(PI_CHANNEL* chan)
    {
        PI_Write(chan, "%d", width);
        PI_Write(chan, "%d", height);
        PI_Write(chan, "%*b", width * height * 3, data);
    }
};

int thumbnail(int q, void* data);
int compress(int q, void* data);

#endif