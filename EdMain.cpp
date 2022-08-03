#include <EdMain.h>
#include <BFCManager.h>
#include <BFCSaveLoad.h>


MainEditor *mMainEditor = NULL;
static char gBuff[256];


//************************************************************************
//
//						MainEditor
//
//************************************************************************


MainEditor::MainEditor() :	LastMDownX(-1), LastMDownY(-1), XRot(0.0f), YRot(0.0f), ActViewPort(0),
							MoveInc(1.0f), LastSelectMesh(NULL), LastSelectVert(-1), LastPrim(NULL), LastPrimI(NULL),
							XRes(0), YRes(0), LastTValue(0.0f), Grid(NULL), LastSelectFace(-1), CoordDialog(NULL), 
							SaveTexture(NULL), NumberEqualVertex(0), BFCPipeline("Main Editor"), VertData(NULL),
							IndexData(NULL), MouseOutPosX(0), MouseOutPosY(0), PreLastSelectMesh(NULL)
{
	// Get actual x and y res;
#ifdef WIN32
	DesktopHDC = GetDC( GetDesktopWindow() );
	XRes = GetDeviceCaps( DesktopHDC, HORZRES );
	YRes = GetDeviceCaps( DesktopHDC, VERTRES );	
#endif
	mMainEditor = this;
}

BFCVoid MainEditor::Message(BFCChar *msg)
{
	SetAllKey(false);
#ifdef WIN32
	MessageBox(NULL,msg,"!",MB_OK);
#endif
}

BFCVoid MainEditor::Close()
{
	if (CoordDialog) delete CoordDialog;
}

BFCVoid MainEditor::Init()
{
	SelectTex = new BFCTexture(bfc_rgba_tex);
	SelectTex->LoadRawFile("Select.raw");
	//BFCColorByte color(0xFFFFFFFF);
	BFCColorByte color(0x33333333);
	mGraphicEngine->SetAmbient(&color);
	mGraphicEngine->SetPointSize(8.0f);
	InitView();
	AddBorders();
	InitDialog();
	InitPipeline();
}

BFCVoid MainEditor::InitDialog()
{
	BFCPipeFunc *func;
	CoordDialog = new BFCDoubleEdit("Coordinates values","U Coord :","V Coord :");
	// Set the pipeline callback
	func = new BFCPipeFunc();
	func->ThisPtr = this;
	func->pipe_func = (BFCVoid (BFCPipeline::*)()) CoordChanged;
	CoordDialog->SetEditCallback(func);
}

BFCVoid MainEditor::RefreshCam()
{
	mViewPort->SetRotateY(ActViewPort, YRot, false);
	mViewPort->SetRotateX(ActViewPort, XRot, true);
	mGraphicEngine->ViewPort.SetCameraPosition(ActViewPort,&CamPos);
}

// Use VTmp to change the cam pos
BFCVoid MainEditor::ChangeCamPos()
{
	MTmp = *mGraphicEngine->ViewPort.GetView(ActViewPort);
	// Matrix Inverse because it's a view one ( not very clear because directx does the inverse himself then? )
	MTmp.Inverse();
	// Transform cam move for taking care of cam orientation
	VTmp = MTmp.MulVecOnly(VTmp);
	// Set the new pos
	CamPos = CamPos + VTmp;
	RefreshCam();
}

BFCVoid MainEditor::CheckChangeView(BFCInt mx, BFCInt my)
{
	mGraphicEngine->TransWithRes(&mx,&my);
	if (!Keys.GetLMouse() && !Keys.GetMMouse() && !Keys.GetRMouse())
	{
		if (mx>(BFCInt)mViewPort->GetFrustum(ActViewPort)->width)
		{
			if (ActViewPort == 0) ActViewPort = 1; 
			if (ActViewPort == 2) ActViewPort = 3; 
		} else
		{
			if (ActViewPort == 1) ActViewPort = 0; 
			if (ActViewPort == 3) ActViewPort = 2; 
		}
		if (my>(BFCInt)mViewPort->GetFrustum(ActViewPort)->height)
		{
			if (ActViewPort == 0) ActViewPort = 2; 
			if (ActViewPort == 1) ActViewPort = 3; 
		} else
		{
			if (ActViewPort == 2) ActViewPort = 0; 
			if (ActViewPort == 3) ActViewPort = 1; 
		}
	}
	// Now we refresh some view value
	RefreshViewInfo();
}

BFCVoid MainEditor::RefreshViewInfo()
{
	CamPos = mViewPort->GetCameraPos(ActViewPort);
	XRot = mViewPort->GetXRot(ActViewPort);
	YRot = mViewPort->GetYRot(ActViewPort);
}

BFCVoid MainEditor::MouseOutScreen(BFCInt &mx, BFCInt &my)
{

	// Here we reset the mouse pos
	POINT mp;
	memset(&mp,0,sizeof(POINT));
	POINT k;
	memset(&k,0,sizeof(POINT));

	if (Keys.GetLMouse() || Keys.GetRMouse() || Keys.GetMMouse() )
	{	
 		GetCursorPos(&mp);
		if (mp.x<=7)
		{
			// This is to correct mx and my in mouse moved
			k.x=XRes-8;
			k.y=mp.y;
			// we need k in client coord because we will compare it with mx (client coord) in MouseMoved
			ScreenToClient(mhwnd,&k);
			// Here we substract the difference between current pos and new next pos
			MouseOutPosX -= MouseOutPosX+(k.x) - mx;
			SetCursorPos(XRes-8,mp.y);
			LastMDownX = k.x + MouseOutPosX;
			LastMDownY = k.y + MouseOutPosY;
		}
		if (mp.y<=7)
		{
			// This is to correct mx and my in mouse moved
			k.x=mp.x;
			k.y=YRes-8;
			// we need k in client coord because we will compare it with mx (client coord) in MouseMoved
			ScreenToClient(mhwnd,&k);
			// Here we substract the difference between current pos and new next pos
			MouseOutPosY -= MouseOutPosY+(k.y-8) - my;
			SetCursorPos(mp.x,YRes-8);
			LastMDownX = k.x + MouseOutPosX;
			LastMDownY = k.y + MouseOutPosY;
		}
		if (mp.x>=XRes-7)
		{
			// This is to correct mx and my in mouse moved
			k.x=8;
			k.y=mp.y;
			// we need k in client coord because we will compare it with mx (client coord) in MouseMoved
			ScreenToClient(mhwnd,&k);
			// Here we substract the difference between current pos and new next pos
			MouseOutPosX -= MouseOutPosX+(k.x) - mx;
			SetCursorPos(8,mp.y);
			LastMDownX = k.x + MouseOutPosX;
			LastMDownY = k.y + MouseOutPosY;
		}
		
		if (mp.y>=YRes-7)
		{
			// This is to correct mx and my in mouse moved
			k.x=mp.x;
			k.y=8;
			// we need k in client coord because we will compare it with mx (client coord) in MouseMoved
			ScreenToClient(mhwnd,&k);
			// Here we substract the difference between current pos and new next pos
			MouseOutPosY -= MouseOutPosY+(k.y-8) - my;
			SetCursorPos(mp.x,8);
			LastMDownX = k.x + MouseOutPosX;
			LastMDownY = k.y + MouseOutPosY;
		}
	}
}

