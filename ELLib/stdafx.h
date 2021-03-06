// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#import "../../bin/CanBusServer.tlb"				raw_interfaces_only, raw_native_types,   no_namespace ,  named_guids//

#include "LinkedList.h"
using namespace dtl;

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the ELLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// ELLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef ELLIB_EXPORTS
#define ELLIB_API __declspec(dllexport)
#else
#define ELLIB_API __declspec(dllimport)
#endif

// TODO: reference additional headers your program requires here
