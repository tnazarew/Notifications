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
            for (int i =0; i < sockets.size(); i++)
            {
                if(sockets[i]== -1)
                    continue;
                int retval = getsockopt(sockets[i], SOL_SOCKET, SO_ERROR, &error, &len);
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
                    active_threads[i].join();


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
    if(b == -1)
        std::cout <<"FAIL" <<std::endl;
    auto f = std::bind(&Server::mainSocketThread, std::ref(*this), sd);
    main_socket_thread = std::thread(f);

    running_mutex.unlock();
    main_socket_thread.join();
    return 0;
}

void notifications::Server::processClient(int client_socket)
{
    char *input;
    int sec = 2;
    int usec = 0;
    std::map<std::string, void *> args;
    args[SOCEKT_NUMBER] = (void *) (&client_socket);
    args[TIMEOUT_SEC] =  (void *) (&sec);
    args[TIMEOUT_USEC] = (void *) (&usec);
    int code = upper_layer->recive(input, args);
    if(code < 0)
    {
        std::cout << "error in processing client request";
    }
    else
    {
        int action = *(int*)args[ACTION];
        char* output;
        if (action != 5)
        {
            std::cout << input << std::endl;
            std::string a(input);
            JsonResolver res(*(int*)args[ACTION], *(int*)args[DEVICE_ID], a);
            res.execute(&s);
            res.print();
            Json::FastWriter fastWriter;
            std::string temp = fastWriter.write(res.getReturnedJson());
            output = new char[temp.size()];
            *(int*)args[MES_SIZE] = temp.size();
            strcpy(output, temp.c_str());
            std::cout << temp <<std::endl;

        }
        upper_layer->send(output, args);
    }
    close(client_socket);
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
        bool found = false;


        std::thread t(f);
        t.detach();
        running_mutex.unlock();

    }
    delete s;
}

void notifications::Server::add(int i, std::thread &t)
{


}

void notifications::Server::run()
{
    mainThread();
}
