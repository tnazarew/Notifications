//
// Created by tomasz on 10.06.16.
//

#ifndef NOTIFICATOR_SERVER_H
#define NOTIFICATOR_SERVER_H
#include <time.h>
#include <map>
#include <Configuration.h>

#include <thread>
#include <Layer.h>
#include <mutex>
#include <vector>
#include <DatabaseService.h>

namespace notifications
{
    class Server
    {
    private:
        Configuration config, temp_config;
        std::thread conf_thread, main_socket_thread;
        void reload();
        void readInput();
        void shutdown();
        void exit();
        void start();
        void mainThread();
        void mainSocketThread(int);
        void add(int, std::thread& t);
        int initServer();
        void  processClient(int);
        bool running, exitServer;
        DatabaseService s;
        std::vector<std::pair<int, std::thread> > active_threads;
        std::mutex running_mutex;
        std::map<int, std::thread> client_threads;
        Layer* upper_layer;

    public:
        ~Server();
        Server(notifications::Layer *upper_layer);
        Server(std::string, notifications::Layer *upper_layer);
        void run();
        void getData();

    };
}


#endif //NOTIFICATOR_SERVER_H
