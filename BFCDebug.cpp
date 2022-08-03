#include "BFCDebug.h"
#include "BFCManager.h"




BFCDebugLineMesh::BFCDebugLineMesh()
{
	BFCInt WhichDebugVisualizeCall = 1;
	for (BFCUInt i=0;i<MAX_DEBUG_LINE_MESH;i++)
	{
		LineMesh[i] = NULL;
	}
}

BFCVoid BFCDebugLineMesh::CreateDebugLine(BFCUInt line_mesh, BFCUInt num_line, BFCUInt which_call)
{
#ifdef _DEBUGV
	BFCChar text[256];
	WhichDebugVisualizeCall[line_mesh] = which_call;
	// if one already exist remove and delete it
	if (LineMesh[line_mesh])
	{
		mGraphicEngine->RemoveMesh(LineMesh[line_mesh]);
	}
	sprintf(text,"debug line %i",line_mesh);
	LineMesh[line_mesh] = new BFCMesh(lit_ut_line_mesh,text);
	LineMesh[line_mesh]->SetType2(interface_mesh);
	LineMesh[line_mesh]->SetNumData(num_line*2);
	Win32_lit_ut_mesh *ddata = (Win32_lit_ut_mesh *)LineMesh[line_mesh]->GetDataPointer();
	for (BFCUInt i=0;i<num_line;i++)
	{
		ddata[i].pos.x = 0.0f;
		ddata[i].pos.y = 0.0f;
		ddata[i].pos.z = 0.0f;
		ddata[i].color = 0xFF000000;
	}
	LineMesh[line_mesh]->DataSet();
	mGraphicEngine->AddMesh(LineMesh[line_mesh]);
#endif
}

BFCVoid BFCDebugLineMesh::SetDebugLine(BFCUInt line_mesh, BFCVector *v1, BFCVector *v2, BFCUInt num, BFCColorByte color)
{
#ifdef _DEBUGV
	Win32_lit_ut_mesh *ddata = (Win32_lit_ut_mesh *)LineMesh[line_mesh]->GetDataPointer();
	if (v1)
	{
		ddata[num*2].pos = *v1;
		ddata[num*2].color = color.color;
	}
	if (v2)
	{
		ddata[num*2+1].pos = *v2;
		ddata[num*2+1].color = color.color;
	}
#endif
}

BFCVoid BFCDebugLineMesh::DebugLineSet(BFCUInt line_mesh, BFCChar *msg)
{
#ifdef _DEBUGV
	LineMesh[line_mesh]->DataSet();
	switch (WhichDebugVisualizeCall[line_mesh])
	{
	case 1:
		{
			if (DebugVisualizeCall)
			{
				if (msg)
				{
					SendMessage(mhwnd,WM_SETTEXT,0,(long)msg);
				}
				DebugVisualize();
			}
		} break;
	case 2:
		{
			if (DebugVisualizeCall2)
			{
				if (msg)
				{
					SendMessage(mhwnd,WM_SETTEXT,0,(long)msg);
				}
				DebugVisualize2();
			}
		} break;
	}
#endif
}

BFCDebugLineMesh mDebugLineMesh;
