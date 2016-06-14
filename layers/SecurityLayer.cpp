//
// Created by tomasz on 10.06.16.
//

#include <netinet/in.h>
#include <json/json.h>
#include <RC4.h>
#include "SecurityLayer.h"



int notifications::SecurityLayer::send(char *string, std::map<std::string, void *> &map)
{
    int code;
    int action = *(int *) map[ACTION];
    if(action == REGISTRATION_REQUEST || action == LOGIN_REQUEST)
    {
        unsigned pe, pn;
        pe = *(unsigned*)map[PUB_KEY];
        pn = *(unsigned*)map[MODULUS];
        Json::Value root, value;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse(string, root);
        if (!parsingSuccessful)
        {
            std::cout << "Failed to parse" << reader.getFormattedErrorMessages();
            return false;
        }
        int size;
        bool suc = root["success"].asBool();
        if(suc)
        {
            int devid = atoi(root["value"]["deviceId"].asString().c_str());
            char* session_key;
            char *temp1 = new char[8];
            if(action == REGISTRATION_REQUEST || keys.find(devid) == keys.end())
            {
                generateKey(session_key);
                keys[devid] = std::string(session_key);
                *(int*)map[DEVICE_ID] = devid;
            }
            else
            {
                strcpy(temp1, keys[devid].c_str());
                *(int*)map[DEVICE_ID] = devid;
                session_key = temp1;
            }
            root["value"]["sessionKey"] = session_key;
            std::cout << session_key << std::endl;
            Json::FastWriter fastWriter;
            std::string temp = fastWriter.write(root);
            delete string;
            string = encryptAsym(pe, pn, temp);
            size = ((temp.size()%4)!=0)?temp.size() +4 -(temp.size()%4) : temp.size();
            delete temp1;
        }
        else
        {
            char* temp_c = string;
            std::string s(temp_c);
            size = ((s.size()%4)!=0)?s.size() +4 -(s.size()%4) : s.size();
            string = encryptAsym(pe, pn, s);
            delete temp_c;
        }
        printHex(string, size);
        *(int*)map[MES_SIZE] = size;
        lower_layer->send(string, map);
    }
    else if(action == MESSAGE_REQUEST || action == MESSAGE_RECIVED || action == MESSAGE_READ)
    {
//        std::cout <<  keys[*(int*)map[DEVICE_ID]].c_str() << std::endl;
        int i = std::string(string).size();
//        std::cout << i <<  " " << std::string(string).size() << std::endl;
//        std::cout << keys[*(int*)map[DEVICE_ID]].size() << std::endl;
//        const char *new_string = RC4::decrypt(string , keys[*(int*)map[DEVICE_ID]].c_str(), std::string(string).size()).c_str();
        char *temp = new char[std::string(string).size()];
//        printHex(new_string, std::string(string).size());
        //std::cout << RC4::decrypt(new_string , keys[*(int*)map[DEVICE_ID]].c_str(), std::string(string).size()+1) << std::endl;
        memcpy(temp, RC4::decrypt(string , keys[*(int*)map[DEVICE_ID]].c_str(), std::string(string).size()).c_str(), std::string(string).size());
//        *(int*)map[MES_SIZE] = std::string(string).size()+1;
        lower_layer->send(temp , map);
    }
    else if(action == HANDSHAKE_REQUEST)
        code = sendPublicKey(map);
    else
        code = -1;

    return code;
}

int notifications::SecurityLayer::sendPublicKey(std::map<std::string, void *> &map) const
{
    int code;
    char* public_key = new char[PUBLIC_KEY_SIZE];
    notifications::SecurityLayer::rsa.getPublicKey(*(unsigned*)public_key, *(unsigned*)(public_key + 4));
    *(unsigned*)public_key = htonl(*(unsigned*)public_key);
    *(unsigned*)(public_key + 4) = htonl(*(unsigned*)(public_key + 4));
    *(int*)map[MES_SIZE] = 8;
    code = this->lower_layer->send(public_key, map);
    delete public_key;
    return code;
}

