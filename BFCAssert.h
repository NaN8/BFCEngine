#ifndef __BFCASSERT__H
#define __BFCASSERT__H


#ifdef WIN32
	#include <assert.h>
	#ifdef _DEBUG
		#define BFCAssert(a,b) assert(a && b )
	#else
		#define BFCAssert(a,b) if (a==0) MessageBox(NULL,b,"Error in "CLASS"::"method"()",MB_OK)
	#endif
#else
	#define BFCAssert(a,b)
#endif







#endif __BFCASSERT__H