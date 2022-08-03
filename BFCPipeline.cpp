#include <BFCPipeline.h>
#include <BFCManager.h>





//************************************************************************
//
//						BFCPipeline
//
//************************************************************************

BFCPipeline::BFCPipeline(BFCChar *name) : Counter(0),Executed(false),Type(noneline), Enabled(true)
{
	BFCUInt len = strlen(name);
	if (len>32) len=32;	//Don't let len be more then 32
	memcpy(Name,name,len);
}

BFCVoid BFCPipeline::AddFunction(BFCPipeFunc *PipeFunc, BFCBool put_begin)
{
	FunctionList.Add(PipeFunc, put_begin);
}

BFCVoid BFCPipeline::RemoveFunction(BFCPipeFunc *PipeFunc)
{
	FunctionList.RemoveDelete(PipeFunc);
}

BFCVoid BFCPipeline::RemoveAllFunction()
{
	FunctionList.RemoveDeleteAll();
}

void BFCPipeline::Execute()
{
	BFCPipeline *pipeline;
	if (Enabled)
	{
		// Check stalling pipeline that could block this execute
		pipeline = (BFCPipeline *)Stalling.GetFirst();
		while (pipeline)
		{
			// if one pipeline has not been executed ignore this execute
			if (!pipeline->GetExecuted()) return;
			pipeline = (BFCPipeline *)Stalling.GetNext();
		}
		// Now execute the pipeline
			
		PipeFuncStruct = (BFCPipeFunc *)FunctionList.GetFirst();
		while (PipeFuncStruct)
		{
			PFunc = PipeFuncStruct->pipe_func;
			PThis = PipeFuncStruct->ThisPtr;
			(PThis->*PFunc)();
			PipeFuncStruct = (BFCPipeFunc *)FunctionList.GetNext();
		}
		Executed = true;
		
		// Check pipeline that should be called back
		pipeline = (BFCPipeline *)CallBack.GetFirst();
		while (pipeline)
		{
			pipeline->Execute();	// Execute the pipeline
			pipeline = (BFCPipeline *)CallBack.GetNext();
		}
	}
}



