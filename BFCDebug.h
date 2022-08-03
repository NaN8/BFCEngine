#ifndef __BFCDEBUG__H
#define __BFCDEBUG__H

#include "BFCGraphicEngine.h"


const BFCUInt MAX_DEBUG_LINE_MESH = 10;

struct BFCDebugLineMesh
{
	BFCInt WhichDebugVisualizeCall[MAX_DEBUG_LINE_MESH];
	BFCDebugLineMesh();
	BFCMesh *LineMesh[MAX_DEBUG_LINE_MESH];

	BFCVoid CreateDebugLine(BFCUInt line_mesh, BFCUInt num_line, BFCUInt which_call = 1);
	BFCVoid SetDebugLine(BFCUInt line_mesh, BFCVector *v1, BFCVector *v2, BFCUInt num, BFCColorByte color);
	BFCVoid DebugLineSet(BFCUInt line_mesh,BFCChar *msg = NULL);
	BFCVoid Disable(BFCUInt line_mesh) 
	{ 
#ifdef _DEBUGV
		LineMesh[line_mesh]->SetEnablePipe(false); 
#endif
	}
};

#ifdef _DEBUG
	extern BFCDebugLineMesh mDebugLineMesh;
#endif
 







#endif