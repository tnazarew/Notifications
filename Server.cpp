//
// Created by tomasz on 10.06.16.
//

#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <JsonResolver.h>
#include "Server.h"

notifications::Server::Server(notifications::Layer *upper_layer) : upper_layer(upper_layer)
{
    running = false;
    exitServer = false;
}


void notifications::Server::getData()
{
    config = temp_config;
}


void notifications::Server::readInput()
{
    conf_thread = std::thread(&Configuration::dynamicReconf, std::ref(temp_config));
}

void notifications::Server::shutdown()
{
    running = false;
}

void notifications::Server::start()
{
    running = true;
}

notifications::Server::Server(std::string string, notifications::Layer *upper_layer)
{

}

void notifications::Server::mainThread()
{
    readInput();

    int error = 0;
    socklen_t len = sizeof(error);

    while (!temp_config.exitServer())
    {
        if (temp_config.isReady())
            initServer();
        else
            sleep(1);

        while (temp_config.isRunning())
        {
            for (auto& i: active_threads)
            {
                if(i.first == -1)
                    continue;
                int retval = getsockopt(i.first, SOL_SOCKET, SO_ERROR, &error, &len);
                if (retval != 0)
                {
                    // there was a problem getting the error code
                    fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
                    return;
                }

                if (error != 0)
                {
                    // socket has a non zero error status
                    fprintf(stderr, "socket error: %s\n", strerror(error));
                    i.first = -1;
                    i.second.join();
                }
            }
            sleep(1);

        }

    }
    conf_thread.join();
}


void notifications::Server::exit()
{
    running = false;
    exitServer = true;
}

int notifications::Server::initServer()
{
    running_mutex.lock();
    shutdown();
    getData();
    s.initConnection(config.getDbName(), config.getDbLogin(), config.getDbPass(), config.getDbHost(), config.getDbPort());
    sockaddr_in socket_adress;
    socket_adress.sin_family = AF_INET;
    socket_adress.sin_addr.s_addr = htonl(INADDR_ANY);
    socket_adress.sin_port = htons(config.getPort());
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    int b = bind(sd, (sockaddr *) &socket_adress, sizeof(socket_adress));
    auto f = std::bind(&Server::mainSocketThread, std::ref(*this), sd);
    main_socket_thread = std::thread(f);
    running_mutex.unlock();
    return 0;
}

void notifications::Server::processClient(int client_socket)
{
    char *input;
    std::string output;
    int sec = 2;
    int usec = 0;
    std::map<std::string, void *> args;
    args[SOCEKT_NUMBER] = (void *) (&client_socket);
    args[TIMEOUT_SEC] =  (void *) (&sec);
    args[TIMEOUT_USEC] = (void *) (&usec);
    upper_layer->recive(input, args);


    int action = *(int*)args[ACTION];
    if (action != 5)
    {
        std::cout << input << std::endl;
        std::string gowno(input);
        JsonResolver res(*(int*)args[ACTION], *(int*)args[DEVICE_ID], gowno);
        res.execute(&s);
        res.print();
        Json::FastWriter fastWriter;
        std::string output = fastWriter.write(res.getReturnedJson());
//        output = res.getReturnedJsonString();
        char* realOutput = new char[output.size()];
        strcpy(realOutput, output.c_str());
        std::cout << output <<std::endl;
        upper_layer->send(realOutput, args);
    }

}


notifications::Server::~Server()
{

}


void notifications::Server::mainSocketThread(int i)
{
    int sd, bind_res;
    sockaddr* s = new sockaddr;
    unsigned adr_size;
    while (temp_config.isRunning())
    {
        bind_res = listen(i, 1);
        sd = accept(i, s, &adr_size);
        running_mutex.lock();
        auto f = std::bind(&Server::processClient, std::ref(*this), sd);
        std::thread t(f);
        add(sd, t);
        running_mutex.unlock();
        sleep(5);
    }
    delete s;
}

void notifications::Server::add(int i, std::thread &t)
{
//    for(int j = 0; j < active_threads.size(); j++)
//    {
//        if(active_threads[j].first == -1)
//        {
//            active_threads[j] = std::move(std::make_pair(i,std::move(t)));
//            return;
//        }
//    }
    active_threads.push_back(std::move(std::make_pair(i, std::move(t))));
}

void notifications::Server::run()
{
    mainThread();
}
