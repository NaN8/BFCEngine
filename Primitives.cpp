#include <Primitives.h>
#include <EdMain.h>



//************************************************************************
//
//						BFCPimitive
//
//************************************************************************


BFCPrimitive::BFCPrimitive(BFCFloat scalex,BFCFloat scaley,BFCFloat scalez,BFCUInt NumData,BFCChar *name) 
	: Vert(NULL), BFCMesh(unlit_tex_ut_tri_mesh,name), PointData(NULL), PointColor(0xFFFFFFFF)
{
	SetNumData(NumData);
	Vert = (Win32_unlit_tex_ut_mesh *) GetDataPointer();
	BFCMesh::SetScale(scalex,scaley,scalez);
}

BFCPrimitive::FinishCon()
{
	// Calc normal of vertex now
	CalcNormal();
	// Now create the editing points
	PointData = new Win32_lit_ut_mesh[NumData];
	// And now we fill points data
	BFCUInt i = 0;
	while (i<NumData)
	{
		PointData[i].pos = Vert[i].pos; PointData[i].color = 0xFFFFFFFF;
		i++;
	}

	// Adding pipeline editing draw functions for this kind of mesh
	BFCPipeFunc *func;
	func = new BFCPipeFunc();
	func->ThisPtr = this;
	func->pipe_func = (BFCVoid (BFCPipeline::*)())(BFCPrimitive::DrawPoint);
	AddFunction(func);

	// Add tri and point mesh to the engine
	mGraphicEngine->AddMesh(this);
}

BFCVoid BFCPrimitive::DrawSquare(BFCUInt *mem,BFCUInt x,BFCUInt y,BFCUInt color,BFCUInt pitch,BFCUInt size)
{
	BFCUInt i,j;
	y -= size/2;
	x -= size/2;
	for (i=0;i<size;i++)
	{
		for (j=0;j<size;j++)
		{
			mem[j+x+(y+i)*pitch] = color;
		}
	}
}

BFCVoid BFCPrimitive::DrawPoint()	// Draw Points method
{
	IDirect3DSurface8 *surf;
	mD3dDevice->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&surf);
	// Now we lock the back buffer surface
	D3DLOCKED_RECT rect;
	if ( surf->LockRect(&rect,NULL,0)== D3D_OK )
	{
		BFCUInt *mem = (BFCUInt *)rect.pBits;
		BFCVector vec;
		// To save perspestive division
		BFCFloat zdiv;
		BFCMatrix view = *mGraphicEngine->ViewPort.GetView(mGraphicEngine->ViewPort.GetActViewPort());
		BFCMatrix proj = *mGraphicEngine->ViewPort.GetProj(mGraphicEngine->ViewPort.GetActViewPort());
		// Now we draw point for every vertex of the mesh
		BFCUInt i = 0;
		while (i<NumData)
		{
			vec = PointData[i].pos;
			// Transform view
			vec = M * vec;
			// Transform cam
			vec = view * vec;
			// we save the z
			zdiv = vec.z;
			// perspec transform
			vec = proj * vec;
			// From screen space to "surface space"
			BFCFrustum *fru = mGraphicEngine->ViewPort.GetFrustum(mGraphicEngine->ViewPort.GetActViewPort());

			switch ( mViewPort->GetType(mGraphicEngine->ViewPort.GetActViewPort()) )
			{
			case bfc_ortho_proj:
				{
					vec.x = ((vec.x+1.0f) * fru->width)/2.0f;
					vec.y = ((-vec.y+1.0f) * fru->height)/2.0f;
				} break;
			case bfc_perspec_proj:
				{
					vec.x = ((vec.x/zdiv+1.0f) * fru->width)/2.0f;
					vec.y = ((-vec.y/zdiv+1.0f) * fru->height)/2.0f;
				} break;
			}

			BFCFrustum sysfru = mGraphicEngine->ViewPort.GetSysFrustum();
			if (vec.x<fru->width && vec.x>0 && vec.y<fru->height && vec.y>0 && zdiv>0)
			{
				// if color of vertex is not white we set the right selection color him
				if (PointData[i].color != 0xFFFFFFFF)
				{
					DrawSquare(mem,(BFCUInt)vec.x+fru->x,(BFCUInt)(vec.y+fru->y),PointData[i].color,(rect.Pitch/4),4);
				} else
				{
					DrawSquare(mem,(BFCUInt)vec.x+fru->x,(BFCUInt)(vec.y+fru->y),PointColor,(rect.Pitch/4),4);
				}
				//mem[(BFCUInt)vec.x+fru->x+(BFCUInt)(vec.y+fru->y)*(rect.Pitch/4)] = 0xFFFFFFFF;
			}
			i++;
		}

		surf->UnlockRect();
	}
}

BFCVoid BFCPrimitive::SetScale(BFCVector scale)
{
	BFCUInt i;
	for (i=0;i<NumData;i++)
	{
		PointData[i].pos = scale / Scale;
	}
	BFCMesh::SetScale(scale);
}

BFCVoid BFCPrimitive::CalcNormal()
{
	BFCUInt i;
	// Create normal for every vertex
	i = 0;
	while (i<NumData)
	{
		Vert[i].normal = Vert[i].pos.Normal(Vert[i+1].pos,Vert[i+2].pos);
		Vert[i+1].normal = Vert[i+1].pos.Normal(Vert[i+2].pos,Vert[i].pos);
		Vert[i+2].normal = Vert[i+2].pos.Normal(Vert[i].pos,Vert[i+1].pos);

		i+=3;
	}
}

BFCPrimitive::~BFCPrimitive()
{
	if (PointData) delete [] PointData;
}

