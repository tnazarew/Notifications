//
// Created by tomasz on 12.06.16.
//

#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include "json/json.h"
#include "DatabaseService.h"
#include "JsonResolver.h"

using namespace std;
using namespace pqxx;

using namespace std;

int main() {

    string json, deviceId, action;
    DatabaseService s;
    s.initConnection("tin", "tin", "tin", "127.0.0.1", "5433");
    while(getline(cin, json)){
//        getline(cin, deviceId);
//        getline(cin, action);
//        if(json == "newNotif") {
//            string i;
//            string str;
//            getline(cin, i);
//            getline(cin, str);
//            Notification n(0, str, "", atoi(i.c_str()), 0);
//            s.addNewNotification(n);
//            continue;
//        }
        JsonResolver res(0, -1, json);
        res.execute(&s);
        res.print();
    }



}