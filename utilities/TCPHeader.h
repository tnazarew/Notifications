//
// Created by tomasz on 10.06.16.
//

#ifndef NOTIFICATOR_HEADER_H
#define NOTIFICATOR_HEADER_H

namespace notifications
{
    class TCPHeader
    {
    public:
        static const int ACTION = 1;
        static const int SIZE = 0;
        static const int DEVICE_ID = 2;
        TCPHeader(unsigned int size, unsigned int action, unsigned int device_id);
        TCPHeader(char*);
        char* getNet();
        int getActon();
        int getSize();
        int getDeviceId();
    private:
        unsigned int flags[3];
    };
}


#endif //NOTIFICATOR_HEADER_H
