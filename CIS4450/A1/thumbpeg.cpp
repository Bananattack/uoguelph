// Andrew Crowell
// 0545826
// acrowell@uoguelph.ca
//
// thumbpeg.cpp
//
// Performs the setup for PILOT and
// other various setup and maintenance chores.
// main() is The PI_MAIN process for thumbpeg.
// Program expects at least 3 processes to run.
// PI_MAIN performs the disk IO tasks required
// the assignment and manages the D[i] (thumbnailer)
// and C (compressor) workers.
#include "thumbpeg.h"
#include <sys/stat.h>

// Worker processes that each deal with the decompression and thumbnailing of some jpeg source passed.
PI_PROCESS** thumbnailProcesses;
// A single worker process responsible for compressing thumbnails supplied.
PI_PROCESS* compressProcess;

// PI_MAIN -> thumbnailProcesses[i] to tell a worker to start working with their data.
// Send length, byte data.
// If length = 0, shut down that process.
PI_CHANNEL** thumbnailRequests;
// thumbnailProcesses[i] -> compressProcess to tell the compressor to compress a thumbnailed image.
PI_CHANNEL** compressRequests; 
// thumbnailProcesses[i] -> PI_MAIN to ask for another stream to decompress.
PI_CHANNEL** readyRequests; 
// PI_MAIN -> compressProcess to give number of thumbnails expected to compress.
// When the compressor gets its remaining thumbnails to compress to 0, it can shut down.
PI_CHANNEL* sizeRequest;
// compressProcess -> PI_MAIN to ask to write a file.
PI_CHANNEL* writeRequest;

// A selector of compressRequests[i], used by the compressor to acknowledge first task supplied.
PI_BUNDLE* compressBundle;

static std::string getFileNameSansDirectory(const std::string& s)
{
    unsigned int p = s.find_last_of("/");

    if (p == std::string::npos)
    {
        return s;
    }
    else
    {
        return s.substr(p + 1);
    }
}

std::string replaceExtension(const std::string& s, const std::string& extension)
{
    unsigned int pos = s.rfind('.');
    if (pos == std::string::npos)
    {
        return s + extension;
    }
    else
    {
        return s.substr(0, pos) + extension;
    }
}