// pos is pos of the last selected vector

BFCInt MainEditor::GetEqualVertex(BFCInt n, BFCVector pos)
	{
	NumberEqualVertex = 0;
	BFCInt found = -1;
	if (LastSelectMesh)
	{
		Win32_unlit_tex_ut_mesh *vert_data = (Win32_unlit_tex_ut_mesh *)LastSelectMesh->GetDataPointer();
		BFCWord *index_data = (BFCWord *)LastSelectMesh->GetDataPointer2();
		
		// Go through all the vertex
		for (BFCUInt i=0;i<LastSelectMesh->GetNumData2();i++)
		{
			if ( fabs(pos.x - vert_data[index_data[i]].pos.x) < BFCPrecision )
			if ( fabs(pos.y - vert_data[index_data[i]].pos.y) < BFCPrecision )
			if ( fabs(pos.z - vert_data[index_data[i]].pos.z) < BFCPrecision )
			{
				if ( LastSelectVert != (BFCInt)i )
				{
					NumberEqualVertex++;
					// if it's the right equal number use it as return value

					if (NumberEqualVertex == n)
					{
						found = i;
					}
				}
			}
		}
		LastSelectMesh->Data2Set();
		LastSelectMesh->DataSet();
	}
	// return vertex found can be -1 for no found
	return found;
}


BFCVoid MainEditor::FillVertexToMove()
{
	// we save the last selected vector pos for later use in GetEqualVertex
	BFCVector _vec = VertData[IndexData[LastSelectVert]].pos;
	BFCInt _i = 0;
	BFCInt _num_equal = 0;
	// Empty the list
	VertexMoveList.RemoveAll();
	// we add the first vertex to the list
	// !! everytime +1 because we use BFCVoid * list and we need to know if there is a null pointer !!
	VertexMoveList.Add((BFCVoid *)(BFCInt)(LastSelectVert+1));
	// if we move also equal vertex we check them
	if (EdState.GetGetPoint())
	{
		// Get number of them
		_num_equal = GetEqualVertex(1, _vec);
		while (_i<_num_equal)
		{
			// +1 because the first one is 1
			BFCInt _n = GetEqualVertex(_i+1, _vec);
			// if found one we add it
			if (_n!=-1) VertexMoveList.Add((BFCVoid *)(BFCInt)(_n+1));
			_i++;
		}
	}
}


BFCVoid MainEditor::VertexMove(BFCInt mx,BFCInt my)
{
	VertData = (Win32_unlit_tex_ut_mesh *)LastSelectMesh->GetDataPointer();

	if (mViewPort->GetType(ActViewPort) == bfc_perspec_proj)
	{
		// For perspec we don't use IsNearVert because of the frustum. SpectRe

		switch (LastSelectMesh->GetType())
		{
		case unlit_tex_ut_i_tri_mesh:
			{
				IndexData = (BFCWord *)LastSelectMesh->GetDataPointer2();
				// we fill the vertex list to move
				FillVertexToMove();
				// we get the first vertex to move
				// !! Don't forget -1 because of pointer recongnise
				BFCInt _i = (BFCInt)VertexMoveList.GetFirstPtr()-1;
				// -1 means null pointer
				while (_i != -1)
				{
					// we want a distance from the front plane constant so we get z dist in camera space.
					BFCFloat _z = mGraphicEngine->GetCameraZPosFromLocal(LastSelectMesh,ActViewPort,VertData[IndexData[_i]].pos);
					BFCLine line;
					// the line we'll use to get the point
					line  = mGraphicEngine->GetCameraLineFromPos(ActViewPort,mx,my);
					// we get the t param
					BFCFloat _t = line.GetTFromZ(_z);
					// we get the point in camera space
					BFCVector trans_pos = line.GetTPos(_t);
					// we transform the vector back with view matrix
					mGraphicEngine->CameraToLocal(LastSelectMesh,ActViewPort,&trans_pos);
					// we set the pos
					VertData[IndexData[_i]].pos = trans_pos;
					// we set also the editing point and line position
					Win32_lit_ut_mesh *_line = (Win32_lit_ut_mesh *)LastPrimI->LineMesh->GetDataPointer();
					LastPrimI->PointData[LastPrimI->PointIndex[_i]].pos = trans_pos;
					_line[LastPrimI->PointIndex[_i]].pos = trans_pos;
					// we get the next value
					_i = (BFCInt)VertexMoveList.GetNextPtr()-1;
				}

				LastPrimI->Data2Set();
				LastPrimI->LineMesh->DataSet();
			} break;
		case unlit_tex_ut_tri_mesh:
			{
				// we want a distance from the front plane constant so we get z dist in camera space.
				BFCFloat _z = mGraphicEngine->GetCameraZPosFromLocal(LastSelectMesh,ActViewPort,VertData[LastSelectVert].pos);
				BFCLine line;
				// the line we'll use to get the point
				line  = mGraphicEngine->GetCameraLineFromPos(ActViewPort,mx,my);
				// we get the t param
				BFCFloat _t = line.GetTFromZ(_z);
				// we get the point in camera space
				BFCVector trans_pos = line.GetTPos(_t);
				// we transform the vector back with view matrix
				mGraphicEngine->CameraToLocal(LastSelectMesh,ActViewPort,&trans_pos);
				// we set the pos
				VertData[LastSelectVert].pos = trans_pos;
				// we set also the editing point position
				LastPrim->PointData[LastSelectVert].pos = trans_pos;
			} break;
		}
	} else
	{
		// For ortho we get the line from pick point
		BFCLine line = mGraphicEngine->GetLocalLineFromPos(LastSelectMesh,ActViewPort,mx,my);

		switch (LastSelectMesh->GetType())
		{
		case unlit_tex_ut_i_tri_mesh:
			{
				IndexData = (BFCWord *)LastSelectMesh->GetDataPointer2();
				// we fill the vertex list to move
				FillVertexToMove();
				// we get the first vertex to move
				// !! Don't forget -1 because of pointer recongnise
				BFCInt _i = (BFCInt)VertexMoveList.GetFirstPtr()-1;
				// -1 means null pointer
				while (_i != -1)
				{
					// we get the nearest point from the line and the vertex
					line.IsNearVert(VertData[IndexData[_i]].pos,1.0f);
					BFCVector trans_pos = line.LastPointIntersect;
					// we set the pos
					if (EdState.GetStickGrid()) StickPoint(LastSelectMesh, &trans_pos);
					VertData[IndexData[_i]].pos = trans_pos;
					// we set also the editing point and line position
					Win32_lit_ut_mesh *_line = (Win32_lit_ut_mesh *)LastPrimI->LineMesh->GetDataPointer();
					LastPrimI->PointData[LastPrimI->PointIndex[_i]].pos = trans_pos;
					_line[LastPrimI->PointIndex[_i]].pos = trans_pos;
					// we get the next value
					_i = (BFCInt)VertexMoveList.GetNextPtr()-1;
				}
				LastPrimI->Data2Set();
				LastPrimI->LineMesh->DataSet();
			} break;
		case unlit_tex_ut_tri_mesh:
			{
				// we get the nearest point from the line and the vertex
				line.IsNearVert(VertData[LastSelectVert].pos,1.0f);
				BFCVector trans_pos = line.LastPointIntersect;
				// we set the pos
				VertData[LastSelectVert].pos = trans_pos;
				// we set also the editing point position
				LastPrim->PointData[LastSelectVert].pos = trans_pos;
			} break;
		}
	}
	LastSelectMesh->DataSet();
}