//************************************************************************
//
//						BFCIPimitive ( indexed primitive )
//
//************************************************************************


BFCIPrimitive::BFCIPrimitive(BFCFloat scalex,BFCFloat scaley,BFCFloat scalez,BFCUInt NumData,BFCUInt NumData2,BFCChar *name, BFCTypePrimGen type_gen) 
	:	Index(NULL), Vert(NULL), BFCMesh(unlit_tex_ut_i_tri_mesh,name), PointData(NULL), PointIndex(NULL), PointColor(0xFFFFFFFF),
		LineMesh(NULL), TypeGen(type_gen)
{
	SetNumData(NumData);
	Vert = (Win32_unlit_tex_ut_mesh *) GetDataPointer();
	SetNumData2(NumData2);
	Index = (BFCWord *) GetDataPointer2();
	BFCMesh::SetScale(scalex,scaley,scalez);
}

BFCVoid BFCIPrimitive::SetScale(BFCVector scale)
{
	BFCUInt i;
	for (i=0;i<NumData;i++)
	{
		PointData[i].pos = PointData[i].pos * (scale / Scale);
	}
	Win32_lit_ut_mesh *data = (Win32_lit_ut_mesh *)LineMesh->GetDataPointer();
	for (i=0;i<LineMesh->GetNumData();i++)
	{
		data[i].pos = data[i].pos * (scale / Scale);
	}
	LineMesh->DataSet();
	BFCMesh::SetScale(scale);
}

BFCVoid BFCIPrimitive::ScaleIt(BFCFloat x, BFCFloat y, BFCFloat z)
{
	BFCUInt i;
	for (i=0;i<NumData;i++)
	{
		PointData[i].pos.x = PointData[i].pos.x * x;
		PointData[i].pos.y = PointData[i].pos.y * y;
		PointData[i].pos.z = PointData[i].pos.z * z;
	}
	Win32_lit_ut_mesh *data = (Win32_lit_ut_mesh *)LineMesh->GetDataPointer();
	for (i=0;i<LineMesh->GetNumData();i++)
	{
		data[i].pos.x = data[i].pos.x * x;
		data[i].pos.y = data[i].pos.y * y;
		data[i].pos.z = data[i].pos.z * z;
	}
	LineMesh->DataSet();
	BFCMesh::ScaleIt(x, y, z);
}

BFCVoid BFCIPrimitive::DrawSquare(BFCUInt *mem,BFCUInt x,BFCUInt y,BFCUInt color,BFCUInt pitch,BFCUInt size)
{
	BFCUInt i,j;
	y -= size/2;
	x -= size/2;
	for (i=0;i<size;i++)
	{
		BFCUInt pre_calc = x+(y+i)*pitch;
		for (j=0;j<size;j++)
		{
			mem[j+pre_calc] = color;
		}
	}
}

// DrawPoint code is "heavy code" because optimized. SpectRe

BFCVoid BFCIPrimitive::DrawPoint()	// Draw Points method
{
	if (this != mMainEditor->GetSelectedMesh() ) return;
	if (mGraphicEngine->ViewPort.GetActViewPort() != mMainEditor->GetActViewPort()) return;
#ifdef WIN32
	IDirect3DSurface8 *surf;
	mD3dDevice->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO,&surf);
	// Now we lock the back buffer surface
	D3DLOCKED_RECT rect;
	if ( surf->LockRect(&rect,NULL,0)== D3D_OK )
	{
		
		BFCUInt *mem = (BFCUInt *)rect.pBits;
		
		BFCVector vec;
		// To save perspestive division
		BFCFloat zdiv;
		BFCMatrix view = *mGraphicEngine->ViewPort.GetView(mGraphicEngine->ViewPort.GetActViewPort());
		BFCMatrix proj = *mGraphicEngine->ViewPort.GetProj(mGraphicEngine->ViewPort.GetActViewPort());
		BFCFrustum *fru = mGraphicEngine->ViewPort.GetFrustum(mGraphicEngine->ViewPort.GetActViewPort());
		BFCFrustum sysfru = mGraphicEngine->ViewPort.GetSysFrustum();
		BFCProjectionType proj_type = mViewPort->GetType(mGraphicEngine->ViewPort.GetActViewPort());
		// Now we draw point for every vertex of the mesh
		BFCUInt i = 0;
		BFCMatrix to_view = M * view;
		while (i<NumData)
		{
			vec = PointData[i].pos;
			// Transform  to screen
			vec = to_view * vec;
			// we save the z
			zdiv = vec.z;
			// simplified and optimized perspec transform. SpectRe
			vec.x = proj._00 * vec.x;
			vec.y = proj._11 * vec.y;
			// From screen space to "surface space"

			switch ( proj_type )
			{
			case bfc_ortho_proj:
				{
					vec.x = ((vec.x+1.0f) * fru->width)*0.5f;
					vec.y = ((-vec.y+1.0f) * fru->height)*0.5f;
				} break;
			case bfc_perspec_proj:
				{
					BFCFloat inv_zdiv = 1 / zdiv;
					vec.x = ((vec.x*inv_zdiv+1.0f) * fru->width)*0.5f;
					vec.y = ((-vec.y*inv_zdiv+1.0f) * fru->height)*0.5f;
				} break;
			}

			if (vec.x<fru->width && vec.x>0 && vec.y<fru->height && vec.y>0 && zdiv>0)
			{
				// if color of vertex is not white we set the right selection color him
				if (PointData[i].color != 0xFFFFFFFF)
				{
					DrawSquare(mem,(BFCUInt)vec.x+fru->x,(BFCUInt)(vec.y+fru->y),PointData[i].color,(rect.Pitch/4),2);
				} else
				{
					DrawSquare(mem,(BFCUInt)vec.x+fru->x,(BFCUInt)(vec.y+fru->y),PointColor,(rect.Pitch/4),2);
				}
				//mem[(BFCUInt)vec.x+fru->x+(BFCUInt)(vec.y+fru->y)*(rect.Pitch/4)] = 0xFFFFFFFF;
			}
			i++;
		}

		surf->UnlockRect();
	}
