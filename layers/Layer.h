//
// Created by tomasz on 10.06.16.
//

#ifndef NOTIFICATOR_LAYER_H
#define NOTIFICATOR_LAYER_H

#include <string>
#include <map>

namespace notifications
{
    const static std::string MES_SIZE = "MessageSize";
    const static std::string DEVICE_ID = "DeviceID";
    const static std::string ACTION = "Action";
    const static std::string SOCEKT_NUMBER = "socketNumber";
    const static std::string TIMEOUT_SEC = "timeoutSec";
    const static std::string TIMEOUT_USEC = "timeoutNsec";
    const static std::string PUB_KEY = "publicKey";
    const static std::string MODULUS = "modulus";

    static const int REGISTRATION_REQUEST = 0;
    static const int LOGIN_REQUEST = 1;
    static const int MESSAGE_REQUEST = 2;
    static const int MESSAGE_RECIVED = 3;
    static const int MESSAGE_READ = 4;

    static const int HANDSHAKE_REQUEST = 5;
    static const int PUBLIC_KEY_SIZE = 8;
    static const int SESSION_KEY_SIZE = 8;

    class Layer
    {
    protected:
        Layer *lower_layer;
    public:
        Layer();
        Layer(Layer*);
        virtual ~Layer();



        virtual int recive(char *&, std::map<std::string, void *> &) = 0;

        virtual int send(char *, std::map<std::string, void *> &) = 0;
    };
}


#endif //NOTIFICATOR_LAYER_H
