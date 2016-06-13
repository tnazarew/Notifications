#include <iostream>
#include "Server.h"
#include <ApplicationLayer.h>
#include <SecurityLayer.h>
#include <TransportLayer.h>


int main()
{
    notifications::Server s(
            new notifications::ApplicationLayer(
                    new notifications::SecurityLayer(
                            new notifications::TransportLayer(NULL))));
    s.run();
}