#endif
	
}

BFCVoid BFCIPrimitive::DrawLine()
{
	LineMesh->M = M;
	//LineMesh->Execute();
}

BFCVoid BFCIPrimitive::DettachTriangle()
{
	if (TypeGen == bfc_angle_shared) TypeGen = bfc_angle_not_shared;

	// Now Create new array
	DynamicMan->SetNumData(NumData2);
	DynamicMan->SetNumData2(NumData2);
	Win32_unlit_tex_ut_mesh *new_data = (Win32_unlit_tex_ut_mesh *)DynamicMan->GetDataPointer();
	BFCWord *new_index = (BFCWord *)DynamicMan->GetDataPointer2();
	Win32_unlit_tex_ut_mesh *data = (Win32_unlit_tex_ut_mesh *)GetDataPointer();
	BFCWord *index = (BFCWord *)GetDataPointer2();
	for (BFCUInt i=0;i<NumData2;i++)
	{
		new_data[i] = data[index[i]];
		new_index[i] = i;
	}
	DynamicMan->SetNewData();
}

BFCIPrimitive::FinishCon()
{
	BFCUInt i,j;

	if (TypeGen == bfc_angle_not_shared) DettachTriangle();

	Vert = (Win32_unlit_tex_ut_mesh *) GetDataPointer();
	Index = (BFCWord *) GetDataPointer2();

	// Calc normal of vertex now
	CalcNormal();

	// Now create the editing points
	PointData = new Win32_lit_ut_mesh[NumData];
	PointIndex = new BFCWord[NumData2];
	// And now we fill points data and point index
	i = 0;
	while (i<NumData)
	{
		PointData[i].pos = Vert[i].pos; PointData[i].color = 0xFFFFFFFF;
		i++;
	}

	i = 0;
	while (i<NumData2)
	{
		PointIndex[i] = Index[i];
		i++;
	}

	// And we create the line mesh

	LineMesh = new BFCMesh(lit_ut_i_line_mesh,"Line Mesh");
	// * 2 because there are 2*3 line per tri
	LineMesh->SetNumData(NumData);
	Win32_lit_ut_mesh *data = (Win32_lit_ut_mesh *) LineMesh->GetDataPointer();
	LineMesh->SetNumData2(NumData2*2);
	BFCWord *index = (BFCWord *) LineMesh->GetDataPointer2();

	i = 0;
	while (i<NumData)
	{
		data[i].pos = Vert[i].pos;data[i].color = 0xFFFFFFFF;
		i++;
	}

	i = 0;
	j = 0;
	while (i<NumData2)
	{
		index[j] = Index[i];
		index[j+1] = Index[i+1];
		index[j+2] = Index[i+1];
		index[j+3] = Index[i+2];
		index[j+4] = Index[i+2];
		index[j+5] = Index[i];

		j+=6;
		i+=3;
	}

	LineMesh->DataSet();
	LineMesh->Data2Set();


	// Adding pipeline editing draw functions for this kind of mesh
	BFCPipeFunc *func;
	// For lines
	func = new BFCPipeFunc();
	func->ThisPtr = this;
	func->pipe_func = (BFCVoid (BFCPipeline::*)())(BFCIPrimitive::DrawLine);
	LineMesh->AddFunction(func);
	// For points
	func = new BFCPipeFunc();
	func->ThisPtr = this;
	func->pipe_func = (BFCVoid (BFCPipeline::*)())(BFCIPrimitive::DrawPoint);
	AddFunction(func);

	// if angle are not shared maybe we still need to change something, in most case coordinates
	if (TypeGen == bfc_angle_shared)
	{
		DataSet();
		Data2Set();
	}

	mGraphicEngine->AddMesh(this);
	mGraphicEngine->AddMesh(LineMesh);

}

BFCVoid BFCIPrimitive::CalcNormal()
{
	BFCUInt i,j;
	BFCUInt calc;
	// Create normal for every vertex
	for (i=0;i<NumData;i++)
	{
		// Find all triangle using this vertex and adding their normal to the final normal
		for (j=0;j<NumData2;j++)
		{
			// if index is using the actual vert
			if (Index[j]==i)
			{
				// calculate the tri index
				calc = j - (j % 3);
				Vert[i].normal = Vert[i].normal + Vert[Index[calc]].pos.Normal(Vert[Index[calc+1]].pos,Vert[Index[calc+2]].pos);
			}
		}
		Vert[i].normal.Normalize();
	}
}

BFCIPrimitive::~BFCIPrimitive()
{
	if (PointData) delete [] PointData;
	if (PointIndex) delete [] PointIndex;
	mGraphicEngine->RemoveMesh(LineMesh);
	mGraphicEngine->RemoveMesh(this, false);
}

//************************************************************************
//
//			BFCIPyramid
//
//************************************************************************