BFCVoid MainEditor::ObjectMove(BFCInt mx,BFCInt my)
{
	// difference from vert and center of the mesh
	BFCVector diff;
	BFCVector picked_point = *mGraphicEngine->GetLocalPickedPoint();
	BFCVector mesh_pos = LastSelectMesh->M.GetPos();
	diff = picked_point - mesh_pos; 

	// we want a distance from the front plane constant so we get z dist in camera space.
	BFCFloat _z = mGraphicEngine->GetCameraZPosFromLocal(LastSelectMesh,ActViewPort,picked_point);
	BFCLine line;
	// the line we'll use to get the point
	line  = mGraphicEngine->GetCameraLineFromPos(ActViewPort,mx,my);
	// we get the t param
	BFCFloat _t = line.GetTFromZ(_z);
	// we get the point in camera space
	BFCVector trans_pos = line.GetTPos(_t);
	// we transform the vector back with view matrix
	mGraphicEngine->CameraToWorld(LastSelectMesh,ActViewPort,&trans_pos);
	// we add the difference
	//trans_pos = trans_pos + diff;
	// we transform picked_point to world
	// we don't need the pos because the difference between the center and vert is not influenced by the transformed by the pos
	picked_point = LastSelectMesh->M.MulVecOnly(picked_point);

	// we set the pos
	LastSelectMesh->M.SetPos(trans_pos-picked_point);
}

//******************************************************************************
//
//						MouseMoved
//
//******************************************************************************


