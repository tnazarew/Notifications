//
// Created by tomasz on 10.06.16.
//

#include "ApplicationLayer.h"

int notifications::ApplicationLayer::send(char *string, std::map<std::string, void *> &map)
{
    return lower_layer->send(string, map);
}

int notifications::ApplicationLayer::recive(char *&string, std::map<std::string, void *> &map)
{
    int code = lower_layer->recive(string, map);
    return code;
}



notifications::ApplicationLayer::ApplicationLayer(notifications::Layer *layer)
{
    lower_layer = layer;
}