BFCIPyramid::BFCIPyramid(BFCFloat scalex,BFCFloat scaley,BFCFloat scalez, BFCTypePrimGen type_gen) : BFCIPrimitive(scalex,scaley,scalez,4,12,"IPyramid", type_gen)
{
	// These formulas are derived from the idea that there is always 60° of angle. And also of the fact that circle ray size depends on y value and we have 120° of incrementation
    Vert[0].pos.x=(float)cos((PI/1.5f)*1.0f)*(float)sin(PI/3.0f)*Scale.x;Vert[0].pos.y=(float)-sin(PI/3.0f)*Scale.y;Vert[0].pos.z=(float)sin((PI/1.5f)*1.0f)*(float)sin(PI/3.0f)*Scale.z;Vert[0].color=0xFFFFFFFF;Vert[0].u = 0.0f;Vert[0].v = 1.0f;
    Vert[1].pos.x=(float)cos((PI/1.5f)*2.0f)*(float)sin(PI/3.0f)*Scale.x;Vert[1].pos.y=(float)-sin(PI/3.0f)*Scale.y;Vert[1].pos.z=(float)sin((PI/1.5f)*2.0f)*(float)sin(PI/3.0f)*Scale.z;Vert[1].color=0xFFFFFFFF;Vert[1].u = 0.0f;Vert[1].v = 0.0f;
    Vert[2].pos.x=(float)cos((PI/1.5f)*3.0f)*(float)sin(PI/3.0f)*Scale.x;Vert[2].pos.y=(float)-sin(PI/3.0f)*Scale.y;Vert[2].pos.z=(float)sin((PI/1.5f)*3.0f)*(float)sin(PI/3.0f)*Scale.z;Vert[2].color=0xFFFFFFFF;Vert[2].u = 1.0f;Vert[2].v = 0.0f;
    Vert[3].pos.x=(float)0.0f*Scale.x;Vert[3].pos.y=(float)sin(PI/3.0f)*Scale.y;Vert[3].pos.z=(float)0.0f*Scale.z;Vert[3].color=0xFFFFFFFF;Vert[3].u = 1.0f;Vert[3].v = 1.0f;

    Index[0]=0;Index[1]=3;Index[2]=1;
    Index[3]=1;Index[4]=3;Index[5]=2;
    Index[6]=2;Index[7]=3;Index[8]=0;
    Index[9]=0;Index[10]=1;Index[11]=2;

	FinishCon();
}

//************************************************************************
//
//			BFCICube ( Indexed Cube : not perfect for mapping )
//
//************************************************************************


BFCICube::BFCICube(BFCFloat scalex,BFCFloat scaley,BFCFloat scalez, BFCTypePrimGen type_gen) : BFCIPrimitive(scalex,scaley,scalez,8,36,"ICube", type_gen)
{
    Vert[0].pos.x=-1.0f*Scale.x;Vert[0].pos.y=-1.0f*Scale.y;Vert[0].pos.z=1.0f*Scale.z;Vert[0].color=0xFFFFFFFF;Vert[0].u = 0.0f;Vert[0].v = 1.0f;
    Vert[1].pos.x=-1.0f*Scale.x;Vert[1].pos.y=1.0f*Scale.y;Vert[1].pos.z=1.0f*Scale.z;Vert[1].color=0xFFFFFFFF;Vert[1].u = 0.0f;Vert[1].v = 0.0f;
    Vert[2].pos.x=1.0f*Scale.x;Vert[2].pos.y=1.0f*Scale.y;Vert[2].pos.z=1.0f*Scale.z;Vert[2].color=0xFFFFFFFF;Vert[2].u = 1.0f;Vert[2].v = 0.0f;
    Vert[3].pos.x=1.0f*Scale.x;Vert[3].pos.y=-1.0f*Scale.y;Vert[3].pos.z=1.0f*Scale.z;Vert[3].color=0xFFFFFFFF;Vert[3].u = 1.0f;Vert[3].v = 1.0f;
    Vert[4].pos.x=-1.0f*Scale.x;Vert[4].pos.y=-1.0f*Scale.y;Vert[4].pos.z=-1.0f*Scale.z;Vert[4].color=0xFFFFFFFF;Vert[4].u = 1.0f;Vert[4].v = 0.0f;
    Vert[5].pos.x=-1.0f*Scale.x;Vert[5].pos.y=1.0f*Scale.y;Vert[5].pos.z=-1.0f*Scale.z;Vert[5].color=0xFFFFFFFF;Vert[5].u = 1.0f;Vert[5].v = 1.0f;
    Vert[6].pos.x=1.0f*Scale.x;Vert[6].pos.y=1.0f*Scale.y;Vert[6].pos.z=-1.0f*Scale.z;Vert[6].color=0xFFFFFFFF;Vert[6].u = 0.0f;Vert[6].v = 1.0f;
    Vert[7].pos.x=1.0f*Scale.x;Vert[7].pos.y=-1.0f*Scale.y;Vert[7].pos.z=-1.0f*Scale.z;Vert[7].color=0xFFFFFFFF;Vert[7].u = 0.0f;Vert[7].v = 0.0f;

	// 90° with 90° and 45° with 45° for a good normal calculation. SpectRe
	// Right way to respect cross product
    Index[0]=0;Index[1]=2;Index[2]=1;
    Index[3]=0;Index[4]=3;Index[5]=2;
    Index[6]=4;Index[7]=5;Index[8]=7;
    Index[9]=6;Index[10]=7;Index[11]=5;
	Index[12]=0;Index[13]=1;Index[14]=5;
    Index[15]=0;Index[16]=5;Index[17]=4;
	Index[18]=7;Index[19]=6;Index[20]=2;
    Index[21]=7;Index[22]=2;Index[23]=3;
    Index[24]=5;Index[25]=1;Index[26]=2;
    Index[27]=5;Index[28]=2;Index[29]=6;
	Index[30]=4;Index[31]=7;Index[32]=0;
    Index[33]=3;Index[34]=0;Index[35]=7;

	FinishCon();

	if (TypeGen == bfc_angle_not_shared)
	{
		Vert[0].u = 0.0f;Vert[0].v = 1.0f;
		Vert[1].u = 1.0f;Vert[1].v = 0.0f;
		Vert[2].u = 0.0f;Vert[2].v = 0.0f;
		Vert[3].u = 0.0f;Vert[3].v = 1.0f;
		Vert[4].u = 1.0f;Vert[4].v = 1.0f;
		Vert[5].u = 1.0f;Vert[5].v = 0.0f;

		Vert[6].u = 1.0f;Vert[6].v = 1.0f;
		Vert[7].u = 1.0f;Vert[7].v = 0.0f;
		Vert[8].u = 0.0f;Vert[8].v = 1.0f;
		Vert[9].u = 0.0f;Vert[9].v = 0.0f;
		Vert[10].u = 0.0f;Vert[10].v = 1.0f;
		Vert[11].u = 1.0f;Vert[11].v = 0.0f;

		Vert[12].u = 1.0f;Vert[12].v = 1.0f;
		Vert[13].u = 1.0f;Vert[13].v = 0.0f;
		Vert[14].u = 0.0f;Vert[14].v = 0.0f;
		Vert[15].u = 1.0f;Vert[15].v = 1.0f;
		Vert[16].u = 0.0f;Vert[16].v = 0.0f;
		Vert[17].u = 0.0f;Vert[17].v = 1.0f;

		Vert[18].u = 1.0f;Vert[18].v = 1.0f;
		Vert[19].u = 1.0f;Vert[19].v = 0.0f;
		Vert[20].u = 0.0f;Vert[20].v = 0.0f;
		Vert[21].u = 1.0f;Vert[21].v = 1.0f;
		Vert[22].u = 0.0f;Vert[22].v = 0.0f;
		Vert[23].u = 0.0f;Vert[23].v = 1.0f;

		Vert[24].u = 1.0f;Vert[24].v = 1.0f;
		Vert[25].u = 1.0f;Vert[25].v = 0.0f;
		Vert[26].u = 0.0f;Vert[26].v = 0.0f;
		Vert[27].u = 1.0f;Vert[27].v = 1.0f;
		Vert[28].u = 0.0f;Vert[28].v = 0.0f;
		Vert[29].u = 0.0f;Vert[29].v = 1.0f;

		Vert[30].u = 1.0f;Vert[30].v = 0.0f;
		Vert[31].u = 0.0f;Vert[31].v = 0.0f;
		Vert[32].u = 1.0f;Vert[32].v = 1.0f;
		Vert[33].u = 0.0f;Vert[33].v = 1.0f;
		Vert[34].u = 1.0f;Vert[34].v = 1.0f;
		Vert[35].u = 0.0f;Vert[35].v = 0.0f;

		Data2Set();
		DataSet();
	}
}

