#ifndef __BFCMANAGER__H
#define __BFCMANAGER__H

#include "BFCGraphicEngine.h"
#include "BFCDebug.h"

//************************************************************************
//
//						BFCManager
//
//************************************************************************


class BFCManager
{
	
	BFCGraphicEngine GraphicEngine;
	BFCBool Pause;

public:

	BFCVoid Init(BFCUInt xres,BFCUInt yres);
	BFCVoid Execute();
	BFCVoid Close();

	BFCManager();

	BFCBool GetPause() { return Pause; }
	BFCVoid SetPause(BFCBool pause) { Pause = pause; }

	// Usefull routines

	BFCULong CalcKindCRC(BFCChar *p, BFCUInt len);

};

extern BFCGraphicEngine *mGraphicEngine;
extern BFCViewPort *mViewPort;
extern BFCManager *mManager;
extern BFCEngineState *mEngineState;
extern BFCDebugLineMesh mDebugLineMesh;




#endif __BFCMANAGER__H