BFCVoid MainEditor::MouseMoved(BFCInt mx, BFCInt my)
{
	// Here we transform mx and my to the right value since there is an intern overflow (on a WORD) in WIN32. SpectRe
	if (mx>32768)
	{
		mx-=65536;
	}
	if (my>32768)
	{
		my-=65536;
	}

	// Correct mx and my if they were out sometime
	mx += MouseOutPosX;
	my += MouseOutPosY;

	BFCBool move_something = true;
	// Check if there is a view rect change
	CheckChangeView(mx,my);
	if (Keys.GetLMouse())
	{
		// if Ctrl down it's a view change
		if (Keys.GetControl())
		{
			YRot -= (PI/1000.0f)*(mx-LastMDownX);
			XRot -= (PI/1000.0f)*(my-LastMDownY);
			RefreshCam();
			move_something = false;
		} 
		
		// Here it's rotation around x
		if (Keys.GetX())
		{
			if (LastSelectMesh)
			{
				BFCFloat ang = (BFCFloat)-(my-LastMDownY)/40;
				LastSelectMesh->RotateX(ang);
			}
		}

		// Here it's rotation around y
		if (Keys.GetY())
		{
			if (LastSelectMesh)
			{
				BFCFloat ang = (BFCFloat)-(mx-LastMDownX)/40;
				LastSelectMesh->RotateY(ang);
			}
		}

		// Here it's rotation around z
		if (Keys.GetZ())
		{
			if (LastSelectMesh)
			{
				BFCFloat ang = (BFCFloat)-(mx-LastMDownX)/40;
				LastSelectMesh->RotateZ(ang);
			}
		}

		if (Keys.GetShift())
		{
			if (move_something)
			{
				// here it's an object move
				if (LastSelectMesh)
				{
					ObjectMove(mx,my);
				}
			}
		} else
		{
			if (move_something)
			{
			// Here it's vertex move
			if (LastSelectMesh)
			{
				// if a vertex is selected
				if (LastSelectVert != -1)
				{
					/*
					// increment vector
					BFCVector inc;
					inc.x = (BFCFloat)(mx-LastMDownX)/2;
					inc.y = (BFCFloat)-(my-LastMDownY)/2;*/
/*					// Transform inc vector with cam view
					BFCMatrix mat = *mViewPort->GetView(ActViewPort);
					mat.Inverse();
					inc = mat.MulVecOnly(inc);
					// Transform inc vector with obj mat
					mat = LastSelectMesh->M;
					mat.Inverse();
					inc = mat.MulVecOnly(inc);*/
					VertexMove(mx,my);
				}
			}
			} // end of move_something
			move_something = false;
		}
		LastMDownX = mx;
		LastMDownY = my;


		MouseOutScreen(mx,my);

	}

//	*********************************
//
//			RMouseDown
//
//	*********************************

	
	if (Keys.GetRMouse())
	{
		MouseOutScreen(mx,my);
		
		if (LastSelectMesh)
		{
			BFCBool done_scale = false;
			BFCFloat factor;
			if (Keys.GetX())
			{
				factor = (BFCFloat)(mx-LastMDownX)/40.0f;
				BFCVector _scale = LastSelectMesh->GetScale();
				_scale.x = _scale.x * (1.0f + factor);

				switch (LastSelectMesh->GetType())
				{
				case unlit_tex_ut_i_tri_mesh:
					{
						LastPrimI->SetScale(_scale);
					} break;
				case unlit_tex_ut_tri_mesh:
					{
						LastPrim->SetScale(_scale);
					} break;
				}
				done_scale = true;
			}
			if (Keys.GetY())
			{
				factor = (BFCFloat)(mx-LastMDownX)/40.0f;
				BFCVector _scale = LastSelectMesh->GetScale();
				_scale.y = _scale.y * (1.0f + factor);

				switch (LastSelectMesh->GetType())
				{
				case unlit_tex_ut_i_tri_mesh:
					{
						LastPrimI->SetScale(_scale);
					} break;
				case unlit_tex_ut_tri_mesh:
					{
						LastPrim->SetScale(_scale);
					} break;
				}
				done_scale = true;
			}
			if (Keys.GetZ())
			{
				factor = (BFCFloat)(mx-LastMDownX)/40.0f;
				BFCVector _scale = LastSelectMesh->GetScale();
				_scale.z = _scale.z * (1.0f + factor);

				switch (LastSelectMesh->GetType())
				{
				case unlit_tex_ut_i_tri_mesh:
					{
						LastPrimI->SetScale(_scale);
					} break;
				case unlit_tex_ut_tri_mesh:
					{
						LastPrim->SetScale(_scale);
					} break;
				}
				done_scale = true;
			}
			if (!done_scale)
			{
				factor = (BFCFloat)(mx-LastMDownX)/40.0f;
				BFCVector _scale = LastSelectMesh->GetScale();
				_scale = _scale * (1.0f + factor);

				switch (LastSelectMesh->GetType())
				{
				case unlit_tex_ut_i_tri_mesh:
					{
						LastPrimI->SetScale(_scale);
					} break;
				case unlit_tex_ut_tri_mesh:
					{
						LastPrim->SetScale(_scale);
					} break;
				}
				done_scale = true;
			}
		}
		LastMDownX = mx;
		LastMDownY = my;


		MouseOutScreen(mx,my);
	}
}

BFCVoid MainEditor::StickPoint(BFCMesh *mesh, BFCVector *vector)
{
	BFCInt size = Grid->GetSize();
	BFCInt def = Grid->GetDef();
	BFCFloat unit = (BFCFloat)(size / def);
	// transfrom vector with obj mat
	*vector = mesh->M * (*vector);
	switch (mViewPort->GetViewType( ActViewPort ) )
	{
	case bfc_perspec_view:
		{
		} break;
	case bfc_front_view:
		{
			BFCInt mul = (BFCInt)(vector->x / unit);
			BFCFloat val = unit * mul;
			if ( vector->x - val > unit/2.0f)
			{
				vector->x = unit * (mul+1);
			} else
			{
				if ( vector->x - val < -unit/2.0f)
				{
					vector->x = unit * (mul-1);
				} else vector->x = unit * mul;
			}
			// Now y
			mul = (BFCInt)(vector->y / unit);
			val = unit * mul;
			if ( vector->y - val > unit/2.0f)
			{
				vector->y = unit * (mul+1);
			} else
			{
				if ( vector->y - val < -unit/2.0f)
				{
					vector->y = unit * (mul-1);
				} else vector->y = unit * mul;
			}
		} break;
	case bfc_top_view:
		{
			BFCInt mul = (BFCInt)(vector->x / unit);
			BFCFloat val = unit * mul;
			if ( vector->x - val > unit/2.0f)
			{
				vector->x = unit * (mul+1);
			} else
			{
				if ( vector->x - val < -unit/2.0f)
				{
					vector->x = unit * (mul-1);
				} else vector->x = unit * mul;
			}
			// Now z
			mul = (BFCInt)(vector->z / unit);
			val = unit * mul;
			if ( vector->z - val > unit/2.0f)
			{
				vector->z = unit * (mul+1);
			} else
			{
				if ( vector->z - val < -unit/2.0f)
				{
					vector->z = unit * (mul-1);
				} else vector->z = unit * mul;
			}
		} break;
	case bfc_left_view:
		{
			BFCInt mul = (BFCInt)(vector->z / unit);
			BFCFloat val = unit * mul;
			if ( vector->z - val > unit/2.0f)
			{
				vector->z = unit * (mul+1);
			} else
			{
				if ( vector->z - val < -unit/2.0f)
				{
					vector->z = unit * (mul-1);
				} else vector->z = unit * mul;
			}
			// Now y
			mul = (BFCInt)(vector->y / unit);
			val = unit * mul;
			if ( vector->y - val > unit/2.0f)
			{
				vector->y = unit * (mul+1);
			} else
			{
				if ( vector->y - val < -unit/2.0f)
				{
					vector->y = unit * (mul-1);
				} else vector->y = unit * mul;
			}
		} break;
	}
	// transfrom back the vector
	BFCMatrix mat = mesh->M;
	BFCVector pos = mat.GetPos();
	mat.Inverse();
	pos = mat.MulVecOnly(pos);
	*vector = mat.MulVecOnly(*vector) - pos;
	//*vector = mat.MulVecOnly(*vector);
}