//************************************************************************
//
//			BFCISphere ( Indexed Sphere )
//
//************************************************************************


BFCISphere::BFCISphere(BFCFloat scalex,BFCFloat scaley,BFCFloat scalez, BFCUInt def)
							: BFCIPrimitive(scalex,scaley,scalez,def*(def-1)+2,6*def*(def-1),"ISphere"), 
							Def(def), MapStyle(bfc_double_map)
{
	BFCUInt i,j;
    BFCFloat x,z;
	BFCUInt nvert;
	z=0;
    x=0;
	// Create Vertice
	nvert=0;
	//top
    Vert[nvert].pos.x=(float)( sin(x)*sin(z) )*scalex;
    Vert[nvert].pos.y=(float)cos(x)*scaley;
    Vert[nvert].pos.z=(float)( cos(z)*sin(x) )*scalez;
	Vert[nvert].color = 0xFFFFFFFF;
	x+=PI/def;
	nvert++;
    for (i=0;i<def-1;i++)
	{
            z=0;
            for (j=0;j<def;j++)
			{
                    Vert[nvert].pos.x=(float)( sin(x)*sin(z) )*scalex;
                    Vert[nvert].pos.y=(float)cos(x)*scaley;
                    Vert[nvert].pos.z=(float)( cos(z)*sin(x) )*scalez;
					Vert[nvert].color = 0xFFFFFFFF;
                    nvert++;
                    z+=(2*PI)/def;
            }
            x+=PI/def;
    }
	// bottom
    Vert[nvert].pos.x=(float)( sin(x)*sin(z) )*scalex;
    Vert[nvert].pos.y=(float)cos(x)*scaley;
    Vert[nvert].pos.z=(float)( cos(z)*sin(x) )*scalez;
	Vert[nvert].color = 0xFFFFFFFF;
	nvert++;

    // Create face
	BFCInt index = 0;

	//top 
    for (i=0;i<def;i++)
	{
            // Last vert of this line must be the first vert and not the first of next line (condition here is for this)
            if ((i % def)!=def-1)
			{
					Index[index]=0;index++;
					Index[index]=i+1;index++;
                    Index[index]=i+2;index++;
            } else 
			{
                    Index[index]=0;index++;
					Index[index]=i+1;index++;
                    Index[index]=1;index++;
            }
    }

	// i from one because of top already made
    for (i=1;i<nvert-def-1;i++)
	{
            // Last vert of this line must be the first vert and not the first of next line (condition here is for this)
            if ( ((i-1) % def)!=def-1 )
			{
					Index[index]=i;index++;
					Index[index]=i+def;index++;
                    Index[index]=i+def+1;index++;
                    
                    Index[index]=i;index++;
                    Index[index]=i+def+1;index++;
                    Index[index]=i+1;index++;
            } else 
			{
                    Index[index]=i;index++;
                    Index[index]=i+def;index++;
                    Index[index]=i+1;index++;

                    Index[index]=i;index++;
                    Index[index]=i+1;index++;
                    Index[index]=i-(def-1);index++;
            }
    }

	// bottom
    for (i=nvert-1-def;i<nvert-1;i++)
	{
            // Last vert of this line must be the first vert and not the first of next line (condition here is for this)
            if ( ((i-1)  % def)!=def-1)
			{
					Index[index]=i;index++;
					Index[index]=nvert-1;index++; 
                    Index[index]=i+1;index++;
            } else 
			{
                    Index[index]=i;index++;
                    Index[index]=nvert-1;index++;
                    Index[index]=i-(def-1);index++;
            }
    }

	// Create coordinates


    BFCFloat u,v;
    u=0.0f;
    v=0.0f;
	i=0;
	j=0;

	switch (MapStyle)
	{
	case bfc_linear_map :
		{
		//  Linear Mapping ( not perfect on an indexed mesh)

		// top

		Vert[0].u=0.5f;
		Vert[0].v=0.0f;
		v+=1.0f/(BFCFloat)Def;

		i++;

		// center

		while (i<NumData-1)
		{
			Vert[i].u=u;
			Vert[i].v=v;
			i++;

			u+=1.0f/(BFCFloat)Def;

			// Next "circle"
			if ( (i-1) % (Def)==0) 
			{
					v+=1.0f/(BFCFloat)Def;
					u=0.0f;
			}
		}

		// bottom

		Vert[NumData-1].u=0.5f;
		Vert[NumData-1].v=1.0f;
		} break;
	case bfc_double_map:
		{
			// calc the middle line of the sphere
			BFCUInt middle = (Def-1) / 2 + ( Def % 2 );

			// top

			Vert[0].u = 0.5f;
			Vert[0].v = 0.5f;

			// middle

            BFCFloat ang;
			// Ray loop from in to out
			for (BFCUInt ray=1;ray<middle+1;ray++)
			{
				ang = 0.0f;
				// Circle loop
				for (BFCUInt j=0;j<Def;j++)
				{
						Vert[j+(ray-1)*Def+1].u=( (BFCFloat)sin(ang) / (BFCFloat)middle * (BFCFloat)ray ) + 0.5f;
						Vert[j+(ray-1)*Def+1].v=( (BFCFloat)cos(ang) / (BFCFloat)middle * (BFCFloat)ray ) + 0.5f;
						ang+=(2*PI)/def;
				}
			}

			// Ray loop from out to in
			for (ray=middle+1;ray>0;ray--)
			{
				ang = 0.0f;
				// Circle loop
				for (j=0;j<Def;j++)
				{
						Vert[j+(middle + (middle +1 - ray))*Def+1].u=( (BFCFloat)sin(ang) / (BFCFloat)middle * (BFCFloat)ray ) + 0.5f;
						Vert[j+(middle + (middle +1 - ray))*Def+1].v=( (BFCFloat)cos(ang) / (BFCFloat)middle * (BFCFloat)ray ) + 0.5f;
						ang+=(2*PI)/def;
				}
			}

			// bottom

			Vert[nvert-1].u = 0.5f;
			Vert[nvert-1].v = 0.5f;

		} break;
	}

	FinishCon();
}

