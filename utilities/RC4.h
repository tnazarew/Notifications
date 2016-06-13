//
// Created by tomasz on 10.06.16.
//

#ifndef NOTIFICATOR_RC4_H
#define NOTIFICATOR_RC4_H


#include <string>
namespace notifications
{
    class RC4
    {
        const static int KEY_SIZE = 8; // key size in bytes
        // klucz summetryczny
        // syfrowanie deszyfrowanie kluczem symmetrycznym
        static void encrypt(const char * ByteInput, const char * pwd, char * &ByteOutput, size_t length);
        char * key;
    public:

        // Client methods:
        RC4(char*);
        RC4();
        ~RC4();
        bool init(const char * str);
        char * encrypt(std::string msg);
        std::string decrypt(const char * BinaryInput, int length=0);
        // Server methods
        static char * encrypt(std::string msg, const char * givenKey);
        static std::string decrypt(const char * BinaryInput, const char * givenKey,int length=0);
    };
}

#endif //NOTIFICATOR_RC4_H
