//
// Created by tomasz on 10.06.16.
//

#ifndef NOTIFICATOR_CONFIGURATION_H
#define NOTIFICATOR_CONFIGURATION_H

#include <string>
#include <iostream>

namespace notifications
{
    static const std::string DEFAULT_CONF_FILE = "~/.notifier/default.conf";
    class Configuration
    {
    private:

        bool priv, pub, mo, dbh, dbp, dbn, dbl, dbpass, p, fp;
    public:
        const std::string &getFilepath() const
        {
            return filepath;
        }

        const std::string &getDbPass() const
        {
            return dbPass;
        }

        const std::string &getDbLogin() const
        {
            return dbLogin;
        }

        const std::string &getDbName() const
        {
            return dbName;
        }

        const std::string &getDbPort() const
        {
            return dbPort;
        }

        const std::string &getDbHost() const
        {
            return dbHost;
        }

        int getPort() const
        {
            return port;
        }

        unsigned int getMod() const
        {
            return mod;
        }

        unsigned int getPubKey() const
        {
            return pubKey;
        }

        unsigned int getPrivKey() const
        {
            return privKey;
        }

    private:
        unsigned privKey, pubKey, mod;
        int port;
        bool active, ready, run, exit_server;
        std::string dbHost, dbPort, dbName, dbLogin, dbPass;
        std::string filepath;
    public:
        Configuration();
        ~Configuration();
        void load(std::string file);
        void dynamicReconf();
        void stopInput();
        std::string toString();
        Configuration operator=(Configuration&);
        friend std::ostream& operator<<(std::ostream&, Configuration&);

        bool isReady();
        bool isRunning();
        bool exitServer();
    };

    inline std::ostream& operator<<(std::ostream& o , Configuration& c)
    {
        o << c.toString();
    }
}

#endif //NOTIFICATOR_CONFIGURATION_H