//************************************************************************
//
//						BFCCube
//
//************************************************************************

BFCCube::BFCCube(BFCFloat scalex,BFCFloat scaley,BFCFloat scalez) : BFCPrimitive(scalex,scaley,scalez,36,"Cube")
{
    Vert[0].pos.x=-1.0f*Scale.x;Vert[0].pos.y=-1.0f*Scale.y;Vert[0].pos.z=1.0f*Scale.z;Vert[0].color=0xFFFFFFFF;Vert[0].u = 1.0f;Vert[0].v = 1.0f;
	Vert[1].pos.x=1.0f*Scale.x;Vert[1].pos.y=1.0f*Scale.y;Vert[1].pos.z=1.0f*Scale.z;Vert[1].color=0xFFFFFFFF;Vert[1].u = 0.0f;Vert[1].v = 0.0f;
	Vert[2].pos.x=-1.0f*Scale.x;Vert[2].pos.y=1.0f*Scale.y;Vert[2].pos.z=1.0f*Scale.z;Vert[2].color=0xFFFFFFFF;Vert[2].u = 1.0f;Vert[2].v = 0.0f;

	Vert[3].pos.x=-1.0f*Scale.x;Vert[3].pos.y=-1.0f*Scale.y;Vert[3].pos.z=1.0f*Scale.z;Vert[3].color=0xFFFFFFFF;Vert[3].u = 1.0f;Vert[3].v = 1.0f;
	Vert[4].pos.x=1.0f*Scale.x;Vert[4].pos.y=-1.0f*Scale.y;Vert[4].pos.z=1.0f*Scale.z;Vert[4].color=0xFFFFFFFF;Vert[4].u = 0.0f;Vert[4].v = 1.0f;
	Vert[5].pos.x=1.0f*Scale.x;Vert[5].pos.y=1.0f*Scale.y;Vert[5].pos.z=1.0f*Scale.z;Vert[5].color=0xFFFFFFFF;Vert[5].u = 0.0f;Vert[5].v = 0.0f;

    Vert[6].pos.x=-1.0f*Scale.x;Vert[6].pos.y=-1.0f*Scale.y;Vert[6].pos.z=-1.0f*Scale.z;Vert[6].color=0xFFFFFFFF;Vert[6].u = 0.0f;Vert[6].v = 1.0f;
    Vert[7].pos.x=-1.0f*Scale.x;Vert[7].pos.y=1.0f*Scale.y;Vert[7].pos.z=-1.0f*Scale.z;Vert[7].color=0xFFFFFFFF;Vert[7].u = 0.0f;Vert[7].v = 0.0f;
    Vert[8].pos.x=1.0f*Scale.x;Vert[8].pos.y=-1.0f*Scale.y;Vert[8].pos.z=-1.0f*Scale.z;Vert[8].color=0xFFFFFFFF;Vert[8].u = 1.0f;Vert[8].v = 1.0f;

    Vert[9].pos.x=1.0f*Scale.x;Vert[9].pos.y=1.0f*Scale.y;Vert[9].pos.z=-1.0f*Scale.z;Vert[9].color=0xFFFFFFFF;Vert[9].u = 1.0f;Vert[9].v = 0.0f;
	Vert[10].pos.x=1.0f*Scale.x;Vert[10].pos.y=-1.0f*Scale.y;Vert[10].pos.z=-1.0f*Scale.z;Vert[10].color=0xFFFFFFFF;Vert[10].u = 1.0f;Vert[10].v = 1.0f;
	Vert[11].pos.x=-1.0f*Scale.x;Vert[11].pos.y=1.0f*Scale.y;Vert[11].pos.z=-1.0f*Scale.z;Vert[11].color=0xFFFFFFFF;Vert[11].u = 0.0f;Vert[11].v = 0.0f;

	Vert[12].pos.x=-1.0f*Scale.x;Vert[12].pos.y=-1.0f*Scale.y;Vert[12].pos.z=1.0f*Scale.z;Vert[12].color=0xFFFFFFFF;Vert[12].u = 0.0f;Vert[12].v = 1.0f;
	Vert[13].pos.x=-1.0f*Scale.x;Vert[13].pos.y=1.0f*Scale.y;Vert[13].pos.z=1.0f*Scale.z;Vert[13].color=0xFFFFFFFF;Vert[13].u = 0.0f;Vert[13].v = 0.0f;
	Vert[14].pos.x=-1.0f*Scale.x;Vert[14].pos.y=1.0f*Scale.y;Vert[14].pos.z=-1.0f*Scale.z;Vert[14].color=0xFFFFFFFF;Vert[14].u = 1.0f;Vert[14].v = 0.0f;

	Vert[15].pos.x=-1.0f*Scale.x;Vert[15].pos.y=-1.0f*Scale.y;Vert[15].pos.z=1.0f*Scale.z;Vert[15].color=0xFFFFFFFF;Vert[15].u = 0.0f;Vert[15].v = 1.0f;
	Vert[16].pos.x=-1.0f*Scale.x;Vert[16].pos.y=1.0f*Scale.y;Vert[16].pos.z=-1.0f*Scale.z;Vert[16].color=0xFFFFFFFF;Vert[16].u = 1.0f;Vert[16].v = 0.0f;
	Vert[17].pos.x=-1.0f*Scale.x;Vert[17].pos.y=-1.0f*Scale.y;Vert[17].pos.z=-1.0f*Scale.z;Vert[17].color=0xFFFFFFFF;Vert[17].u = 1.0f;Vert[17].v = 1.0f;

	Vert[18].pos.x=1.0f*Scale.x;Vert[18].pos.y=-1.0f*Scale.y;Vert[18].pos.z=-1.0f*Scale.z;Vert[18].color=0xFFFFFFFF;Vert[18].u = 0.0f;Vert[18].v = 1.0f;
	Vert[19].pos.x=1.0f*Scale.x;Vert[19].pos.y=1.0f*Scale.y;Vert[19].pos.z=-1.0f*Scale.z;Vert[19].color=0xFFFFFFFF;Vert[19].u = 0.0f;Vert[19].v = 0.0f;
	Vert[20].pos.x=1.0f*Scale.x;Vert[20].pos.y=1.0f*Scale.y;Vert[20].pos.z=1.0f*Scale.z;Vert[20].color=0xFFFFFFFF;Vert[20].u = 1.0f;Vert[20].v = 0.0f;

	Vert[21].pos.x=1.0f*Scale.x;Vert[21].pos.y=-1.0f*Scale.y;Vert[21].pos.z=-1.0f*Scale.z;Vert[21].color=0xFFFFFFFF;Vert[21].u = 0.0f;Vert[21].v = 1.0f;
	Vert[22].pos.x=1.0f*Scale.x;Vert[22].pos.y=1.0f*Scale.y;Vert[22].pos.z=1.0f*Scale.z;Vert[22].color=0xFFFFFFFF;Vert[22].u = 1.0f;Vert[22].v = 0.0f;
	Vert[23].pos.x=1.0f*Scale.x;Vert[23].pos.y=-1.0f*Scale.y;Vert[23].pos.z=1.0f*Scale.z;Vert[23].color=0xFFFFFFFF;Vert[23].u = 1.0f;Vert[23].v = 1.0f;

    Vert[24].pos.x=-1.0f*Scale.x;Vert[24].pos.y=1.0f*Scale.y;Vert[24].pos.z=-1.0f*Scale.z;Vert[24].color=0xFFFFFFFF;Vert[24].u = 0.0f;Vert[24].v = 1.0f;
	Vert[25].pos.x=-1.0f*Scale.x;Vert[25].pos.y=1.0f*Scale.y;Vert[25].pos.z=1.0f*Scale.z;Vert[25].color=0xFFFFFFFF;Vert[25].u = 0.0f;Vert[25].v = 0.0f;
	Vert[26].pos.x=1.0f*Scale.x;Vert[26].pos.y=1.0f*Scale.y;Vert[26].pos.z=1.0f*Scale.z;Vert[26].color=0xFFFFFFFF;Vert[26].u = 1.0f;Vert[26].v = 0.0f;

    Vert[27].pos.x=-1.0f*Scale.x;Vert[27].pos.y=1.0f*Scale.y;Vert[27].pos.z=-1.0f*Scale.z;Vert[27].color=0xFFFFFFFF;Vert[27].u = 0.0f;Vert[27].v = 1.0f;
	Vert[28].pos.x=1.0f*Scale.x;Vert[28].pos.y=1.0f*Scale.y;Vert[28].pos.z=1.0f*Scale.z;Vert[28].color=0xFFFFFFFF;Vert[28].u = 1.0f;Vert[28].v = 0.0f;
	Vert[29].pos.x=1.0f*Scale.x;Vert[29].pos.y=1.0f*Scale.y;Vert[29].pos.z=-1.0f*Scale.z;Vert[29].color=0xFFFFFFFF;Vert[29].u = 1.0f;Vert[29].v = 1.0f;

    Vert[30].pos.x=-1.0f*Scale.x;Vert[30].pos.y=-1.0f*Scale.y;Vert[30].pos.z=-1.0f*Scale.z;Vert[30].color=0xFFFFFFFF;Vert[30].u = 0.0f;Vert[30].v = 1.0f;
	Vert[31].pos.x=1.0f*Scale.x;Vert[31].pos.y=-1.0f*Scale.y;Vert[31].pos.z=-1.0f*Scale.z;Vert[31].color=0xFFFFFFFF;Vert[31].u = 1.0f;Vert[31].v = 1.0f;
	Vert[32].pos.x=-1.0f*Scale.x;Vert[32].pos.y=-1.0f*Scale.y;Vert[32].pos.z=1.0f*Scale.z;Vert[32].color=0xFFFFFFFF;Vert[32].u = 0.0f;Vert[32].v = 0.0f;

	Vert[33].pos.x=1.0f*Scale.x;Vert[33].pos.y=-1.0f*Scale.y;Vert[33].pos.z=1.0f*Scale.z;Vert[33].color=0xFFFFFFFF;Vert[33].u = 1.0f;Vert[33].v = 0.0f;
	Vert[34].pos.x=-1.0f*Scale.x;Vert[34].pos.y=-1.0f*Scale.y;Vert[34].pos.z=1.0f*Scale.z;Vert[34].color=0xFFFFFFFF;Vert[34].u = 0.0f;Vert[34].v = 0.0f;
	Vert[35].pos.x=1.0f*Scale.x;Vert[35].pos.y=-1.0f*Scale.y;Vert[35].pos.z=-1.0f*Scale.z;Vert[35].color=0xFFFFFFFF;Vert[35].u = 1.0f;Vert[35].v = 1.0f;

	FinishCon();

}


