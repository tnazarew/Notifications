//
// Created by paulina on 02.06.16.
//

#ifndef TIN_SERVER_ANSWERBUILDER_H
#define TIN_SERVER_ANSWERBUILDER_H
#include <string>
#include <iostream>
#include "json/json.h"
#include "DatabaseService.h"

class JsonResolver {

    DatabaseService * service;
    int action;
    int deviceId;

    Json::Value parsedJson;
    Json::Value returnedJson;

    void resolveRegisterRequest();
    void resolveLoginRequest();
    void resolveNewMessagesRequest();
    void resolveMessageRead();
    void resolveMessageReceived();
    bool parsed;
public:
    JsonResolver(int action_, int deviceId_, std::string jsonString);
    void execute( DatabaseService * service);
    void print() { std::cout<<returnedJson.toStyledString() ;};
    Json::Value getReturnedJson() { return returnedJson;};
    std::string getReturnedJsonString() { return returnedJson.asString(); };
    bool isParsed() { return parsed; }

    std::string getFromParsedRoot(std::string label);
    std::string getFromParsedCiphered(std::string label);

    void addToReturnedRoot(std::string label, std::string value);
    void addToReturnedCiphered(std::string label, std::string value);
};


#endif //TIN_SERVER_ANSWERBUILDER_H
