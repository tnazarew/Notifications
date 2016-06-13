//
// Created by paulina on 02.06.16.
//

#include "JsonResolver.h"
#include "json/json.h"
#include "JsonBuilder.h"

JsonResolver::JsonResolver(int action_, int deviceId_, std::string jsonString) : action(action_), deviceId(deviceId_) {
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonString.c_str(), parsedJson);
    if (!parsingSuccessful) {
        parsed = false;
        return;
    }
    parsed =  true;
}

void JsonResolver::execute(DatabaseService * service) {
    if(!parsed)
        return;
    this->service = service;
    switch(action) {
        case 0:
            resolveRegisterRequest();
            break;
        case 1:
            resolveLoginRequest();
            break;
        case 2:
            resolveNewMessagesRequest();
            break;
        case 3:
            resolveMessageReceived();
            break;
        case 4:
            resolveMessageRead();
            break;
        default:
            returnedJson = JsonBuilder::failureResponse("invalidQuery");
            break;
    }
    return;
}

void JsonResolver::resolveLoginRequest() {
    Json::Value cipheredData = parsedJson["cipheredData"];
    std::string login = cipheredData.get("user", "").asString();
    std::string hashpassword = cipheredData.get("password", "").asString();
    std::string deviceid = cipheredData.get("deviceId","").asString();
    if(login == "" || hashpassword == "") {
        returnedJson = JsonBuilder::failureResponse("invalidQuery");
        return;
    }
    User user = service->getUserByLogin(login);
    if(user.id == 0) {
        returnedJson = JsonBuilder::failureResponse("noSuchUser");
        return;
    }
    if(user.hashpassword != hashpassword) {
        returnedJson = JsonBuilder::failureResponse("invalidPassword");
        return;
    }
    int did;
    if(deviceid == "") {
        Device device(0, "", user.id, 0, 0, 0, 0);
        did = service->addNewDevice(device);
    } else {
        Device device = service->getDeviceById(deviceid);
        if(device.id == 0) {
            returnedJson = JsonBuilder::failureResponse("noSuchDevice");
            return;
        }
        if(user.id != device.userid) {
            returnedJson = JsonBuilder::failureResponse("noSuchDevice");
            return;
        }
        did = device.id;
        service->updateDeviceLoginDate(deviceid);
    }
    returnedJson = JsonBuilder::loginOrRegisterSuccess(did);
    return;
}

void JsonResolver::resolveNewMessagesRequest() {
    Device device = service->getDeviceById(std::to_string(deviceId));
    if(device.id == 0) {
        returnedJson = JsonBuilder::failureResponse("noSuchDevice");
        return;
    }
    std::vector<Notification> read = service->getFreshlyReadDeviceNotifications(device);
    std::vector<Notification> newNotifications = service->getNewDeviceNotifications(device);
    returnedJson = JsonBuilder::messagesResponse(newNotifications, read);
    return;
}

void JsonResolver::resolveMessageRead() {
    if(deviceId < 1) {
        returnedJson = JsonBuilder::failureResponse("invalidQuery");
        return;
    }
    Json::Value cipheredData = parsedJson["cipheredData"];
    std::string messageId = cipheredData.get("messageId", "").asString();
    Device device = service->getDeviceById(std::to_string(deviceId));
    if(device.id == 0) {
        returnedJson = JsonBuilder::failureResponse("noSuchDevice");
        return;
    }
    Notification notification = service->getNotificationById(messageId);
    if(notification.id == 0) {
        returnedJson = JsonBuilder::failureResponse("noSuchMessage");
        return;
    }
    if(device.userid != notification.userid) {
        returnedJson = JsonBuilder::failureResponse("accessDenied");
        return;
    }
    if(notification.readid != -1) {
        returnedJson = JsonBuilder::successResponse();
        return;
    }
    service->setNotificationRead(notification);
    returnedJson = JsonBuilder::successResponse();
    return;
}

void JsonResolver::resolveRegisterRequest() {
    Json::Value cipheredData = parsedJson["cipheredData"];
    std::string login = cipheredData.get("user", "").asString();
    std::string hashpassword = cipheredData.get("password", "").asString();
    if(login == "" || hashpassword == ""){
        returnedJson = JsonBuilder::failureResponse("invalidQuery");
        return;
    }
    User user = service->getUserByLogin(login);
    if(user.id != 0) {
        returnedJson =  JsonBuilder::failureResponse("userAlreadyExists");
        return;
    }
    User newUser(0, login, hashpassword);
    int userid = service->addNewUser(newUser);
    Device device(0, "", userid, 0, 0, 0, 0);
    int deviceid = service->addNewDevice(device);
    returnedJson = JsonBuilder::loginOrRegisterSuccess(deviceid);
    return;
}

void JsonResolver::resolveMessageReceived() {
    Device device = service->getDeviceById(std::to_string(deviceId));
    if(device.id == 0) {
        returnedJson = JsonBuilder::failureResponse("noSuchDevice");
        return;
    }
    service->updateDevice(device);
    returnedJson = JsonBuilder::successResponse();
    return;
}

void JsonResolver::addToReturnedCiphered(std::string label, std::string value) {
    JsonBuilder::addToJson(label, value, returnedJson["cipheredData"]);
}

void JsonResolver::addToReturnedRoot(std::string label, std::string value) {
    JsonBuilder::addToJson(label, value, returnedJson);
}

std::string JsonResolver::getFromParsedCiphered(std::string label) {
    return parsedJson["cipheredData"].get(label, "").asString();
}

std::string JsonResolver::getFromParsedRoot(std::string label) {
    return parsedJson.get(label, "").asString();
}