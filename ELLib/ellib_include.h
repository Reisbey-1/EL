
// ELLib_include.h
#if !defined(ELLIb_H_INCLUDED_)
#define ELLIb_H_INCLUDED_

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

#include "ElevatorState.h" 
#include "Building.h"
#include "Elevator.h"
#include "Lift.h"
#include "RelativeRqst.h"
#include "defines.h"
#include "Passenger.h"
#include "Hall.h"

#endif // ELLIb_H_INCLUDED_