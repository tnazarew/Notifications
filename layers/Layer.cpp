//
// Created by tomasz on 10.06.16.
//

#include "Layer.h"

notifications::Layer::~Layer()
{

}

notifications::Layer::Layer(notifications::Layer *layer)
{
    lower_layer = layer;
}

notifications::Layer::Layer()
{

}
