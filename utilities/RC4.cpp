//
// Created by tomasz on 10.06.16.
//

#include "RC4.h"

#include <string.h>
#include <string>
namespace notifications
{
    RC4::RC4() : key(NULL)
    { }
    RC4::RC4(char * k)
    {
        key = new char[KEY_SIZE];
        memcpy(key, k, KEY_SIZE);
    }
    bool RC4::init(const char * k)
    {
        key = new char[KEY_SIZE];
        memcpy(key, k, KEY_SIZE);
    }


    char * RC4::encrypt(std::string msg, const char * givenKey)
    {
        char * BinaryOutput;
        encrypt(msg.c_str(), givenKey, BinaryOutput, msg.size());
        return BinaryOutput;
    }
    char * RC4::encrypt(std::string msg)
    {
        char * BinaryOutput;
        encrypt(msg.c_str(), key, BinaryOutput, msg.size());
        return BinaryOutput;
    }

    std::string RC4::decrypt(const char * BinaryInput, const char * givenKey,int length)
    {
        char * CharOutput;
        encrypt(BinaryInput, givenKey, CharOutput, (size_t)length);
        std::string temp(CharOutput);
        delete CharOutput;
        return temp;
    }
    std::string RC4::decrypt(const char * BinaryInput, int length)
    {
        char * CharOutput;
        encrypt(BinaryInput, key, CharOutput, (size_t)length);
        std::string temp(CharOutput);
        delete CharOutput;
        return temp;
    }

    void RC4::encrypt(const char * ByteInput, const char * pwd, char* &ByteOutput, size_t length)
    {
        //unsigned char * temp;
        char * temp;
        int i,j=0,t,tmp,tmp2,s[256], k[256];
        int len = KEY_SIZE;
        for (tmp=0;tmp<256;tmp++)
        {
            s[tmp]=tmp;
            k[tmp]=pwd[tmp % len];
        }
        for (i=0;i<256;i++)
        {
            j = ((unsigned char)(j + s[i] + k[i])) % 256;
            tmp=s[i];
            s[i]=s[j];
            s[j]=tmp;
        }
        int len2;
        if(length<=0) len2 = (int)strlen((char *)ByteInput);
        else len2 = length;

        temp = new char[len2+1];
        i=j=0;
        for (tmp=0;tmp<len2;tmp++)
        {
            i = ((unsigned char)(i + 1)) % 256;
            j = ((unsigned char)(j + s[i])) % 256;
            tmp2=s[i];
            s[i]=s[j];
            s[j]=tmp2;
            t = ((unsigned char)(s[i] + s[j])) % 256;
            if (s[t]==ByteInput[tmp])
                temp[tmp]=ByteInput[tmp];
            else
                temp[tmp]=s[t]^ByteInput[tmp];
        }
        temp[tmp]='\0';
        ByteOutput=temp;
    }



    RC4::~RC4()
    {
        delete key;
    }

}