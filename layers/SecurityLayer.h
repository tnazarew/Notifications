//
// Created by tomasz on 10.06.16.
//

#ifndef NOTIFICATOR_SECURITYLAYER_H
#define NOTIFICATOR_SECURITYLAYER_H


#include <map>
#include <RSA.h>
#include <Layer.h>

namespace notifications
{
    static const std::string alphanum =
        "0123456789"
        "!@#$%^&*"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";


    class SecurityLayer : public Layer
    {
        RSA rsa;
        std::map<int, std::string> keys;

        int sendPublicKey(std::map<std::string, void *> &map) const;
        char* encryptAsym(unsigned p_e, unsigned p_n, const std::string &input) const;
        char* decryptAsym(const char *enc_mes, int size) const;
        bool getKeys(char*, unsigned&, unsigned&);
        void generateKey(char*&);
        void printHex(const char*, int) const;
        char genRandom();
    public:
        SecurityLayer(Layer *);

        void shutdown();

        int recive(char *&, std::map<std::string, void *> &);

        int send(char *, std::map<std::string, void *> &);
    };
}


#endif //NOTIFICATOR_SECURITYLAYER_H
