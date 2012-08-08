/*

    CIS*3210
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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#define TIMEOUT 3000
#define BUFFER_SIZE 16
typedef int SocketHandle;

SocketHandle OpenHTTP(char* url)
{
	SocketHandle s;
	struct sockaddr_in addr;
	struct hostent* server;

    memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
    
	server = gethostbyname(url);
    if(server == NULL)
    {
		printf("Could not resolve the address of the server by name '%s'.\n", url);
		exit(-1);
    }
    
    memcpy((char*) &addr.sin_addr, server->h_addr, server->h_length);

    s = socket(AF_INET, SOCK_STREAM, 0);
	if(s == -1)
    {
		printf("Call to socket failed.\n");
		exit(-1);
	}
	while(connect(s, (struct sockaddr*) &addr, sizeof(addr)) != 0)
    {
		printf("Call to connect failed. (sleeping at least %d ms)\n", TIMEOUT);
		usleep(TIMEOUT);
		/* exit(-1); */
	}
	return s;
}


typedef enum
{
    Header,
    Body,
    Footer
} ParserMode;

char* ParserModeToString(ParserMode mode)
{
    switch(mode)
    {
        case Header: return "Header";
        case Body: return "Body";
        case Footer: return "Footer";
        default: return "Unknown";
    }
}

int FindTextSequence(const char* expectedMessage, int length, int* sequenceOffset, char c)
{
    if(c == expectedMessage[*sequenceOffset])
    {
        (*sequenceOffset)++;
        return (*sequenceOffset >= length);
    }
    else
    {
        *sequenceOffset = 0;
        return 0;
    }
}


typedef struct
{
    char* data;
    int capacity;
    int len;
} String;

#define StringGetData(s) (s)->data

String* StringNew()
{
    String* s = malloc(sizeof(String));
    s->len = 0;
    s->capacity = 64;
    s->data = malloc(sizeof(char) * s->capacity);
    s->data[s->len] = '\0';
    return s;
}

void StringFree(String* s)
{
    free(s);
}

void StringAddChar(String* s, char c)
{
    if(s->len == s->capacity)
    {
        s->capacity <<= 1;
        s->data = realloc(s->data, sizeof(char) * s->capacity);
    }
    s->len++;
    s->data[s->len - 1] = c;
    s->data[s->len] = '\0';
}

void StringAddString(String* s, const char* text)
{
    int i;
    for(i = 0; i < strlen(text); i++)
    {
        StringAddChar(s, text[i]);
    }
}

void StringWipe(String* s)
{
    s->len = 0;
    memset(s->data, 0, sizeof(char) * s->capacity);
}

int StringEquals(String* lhs, const char* rhs)
{
    return strcmp(lhs->data, rhs) == 0;
}

int StringContains(String* lhs, const char* rhs)
{
    return strstr(lhs->data, rhs) != 0;
}


void ParseHTTP(SocketHandle sock, char* buffer, int* outBytes)
{
    ParserMode mode = Header;

    int i;
    int sequenceOffset = 0;
    char temp[BUFFER_SIZE];
    int bytesReceived = *outBytes;
    String* s = StringNew();
    
    do
    {
        for(i = 0; i < bytesReceived; i++)
        {
            switch(mode)
            {
                case Header:
                    StringAddChar(s, buffer[i]);
                    
                    if(FindTextSequence("\r\n\r\n", 4, &sequenceOffset, buffer[i]))
                    {
                        sequenceOffset = 0;
                        if(!StringContains(s, "HTTP/1.1"))
                        {
                            StringWipe(s);
                        }
                        else
                        {
                            printf("%s", StringGetData(s));
                            
                            StringFree(s);
                            mode = Body;
                            fputs("..........\r\n", stdout);
                        }
                    }
                    break;
                case Body:
                    if(FindTextSequence("</body>", 7, &sequenceOffset, buffer[i]))
                    {
                        mode = Footer;
                        sequenceOffset = 0;
                        fputs("</body>", stdout);
                    }
                    break;
                case Footer:
                    putchar(buffer[i]);
                    /* End of footer, we're done, return! */
                    if(FindTextSequence("\r\n\r\n", 4, &sequenceOffset, buffer[i]))
                    {
                        sequenceOffset = 0;

                        /*
                            We might have read the next chunk already.
                            Copy the substring we found into a temp buffer
                            and then copy that over the result buffer.
                            
                            \r\n\r\n = 4 characters
                        */
                        memcpy(temp, buffer, bytesReceived);
                        memcpy(buffer, temp + i, bytesReceived - i);
                        *outBytes = bytesReceived - i;
                        
                        fputs("..........\r\n", stdout);
                        return;
                    }
                    break;
                default:
                    break;
            }
        }
        bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        if(bytesReceived == 0)
        {
            fprintf(stderr, "\n\n*** Page ended prematurely, during the %s scan phase", ParserModeToString(mode));
            exit(-1);
        }
    } while(1);
}

int main()
{
	SocketHandle s;
	char req[] = "GET /news/ HTTP/1.1\r\n" \
                "Host: www.cbc.ca\r\n" \
                "Content-Length: 0\r\n" \
                "Connection: Keep-Alive\r\n" \
                "Keep-Alive: timeout=15\r\n";
	char req2[] = "GET /money/ HTTP/1.1\r\n" \
                "Host: www.cbc.ca\r\n" \
                "Content-Length: 0\r\n" \
                "Connection: Close\r\n" \
                "Keep-Alive: timeout=15\r\n";
    char end_req[] = "\r\n";
    char result[BUFFER_SIZE];
    int bytes = 0;
    
    int size = sizeof(req) + sizeof(end_req) + sizeof(req2) + sizeof(end_req);
    int len = 0;
    char* requests = calloc(size, sizeof(char));
    strcat(requests, req);
    strcat(requests, end_req);
    strcat(requests, req2);
    strcat(requests, end_req);
    
    printf("Connecting...\n");
    printf("----------\n");
                
    /* Creates a socket and opens a connection to the HTTP server specified */
	s = OpenHTTP("www.cbc.ca"); 
    
    printf("Sending request packet: '%s'\n", requests);
    printf("----------\n\n");
    
    /* Send requests */
	send(s, requests, size, 0);

    printf("Getting responses:\n");
    printf("----------\n\n");
    
    /* Get responses. */
    memset(result, 0, BUFFER_SIZE);
    ParseHTTP(s, result, &bytes);
    ParseHTTP(s, result, &bytes);
    
	close(s);
    free(requests);

	return 0;
}