int notifications::SecurityLayer::recive(char *&string, std::map<std::string, void *> &map)
{
    char *new_string;
    int code = lower_layer->recive(new_string, map);
    if(code == 0)
    {
        int action = *(int *) map[ACTION];
        if(action == REGISTRATION_REQUEST || action == LOGIN_REQUEST)
        {
            unsigned pe, pn;
            string = decryptAsym(new_string, *(int *) map[MES_SIZE]);
            if(!getKeys(string, pe, pn))
                return -1;
            map[PUB_KEY] = new char[4];
            *(unsigned*)map[PUB_KEY] = pe;
            map[MODULUS] = new char[4];
            *(unsigned*)map[MODULUS] = pn;
            code = 0;

        }
        else if(action == MESSAGE_REQUEST || action == MESSAGE_RECIVED || action == MESSAGE_READ)
        {
            int devid = *(int*)map[DEVICE_ID];
            int size = *(int*)map[MES_SIZE];
            if(keys.find(devid) == keys.end())
                return -1;
            string = new char[size];
            std::cout << keys[devid].c_str() <<std::endl;
            strcpy(string, RC4::decrypt(new_string , keys[devid].c_str(), size).c_str());
            code = 0;
        }
        else if(action == HANDSHAKE_REQUEST)
        {
            *(int*)map[MES_SIZE] = 8;
        }
        else
            code = -1;
    }
    return code;
}



notifications::SecurityLayer::SecurityLayer(notifications::Layer *layer)
{
    lower_layer = layer;
}

char *notifications::SecurityLayer::encryptAsym(unsigned p_e, unsigned p_n, const std::string &input) const
{
    int tempsize = input.size();
    unsigned long mod4 = tempsize % 4;
    unsigned size = (mod4 ? (tempsize ) / 4 + 1 : ((tempsize) / 4));
    char* output = new char[4*size];
    char* in = new char[4*size];
    int temp_mem_size = (mod4 ? 4*size -4 + mod4: 4*size);
    memcpy(in, input.c_str(), temp_mem_size);
    if(mod4!=0)
        for(int i = 0; i < 4-mod4; i++)
            in[4*size - 1 - i] = 0;


    for (int i = 0; i < 4*size; i += 4)
    {
        *((unsigned *) (output + i)) = rsa.encrypt((*(unsigned *) (in + i)), p_e, p_n);
        *((unsigned *) (output + i)) = htonl(*((unsigned *) (output + i)));
    }
    return output;
}

char *notifications::SecurityLayer::decryptAsym(const char *enc_mes, int size) const
{
    char *dec = new char[size];
    for (int i = 0; i < size; i += 4)
    {
        *((unsigned *) (dec + i)) = rsa.decrypt(ntohl(*(unsigned *) (enc_mes + i)));
    }
    return dec;
}

bool notifications::SecurityLayer::getKeys(char *string, unsigned int& p_e, unsigned int& p_n)
{
    Json::Value root;
    Json::Reader reader;

    bool parsingSuccessful = reader.parse(string, root);
    if (!parsingSuccessful)
    {
        std::cout << "Failed to parse" << reader.getFormattedErrorMessages();
        return false;
    }
    if(!root.isMember("cipheredData"))
        return false;
    Json::Value c_data = root["cipheredData"];
    if(!c_data.isMember("publicKey") || !c_data.isMember("modulus"))
        return false;
    std::string s1 = c_data["publicKey"].asString();
    std::string s2 = c_data["modulus"].asString();
    p_e = (unsigned)atoi(s1.c_str());
    p_n = (unsigned)atoi(s2.c_str());

    return true;
}

void notifications::SecurityLayer::generateKey(char *&key)
{
    key = new char[8];

    strcpy(key, "ABCDEFGH");
}

void notifications::SecurityLayer::printHex(const char *mes, int size) const
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

char notifications::SecurityLayer::genRandom()
{
    return alphanum[rand() % alphanum.size()];
}