BFCVoid MainEditor::UpArrow()
{
	VTmp.Zero();
	if (mViewPort->GetViewType(ActViewPort) == bfc_perspec_view)
	{
		VTmp.z = MoveInc;
	} else
	{
		VTmp.y = MoveInc;
	}
	ChangeCamPos();
}

BFCVoid MainEditor::DownArrow()
{
	VTmp.Zero();
	if (mViewPort->GetViewType(ActViewPort) == bfc_perspec_view)
	{
		VTmp.z = -MoveInc;
	} else
	{
		VTmp.y = -MoveInc;
	}
	ChangeCamPos();
}

BFCVoid MainEditor::LeftArrow()
{
	VTmp.Zero();
	VTmp.x = -MoveInc;
	ChangeCamPos();
}

BFCVoid MainEditor::RightArrow()
{
	VTmp.Zero();
	VTmp.x = MoveInc;
	ChangeCamPos();
}

BFCVoid MainEditor::HomeDown()
{
	if (LastSelectFace != -1)
	{
		for (BFCUInt i=0;i<3;i++)
		{
			TmpMeshData = LastSelectMesh->GetMeshData(LastSelectFace*3+i);
			TmpMeshData.v -= 0.1f;
			LastSelectMesh->SetMeshData(LastSelectFace*3+i, &TmpMeshData);
		}
	}
}

BFCVoid MainEditor::EndDown()
{
	if (LastSelectFace != -1)
	{
		for (BFCUInt i=0;i<3;i++)
		{
			TmpMeshData = LastSelectMesh->GetMeshData(LastSelectFace*3+i);
			TmpMeshData.v += 0.1f;
			LastSelectMesh->SetMeshData(LastSelectFace*3+i, &TmpMeshData);
		}
	}
}

BFCVoid MainEditor::PageDown()
{
	switch (EdState.GetEdMode())
	{
	case bfc_normal_mode:
		{
			MouseWheel(-5);
		} break;
	case bfc_face_mode:
		{
			if (LastSelectFace != -1)
			{
				for (BFCUInt i=0;i<3;i++)
				{
					TmpMeshData = LastSelectMesh->GetMeshData(LastSelectFace*3+i);
					TmpMeshData.u += 0.1f;
					//TmpMeshData.v += 0.1f;
					LastSelectMesh->SetMeshData(LastSelectFace*3+i, &TmpMeshData);
				}
			}
		} break;
	}
}

BFCVoid MainEditor::PageUp()
{
	switch (EdState.GetEdMode())
	{
	case bfc_normal_mode:
		{
			MouseWheel(5);
		} break;
	case bfc_face_mode:
		{
			if (LastSelectFace != -1)
			{
				for (BFCUInt i=0;i<3;i++)
				{
					TmpMeshData = LastSelectMesh->GetMeshData(LastSelectFace*3+i);
					TmpMeshData.u -= 0.1f;
					//TmpMeshData.v += 0.1f;
					LastSelectMesh->SetMeshData(LastSelectFace*3+i, &TmpMeshData);
				}
			}
		} break;
	}
}

BFCVoid MainEditor::SetAllKey(BFCBool val)
{
	AltIsDown(val);
	CtrlIsDown(val);
	ShiftIsDown(val);
	MIsDown(val);
	XIsDown(val);
	YIsDown(val);
	ZIsDown(val);
	EIsDown(val);
	PIsDown(val);
}

BFCVoid MainEditor::MIsDown(BFCBool val)
{
	if (val)
	{
		if (EdState.GetEdMode() < bfc_ed_max_mode -1) EdState.SetEdMode( (BFCEditingMode)(EdState.GetEdMode() + 1) ); 
		else EdState.SetEdMode( (BFCEditingMode)0 );
	}
}

BFCVoid MainEditor::PIsDown(BFCBool val)
{
	Keys.PIsDown(val);
	if (val)
	{
		if (EdState.GetGetPoint())
		{
			EdState.SetGetPoint(false);
		} else
		{
			EdState.SetGetPoint(true);
		}
	}
}

BFCVoid MainEditor::XIsDown(BFCBool val)
{
	Keys.XIsDown(val);
}

BFCVoid MainEditor::YIsDown(BFCBool val)
{
	Keys.YIsDown(val);
}


BFCVoid MainEditor::ZIsDown(BFCBool val)
{
	Keys.ZIsDown(val);
}

BFCVoid MainEditor::OIsDown(BFCBool val)
{
	Keys.OIsDown(val);
	if (Keys.GetControl())
	{
		switch (EdState.GetEdMode())
		{
		case bfc_face_mode:
			{
				LoadTexture();
			} break;
		case bfc_normal_mode:
			{
				Load();
			} break;			
		}
	}
}

BFCVoid MainEditor::SIsDown(BFCBool val)
{
	Keys.SIsDown(val);
	if (Keys.GetControl())
	{
		switch (EdState.GetEdMode())
		{
		case bfc_face_mode:
			{
			} break;
		case bfc_normal_mode:
			{
				Save();
			} break;			
		}
	}
}

BFCVoid MainEditor::EIsDown(BFCBool val)
{
	Keys.EIsDown(val);
	if (val)
	{
		if (EdState.GetStickGrid()) EdState.SetStickGrid(false); else EdState.SetStickGrid(true);
	}
}

BFCVoid MainEditor::AltIsDown(BFCBool val)
{
	Keys.AltIsDown(val);
}

BFCVoid MainEditor::CtrlIsDown(BFCBool val)
{
	Keys.CtrlIsDown(val);
}

BFCVoid MainEditor::ShiftIsDown(BFCBool val)
{
	Keys.ShiftIsDown(val);
}

BFCVoid MainEditor::RMouseIsDown(BFCInt mx, BFCInt my)
{
	Keys.RMouseIsDown(true);
	MouseIsDown(mx,my);
}

BFCVoid MainEditor::RMouseIsUp(BFCInt mx, BFCInt my)
{
	// Set mouse out manage var to 0
	MouseOutPosX = 0;
	MouseOutPosY = 0;
#ifdef WIN32
	ReleaseCapture();
	SetCursor(LoadCursor(NULL, IDC_ARROW));
#endif
	Keys.RMouseIsDown(false);
}

