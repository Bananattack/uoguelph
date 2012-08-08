#include "client.h"

Client::Client()
{
    R = L = F = socketDescriptor = 0;
}

Client::~Client()
{
    close();
}

void Client::close()
{   
    if(socketDescriptor)
    {
        ::close(socketDescriptor);
    }
}

bool Client::receiveData()
{
    fd_set recvSet;
    FD_ZERO(&recvSet);
    FD_SET(socketDescriptor, &recvSet);
    
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT;
    timeout.tv_usec = 0;
    
    int ret = select(socketDescriptor + 1, &recvSet, NULL, NULL, &timeout);
    switch(ret)
    {
        case -1:
            perror("Select failed");
            close();
            exit(-1);
            break;
        case 0:
            return false;
    }

    char buffer[MAX_DATAGRAM_SIZE];
    int bufferSize;
    
    bufferSize = recvfrom(socketDescriptor, buffer, MAX_DATAGRAM_SIZE, 0, (struct sockaddr*) &addr, &addrLength);
    
    std::cout << "Received part in " << bufferSize << " bytes. " << std::endl;
        
    FilePart f;
    f.data = std::string(buffer, buffer + bufferSize);
    
    parts.push_back(f);
    
    return true;
}

void Client::requestParts(const std::vector<ushort>& partNumbers)
{   
    // Nothing to do. No point in bugging the server
    // with an empty request list.
    if(partNumbers.size() == 0) return;

    int bytes;
    int len = partNumbers.size() * 2 + 2;
    char* req = new char[len];
    
    std::cout << "Constructing a retransmit request with " << partNumbers.size() << " entries: " << std::endl;    
    
    // Pack the list length in network order.
    short temp = htons(partNumbers.size());
    req[1] = (temp & 0xFF00) >> 8;
    req[0] = (temp & 0x00FF);
    
    for(int i = 0; i < partNumbers.size(); i++)
    {
        // Pack the request item in network order.
        temp = htons(partNumbers[i]);
        req[(i + 1) * 2 + 1] = (temp & 0xFF00) >> 8;
        req[(i + 1) * 2] = (temp & 0x00FF);
        
        
        if(i > 0)
        {
            std::cout << ", ";
        }
        std::cout << partNumbers[i];
    }
    
    // Send the retransmit request.
    bytes = sendto(socketDescriptor, req, len, 0, (struct sockaddr*) &addr, addrLength);
    
    std::cout << std::endl << "Sent the retransmit request in " << bytes << " bytes." << std::endl;

    delete req;
}

void Client::requestFile(const std::string& requestedFile, const std::string& outputFile)
{
    std::vector<ushort> missing;
    bool askForRLF = true;
    int bytes;
    size_t missingCount;
    
    parts.clear();
    
    // Send the file request.
    bytes = sendto(socketDescriptor, requestedFile.c_str(), requestedFile.length() + 1, 0, (struct sockaddr*) &addr, addrLength);
    
    std::cout << "*** Sent a file request of " << bytes << " bytes: '" << requestedFile << "'" << std::endl;
    
    // Repeatedly receive until no parts are missing.
    do
    {        
        missingCount = missing.size();
        missing.clear();
        std::cout << "*** Receiving data for " << TIMEOUT << " seconds." << std::endl;
        bool receiving = true;
        while(receiving)
        {
            receiving = receiveData();
            if(missingCount)
            {
                missingCount--;
                if(missingCount == 0)
                {
                    receiving = false;
                }
            }
        }
        std::cout << "*** Sorting parts..." << std::endl;
        // Sort the parts by descending length.
        std::sort(parts.begin(), parts.end(), std::greater<FilePart>());
        // Remove any duplicates (which can happen due to boundary retransmit).
        parts.erase(std::unique(parts.begin(), parts.end()), parts.end());
        
        std::cout << "*** Searching for missing parts..." << std::endl;
        
        // Find missing parts, by checking the parts with lengths between F .. L.
        if(parts.size() > 1)
        {
            for(int i = 1; i < parts.size() - 1; i++)
            {   
                // If there is a gap in lengths, there are missing file parts.
                if(parts[i - 1].difference(parts[i]) > 1)
                {
                    size_t idx = parts[i].length() + 1;
                    size_t end = parts[i - 1].length();
                    while(idx < end)
                    {
                        missing.push_back(idx);
                        idx++;
                    }
                }
            }
            
            // If this is the first try, we're skeptical that the sizes received are correct.
            // And if there is something missing, better make sure the boundary bytes aren't missing too.
            if(askForRLF || missing.size())
            {
                missing.push_back(1);
                missing.push_back(2);
                missing.push_back(1024);
                
                askForRLF = false;
            }
            // Request missing parts.
            requestParts(missing);
        }
        // Huh. Didn't really seem to receive anything. Try again.
        else
        {
            std::cout << std::endl << "*** Insufficient number of parts received to actually assemble file." << std::endl
                << "Scrapping old parts and retrying again in " << TIMEOUT << " seconds..." << std::endl << std::endl;
            
            parts.clear();
            askForRLF = true;
            sleep(TIMEOUT);
            
            bytes = sendto(socketDescriptor, requestedFile.c_str(), requestedFile.length() + 1, 0, (struct sockaddr*) &addr, addrLength);
            std::cout << "Sent a file request of " << bytes << " bytes: '" << requestedFile << "'" << std::endl;
        }
    } while(parts.size() <= 1 || missing.size() > 0);
    
    std::cout << "*** File received. Saving..." << std::endl;
    
    // Write disk
    std::ofstream stream;
    stream.open(outputFile.c_str(), std::ios::out | std::ios::binary);
    // F
    stream << parts[0];
    // L
    stream << parts[parts.size() - 2];
    // R
    stream << parts[parts.size() - 1];
    // Remove the parts we wrote.
    parts.erase(parts.begin());
    parts.pop_back();
    parts.pop_back();
    // Everything else is sequential, 
    // copy the rest all at once using an iterator.
    std::copy(parts.begin(), parts.end(), std::ostream_iterator<FilePart>(stream));
    // Done.
    stream.close();
    
    std::cout << "*** Saved to file '" << outputFile << "'!" << std::endl;
    
}

void Client::connect(const std::string& host, int port)
{
    struct hostent* server;
    
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addrLength = sizeof(struct sockaddr_in);
    
    std::cout << "Resolving host " << host << ":" << port << std::endl; 
    
    server = gethostbyname(host.c_str());
    if(server == NULL)
    {
        std::cerr << "Could not resolve the address of the server by name " << host << std::endl;
        exit(-1);
    }
    
    memcpy(&addr.sin_addr, server->h_addr, server->h_length);

    std::cout << "Creating socket to server..." << std::endl;
    
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if(socketDescriptor == -1)
    {
        socketDescriptor = 0;
        
        std::cerr << "Call to socket failed." << std::endl;
        exit(-1);
    }
}

void printUsage(int argc, char** argv)
{
    std::cout << std::endl
        << argv[0] << std::endl
        << std::endl
        << "Usage:" << std::endl
        << "\t" << argv[0] << " host port requestedFile outputFile" << std::endl
        << std::endl;
}

int main(int argc, char** argv)
{
    // Arguments: host port filename output
    if(argc < 5)
    {
        printUsage(argc, argv);
        return 0;
    }
    
    int port;
    std::string host, requestedFile, outputFile;
    
    host = argv[1];
    port = atoi(argv[2]);
    requestedFile = argv[3];
    outputFile = argv[4];
    
    Client client;
    client.connect(host, port);
    client.requestFile(requestedFile, outputFile);
    client.close();
}
