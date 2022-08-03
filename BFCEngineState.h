#ifndef __BFCENGINESTATE__H
#define __BFCENGINESTATE__H


#include <BFCBase.h>

//************************************************************************
//
//						BFCEngineState
//
//************************************************************************

class BFCEngineState
{

	BFCBool ZBufferEnabled;

public:

	BFCEngineState();

	BFCVoid SetZBuffer(BFCBool val) { ZBufferEnabled = val; };

	BFCBool GetZBuffer() { return ZBufferEnabled; };

};







#endif