BFCVoid MainEditor::Deselect()
{
	if (LastSelectMesh)
	{
		// we check if a face before has been selected
		if (LastSelectFace != -1)
		{
			// Set line of the face to the right color
			SetSelectedFaceColor(0xFFFFFFFF);

			// if so we set back the normal texture
			//LastSelectMesh->InsertTexture(SaveTexture,LastSelectFace*3,LastSelectFace*3+3);
		}
		switch (LastSelectMesh->GetType())
		{
		case unlit_tex_ut_i_tri_mesh:
			{
				LastPrimI = (BFCIPrimitive *) LastSelectMesh;
				LastPrimI->SetPointColor(0xFFFFFFFF);		
				// if a vertex was selected
				if (LastSelectVert!=-1)
				{
					LastPrimI->SetPointColor(0xFFFFFFFF,mGraphicEngine->GetPickedVert() );						
				}
			} break;
		case unlit_tex_ut_tri_mesh:
			{
				LastPrim = (BFCPrimitive *) LastSelectMesh;
				LastPrim->SetPointColor(0xFFFFFFFF);		
				// if a vertex was selected
				if (LastSelectVert!=-1)
				{
					LastPrim->SetPointColor(0xFFFFFFFF,mGraphicEngine->GetPickedVert() );
				}
			} break;
		}
	}
	LastSelectFace = -1;
	LastSelectVert = -1;
	PreLastSelectMesh = LastSelectMesh;
	LastSelectMesh = NULL;
}

BFCVoid MainEditor::SelectObject(BFCInt mx,BFCInt my)
{
	BFCMesh *mesh = mGraphicEngine->PickMesh(ActViewPort,mx,my);
	// if object selected
	if (mesh)
	{
		// Refresh new last select primitive
		LastSelectMesh = mesh;
		// Set point of the prim to the right color
		switch (mesh->GetType())
		{
		case unlit_tex_ut_i_tri_mesh:
			{
				LastPrimI = (BFCIPrimitive *) mesh;
				LastPrimI->SetPointColor(0xFFF0000F);		
			} break;
		case unlit_tex_ut_tri_mesh:
			{
				LastPrim = (BFCPrimitive *) mesh;
				LastPrim->SetPointColor(0xFFF0000F);
			} break;
		}
	} else
	{
		LastSelectMesh = NULL;
	}
}

BFCVoid MainEditor::SelectFace(BFCInt mx,BFCInt my)
{
	BFCMesh *mesh = mGraphicEngine->PickMesh(ActViewPort,mx,my);
	// if an object face selected
	if (mesh)
	{
		LastSelectMesh = mesh;
		LastSelectFace = mGraphicEngine->GetPickedTri();
		// Set line of the face to the right color
		SetSelectedFaceColor(0xFFF0000F);
		// Set the SelectTex to indicate the face has been selected
		//SaveTexture = mesh->InsertTexture(SelectTex,LastSelectFace*3,LastSelectFace*3+3);
	} else
	{
	}
}

BFCVoid MainEditor::SelectVert(BFCInt mx,BFCInt my)
{
	BFCMesh *mesh = mGraphicEngine->PickVert(ActViewPort,mx,my);
	LastTValue = mGraphicEngine->GetLastTValue();

	if (mesh)
	{
	#ifdef WIN32
		SetCursor(LoadCursor(NULL, IDC_CROSS));
	#endif
		LastSelectMesh = mesh;
		switch (mesh->GetType())
		{
		case unlit_tex_ut_i_tri_mesh:
			{
				LastPrimI = (BFCIPrimitive *) mesh;
				LastPrimI->SetPointColor(0xFFF0000F,mGraphicEngine->GetPickedVert() );
			} break;
		case unlit_tex_ut_tri_mesh:
			{
				LastPrim = (BFCPrimitive *) mesh;
				LastPrim->SetPointColor(0xFFF0000F,mGraphicEngine->GetPickedVert() );
			} break;
		}
		LastSelectVert = mGraphicEngine->GetPickedVert();
	} else
	{
		// No vertex selected
		LastSelectVert = -1;

	}
	RefreshCoord();
}

BFCVoid MainEditor::LMouseIsDown(BFCInt mx, BFCInt my)
{
	Keys.LMouseIsDown(true);
	MouseIsDown(mx,my);
}

BFCVoid MainEditor::MouseIsDown(BFCInt mx, BFCInt my)
{
#ifdef WIN32
	SetCapture(mhwnd);
#endif
	LastMDownX = mx;
	LastMDownY = my;
	// Set last primitive back to normal color ( if one was selected )
	if (Keys.GetControl())
	{
	} else
	{
		// we deselect last object and vert if they were selected
		Deselect();

		switch (EdState.GetEdMode())
		{
		case bfc_normal_mode:
			{
				if (Keys.GetShift() || Keys.GetX() || Keys.GetY() || Keys.GetZ())
				{
					// Here it's an object selection
					SelectObject(mx,my);
				} else
				{
					// Here it's a vertex selection
					SelectVert(mx,my);
				}				
			} break;
		case bfc_face_mode:
			{
				SelectFace(mx,my);
			} break;
		}
	}
}

BFCVoid MainEditor::LMouseIsUp(BFCInt mx, BFCInt my)
{
	// Set mouse out manage var to 0
	MouseOutPosX = 0;
	MouseOutPosY = 0;
#ifdef WIN32
	ReleaseCapture();
	SetCursor(LoadCursor(NULL, IDC_ARROW));
#endif
	Keys.LMouseIsDown(false);
	LastMDownX = mx;
	LastMDownY = my;
}

BFCVoid MainEditor::MouseWheel(BFCShort val)
{
	if (mViewPort->GetType(ActViewPort)== bfc_perspec_proj)
	{
		if (val < 0)
		{
			VTmp.Zero();
			VTmp.z = -MoveInc * 4;
			ChangeCamPos();
		} else
		{
			VTmp.Zero();
			VTmp.z = MoveInc * 4;
			ChangeCamPos();
		}
	} else
	{
		BFCFloat scale = mViewPort->GetScale(ActViewPort);
		if (val < 0)
		{
			// Set scale
			scale *= 0.8f;
		} else
		{
			// Set scale
			scale *= 1.2f;
		}
		mViewPort->SetScale(ActViewPort,scale);
	}
}

