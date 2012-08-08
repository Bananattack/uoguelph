/*

    CIS*3210 A2 Client
    Andrew Crowell
    0545826
    acrowell@uoguelph.ca

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define TIMEOUT 3000000
typedef int SocketHandle;

SocketHandle InitClient(char* url, int port)
{    
    SocketHandle s;
    struct sockaddr_in addr;
    struct hostent* server;
    
    struct timeval tv;
    
    struct timeval old_tv;
    unsigned int old_tv_len = sizeof(struct timeval);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    printf("Resolving url %s...\r\n", url);
    server = gethostbyname(url);
    if(server == NULL)
    {
        printf("Could not resolve the address of the server by name '%s'.\n", url);
        exit(-1);
    }
    
    memcpy((char*) &addr.sin_addr, server->h_addr, server->h_length);

    printf("Creating socket to server...\r\n");
    s = socket(AF_INET, SOCK_STREAM, 0);
    
    if(s == -1)
    {
        printf("Call to socket failed.\n");
        exit(-1);
    }
    
    printf("Connecting to %s on port %d...\r\n", url, port);
    
    getsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*) (&old_tv), &old_tv_len);
    
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*) (&tv), sizeof(struct timeval));
    
    while(connect(s, (struct sockaddr*) &addr, sizeof(addr)) != 0)
    {
        printf("Call to connect failed. (sleeping at least %f seconds)\n", (float)((float)TIMEOUT / 1000000.0f));
        usleep(TIMEOUT);
    }
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*) (&old_tv), sizeof(struct timeval));
    
    return s;
}

/*SocketHandle InitListener(char* url, int port)
{
    SocketHandle s;
    struct sockaddr_in addr;
    struct hostent* server;

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    
    server = gethostbyname(url);
    if(server == NULL)
    {
        printf("Could not resolve the address of the server by name '%s'.\n", url);
        exit(-1);
    }
    
    memcpy((char*) &addr.sin_addr, server->h_addr, server->h_length);


    printf("Creating socket from server...\r\n");
    s = socket(AF_INET, SOCK_STREAM, 0);
    
    if(s == -1)
    {
        printf("Call to socket failed.\n");
        exit(-1);
    }
    
    printf("Binding to port %d...\r\n", port);
    while(bind(s, (void *) &addr, sizeof(addr)) == -1)
    {
        printf("Call to bind failed. (sleeping at least %f seconds)\n", (float)((float)TIMEOUT / 1000000.0f));
        usleep(TIMEOUT);    
    }
    
    printf("Attempting to listen on port %d...\r\n", port);
    while(listen(s, 5) < 0)
    {
        printf("Could not listen. (timeout = 5 seconds)\n");
    }
    printf("Ready!\r\n", port);
    return s;
}*/

SocketHandle InitListener(url, port)
{
    short int PORT = 11708;

    struct sockaddr_in lsSA;
    SocketHandle ls;

    printf("Server will use port number %d\n" , port);
    ls = socket(PF_INET, SOCK_STREAM, 0) ; // 0 = TCP
    if( ls == -1 )
    {
        perror("Socket");
        exit( -1 ) ;
    }

    bzero(&lsSA , sizeof(lsSA));
    lsSA.sin_family = AF_INET;
    lsSA.sin_addr.s_addr = htonl(INADDR_ANY);
    lsSA.sin_port = htons(port);

    if(bind(ls, (void *) &lsSA, sizeof(lsSA)) == -1)
    {
        perror("Bind");
        exit(-1) ;
    }
    if(listen(ls, 5) < 0)
    {
        printf("Could not listen. (timeout = 5 seconds)\n");
        exit(-1);
    }
    return ls ;
}

typedef struct
{
    int len;
    int capacity;
    
    char* data;
} FileChunk;


FileChunk* FileChunkNew()
{
    FileChunk* self = malloc(sizeof(FileChunk));
    self->len = 0;
    self->capacity = 128;
    self->data = malloc(sizeof(char) * self->capacity);
}

void FileChunkFree(FileChunk* self)
{
    free(self->data);
    free(self);
}

void FileChunkAddByte(FileChunk* self, char byte)
{
    self->data[self->len] = byte;
    self->len++;
    if(self->len == self->capacity)
    {
        self->capacity <<= 1;
        self->data = realloc(self->data, sizeof(char) * self->capacity);
    }
}

void FileChunkAddChunk(FileChunk* self, char* data, int len)
{
    int i;
    for(i = 0; i < len; i++)
    {
        FileChunkAddByte(self, data[i]);
    }
}

typedef struct
{
    const char* filename;
    
    int num_connections;
    int connection_started;
    int connection_ended;
    
    SocketHandle listener;
    
    int* file_offset;
    int* bytes_read;
    FileChunk** chunk;
    SocketHandle* sock;
    
    int max_file_descriptor;
    fd_set fileset;
    
    struct timeval timeout;
} FileReceiver;

