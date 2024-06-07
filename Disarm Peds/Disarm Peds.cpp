// Disarm Peds.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "Disarm Peds.h"


// This is an example of an exported variable
DISARMPEDS_API int nDisarmPeds=0;

// This is an example of an exported function.
DISARMPEDS_API int fnDisarmPeds(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
CDisarmPeds::CDisarmPeds()
{
    return;
}