BFCVoid MainEditor::InitView()
{
	BFCMatrix view;
	BFCMatrix proj;
	BFCFrustum fru;

	// Editor has four viewport by default
	mGraphicEngine->ViewPort.AddViewPort();
	mGraphicEngine->ViewPort.AddViewPort();
	mGraphicEngine->ViewPort.AddViewPort();

	// Set all four viewports settings

	// Set View Matrix
	view.Identity();
	mGraphicEngine->ViewPort.SetView(0,&view);
	// Set Projection matrix
	proj.ProjectionMatrix(bfc_perspec_proj,1.0f,10000.0f,PI*0.3333f,PI*0.3333f); // 0.3333 ~= 1/3
	mGraphicEngine->ViewPort.SetProj(0,&proj,bfc_perspec_proj);	
	// Set Actual Client screen Frustum
	fru.width = m_x / 2;
	fru.height = m_y / 2;
	mGraphicEngine->ViewPort.SetFrustum(0,&fru);
	mViewPort->SetViewType(1,bfc_perspec_view);

	// Set View Matrix
	mGraphicEngine->ViewPort.SetView(1,&view);
	// Set Actual Client screen Frustum
	fru.x = m_x / 2;
	mGraphicEngine->ViewPort.SetFrustum(1,&fru);
	// We set the front view for this viewport
	mViewPort->SetViewType(1,bfc_front_view);

	// Set View Matrix
	mGraphicEngine->ViewPort.SetView(2,&view);
	// Set Actual Client screen Frustum
	fru.x = 0;
	fru.y = m_y / 2;
	mGraphicEngine->ViewPort.SetFrustum(2,&fru);
	// We set the top view for this viewport
	mViewPort->SetViewType(2,bfc_top_view);


	// Set View Matrix
	mGraphicEngine->ViewPort.SetView(3,&view);
	// Set Actual Client screen Frustum
	fru.x = m_x / 2;
	fru.y = m_y / 2;
	mGraphicEngine->ViewPort.SetFrustum(3,&fru);
	// We set the top left view for this viewport
	mViewPort->SetViewType(3,bfc_left_view);

	Grid = new BFCGrid(40,400);
};

BFCVoid MainEditor::AddBorders()
{
	int i = 0;
	BFCMesh *border =  new BFCMesh(lit_t_line_mesh,"Ed_Border");
	border->SetNumData(4);
	Win32_lit_t_mesh *data = (Win32_lit_t_mesh *) border->GetDataPointer();
	data[0].pos.x = (float)m_x / 2;data[0].pos.y = 0.0f;data[0].pos.z = 0.5f;data[0].rhw = 1.0f;data[0].color = 0xFFFFFFFF;
	data[1].pos.x = (float)m_x / 2;data[1].pos.y = (float)m_y;data[1].pos.z = 0.5f;data[1].rhw = 1.0f;data[1].color = 0xFFFFFFFF;

	data[2].pos.x = 0.0f;data[2].pos.y = (float)m_y / 2;data[2].pos.z = 0.5f;data[2].rhw = 1.0f;data[2].color = 0xFFFFFFFF;
	data[3].pos.x = (float)m_x;data[3].pos.y = (float)m_y / 2;data[3].pos.z = 0.5f;data[3].rhw = 1.0f;data[3].color = 0xFFFFFFFF;

	border->SetType2(interface_mesh);
	mGraphicEngine->AddMesh(border);
}

//********************************************************************************
//
//
//						Saving Loading Functions
//
//
//********************************************************************************

BFCVoid MainEditor::Save()
{
	BFCSaveLoad save;
	SetAllKey(false);
	BFCChar file_choosed[80];
	file_choosed[0]='\0';
	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL; 
	ofn.lpstrFileTitle= NULL; 
	ofn.nMaxFileTitle = 0; 
	ofn.lpstrFilter = "Scene files\0*.scn\0"; 
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = file_choosed; 
	ofn.nMaxFile = 80; 
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER; 

	if (GetSaveFileName(&ofn))
	{
		BFCSaveLoad save;
		save.WriteScene(file_choosed);
	}
}

BFCVoid MainEditor::Load()
{
	BFCChar file_choosed[80];
	SetAllKey(false);
	file_choosed[0]='\0';
	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL; 
	ofn.lpstrFileTitle= NULL; 
	ofn.nMaxFileTitle = 0; 
	ofn.lpstrFilter = "Scene files\0*.scn\0"; 
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = file_choosed; 
	ofn.nMaxFile = 80; 
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER; 

	if (GetOpenFileName(&ofn))
	{
		BFCSaveLoad load;
		BFCBool pause_state = mManager->GetPause();
		// we pause all pipelines for the moment
		mManager->SetPause(true);
		// First we empty the scene
		EmptyScene();	
		// then we load the new scene
		load.Load(file_choosed);
		// Set pause as before
		mManager->SetPause(pause_state);
	}
}

BFCVoid MainEditor::RemoveAllMeshScene()
{
	BFCBool pause_state = mManager->GetPause();
	// we pause all pipelines for the moment
	mManager->SetPause(true);
	BFCMesh *mesh = (BFCMesh *)mGraphicEngine->GetMeshTree()->GetFirst();
	while (mesh)
	{
		if (mesh->GetType() == unlit_tex_ut_i_tri_mesh)
		{
			BFCIPrimitive *prim = (BFCIPrimitive *) mesh;
			delete prim;
			// we don't know how many mesh were cancelled so we read again from the beginning
			mesh = (BFCMesh *)mGraphicEngine->GetMeshTree()->GetFirst();
		} else mesh = (BFCMesh *)mGraphicEngine->GetMeshTree()->GetNext();
	}
	// Set pause as before
	mManager->SetPause(pause_state);
}

BFCVoid MainEditor::EmptyScene()
{
	BFCBool pause_state = mManager->GetPause();
	// we pause all pipelines for the moment
	mManager->SetPause(true);
	RemoveAllMeshScene();
	mGraphicEngine->ViewPort.DeleteAllViewPort();
	// Set pause as before
	mManager->SetPause(pause_state);
}

