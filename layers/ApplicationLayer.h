//
// Created by tomasz on 10.06.16.
//

#ifndef NOTIFICATOR_APPLICATIONLAYER_H
#define NOTIFICATOR_APPLICATIONLAYER_H

#include "Layer.h"

namespace notifications
{
    class ApplicationLayer : public Layer
    {
    public:
        ApplicationLayer(Layer*);



        int recive(char *&, std::map<std::string, void *> &);

        int send(char *, std::map<std::string, void *> &);
    };
}

#endif //NOTIFICATOR_APPLICATIONLAYER_H