FileReceiver* FileReceiverNew(const char* filename, int num_connections, SocketHandle listener)
{
    int i;
    FileReceiver* self = malloc(sizeof(FileReceiver));
    
    self->filename = filename;
    
    self->num_connections = num_connections;
    self->connection_started = 0;
    self->connection_ended = 0;
    
    self->listener = listener;
    
    self->file_offset = malloc(sizeof(int) * self->num_connections);
    self->bytes_read = malloc(sizeof(int) * self->num_connections);
    self->chunk = malloc(sizeof(FileChunk*) * self->num_connections);
    self->sock = malloc(sizeof(SocketHandle) * self->num_connections);
    
    for(i = 0; i < self->num_connections; i++)
    {
        self->file_offset[i] = -1;
        self->bytes_read[i] = 0;
        self->sock[i] = -1;
        self->chunk[i] = FileChunkNew();
    }
    
    self->max_file_descriptor = listener;
    
    self->timeout.tv_sec = 10;
    self->timeout.tv_usec = 0;
    
    FD_ZERO(&(self->fileset));
    FD_SET(self->listener, &(self->fileset));
}

void FileReceiverFree(FileReceiver* self)
{
    free(self->file_offset);
    free(self->bytes_read);
    free(self->chunk);
    free(self->sock);
    free(self);
}

void FileReceiverAddSocket(FileReceiver* self, SocketHandle sock)
{
    int i;
    if(self->connection_started >= self->num_connections)
    {
        printf("TOO MANY CONNECTIONS GUYS\r\n");
        exit(-1);
    }
    
    for(i = 0; i < self->num_connections; i++)
    {
        if(self->sock[i] == -1)
        {
            self->sock[i] = sock;
            break;
        }
    }
    
    FD_SET(sock, &(self->fileset));
    self->max_file_descriptor = (sock > self->max_file_descriptor) ? sock : self->max_file_descriptor;
    
    self->connection_started++;
}

void FileReceiverDropSocket(FileReceiver* self, int index)
{
    FD_CLR(self->sock[index], &(self->fileset));
    self->sock[index] = 0;
    
    self->connection_ended++;
}

int FileReceiverUpdate(FileReceiver* self)
{
    SocketHandle newsock;
    int i, j;
    int bytes;
    fd_set fileset;
    char buf[1024];
    
    /* Copy the fileset into a temporary, so the master set is not accidentally modified. */
    fileset = self->fileset;
    
    /* Return when the timeout is exceeded. */
    if(select(self->max_file_descriptor + 1, &fileset, NULL, NULL, &(self->timeout)) == 0)
    {
        return 0;
    }
    
    for(i = 0; i <= self->max_file_descriptor; i++)
    {
        if(FD_ISSET(i, &fileset))
        {
            if(self->listener == i)
            {
                newsock = accept(self->listener, NULL, NULL);
                if (newsock == -1)
                {
                    perror("Failure to accept from listener.");
                }
                else
                {
                    printf("I accept you.\r\n");
                    FileReceiverAddSocket(self, newsock);
                }
                continue;
            }
            
            if ((bytes = recv(i, buf, sizeof(buf), 0)) <= 0)
            {
                /* Connection closed. */
                if(bytes == 0)
                {
                    printf("Socket %d closed\n", i);
                }
                /* Select messed up, causing a recv on a socket with no data! */
                else
                {
                    perror("Failed to recv from selected socket");   
                }
                close(i);
                
                /* Remove found socket from list. */
                for(j = 0; j < self->num_connections; j++)
                {
                    if(self->sock[j] == i)
                    {
                        FileReceiverDropSocket(self, j);
                    }
                }
            }
            else
            {
                /* Find the socket in our socket array that corresponds to the one in our set. */
                for(j = 0; j < self->num_connections; j++)
                {
                    /* Found it. */
                    if(self->sock[j] == i)
                    {
                        /* Add data to the chunk. */
                        printf("Read %d bytes from channel %d\r\n", bytes, j);
                        FileChunkAddChunk(self->chunk[j], buf, bytes);
                        self->bytes_read[j] += bytes;
                        printf("Total %d bytes in channel %d\r\n", self->bytes_read[j], j);
                    }
                }
            }
        }
    }
    return 1;
}

