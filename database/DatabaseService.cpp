//
// Created by paulina on 01.06.16.
//

#include "DatabaseService.h"
#include <cstdlib>
#include <string>
#include <sstream>
#include "TableSQLs.h"


bool DatabaseService::initConnection(std::string  dbname, std::string  user, std::string password, std::string hostaddr, std::string port) {
    std::string connecttionString = "dbname=" + dbname + " user=" + user + " password=" + password +
        " hostaddr=" + hostaddr + " port=" + port;
    conn = new connection(connecttionString);
    if(!conn->is_open())
        return false;
    return true;
}

DatabaseService::~DatabaseService() {
    if(conn->is_open())
        conn->disconnect();
    delete conn;
}

bool DatabaseService::initOrClearTables() {
    pqxx::work txn(*conn);
    try{
        txn.exec(DROP_TABLES);
        txn.commit();
    } catch(...) {
    }
    pqxx::work txnInit(*conn);
    txnInit.exec(INIT_DATABASE_QUERY);
    txnInit.commit();
}

int DatabaseService::addNewUser(User user) {
    pqxx::work txn(*conn);
    pqxx::result result = txn.exec("insert into users (login, hashpassword) values (" + txn.quote(user.login) + "," + txn.quote(user.hashpassword) + ") returning id;");
    txn.commit();
    for (pqxx::result::const_iterator row = result.begin(); row != result.end(); ++row) {
        return row[0].as<int>();
    }
    return 0;
}

int DatabaseService::addNewDevice(Device device) {
    pqxx::work txn(*conn);
    std::ostringstream sid, srid;
    sid << device.userid;
    pqxx::result result = txn.exec("insert into devices (lastlogindate, userid, lastqueryid, lastreadid, lastquerytmp, lastreadtmp) "
                                           "values (current_date, " + sid.str() + ", 0, -1, 0, -1 ) returning id;");
    txn.commit();
    for (pqxx::result::const_iterator row = result.begin(); row != result.end(); ++row) {
        return row[0].as<int>();
    }
    return 0;
}

bool DatabaseService::addNewNotification(Notification notification) {
    pqxx::work txn(*conn);
    std::ostringstream sid;
    sid << notification.userid;
    txn.exec("insert into notifications (text, created, userid, readid) values ("
             + txn.quote(notification.text) + ", current_date ," + sid.str() + ", -1);");
    txn.commit();


}

User DatabaseService::getUserByLogin(std::string login) {
    pqxx::work txn(*conn);
    pqxx::result result = txn.exec("select id, login, hashpassword from users where users.login = " + txn.quote(login));
    txn.commit();
    for (pqxx::result::const_iterator row = result.begin(); row != result.end(); ++row) {
        return User(row);
    }
    return User(0, "", "");
}

Device DatabaseService::getDeviceById(std::string id) {
    pqxx::work txn(*conn);
    std::ostringstream srid;
    srid << id;
    pqxx::result result = txn.exec("select * from devices where id = " + srid.str() + ";");
    txn.commit();
    for (pqxx::result::const_iterator row = result.begin(); row != result.end(); ++row) {
        return Device(row);
    }
    return Device(0, "", 0, 0, 0, 0, 0);
}

Notification DatabaseService::getNotificationById(std::string id) {
    pqxx::work txn(*conn);
    std::ostringstream srid;
    srid << id;
    pqxx::result result = txn.exec("select * from notifications where id = " + srid.str() + ";");
    txn.commit();
    for (pqxx::result::const_iterator row = result.begin(); row != result.end(); ++row) {
        return Notification(row);
    }
    return Notification(0, "", "", 0, 0);
}

void DatabaseService::updateDeviceLoginDate(std::string deviceid) {
    pqxx::work txn(*conn);
    std::ostringstream srid;
    srid << deviceid;
    txn.exec("update devices set lastlogindate = current_date where id = " + srid.str() + ";");
    txn.commit();
}

void DatabaseService::setNotificationRead(Notification notification) {
    pqxx::work txn(*conn);
    std::ostringstream srid;
    srid << notification.id;
    txn.exec("update notifications set readid = (select max(readid) + 1 as jou from notifications) where id = " + srid.str());
    txn.commit();
}

std::vector<Notification> DatabaseService::getFreshlyReadDeviceNotifications(Device device) {
    pqxx::work txn(*conn);
    std::ostringstream did, lastreadid, id2;
    did << device.userid;
    id2 << device.id;
    lastreadid << device.lastreadid;
    txn.exec("update devices set lastreadtmp = (select max(readid) from notifications where userid = "
                                   + did.str() + ") where id = " + id2.str() + ";");
    pqxx::result result = txn.exec("select * from notifications where userid = " + did.str() + " and readid > " + lastreadid.str() + ";");
    txn.commit();
    std::vector<Notification> end;
    for (pqxx::result::const_iterator row = result.begin(); row != result.end(); ++row) {
        end.push_back(Notification(row));
    }
    return end;
}

std::vector<Notification> DatabaseService::getNewDeviceNotifications(Device device) {
    pqxx::work txn(*conn);
    std::ostringstream uid, last, did;
    uid << device.userid;
    last << device.lastqueryid;
    did << device.id;
    txn.exec("update devices set lastquerytmp = (select max(id) from notifications where userid = "
             + uid.str() + ") where id = " + did.str() + ";");
    pqxx::result result = txn.exec("select * from notifications where userid = " + uid.str() + " and id > " + last.str());
    txn.commit();
    std::vector<Notification> end;
    for (pqxx::result::const_iterator row = result.begin(); row != result.end(); ++row) {
        end.push_back(Notification(row));
    }
    return end;
}

void DatabaseService::updateDevice(Device device) {
    pqxx::work txn(*conn);
    std::ostringstream did;
    did << device.id;
    txn.exec("update devices set lastqueryid = lastquerytmp, lastreadid = lastreadtmp where id = " + did.str());
    txn.commit();
}

