// Andrew Crowell
// 0545826
// acrowell@uoguelph.ca
//
// thumbnailer.cpp
//
// D[i] (thumbnailer) process.
// Receives a data blob from PI_Main,
// which it decompresses using libjpeg.
// It then crops and scales the image,
// resulting in a thumbnail that is
// 1/9th the size of the original.
//
// When finished, it sends the thumbnail
// to the C (compressor) process and
// notifies PI_Main that it is ready for more.
#include "thumbpeg.h"

// Reads an image
static Image* readImage(int length, u8* blob)
{
    // Open a memory blob for reading.
    // NOTE: This is not doing disk IO, so this file is merely virtual and does not violate assignment rules.
    FILE* f = fmemopen(blob, length, "rb");

    // Setup the decompressor.
    jpeg_decompress_struct cinfo;
    jpeg_error_mgr err;

    jpeg_create_decompress(&cinfo);
    cinfo.err = jpeg_std_error(&err);
    jpeg_stdio_src(&cinfo, f);

    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    // Make a temporary sample array to store rows while decompressing scanlines.
    int rowSize = cinfo.output_width * cinfo.output_components;
    JSAMPARRAY buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr)&cinfo, JPOOL_IMAGE, rowSize, 1);

    Image* image = new Image(cinfo.output_width, cinfo.output_height);
    u8* b = image->data;

    while (cinfo.output_scanline < cinfo.output_height)
    {
        int rowCount = jpeg_read_scanlines(&cinfo, buffer, 1);
        if (rowCount == 0)
        {
            break;
        }
        // Greyscale image - same for all channels.
        if (cinfo.output_components == 1)
        {
            u8* in = (u8*)(*buffer);
            for (int i = 0; i < image->width * rowCount; i++)
            {
                *b++ = *in;
                *b++ = *in;
                *b++ = *in;
                in++;
            }
        }
        // RGB image - 3 channels.
        else if (cinfo.output_components == 3)
        {
            memcpy(b, (*buffer), image->width * rowCount * 3);
            b += image->width * rowCount * 3;
        }
    }
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(f);

    return image;
}

// Crops an image to the center 1/3
static Image* cropImage(Image* src)
{
    Image* dest = new Image(src->width / 3, src->height / 3);
    int x = src->width * 1 / 3;
    int y = src->height * 1 / 3;
    for(int j = 0; j < dest->height; j++)
    {
        for(int i = 0; i < dest->width; i++)
        {
            // Copy the RGB of each pixel in the subrect.
            dest->data[(j * dest->width + i) * 3] = src->data[((y + j) * src->width + (x + i)) * 3];
            dest->data[(j * dest->width + i) * 3 + 1] = src->data[((y + j) * src->width + (x + i)) * 3 + 1];
            dest->data[(j * dest->width + i) * 3 + 2] = src->data[((y + j) * src->width + (x + i)) * 3 + 2];
        }
    }
    return dest;
}

// Scale an image to 1/3 its size
static Image* scaleImage(Image* src)
{
    Image* dest = new Image(src->width / 3, src->height / 3);
    for(int j = 0; j < dest->height; j++)
    {
        for(int i = 0; i < dest->width; i++)
        {
            // Copy the RGB of each third pixel in the image.
            dest->data[(j * dest->width + i) * 3] = src->data[((j * 3) * src->width + (i * 3)) * 3];
            dest->data[(j * dest->width + i) * 3 + 1] = src->data[((j * 3) * src->width + (i * 3)) * 3 + 1];
            dest->data[(j * dest->width + i) * 3 + 2] = src->data[((j * 3) * src->width + (i * 3)) * 3 + 2];
        }
    }
    return dest;
}

// Read an image from memory, and crop and scale it to create a thumbnail.
static Image* thumbnailImage(int len, u8* blob)
{
    Image* a = readImage(len, blob);
    Image* b = cropImage(a);
    delete a;
    Image* c = scaleImage(b);
    delete b;
    return c;
}

int thumbnail(int q, void* data)
{
    while(1)
    {
        if(!PI_ChannelHasData(thumbnailRequests[q]))
        {
            // Sleep until data is ready.
            usleep(1000);
            continue;
        }
        
        int filenameLength = 0;
        PI_Read(thumbnailRequests[q], "%d", &filenameLength);
        // No length means the main process ran out of files. Shutdown.
        if(filenameLength == 0)
        {
            printf("THUMBNAILER %d: Acknowledging shutdown request from MAIN.\r\n", q);
            return 0;
        }
        printf("THUMBNAILER %d: Acknowledging new work request from MAIN.\r\n", q);
        // Get the name of the file
        char* filename = new char[filenameLength + 1];
        PI_Read(thumbnailRequests[q], "%*c", filenameLength, filename);
        filename[filenameLength] = 0;
        
        int len = 0;
        PI_Read(thumbnailRequests[q], "%d", &len);
        // Read the binary blob.
        char* blob = new char[len];
        PI_Read(thumbnailRequests[q], "%*c", len, blob);
           
        // Decompress data.
        printf("THUMBNAILER %d: Thumbnailing %s...\r\n", q, filename);
        Image* image = thumbnailImage(len, (u8*) blob);
        
        // Get rid of the file memory blob, we don't need it anymore.
        delete[] blob;
        
        // Tell the compressor this thumbnailer has data to compress.
        printf("THUMBNAILER %d: Asking COMPRESSOR to compress the image...\r\n", q);
        PI_Write(compressRequests[q], "%d", filenameLength);
        PI_Write(compressRequests[q], "%*c", filenameLength, filename);

        // Pass the image data along.
        image->writeToChannel(compressRequests[q]);
        // Get rid of the thumbnail, we don't need it anymore.
        delete image;

        printf("THUMBNAILER %d: Done with file %s.\r\n", q, filename);
        // Get rid of the filename, we don't need it anymore.
        delete[] filename;
        
        // Tell main this thumbnailer is ready for more.
        // The number is rather irrelevant, but main needs a message.
        printf("THUMBNAILER %d: Asking MAIN for new work...\r\n", q);
        PI_Write(readyRequests[q], "%d", 1);
    }
}