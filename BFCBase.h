#ifndef __BFCBASE__H
#define __BFCBASE__H

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winuser.h>
#include <commdlg.h>

// I don't f... know why this is not defined....
#ifndef WM_MOUSEWHEEL
#define WM_MOUSEWHEEL                   0x020A
#endif

#else
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <BFCAssert.h>


#define _DEBUGV


enum BFCReturn
{
	bfc_error = -1000,
	bfc_not_found,
	bfc_bad_arg,
	bfc_not_enough_mem,
	bfc_arg_too_long,
	bfc_already_exist,
	bfc_already_made,
	bfc_already_in_the_state,
	bfc_read_error,
	bfc_in_use,
	bfc_ok = 0
};

enum BFCBigBaseType
{
	bfc_graphic_engine = 0
};

typedef unsigned int	BFCUInt;
typedef int				BFCInt;
typedef float			BFCFloat;
typedef bool			BFCBool;
typedef void			BFCVoid;
typedef char			BFCChar;
typedef unsigned char   BFCUChar;
typedef unsigned char   BFCByte;
typedef unsigned long   BFCDWord;
typedef unsigned long   BFCULong;
typedef long			BFCLong;
typedef short			BFCShort;
typedef unsigned short	BFCUShort;
typedef unsigned short	BFCWord;

const BFCFloat PI = 3.141592653589793238f;
const BFCFloat BFCPI = 3.141592653589793238f;
const BFCFloat BFCFarValue = 300000000.0f;
const BFCFloat BFCPrecision = 0.01f;


//********************************************************************
// These class are here for control and debug of all BFC class
//********************************************************************



// This is the most simple one no alloc of memory needed. Fast and frequent class should derive from this one
//!!! There must be no member for now !!!

struct BFCShortBase
{
};

struct BFCMediumBase
{
};

struct BFCBigBase
{
	BFCBigBaseType BigBaseType;
	BFCBigBase(BFCBigBaseType type) { BigBaseType = type; }
};







#endif