//********************************************************************************
//
//
//						Editing Functions
//
//
//********************************************************************************

BFCVoid MainEditor::BoolSubstract()
{
	if (PreLastSelectMesh)
	{
		if (LastSelectMesh)
		{
			if (LastSelectMesh!=PreLastSelectMesh)
			{
				mGraphicEngine->BoolSubstract(PreLastSelectMesh,LastSelectMesh);
			} else
			{
				Message("Only one mesh has been selected for the boolean substraction, select 2 mesh one after the other");
			}
		} else
		{
			Message("The last two selected mesh will do the boolean substraction, so select two mesh");
		}
	} else
	{
		Message("The last two selected mesh will do the boolean substraction, so select two mesh");
	}
}

//********************************************************************************
//
//
//						Primitive Creation methods
//
//
//********************************************************************************

BFCVoid MainEditor::SetSelectedFaceColor(BFCUInt color)
{
	Win32_lit_ut_mesh data;
	if (LastSelectFace != -1)
	{
		LastPrimI = (BFCIPrimitive *)LastSelectMesh;
		for (int i=0;i<6;i++)
		{
			data = LastPrimI->LineMesh->GetMeshLitData(LastSelectFace*6+i);
			data.color = color;
			LastPrimI->LineMesh->SetMeshLitData(LastSelectFace*6+i, &data);
		}
	}
}

BFCVoid MainEditor::CreateCube()
{
	BFCICube *cube = new BFCICube(12.0f,12.0f,42.0f,bfc_angle_not_shared);
	BFCTexture *tex = new BFCTexture(bfc_rgba_tex);
	tex->LoadRawFile("Fer.raw");
	cube->InsertTexture(tex,0);
}

BFCVoid MainEditor::CreateSphere()
{
	BFCISphere *sphere = new BFCISphere(20.0f,20.0f,20.0f,4);
	BFCTexture *tex = new BFCTexture(bfc_rgba_tex);
	tex->LoadRawFile("Fer.raw");
	sphere->InsertTexture(tex,0);
}

BFCVoid MainEditor::CreatePyramid()
{
	BFCIPyramid *pyr = new BFCIPyramid(20.0f,20.0f,20.0f,bfc_angle_shared);
	BFCTexture *tex = new BFCTexture(bfc_rgba_tex);
	tex->LoadRawFile("Fer.raw");
	pyr->InsertTexture(tex,0);
}

BFCVoid MainEditor::LoadTexture()
{
	SetAllKey(false);
	// if a face is selected
	if (LastSelectFace != -1)
	{
		BFCChar file_choosed[80];
		file_choosed[0]='\0';
		OPENFILENAME ofn;
		memset(&ofn,0,sizeof(ofn));
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL; 
		ofn.lpstrFileTitle= NULL; 
		ofn.nMaxFileTitle = 0; 
		ofn.lpstrFilter = "Raw files\0*.raw\0"; 
		ofn.nFilterIndex = 1;
		ofn.lpstrFile = file_choosed; 
		ofn.nMaxFile = 80; 
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER; 

		if (GetOpenFileName(&ofn))
		{
			BFCTexture *tex = new BFCTexture(bfc_rgba_tex);
			tex->LoadRawFile(file_choosed);
			LastSelectMesh->InsertTexture(tex,LastSelectFace*3,LastSelectFace*3+3);
			// Set SaveTexture with the new value
			SaveTexture = tex;
		}
	} else
	{
		Message("Please select a face (to get in face mode push the m key) before assigning a texture");
	}
}

BFCVoid MainEditor::EditCoord()
{
	CtrlIsDown(false);
	if (LastSelectVert != -1)
	{
		RefreshCoord();
		CoordDialog->Show(true);
		CoordDialog->SetPos(XRes-350,YRes-350);
	}
}


//********************************************************************************
//
//
//						Dialog method here
//
//
//********************************************************************************


BFCVoid MainEditor::RefreshCoord()
{
	if (LastSelectMesh)
	{
		TmpMeshData = LastSelectMesh->GetMeshData(LastSelectVert);
		CoordDialog->SetSema(false);
		CoordDialog->SetEditValue(0,TmpMeshData.u);
		CoordDialog->SetEditValue(1,TmpMeshData.v);
		CoordDialog->SetSema(true);
		sprintf(gBuff,"Vertex %i",mGraphicEngine->GetPickedVert());
		CoordDialog->SetInfoText(0,gBuff);
	}
}

BFCVoid MainEditor::CoordChanged()
{
	if (LastSelectVert != -1)
	{
		TmpMeshData = LastSelectMesh->GetMeshData(LastSelectVert);
		TmpMeshData.u = CoordDialog->GetEditValue(0);
		TmpMeshData.v = CoordDialog->GetEditValue(1);
		LastSelectMesh->SetMeshData(LastSelectVert, &TmpMeshData);
	}
}


//********************************************************************************
//
//
//						Pipeline function
//
//
//********************************************************************************

BFCVoid MainEditor::InitPipeline()
{
	// Adding pipeline change grid for view function
	BFCPipeFunc *func;
	func = new BFCPipeFunc();
	func->ThisPtr = this;
	func->pipe_func = (BFCVoid (BFCPipeline::*)())ChangeViewLoop;
	// We add the function at the top of the pipeline list of the mesh so we set true
	mGraphicEngine->ChangeViewLoop->AddFunction(func,true);
}

BFCVoid MainEditor::ChangeViewLoop()
{
	// Here we disable zbuffer for non perspective view
	switch (mViewPort->GetViewType( mViewPort->GetActViewPort() ) )
	{
	case bfc_perspec_view:
		{
			//mEngineState->SetZBuffer(true);
			//mD3dDevice->SetRenderState( D3DRS_FILLMODE ,D3DFILL_SOLID);
			mGraphicEngine->EnableAllTexMesh();
		} break;
	default:
		{
			//mEngineState->SetZBuffer(false);
			//mD3dDevice->SetRenderState( D3DRS_FILLMODE ,D3DFILL_WIREFRAME);
			mGraphicEngine->DisableAllTexMesh();
		} break;
	}
}