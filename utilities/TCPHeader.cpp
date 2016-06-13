//
// Created by tomasz on 10.06.16.
//

#include <netinet/in.h>
#include "TCPHeader.h"


notifications::TCPHeader::TCPHeader(char *string)
{
    flags[0] = *(unsigned*)string;
    flags[1] = *(unsigned*)(string+4);
    flags[2] = *(unsigned*)(string+8);
}

char* notifications::TCPHeader::getNet()
{
    return (char*)flags;
}



notifications::TCPHeader::TCPHeader(unsigned int size, unsigned int action, unsigned int device_id)
{
    flags[0] = htonl(size);
    flags[1] = htonl(device_id);
    flags[2] = htonl(action);
}

int notifications::TCPHeader::getActon()
{
    return ntohl(flags[2]);
}

int notifications::TCPHeader::getSize()
{
    return ntohl(flags[0]);
}

int notifications::TCPHeader::getDeviceId()
{
    return ntohl(flags[1]);
}
