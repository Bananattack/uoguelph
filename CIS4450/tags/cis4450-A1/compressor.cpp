// Andrew Crowell
// 0545826
// acrowell@uoguelph.ca
//
// thumbnailer.cpp
//
// C (compressor) process.
// Receives a thumbnails from D[i] (thumbnailer) processes
// It compresses these images using libjpeg.
//
// When finished, it sends the compressed blob to PI_Main
// and asks to write that to disk.
// It is told by PI_Main exactly how many files to expect.
// When it runs out of images, it shuts down.
#include "thumbpeg.h"

static void compressImage(Image* image, size_t* size, char** blob)
{
    // Open a memory blob for writing.
    // NOTE: This is not doing disk IO, so this file is merely virtual and does not violate assignment rules.
    FILE* f = open_memstream(blob, size);

    // Setup the compressor.
    jpeg_compress_struct cinfo;
    jpeg_error_mgr err;

    jpeg_create_compress(&cinfo);
    cinfo.err = jpeg_std_error(&err);
    jpeg_stdio_dest(&cinfo, f);

    cinfo.image_width = image->width;
    cinfo.image_height = image->height;
    cinfo.input_components = 3;
    cinfo.in_color_space = JCS_RGB;
    jpeg_set_defaults(&cinfo);

    jpeg_start_compress(&cinfo, TRUE);
    JSAMPROW row_pointer[1];
    int rowStride = image->width * 3;
    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = &(image->data)[cinfo.next_scanline * rowStride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }
    jpeg_finish_compress(&cinfo);
    jpeg_destroy_compress(&cinfo);
    fclose(f);
}

int compress(int ignore, void* data)
{
    int i;
    
    // Get the number of files expected to compress.
    int numberLeft = 0;
    PI_Read(sizeRequest, "%d", &numberLeft);
    printf("COMPRESSOR: Was told there are %d images by MAIN.\r\n", numberLeft);

    while(numberLeft > 0)
    {
        // Wait until the latest thumbnailer to send its data.
        int channelIndex = PI_TrySelect(compressBundle);
        if(channelIndex == -1)
        {
            // Sleep until data is ready
            usleep(1000);
            continue;
        }
        
        int filenameLength = 0;
        PI_Read(compressRequests[channelIndex], "%d", &filenameLength);
        // Get the name of the file
        char* filename = new char[filenameLength + 1];
        PI_Read(compressRequests[channelIndex], "%*c", filenameLength, filename);
        filename[filenameLength] = 0;
        // Get the image that needs compressing.
        Image* img = new Image(compressRequests[channelIndex]);
        
        // Compress data into memory.
        size_t size;
        char* blob;
        printf("COMPRESSOR: Compressing %s by THUMBNAILER %d's request...\r\n", filename, channelIndex);
        compressImage(img, &size, &blob);
        delete img;
        
        // Send the name of the file to save.
        PI_Write(writeRequest, "%d", filenameLength);
        PI_Write(writeRequest, "%*c", filenameLength, filename);
        // Send the compressed file.
        PI_Write(writeRequest, "%d", size);
        PI_Write(writeRequest, "%*b", size, blob);
        free(blob);
        numberLeft--;
        printf("COMPRESSOR: Finished compressing the thumbnail of %s, waiting on %d more.\r\n", filename, numberLeft);
        delete filename;
    }
    printf("COMPRESSOR: Exiting.\r\n");
    return 0;
}