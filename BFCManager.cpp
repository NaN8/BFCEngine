#include "BFCManager.h"


BFCGraphicEngine *mGraphicEngine = NULL;
BFCManager *mManager = NULL;
BFCViewPort *mViewPort = NULL;
BFCEngineState *mEngineState = NULL;


//************************************************************************
//
//						BFCManager
//
//************************************************************************



BFCManager::BFCManager() : Pause(false)
{
	if (mManager!=NULL)
	{
		BFCAssert(0,"Trying to instanciate Manager twice");
		exit(1);
	}
	mManager = this;
	mGraphicEngine = &GraphicEngine;
	mViewPort = &(GraphicEngine.ViewPort);
}

BFCVoid BFCManager::Execute()
{
	if (!Pause)
	{
		GraphicEngine.Execute();
	}
}

BFCVoid BFCManager::Init(BFCUInt xres,BFCUInt yres)
{
	GraphicEngine.Init(xres,yres);
}

BFCVoid BFCManager::Close()
{
	GraphicEngine.Close();
}

BFCULong BFCManager::CalcKindCRC(BFCChar *p, BFCUInt len)
{
	BFCULong CRC = 0;
	BFCUInt Index = 0;
	while (Index<len)
	{
		switch(Index % 4)
		{
		case 0:
			{
				CRC += *p;
			} break;
		case 1:
			{
				CRC += *p * 256;
			} break;
		case 2:
			{
				CRC += *p * 65536;
			} break;
		case 3:
			{
				CRC += *p * 16777216;
			} break;
		}
		p++;
		Index++;
	}
	return CRC;
}