//************************************************************************
//
//						BFCGrid
//
//************************************************************************


BFCGrid::BFCGrid(BFCInt def, BFCInt size) : BFCMesh(lit_ut_line_mesh,"Grid Interface")
{
	Size = size;
	Def = def;
	SetNumData(def*4);
	Win32_lit_ut_mesh *data = (Win32_lit_ut_mesh *)GetDataPointer();
	BFCFloat side_length = (size / 2.0f) - ( (float)size / (float)def);
	// Fill Data
	BFCInt i = 0;
	BFCFloat inc = 0;
	while (i<def*4)
	{
		data[i].pos.x = -side_length + inc;
		data[i].pos.y = -20.0f;
		data[i].pos.z = -side_length;
		data[i].color = 0x00000000;
		//data[i].color = 0xFF00FFFF;

		i++;

		data[i].pos.x = -side_length + inc;
		data[i].pos.y = -20.0f;
		data[i].pos.z = side_length + (size / (float)def);
		data[i].color = 0x00000000;

		i++;

		data[i].pos.x = -side_length;
		data[i].pos.y = -20.0f;
		data[i].pos.z = -side_length + inc;
		data[i].color = 0x00000000;

		i++;

		data[i].pos.x = side_length + (size / (float)def);
		data[i].pos.y = -20.0f;
		data[i].pos.z = -side_length + inc;
		data[i].color = 0x00000000;

		inc += size / (float)def;
		i++;
	}

	// Adding pipeline change grid for view function
	BFCPipeFunc *func;
	func = new BFCPipeFunc();
	func->ThisPtr = this;
	func->pipe_func = (BFCVoid (BFCPipeline::*)())SetGridForView;
	// We add the function at the top of the pipeline list of the mesh so we set true
	AddFunction(func,true);


	//SetType2(interface_mesh);
	mGraphicEngine->AddMesh(this);
}

BFCVoid BFCGrid::SetGridForView()
{
	switch (mViewPort->GetViewType( mViewPort->GetActViewPort() ) )
	{
	case bfc_perspec_view:
		{
			M.Identity();
		} break;
	case bfc_front_view:
		{
			M.Identity();
			RotateX(-PI/2);
			M.SetPos(0.0f,0.0f,500.0f);
		} break;
	case bfc_top_view:
		{
			M.Identity();
			M.SetPos(0.0f,-500.0f,0.0f);
		} break;
	case bfc_left_view:
		{
			M.Identity();
			RotateZ(PI/2);
			M.SetPos(500.0f,0.0f,0.0f);
		} break;
	}
}
