//
// Created by paulina on 02.06.16.
//

#ifndef TIN_SERVER_JSONBUILDER_H
#define TIN_SERVER_JSONBUILDER_H
#include <string>
#include "DatabaseObjects.h"
#include "json/json.h"
#include <vector>

class JsonBuilder {

public:
    static Json::Value loginOrRegisterSuccess(int deviceid);
    static Json::Value messagesResponse(std::vector<Notification>& newMessages, std::vector<Notification> &read);
    static Json::Value failureResponse(std::string msg);
    static Json::Value successResponse();
    static void addToJson(std::string label, std::string value, Json::Value & root);
};


#endif //TIN_SERVER_JSONBUILDER_H