int main(int argc, char** argv)
{
    int N = PI_Configure(&argc, &argv);
    int THUMBNAIL_PROCESS_COUNT = N - 2;
    if(N < 3)
    {
        PI_Abort(0, "\r\n\r\n\r\nError: There are an insufficient number of processes to run this (need at least 3).\r\n\r\n\r\n", __FILE__,  __LINE__);
    }
    if(argc != 3)
    {
        PI_Abort(0, "\r\n\r\n\r\nUSAGE: sourceDirectory destDirectory\r\n\r\n\r\n", __FILE__,  __LINE__);
    }

    // Get directories.
    std::string sourceDir = argv[1];
    std::string destDir = argv[2];
    struct stat st;
    if(stat(sourceDir.c_str(), &st) == -1)
    {
        std::string err = "\r\n\r\n\r\nERROR: sourceDirectory '" + sourceDir + "' provided does not exist.\r\n\r\n\r\n";
        PI_Abort(0, err.c_str(), __FILE__,  __LINE__);
    }
    if(!S_ISDIR(st.st_mode))
    {
        std::string err = "\r\n\r\n\r\nERROR: sourceDirectory '" + sourceDir + "' is not a directory.\r\n\r\n\r\n";
        PI_Abort(0, err.c_str(), __FILE__,  __LINE__);
    }
    if(stat(destDir.c_str(), &st) == -1)
    {
        std::string err = "\r\n\r\n\r\nERROR: destDirectory '" + destDir + "' provided does not exist.\r\n\r\n\r\n";
        PI_Abort(0, err.c_str(), __FILE__,  __LINE__);
    }
    if(!S_ISDIR(st.st_mode))
    {
        std::string err = "\r\n\r\n\r\nERROR: destDirectory '" + destDir + "' is not a directory.\r\n\r\n\r\n";
        PI_Abort(0, err.c_str(), __FILE__,  __LINE__);
    }
   
    thumbnailProcesses = new PI_PROCESS*[THUMBNAIL_PROCESS_COUNT];
    thumbnailRequests = new PI_CHANNEL*[THUMBNAIL_PROCESS_COUNT];
    compressRequests = new PI_CHANNEL*[THUMBNAIL_PROCESS_COUNT];
    readyRequests = new PI_CHANNEL*[THUMBNAIL_PROCESS_COUNT];
    
    // A single worker process responsible for compressing thumbnails supplied.
    compressProcess = PI_CreateProcess(compress, 0, NULL);
    PI_SetName(compressProcess, "compressProcess");
    
    char numbuffer[16];
    for(int i = 0; i < THUMBNAIL_PROCESS_COUNT; i++)
    {
        sprintf(numbuffer, "%d", i);
        
        // Worker processes that each deal with the decompression and thumbnailing of some jpeg source passed.
        thumbnailProcesses[i] = PI_CreateProcess(thumbnail, i, NULL);
        PI_SetName(thumbnailProcesses[i], std::string("thumbnailProcesses[" + std::string(numbuffer) + "]").c_str());
        
        // Requests to/from a thumbnailer.
        thumbnailRequests[i] = PI_CreateChannel(PI_MAIN, thumbnailProcesses[i]);
        PI_SetName(thumbnailRequests[i], std::string("thumbnailRequests[" + std::string(numbuffer) + "](M->Di)").c_str());
        
        compressRequests[i] = PI_CreateChannel(thumbnailProcesses[i], compressProcess);
        PI_SetName(compressRequests[i], std::string("compressRequests[" + std::string(numbuffer) + "](Di->C)").c_str());
        
        readyRequests[i] = PI_CreateChannel(thumbnailProcesses[i], PI_MAIN);
        PI_SetName(readyRequests[i], std::string("readyRequests[" + std::string(numbuffer) + "](Di->M)").c_str());
    }
    // Requests to/from the compressor.
    sizeRequest = PI_CreateChannel(PI_MAIN, compressProcess);
    PI_SetName(sizeRequest, "sizeRequest(M->C)");
    
    writeRequest = PI_CreateChannel(compressProcess, PI_MAIN);
    PI_SetName(writeRequest, "compressProcess(C->M)");
    
    // Bundle all the compress requests, so the compressor can take the first request from a thumbnailer.
    compressBundle = PI_CreateBundle(PI_SELECT, compressRequests, THUMBNAIL_PROCESS_COUNT);
    PI_SetName(writeRequest, "compressBundle(Any Di->C)");
    

    // Start the processes!
    PI_StartAll();
    // Start timer.
    PI_StartTime();
    
    printf("THUMBNAIL_PROCESS_COUNT = %d\r\n", THUMBNAIL_PROCESS_COUNT);
    
    // Get the directory listing.
    // Send the count of all images to the compressor process.
    glob_t listing;
    
    int result = glob(std::string(sourceDir + "/*.jp*g").c_str(), 0, 0, &listing);
    if(result)
    {
        switch(result)
        {
            case GLOB_NOMATCH:
                PI_Abort(0, "\r\n\r\n\r\nNo jp*g files found in this folder. Shutting down.\r\n\r\n\r\n", __FILE__,  __LINE__);
                break;
            default:
                PI_Abort(0, "\r\n\r\n\r\nError: Failed to retreive directory listing.\r\n\r\n\r\n", __FILE__,  __LINE__);
                break;
        }
        return 0;
    }
    
    // For tracking available thumbnailers.
    bool thumbnailerBusy[THUMBNAIL_PROCESS_COUNT];
    // Everything starts available.
    memset(thumbnailerBusy, 0, sizeof(thumbnailerBusy));
    
    // Tell the compressor how many images it should expect to see.
    int fileCount = listing.gl_pathc;
    int numberLeft = fileCount;
    PI_Write(sizeRequest, "%d", numberLeft);
    printf("MAIN: Told the COMPRESSOR there are %d jpegs to read.\r\n", fileCount);
    
    // The progres within the directory listing for jpeg files.
    int listingIndex = 0;
    
    // while there are files left, keep looping.
    while(numberLeft > 0)
    {
        // Assign remaining files to available threads.
        for(int i = 0; i < THUMBNAIL_PROCESS_COUNT; i++)
        {
            // If thumbnailer says it's ready, so it is no longer busy.
            if(PI_ChannelHasData(readyRequests[i]))
            {
                int response;
                PI_Read(readyRequests[i], "%d", &response);
                printf("MAIN: Realized THUMBNAILER %d is not busy anymore.\r\n", i);
                
                thumbnailerBusy[i] = false;
            }
            // Non-busy thumbnailers can be assigned new tasks, if there are any left.
            if(!thumbnailerBusy[i] && listingIndex < fileCount)
            {
                char* filename = listing.gl_pathv[listingIndex];
                int filenameLength = strlen(filename);
                printf("MAIN: Sending off '%s' to THUMBNAILER %d.\r\n", filename, i);
                // Open the file.
                FILE* f = fopen(filename, "rb");
                if(!f)
                {
                    std::string err = "\r\n\r\n\r\nFailed to open file '" + std::string(filename) + "' for reading!\r\n\r\n\r\n";
                    PI_Abort(0, err.c_str(), __FILE__,  __LINE__);
                }
               
                // Calculate length of file in bytes.
                int len = 0;
                fseek(f, 0, SEEK_END);
                len = ftell(f);
                fseek(f, 0, SEEK_SET);
                
                // Allocate buffer.
                char* data = new char[len];
                fread(data, len, 1, f);
                
                // Close the file.
                fclose(f);
                PI_Write(thumbnailRequests[i], "%d", filenameLength);
                PI_Write(thumbnailRequests[i], "%*c", filenameLength, filename);
                PI_Write(thumbnailRequests[i], "%d", len);
                PI_Write(thumbnailRequests[i], "%*b", len, data);
               
                // Sent the bytes, now free the buffer.
                delete[] data;
                
                // Thumbnailer is now occupied.
                thumbnailerBusy[i] = true;
                
                // Advance to next file in listing.
                listingIndex++;
            }
        }
        
        // If the compressor sends back a compressed thumbnail,
        // then write that to a file.
        if(PI_ChannelHasData(writeRequest))
        {
            int filenameLength = 0;
            PI_Read(writeRequest, "%d", &filenameLength);
            
            char* filename = new char[filenameLength + 1];
            PI_Read(writeRequest, "%*c", filenameLength, filename);
            filename[filenameLength] = 0;

            std::string pathToWrite = destDir + "/" + replaceExtension(getFileNameSansDirectory(filename), "-tb.jpg");
            
            int len;
            PI_Read(writeRequest, "%d", &len);
            
            char* bytes = new char[len];
            PI_Read(writeRequest, "%*b", len, bytes);
            
            printf("MAIN: Writing thumbnail of source %s to destination %s...\r\n", filename, pathToWrite.c_str());
            
            FILE* f = fopen(pathToWrite.c_str(), "wb");
            if(!f)
            {
                std::string err = "\r\n\r\n\r\nFailed to open file '" + std::string(filename) + "' for writing!\r\n\r\n\r\n";
                PI_Abort(0, err.c_str(), __FILE__,  __LINE__);
            }
            
            // Save to file.
            fwrite(bytes, 1, len, f);
            // Close
            fclose(f);
            
            // One less item left to write.
            delete[] filename;
            delete[] bytes;
            numberLeft--;
            printf("MAIN: %d files left to write.\r\n", numberLeft);
        }
        // Since the main process just polls, and doesn't actually block,
        // it should sleep to avoid busy waiting.
        usleep(1000);
    }
    // Finally, ensure all thumbnailers are informed of completion.
    for(int i = 0; i < THUMBNAIL_PROCESS_COUNT; i++)
    {
        PI_Write(thumbnailRequests[i], "%d", 0);
    }
    // Free up the directory.
    globfree(&listing);
    
    // Stop timer.
    double executionTime = PI_EndTime();
    printf("MAIN: Exiting. Execution took %lf seconds\r\n", executionTime);
    // Stop PILOT.
    PI_StopMain(0);

    delete[] thumbnailProcesses;
    delete[] thumbnailRequests;
    delete[] compressRequests;
    delete[] readyRequests;

    return 0;
}