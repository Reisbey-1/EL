// ELLib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ELLib.h"


// This is an example of an exported variable
ELLIB_API int nELLib=0;

// This is an example of an exported function.
ELLIB_API int fnELLib(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see ELLib.h for the class definition
CELLib::CELLib()
{
    return;
}
