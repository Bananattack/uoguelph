#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <iterator>

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>

typedef unsigned short ushort;
struct FilePart
{
    std::string data;
    
    int length() const
    {
        return data.length();
    }
  
    int difference(const FilePart& rs) const
    {
        return abs(length() - rs.length());
    }
};

inline bool operator< (const FilePart& ls, const FilePart& rs)
{
    return ls.length() < rs.length();
}

inline bool operator<= (const FilePart& ls, const FilePart& rs)
{
    return ls.length() <= rs.length();
}

inline bool operator> (const FilePart& ls, const FilePart& rs)
{
    return ls.length() > rs.length();
}

inline bool operator>= (const FilePart& ls, const FilePart& rs)
{
    return ls.length() <= rs.length();
}

inline bool operator== (const FilePart& ls, const FilePart& rs)
{
    return ls.length() == rs.length();
}

inline bool operator!= (const FilePart& ls, const FilePart& rs)
{
    return ls.length() != rs.length();
}

inline std::ostream& operator<<(std::ostream& out, const FilePart& part)
{
    out << part.data;
    return out;
}

class Client
{
    public:
        static const int MAX_DATAGRAM_SIZE = 1024;
        static const int TIMEOUT = 30;
    private:
        int socketDescriptor;
        struct sockaddr_in addr;
        socklen_t addrLength;
        
        int R, L, F;
        
        std::vector<FilePart> parts;
    public:
        Client();
        ~Client();
        
        void close();
        void requestFile(const std::string& requestedFile, const std::string& outputFile);
        void connect(const std::string& host, int port);
    private:
        bool receiveData();
        void requestParts(const std::vector<ushort>& partNumbers);
};
