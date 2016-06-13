//
// Created by tomasz on 10.06.16.
//

#include "Configuration.h"


#include <tinyxml.h>

void notifications::Configuration::load(std::string file)
{
//    TiXmlDocument doc(file.c_str());
//    TiXmlElement *config, *asymKey, *regLis, *database;
//    config = doc.FirstChildElement("config");
//    if(config)
//    {
//        asymKey = config->FirstChildElement("asymKey");
//        regLis = config->FirstChildElement("asymKey");
//        database = config->FirstChildElement("asymKey");
//        if(asymKey)
//        {
//            privKey = (unsigned)atoi(asymKey->Attribute("priv"));
//            pubKey = (unsigned)atoi(asymKey->Attribute("module"));
//            mod = (unsigned)atoi(asymKey->Attribute("pub"));
//        }
//        if(regLis)
//        {
//            port = atoi(regLis->Attribute("port"));
//        }
//        if(database)
//        {
//            dbHost = database->Attribute("host");
//            dbPort = database->Attribute("port");
//            dbName = database->Attribute("name");
//            dbLogin = database->Attribute("login");
//        }
//    }
//    else
//    {
//        perror("FAILED TO LOAD CONFIGURATION FROM FILE");
//    }

}

void notifications::Configuration::dynamicReconf()
{
    active = true;
    while (active)
    {
        std::string command, data;
        std::cout << "$ ";
        std::cin >> command;
        if (command == "reload")
        {
            ready = true;
            run = false;
        }
        if (command == "exit")
        {
            ready = false;
            exit_server = true;
            run = false;
            active = false;
        }
        if (command == "file")
        {
            std::cin >> data;
            filepath = data;
        }
        if (command == "load")
        {
            load(data);
        }
        if (command == "privKey")
        {
            std::cin >> data;
            privKey = (unsigned) atoi(data.c_str());
            priv = true;
        }
        if (command == "pubKey")
        {
            std::cin >> data;
            pubKey = (unsigned) atoi(data.c_str());
            pub = true;
        }
        if (command == "module")
        {
            std::cin >> data;
            mod = (unsigned) atoi(data.c_str());
            mo = true;
        }
        if (command == "port")
        {
            std::cin >> data;
            port = atoi(data.c_str());
            p = true;
        }
        if (command == "host")
        {
            std::cin >> data;
            dbHost = data;
            dbh = true;
        }
        if (command == "port_db")
        {
            std::cin >> data;
            dbPort = data;
            dbp = true;
        }
        if (command == "name")
        {
            std::cin >> data;
            dbName = data;
            dbn = true;
        }
        if (command == "login")
        {
            std::cin >> data;
            dbLogin = data;
            dbl = true;
        }
        if (command == "pass")
        {
            std::cin >> data;
            dbPass = data;
            dbpass = true;
        }
        if (command == "print")
        {
            std::cout << *this;
        }

    }
}

notifications::Configuration::Configuration()
{
    priv = pub = mo = dbh = dbp = dbn = dbl = dbpass = p = fp = exit_server = run = false;

    privKey = pubKey = mod = 0;
    port = 0;
    dbName = dbHost = dbLogin = dbPort = "";
    active = true;
    ready = false;
    filepath = DEFAULT_CONF_FILE;
}


bool notifications::Configuration::isReady()
{
    return ready;
}


std::string notifications::Configuration::toString()
{
    std::ostringstream s;
    s << "cofig file= " << (fp ? filepath : "") << std::endl;
    s << "port= " << (p ? std::to_string(port) : "") << std::endl;
    s << "RSA:" << std::endl;
    s << "  private= " << (priv ? std::to_string(privKey) : "") << std::endl;
    s << "  public= " << (pub ? std::to_string(pubKey) : "") << std::endl;
    s << "  mod= " << (mo ? std::to_string(mod) : "") << std::endl;
    s << "Database:= " << std::endl;
    s << "  Host= " << (dbh ? dbHost : "") << std::endl;
    s << "  Port= " << (dbp ? dbPort : "") << std::endl;
    s << "  Name= " << (dbn ? dbName : "") << std::endl;
    s << "  Login= " << (dbl ? dbLogin : "") << std::endl;
    s << "  Pasword= " << (dbpass ? dbPass : "") << std::endl;
    return s.str();
}

notifications::Configuration notifications::Configuration::operator=(notifications::Configuration &c)
{
    if (c.p)
    {
        port = c.port;
        p = true;
    }
    if(c.priv)
    {
        privKey = c.privKey;
        priv = true;
    }
    if (c.pub)
    {
        pubKey = c.pubKey;
        pub = true;
    }
    if (c.mo)
    {
        mod = c.mod;
        mo = true;
    }
    if (c.dbh)
    {
        dbHost = c.dbHost;
        dbh = true;
    }
    if (c.dbn)
    {
        dbName = c.dbName;
        dbn = true;
    }
    if (c.dbp)
    {
        dbPort = c.dbPort;
        dbp = true;
    }
    if (c.dbl)
    {
        dbLogin = c.dbLogin;
        dbl = true;
    }
    if (c.dbpass)
    {
        dbPass = c.dbPass;
        dbpass = true;
    }
    if (c.fp)
    {
        filepath = c.filepath;
        fp = true;
    }
    c.run = true;
    return *this;
}


bool notifications::Configuration::isRunning()
{
    return run;
}

notifications::Configuration::~Configuration()
{

}

bool notifications::Configuration::exitServer()
{
    return exit_server;
}


