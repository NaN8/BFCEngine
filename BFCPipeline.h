#ifndef __BFCPIPELINE__H
#define __BFCPIPELINE__H

#include <BFCBase.h>
#include <BFCUtility.h>


enum BFCPipeLineType
{
	noneline = -1,
	graphicline
};

class BFCPipeline;			// Forward declaration

// Because the micro$ compiler doesn't let me cast from void (BFCMediumBase::*)() to void * I use a structure
// AnyWay I need also the this pointer

struct BFCPipeFunc : public BFCVoidNode
{
	BFCVoid (BFCPipeline::*pipe_func)();	// Pointer to the method
	BFCPipeline *ThisPtr;		// Pointer to the right Pipeline type
	BFCPipeFunc() : pipe_func(NULL), ThisPtr(NULL) { };
};

//************************************************************************
//
//						BFCPipeline
//
//************************************************************************


class BFCPipeline : public BFCMediumBase
{

protected:

	BFCPipeFunc *PipeFuncStruct;	// Temp pointer to pipe func structure
	void (BFCPipeline::*PFunc)();	// Temp pointer to function
	BFCPipeline *PThis;				// Pointer the to right pipeline type to be executed

	BFCUInt Counter;			// How many times executed
	BFCBool Executed;			// Executed in the actual cycle ?
	BFCPipeLineType Type;		// Pipeline Type
	BFCVoidList CallBack;		// Pipeline we must execute when we finished
	BFCVoidList Stalling;		// Pipeline that must be executed before we can execute
	BFCVoidList FunctionList;	// List of function to be called
	BFCChar Name[32];			// Name of this pipeline
	BFCBool Enabled;				// Pipeline enabled ?

	// Methods


public:

	BFCBool GetExecuted() { return Executed; }
	BFCBool GetEnablePipe() { return Enabled; }
	BFCVoid SetEnablePipe(BFCBool val) { Enabled = val; }
	BFCVoid SetExecuted(BFCBool val) { Executed = val; }
	BFCVoid AddFunction(BFCPipeFunc *PipeFunc, BFCBool put_begin = false);
	BFCVoid RemoveFunction(BFCPipeFunc *PipeFunc);
	BFCVoid RemoveAllFunction();	

	// Constructor

	BFCPipeline(BFCChar *name);
	BFCVoid Execute();

};








#endif