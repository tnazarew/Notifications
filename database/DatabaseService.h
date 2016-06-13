//
// Created by paulina on 01.06.16.
//

#ifndef TIN_SERVER_DATABASESERVICE_H
#define TIN_SERVER_DATABASESERVICE_H

#include <string>
#include "DatabaseObjects.h"
using namespace pqxx;

class DatabaseService {

    connection* conn;

public:
    DatabaseService() {};
    bool initConnection(std::string dbname, std::string user, std::string password, std::string hostaddr, std::string port);
    bool initOrClearTables();
    int addNewUser(User user);
    int addNewDevice(Device device);

    User getUserByLogin(std::string login);
    Device getDeviceById(std::string id);
    Notification getNotificationById(std::string id);
    void setNotificationRead(Notification notification);

    void updateDevice(Device device);
    void updateDeviceLoginDate(std::string deviceid);
    bool addNewNotification(Notification notification);
    std::vector<Notification> getNewDeviceNotifications(Device device);
    std::vector<Notification> getFreshlyReadDeviceNotifications(Device device);


    ~DatabaseService();



};

#endif //TIN_SERVER_DATABASESERVICE_H
