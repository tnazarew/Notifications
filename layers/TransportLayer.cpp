//
// Created by tomasz on 10.06.16.
//

#include <unistd.h>
#include <TCPHeader.h>
#include <string.h>
#include <iostream>
#include <sys/socket.h>
#include "TransportLayer.h"
#include "SecurityLayer.h"

int notifications::TransportLayer::send(char *string, std::map<std::string, void *> &map)
{
    int socket, timeout_sec, timeout_usec, size, devid, action, accept, bytes;
    socket = *(int *) map[SOCEKT_NUMBER];
    timeout_sec = *(int *) map[TIMEOUT_SEC];
    timeout_usec = *(int *) map[TIMEOUT_USEC];
    bytes = 0;
    size = *(int *) map[MES_SIZE];
    devid = *(int *) map[DEVICE_ID];
    action = *(int *) map[ACTION];
    TCPHeader h(size, action, devid);
    char *message = new char[4 + size];
    memcpy(message, h.getNet(), 4);
    memcpy(message + 4, string, size);
//    printHex(message, 4+size);
//    std::cout << size << std::endl;
    timeval t;
    t.tv_sec = timeout_sec;
    t.tv_usec = timeout_usec;
    fd_set checked;
    FD_SET(socket, &checked);
    while ((accept = select(socket + 1, NULL, &checked, NULL, &t)) > 0)
    {
        if (FD_ISSET(socket, &checked))
        {

            bytes += ::send(socket, message, size + 4, 0);
            if (bytes == size + 4)
                return 0;
        }
    }
    return -1;

}

int notifications::TransportLayer::recive(char *&string, std::map<std::string, void *> &map)
{
    int socket = *(int *) map[SOCEKT_NUMBER];
    int timeout_sec = *(int *) map[TIMEOUT_SEC];
    int timeout_usec = *(int *) map[TIMEOUT_USEC];
    timeval t;
    t.tv_sec = timeout_sec;
    t.tv_usec = timeout_usec;
    fd_set checked;
    FD_SET(socket, &checked);
    int accept = 0;
    int size, devid, action;
    size = -1;
    int bytes = 0;
    bool header = true;
    char *message_part, *header_part = new char[12];
    while ((accept = select(socket + 1, &checked, NULL, NULL, &t)) > 0 || size == 0)
    {
        if (FD_ISSET(socket, &checked) || size == 0)
        {
            if (header)
            {
                bytes += read(socket, header_part + bytes, 12 - bytes);
                if (bytes == 12)
                {
                    header = false;
                    printHex(header_part, 12);
                    TCPHeader h(header_part);
                    size = h.getSize();
                    action = h.getActon();
                    devid = h.getDeviceId();
                    message_part = new char[size];
                    bytes = 0;
                    //delete header_part;
                }
            }
            else
            {
                bytes += read(socket, message_part + bytes, size - bytes);
                if (bytes == size)
                {
                    string = message_part;
                    map[DEVICE_ID] = new char[4];
                    map[ACTION] = new char[4];
                    map[MES_SIZE] = new char[4];
                    *(int*)map[DEVICE_ID] = devid;
                    *(int*)map[ACTION] = action;
                    *(int*)map[MES_SIZE] = size;
                    std::cout << action << " " << size << " " << devid << std::endl;
                    return 0;
                }
            }
        }
    }

    return -1;
}



notifications::TransportLayer::TransportLayer(notifications::Layer *layer)
{
    lower_layer = layer;
}

void notifications::TransportLayer::printHex(const char *mes, int size) const
{
    unsigned char *p = (unsigned char *) mes;
    for (int i = 0; i < size; i++)
    {
        printf("0x%02x ", p[i]);
        if ((i + 1) % 16 == 0)
            printf("\n");
    }
    printf("\n");
}
