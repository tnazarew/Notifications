//
// Created by paulina on 02.06.16.
//

#include "JsonBuilder.h"
#include "json/json.h"

Json::Value JsonBuilder::failureResponse(std::string msg) {
    Json::Value root;
    root["success"] = false;
    root["value"] = msg;
    return root;
}

Json::Value JsonBuilder::loginOrRegisterSuccess(int id) {
    Json::Value root;
    Json::Value value;
    root["success"] = true;
    value["deviceId"] = std::to_string(id);
    root["value"] = value;
    return root;
}

Json::Value JsonBuilder::successResponse() {
    Json::Value root;
    Json::Value cipheredData;
    cipheredData["success"] = true;
    root["cipheredData"] = cipheredData;
    return root;
}

Json::Value JsonBuilder::messagesResponse(std::vector<Notification> &newMessages, std::vector<Notification> &read) {
    Json::Value root;
    Json::Value cipheredData;
    cipheredData["success"] = true;
    Json::Value messages;
    for(int i = 0; i < newMessages.size(); ++i) {
        messages[i]["id"] = newMessages[i].id;
        messages[i]["notification"] = newMessages[i].text;
    }
    cipheredData["messages"] = messages;
    Json::Value readMessages;
    for(int i = 0; i < read.size(); ++i) {
        readMessages[i] = read[i].id;
    }
    cipheredData["readMessages"] = readMessages;
    root["cipheredData"] = cipheredData;
    return root;
}

void JsonBuilder::addToJson(std::string label, std::string value, Json::Value &root) {
    root[label] = value;
}