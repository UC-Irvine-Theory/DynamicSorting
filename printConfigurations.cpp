#include "initialConfiguration.h"

int
main()
{
    int seed = 0;

    InitialConfiguration config(seed);

    config.printConfigurations(1000);

    return 0;
}