int main(int argc, char** argv)
{
    char* host;
    char* filename_to_request;
    unsigned char* req;
    char* filename_of_output;
    char* data;
    char* received;
    short server_port, listen_port;
    short num_connections;
    short temp;
    unsigned char temp_bytes[2];
    SocketHandle s;
    SocketHandle listener;
    FileReceiver* fr;
    int done = 0;
    int size = 0;
    int i;
    int fail = 0;
    int fail_start = -1;
    int fail_len = -1;
    FILE* f;
    
    if(argc < 7)
    {
        printf("Usage %s host server_port listen_port num_connections filename_to_request filename_of_output\n", argv[0]);
        exit(-1);
    }
    
    host = argv[1];
    server_port = atoi(argv[2]);
    listen_port = atoi(argv[3]);
    num_connections = atoi(argv[4]);
    filename_to_request = argv[5];
    filename_of_output = argv[6];
    
    size = (4 + strlen(filename_to_request) + 1);
    req = malloc(size);

    listener = InitListener("localhost", listen_port);
    
    
    printf("CLIENT NUM_CONN = %d PORT = %d\r\n", num_connections, listen_port);
    
    temp = htons(num_connections);
    req[1] = (temp & 0xFF00) >> 8;
    req[0] = (temp & 0x00FF);
    printf("CLIENT NUM_CONN HEX = 0x%X 0x%X\r\n", req[0], req[1]);
    
    send(s, &temp_bytes, 2, 0); 
    temp = htons(listen_port);
    req[3] = (temp & 0xFF00) >> 8;
    req[2] = (temp & 0x00FF);
    printf("CLIENT PORT HEX = 0x%X 0x%X\r\n", req[2], req[3]);

    printf("Filename %s\r\n", filename_to_request);
    strcpy((char*) (&req[4]), filename_to_request);    
    printf("REQUEST+4 '%s'\r\n", &(req[4]));
    
    s = InitClient(host, server_port);
    for(i =0; i < size; i++)
    {
        printf("0x%X ", req[i]);
    }
    printf("\n");
    send(s, req, size, 0);
    
    printf("Sent the stuff to the serrrrver.\r\n");
    
    fr = FileReceiverNew(filename_to_request, num_connections, listener);
    while(fr->connection_ended < num_connections && FileReceiverUpdate(fr))
    {
    }
    printf("Transmission ended: %d closed of %d expected\r\n", fr->connection_ended, num_connections);
    
    size = 0;
    for(i = 0; i < num_connections; i++)
    {
        /* First sizeof(int) bytes correspond to the file offset */
        if(fr->bytes_read[i] >= sizeof(int) && fr->file_offset[i] == -1)
        {
            fr->file_offset[i] = ntohl(*((int*) &(fr->chunk[i]->data[0])));
        }
    
        fr->bytes_read[i] -= sizeof(int);
        
        if(fr->bytes_read[i] >= 0)
        {            
            printf("%d: %d bytes of the file\r\n", i, fr->bytes_read[i]);
            /*size += fr->bytes_read[i];*/
        }
        else
        {
            printf("%d: closed prematurely\r\n", i);
        }
        
        fr->bytes_read[i] = fr->bytes_read[i] < 0 ? 0 : fr->bytes_read[i];
        size = (size < fr->file_offset[i] + fr->bytes_read[i]) ? fr->file_offset[i] + fr->bytes_read[i] : size;
        
    }
    printf("\r\n\r\nData buffer is %d bytes.\r\n\r\n", size);
    
    /* Nothing received. */
    if(size == 0)
    {
        printf("Server never sent the file in time.\r\nPlease try again, after checking the server is still operational.\r\n");
        
        close(s);
        exit(-1);        
    }
    
    /* Thing to fill */
    data = malloc(size);
    
    /* Checklist of stuff requring filling */
    received = malloc(size);
    memset(received, 0, size);
    
    for(i = 0; i < num_connections; i++)
    {
        if(fr->file_offset[i] == -1)
        {
            printf("%d: closed prematurely\r\n", i);
            continue;
        }
        printf("%d: file_offset = %d\r\n", i, fr->file_offset[i]);
        
        /* Copy the chunk. */
        memcpy(data + fr->file_offset[i], fr->chunk[i]->data + sizeof(int), fr->chunk[i]->len - sizeof(int));
        
        /* Check off those bytes as being received */
        memset(received + fr->file_offset[i], 1, fr->chunk[i]->len - sizeof(int));
    }
    
    /* Print any bytes that were detected as missing. */
    printf("\r\nChecking file integrity...\r\n");
    fail = 0;
    fail_start = -1;
    fail_len = -1;
    for(i = 0; i < size; i++)
    {
        if(!received[i])
        {
            if(!fail)
            {
                printf("Failed to receive the following byte offsets:\r\n");
            }
            if(fail_start == -1)
            {
                fail_start = i;
            }
            fail = 1;
            fail_len++;
        }
        else if(received[i] && fail_start >= 0)
        {
            printf("(%d .. %d) ", fail_start, fail_start + fail_len);
            fail_start = -1;
            fail_len = -1;
        }
    }
    if(fail_start >= 0)
    {
        printf("(%d .. %d) ", fail_start, fail_start + fail_len);
        fail_start = -1;
        fail_len = -1;
    }
    if(fail)
    {
        printf("\r\n\r\n");
        printf("Please try this request again.\r\n");
        
        close(s);
        exit(-1);
    }
    else
    {
        printf("Yes! No problems detected in file.\r\n(Inspect the file manually to be safe though.)\r\n\r\n");
    }
    
    printf("Saving...\r\n");
    /* Otherwise, no known failures. Write the data to disk. */
    f = fopen(filename_of_output, "wb");
    fwrite(data, size, 1, f);
    fclose(f);
    
    printf("Wrote results to %s\r\n", filename_of_output);
    
    close(s);

    return 0;
}

