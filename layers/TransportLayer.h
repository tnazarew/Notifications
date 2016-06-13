//
// Created by tomasz on 10.06.16.
//

#ifndef NOTIFICATOR_TRANSPORTLAYER_H
#define NOTIFICATOR_TRANSPORTLAYER_H

#include "Layer.h"

namespace notifications
{
    class TransportLayer : public Layer
    {

    public:
        TransportLayer(Layer *);

        void shutdown();

        int recive(char *&, std::map<std::string, void *> &);

        int send(char *, std::map<std::string, void *> &);
    };
}

#endif //NOTIFICATOR_TRANSPORTLAYER_H
