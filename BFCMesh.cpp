#include <BFCMesh.h>
#include <BFCManager.h>
#include <BFCGraphicEngine.h>




#ifdef WIN32

// vertex buffer descriptor table for vertex buffer creation  
// !! Respect the order of BFCMeshType
const vb_desc VB_DESC_TAB[]	= 
{ 
	{D3D_LIT_T   , sizeof(Win32_lit_t_mesh)   , BFCMesh::DrawLitTTriMesh   , 3},
	{D3D_LIT_UT  , sizeof(Win32_lit_ut_mesh)  , BFCMesh::DrawLitUTriMesh   , 3},
	{D3D_UNLIT_UT, sizeof(Win32_unlit_ut_mesh), BFCMesh::DrawUnLitUTriMesh , 3},
	{D3D_UNLIT_TEX_UT, sizeof(Win32_unlit_tex_ut_mesh), BFCMesh::DrawUnLitTexUTriMesh , 3},
	{D3D_UNLIT_TEX_UT, sizeof(Win32_unlit_tex_ut_mesh), BFCMesh::DrawUnLitTexUITriMesh, 3},
	{D3D_LIT_T    , sizeof(Win32_lit_t_mesh)	, BFCMesh::DrawLitTLineMesh		, 2},
	{D3D_LIT_UT    , sizeof(Win32_lit_ut_mesh)  , BFCMesh::DrawLitUTLineMesh	, 2},
	{D3D_LIT_UT    , sizeof(Win32_lit_ut_mesh)  , BFCMesh::DrawLitUTILineMesh	, 2},
	{D3D_LIT_UT   , sizeof(Win32_lit_ut_mesh)   , BFCMesh::DrawLitUTPointMesh	, 1}
};	

#endif



//************************************************************************
//
//			Dynamic Mesh : Class to manage dynamic mesh
//
//************************************************************************


BFCDynamicMeshMan::BFCDynamicMeshMan(BFCMesh *p) :	Data(NULL), Data2(NULL), Locked(false), Locked2(false),
													NumData(0), NumData2(0), MyMesh(p), NumPrim(0)
{
#ifdef WIN32
	Vb = NULL;
	Index = NULL;
#endif
}

BFCVoid BFCDynamicMeshMan::CleanData()
{
#ifdef WIN32
	if (Vb)
	{
		Vb->Release();
	}
#endif
}

BFCVoid BFCDynamicMeshMan::CleanData2()
{
#ifdef WIN32
	if (Index)
	{
		Index->Release();
	}
#endif
}

BFCVoid *BFCDynamicMeshMan::GetDataPointer()
{	
	// Check validity for some data of the mesh we are adding
	BFCAssert(NumData,"GetDataPointer called before SetNumData");
	// if not locked
	if (!Locked)
	{
#ifdef WIN32
	// Create mesh vertex buffer if Vb==NULL
	if (Vb==NULL)
	{
		if(FAILED(
		mGraphicEngine->D3dDevice->CreateVertexBuffer(NumData*VB_DESC_TAB[MyMesh->Type].size,0,VB_DESC_TAB[MyMesh->Type].format,D3DPOOL_DEFAULT,&Vb)
		)) 
		{
			BFCAssert(0,"Can't Create a Vertex Buffer");
			return NULL;
		}
		if( FAILED( Vb->Lock( 0, NumData*VB_DESC_TAB[MyMesh->Type].size, (BFCUChar**)&Data, 0 ) ) )
		{
			return NULL;
		}
		if (!Index) NumPrim = NumData/VB_DESC_TAB[MyMesh->Type].NumDataPerPrim;
	} else
	{
		if( FAILED( Vb->Lock( 0, NumData*VB_DESC_TAB[MyMesh->Type].size, (BFCUChar**)&Data, 0 ) ) )
		{
			return NULL;
		}
	}
	Locked = true;
#endif
	} // end of if not locked

	return Data;
}

BFCVoid *BFCDynamicMeshMan::GetDataPointer2()
{
	// Check validity for some data of the mesh we are adding
	BFCAssert(NumData2,"GetDataPointer2 called before SetNumData2");
	// if not locked
	if (!Locked2)
	{
#ifdef WIN32
	// Create mesh index buffer if Index==NULL
	if (Index==NULL)
	{
		// 2 for 32 bit hardcoded for now
		if(FAILED(
		mGraphicEngine->D3dDevice->CreateIndexBuffer(NumData2*2,0,D3DFMT_INDEX16 ,D3DPOOL_DEFAULT,&Index)
		)) 
		{
			BFCAssert(0,"Can't Create an Index Buffer");
			return NULL;
		}
		if( FAILED( Index->Lock( 0, NumData2*2, (BFCUChar**)&Data2, 0 ) ) )
		{
			return NULL;
		}
		NumPrim = NumData2/VB_DESC_TAB[MyMesh->Type].NumDataPerPrim;
	} else
	{
		if( FAILED( Index->Lock( 0, NumData2*2, (BFCUChar**)&Data2, 0 ) ) )
		{
			return NULL;
		}
	}
	Locked2 = true;
#endif
	} // end of if not locked

	return Data2;
}

BFCGMReturn BFCDynamicMeshMan::DataSet()
{
	if (Locked)
	{
#ifdef WIN32
		Vb->Unlock();
#endif
		Locked = false;		
		return g_m_ok;
	} else return g_m_already_locked;
}

BFCGMReturn BFCDynamicMeshMan::Data2Set()
{
	if (Locked2)
	{
#ifdef WIN32
		Index->Unlock();
#endif
		Locked2 = false;		
		return g_m_ok;
	} else return g_m_already_locked;
}

BFCVoid BFCDynamicMeshMan::SetNewData()
{
#ifdef WIN32
	
	if (Vb)
	{
		DataSet();
		MyMesh->DataSet();
		MyMesh->Vb->Release();
		MyMesh->Vb = Vb;
		// Set local Vb back to null
		Vb = NULL;
	}
	if (Index)
	{
		Data2Set();
		MyMesh->Data2Set();
		MyMesh->Index->Release();
		MyMesh->Index = Index;
		// Set local index back to null
		Index = NULL;
	}
	
#endif
	
	MyMesh->NumData = NumData;
	MyMesh->NumData2 = NumData2;
	MyMesh->NumPrim = NumPrim;

	// Set the first texture (if there is one) of the mesh on all the mesh

	BFCTextureIndex *tex_index = (BFCTextureIndex *)MyMesh->Tex.GetFirstPtr();
	if (tex_index)
	{
		BFCTexture *tex = tex_index->Tex;
		MyMesh->Tex.RemoveDeleteAll();
		MyMesh->InsertTexture(tex,0,0);
	}
}

//************************************************************************
//
//						BFCMesh
//
//************************************************************************


BFCVoid BFCMesh::CleanData()
{
#ifdef WIN32
	if (Vb)
	{
		Vb->Release();
	}
#endif
}

BFCVoid BFCMesh::CleanData2()
{
#ifdef WIN32
	if (Index)
	{
		Index->Release();
	}
#endif
}

BFCMesh::~BFCMesh()
{
	CleanData();
	CleanData2();
	Tex.RemoveDeleteAll();
}

BFCMesh::BFCMesh(BFCMeshType type,BFCChar *name) :	BFCPipeline(name) , Type(type) , Data(NULL), Data2(NULL), 
													NumData(0), NumData2(0), Locked(false), NumPrim(0), Locked2(false),
													Type2(normal_mesh), RotX(0.0f), RotY(0.0f), RotZ(0.0f), Enabled(true),
													DynamicMan(NULL)
{
	DynamicMan = new BFCDynamicMeshMan(this);
#ifdef WIN32
	Vb = NULL;
	Index = NULL;
	Material.Ambient.r = 1.0f;
	Material.Ambient.g = 1.0f;
	Material.Ambient.b = 1.0f;
	Material.Ambient.a = 1.0f;

	Material.Diffuse.r = 1.0f;
	Material.Diffuse.g = 1.0f;
	Material.Diffuse.b = 1.0f;
	Material.Diffuse.a = 1.0f;
#endif
	M.Identity();
	Scale.x = 1.0f;Scale.y = 1.0f;Scale.z = 1.0f;
	//Tex = new BFCTexture(bfc_rgba_tex);
}

BFCTexture *BFCMesh::InsertTexture(BFCTexture *tex,BFCUInt index,BFCUInt lastindex)
{
	BFCTexture *a_tex_replaced = NULL;
	BFCTextureIndex *_index = new BFCTextureIndex();
	// if LastIndex = 0 then cover all the mesh
	if (lastindex == 0)  lastindex = NumPrim*3;
	_index->Index = index;
	_index->Tex = tex;
	_index->LastIndex = lastindex;
	// Check if something is not inside the group of face
	BFCTextureIndex *_check = (BFCTextureIndex *)Tex.GetFirstPtr();
	while (_check)
	{
		// Check if it's partially inside
		if ( (_index->Index <= _check->Index) && ( _index->LastIndex > _check->Index) )
		{
			a_tex_replaced = _check->Tex;
			// Check if it's completly inside
			if ( (_index->LastIndex >= _check->LastIndex) )
			{
				// if yes remove the old texture inside
				Tex.RemoveDelete(_check);
				break;
			} else
			{
				// otherwise adjust data
				_check->Index = _index->LastIndex;
			}
		}
		// Check if it's partially inside with a posible second piece
		if ( ( _index->Index > _check->Index ) && ( _index->Index < _check->LastIndex) )
		{
			a_tex_replaced = _check->Tex;
			// Check if it's not split in two
			if ( _index->LastIndex < _check->LastIndex )
			{
				// Add the second piece
				BFCTextureIndex *_index2 = new BFCTextureIndex();
				_index2->Index = _index->LastIndex;
				_index2->Tex = _check->Tex;
				_index2->LastIndex = _check->LastIndex;
				Tex.Add( (BFCVoid *)_index2);

				// Adjust the first piece 
				_check->LastIndex = _index->Index;
			} else
			{
				_check->LastIndex = _index->Index;
			}
		}

		_check = (BFCTextureIndex *) Tex.GetNextPtr();
	}
	Tex.Add( (BFCVoid *)_index);
	return a_tex_replaced;
}

BFCVoid *BFCMesh::GetDataPointer()
{	
	BFCPipeFunc *func;
	// Check validity for some data of the mesh we are adding
	BFCAssert(NumData,"GetDataPointer called before SetNumData");
	// if not locked
	if (!Locked)
	{
#ifdef WIN32
	// Create mesh vertex buffer if Vb==NULL
	if (Vb==NULL)
	{
		if(FAILED(
		mGraphicEngine->D3dDevice->CreateVertexBuffer(NumData*VB_DESC_TAB[Type].size,0,VB_DESC_TAB[Type].format,D3DPOOL_DEFAULT,&Vb)
		)) 
		{
			BFCAssert(0,"Can't Create a Vertex Buffer");
			return NULL;
		}
		// Adding pipeline draw functions for this kind of mesh
		func = new BFCPipeFunc();
		func->ThisPtr = this;
		func->pipe_func = (BFCVoid (BFCPipeline::*)())VB_DESC_TAB[Type].method;
		AddFunction(func);
		if( FAILED( Vb->Lock( 0, NumData*VB_DESC_TAB[Type].size, (BFCUChar**)&Data, 0 ) ) )
		{
			return NULL;
		}
		if (!Index) NumPrim = NumData/VB_DESC_TAB[Type].NumDataPerPrim;
	} else
	{
		if( FAILED( Vb->Lock( 0, NumData*VB_DESC_TAB[Type].size, (BFCUChar**)&Data, 0 ) ) )
		{
			return NULL;
		}
	}
	Locked = true;
#endif
	} // end of if not locked

	return Data;
}

BFCVoid *BFCMesh::GetDataPointer2()
{
	// Check validity for some data of the mesh we are adding
	BFCAssert(NumData2,"GetDataPointer2 called before SetNumData2");
	// if not locked
	if (!Locked2)
	{
#ifdef WIN32
	// Create mesh index buffer if Index==NULL
	if (Index==NULL)
	{
		// 2 for 32 bit hardcoded for now
		if(FAILED(
		mGraphicEngine->D3dDevice->CreateIndexBuffer(NumData2*2,0,D3DFMT_INDEX16 ,D3DPOOL_DEFAULT,&Index)
		)) 
		{
			BFCAssert(0,"Can't Create an Index Buffer");
			return NULL;
		}
		if( FAILED( Index->Lock( 0, NumData2*2, (BFCUChar**)&Data2, 0 ) ) )
		{
			return NULL;
		}
		NumPrim = NumData2/VB_DESC_TAB[Type].NumDataPerPrim;
	} else
	{
		if( FAILED( Index->Lock( 0, NumData2*2, (BFCUChar**)&Data2, 0 ) ) )
		{
			return NULL;
		}
	}
	Locked2 = true;
#endif
	} // end of if not locked

	return Data2;
}

BFCGMReturn BFCMesh::DataSet()
{
	if (Locked)
	{
#ifdef WIN32
		Vb->Unlock();
#endif
		Locked = false;		
		return g_m_ok;
	} else return g_m_already_locked;
}

BFCGMReturn BFCMesh::Data2Set()
{
	if (Locked2)
	{
#ifdef WIN32
		Index->Unlock();
#endif
		Locked2 = false;		
		return g_m_ok;
	} else return g_m_already_locked;
}

Win32_unlit_tex_ut_mesh BFCMesh::GetMeshData(BFCUInt n)
{
	Win32_unlit_tex_ut_mesh *vert_data = (Win32_unlit_tex_ut_mesh *)GetDataPointer();
	BFCWord *index_data = (BFCWord *)GetDataPointer2();
	Win32_unlit_tex_ut_mesh data = vert_data[index_data[n]];
	Data2Set();
	DataSet();
	return data;
}

BFCVoid BFCMesh::SetMeshData(BFCUInt n, Win32_unlit_tex_ut_mesh *data)
{
	Win32_unlit_tex_ut_mesh *vert_data = (Win32_unlit_tex_ut_mesh *)GetDataPointer();
	BFCWord *index_data = (BFCWord *)GetDataPointer2();
	vert_data[index_data[n]] = *data;
	Data2Set();
	DataSet();
}

Win32_lit_ut_mesh BFCMesh::GetMeshLitData(BFCUInt n)
{
	Win32_lit_ut_mesh *vert_data = (Win32_lit_ut_mesh *)GetDataPointer();
	BFCWord *index_data = (BFCWord *)GetDataPointer2();
	Win32_lit_ut_mesh data = vert_data[index_data[n]];
	Data2Set();
	DataSet();
	return data;
}

BFCVoid BFCMesh::SetMeshLitData(BFCUInt n, Win32_lit_ut_mesh *data)
{
	Win32_lit_ut_mesh *vert_data = (Win32_lit_ut_mesh *)GetDataPointer();
	BFCWord *index_data = (BFCWord *)GetDataPointer2();
	vert_data[index_data[n]] = *data;
	Data2Set();
	DataSet();
}



//*****************************************************************************************************
//
//
//									Draw Methods
//
//
//*****************************************************************************************************



BFCVoid BFCMesh::DrawLitTTriMesh()
{
#ifdef WIN32
	mD3dDevice->SetStreamSource( 0, Vb, sizeof(Win32_lit_t_mesh) );
	mD3dDevice->SetVertexShader( D3D_LIT_T );
	mD3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, NumPrim );
#endif
}

BFCVoid BFCMesh::DrawLitUTriMesh()
{
#ifdef WIN32
	mD3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)&M);
	mD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	mD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	mD3dDevice->SetStreamSource( 0, Vb, sizeof(Win32_lit_ut_mesh) );
	mD3dDevice->SetVertexShader( D3D_LIT_UT );
	mD3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, NumPrim );
#endif
}

BFCVoid BFCMesh::DrawUnLitUTriMesh()
{
	CommonLightSetting();
#ifdef WIN32
	mD3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)&M);
	mD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	mD3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	mD3dDevice->SetStreamSource( 0, Vb, sizeof(Win32_unlit_ut_mesh) );
	mD3dDevice->SetVertexShader( D3D_UNLIT_UT );
	mD3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, NumPrim );
#endif
}

BFCVoid BFCMesh::CommonLightSetting()
{
#ifdef WIN32
	D3DMATERIAL8 mtrl;
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
	mtrl.Diffuse.r = Material.Diffuse.r;
	mtrl.Diffuse.g = Material.Diffuse.g;
	mtrl.Diffuse.b = Material.Diffuse.b;
	mtrl.Diffuse.a = Material.Diffuse.a;

	mtrl.Ambient.r = Material.Ambient.r;
	mtrl.Ambient.g = Material.Ambient.g;
	mtrl.Ambient.b = Material.Ambient.b;
	mtrl.Ambient.a = Material.Ambient.a;

	mD3dDevice->SetMaterial( &mtrl );
#endif
}

BFCVoid BFCMesh::CommonTexSetting(BFCTexture *tex)
{
	if ( tex->TextureLoaded() )
	{
		mD3dDevice->SetTexture( 0, tex->MyData->VideoTex );
		mD3dDevice->SetTextureStageState( 0, D3DTSS_MAGFILTER, D3DTEXF_LINEAR );
		mD3dDevice->SetTextureStageState( 0, D3DTSS_MINFILTER, D3DTEXF_LINEAR );
		mD3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
		mD3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
		mD3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
		mD3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
	}
	// Disable clipping since I don't understand the Directx clipping way SpectRe
	mD3dDevice->SetRenderState(D3DRS_CLIPPING , FALSE);
	//mD3dDevice->SetRenderState( D3DRS_FILLMODE ,D3DFILL_WIREFRAME);
	if (mEngineState->GetZBuffer()) mD3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		else mD3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	mD3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)&M);
	mD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	mD3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

BFCVoid BFCMesh::CommonNonTexLightSetting()
{
	mD3dDevice->SetTexture( 0, NULL );
	mD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}

BFCVoid BFCMesh::DrawUnLitTexUTriMesh()
{	
/*
	CommonLightSetting();
	CommonTexSetting();
#ifdef WIN32
	mD3dDevice->SetStreamSource( 0, Vb, sizeof(Win32_unlit_tex_ut_mesh) );
	mD3dDevice->SetVertexShader( D3D_UNLIT_TEX_UT );
	mD3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, NumPrim );
#endif
*/
}

BFCVoid BFCMesh::DrawUnLitTexUITriMesh()
{
	BFCTextureIndex *_tex = (BFCTextureIndex *)Tex.GetFirstPtr();
	for (BFCUInt tex_number=0;tex_number<Tex.GetNumber();tex_number++)
	{
		CommonLightSetting();
		// Get the pointer to the texture number
		CommonTexSetting(_tex->Tex);
#ifdef WIN32
		mD3dDevice->SetStreamSource( 0, Vb, sizeof(Win32_unlit_tex_ut_mesh) );
		//mD3dDevice->SetIndices( Index, 0 );
		mD3dDevice->SetIndices( Index, 0 );
		mD3dDevice->SetVertexShader( D3D_UNLIT_TEX_UT );
		// note : if one texture only last param is = NumPrim
		mD3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, NumData, _tex->Index, (_tex->LastIndex-_tex->Index)/3 );
		//mD3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, NumData, 3, 2 );
#endif
		_tex = (BFCTextureIndex *)Tex.GetNextPtr();
	}
}

BFCVoid BFCMesh::DrawLitTLineMesh()
{
#ifdef WIN32
	if (Type2 == interface_mesh) mD3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		else mD3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	mD3dDevice->SetStreamSource( 0, Vb, sizeof(Win32_lit_t_mesh) );
	mD3dDevice->SetVertexShader( D3D_LIT_T );
	mD3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, NumPrim );
#endif
}

BFCVoid BFCMesh::DrawLitUTLineMesh()
{
	CommonNonTexLightSetting();
#ifdef WIN32
	if (Type2 == interface_mesh) mD3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		else mD3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	mD3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)&M);
	mD3dDevice->SetStreamSource( 0, Vb, sizeof(Win32_lit_ut_mesh) );
	mD3dDevice->SetVertexShader( D3D_LIT_UT );
	mD3dDevice->DrawPrimitive( D3DPT_LINELIST, 0, NumPrim );
#endif
}

BFCVoid BFCMesh::DrawLitUTILineMesh()
{
	CommonNonTexLightSetting();
#ifdef WIN32
	if (Type2 == interface_mesh) mD3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		else mD3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	mD3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)&M);
	mD3dDevice->SetStreamSource( 0, Vb, sizeof(Win32_lit_ut_mesh) );
	mD3dDevice->SetIndices( Index, 0 );
	mD3dDevice->SetVertexShader( D3D_LIT_UT );
	mD3dDevice->DrawIndexedPrimitive( D3DPT_LINELIST, 0, NumData, 0, NumPrim );
#endif
}

BFCVoid	BFCMesh::DrawLitUTPointMesh()
{
#ifdef WIN32
	/*
	CommonNonTexSetting();
	// Since the fu!?! directx for the moment doesn't support point list we do triangle list for the moment. SpectRe
	// And so we set the mesh matrix = the camera matrix
	M = *mGraphicEngine->ViewPort.GetView( mGraphicEngine->ViewPort.GetActViewPort() );
	mD3dDevice->SetTransform(D3DTS_WORLD, (D3DMATRIX *)&M);
	mD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	mD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	mD3dDevice->SetStreamSource( 0, Vb, sizeof(Win32_lit_ut_mesh) );
	mD3dDevice->SetVertexShader( D3D_LIT_UT );
	// Since the fu!?! directx for the moment doesn't support point list we do triangle list for the moment. SpectRe
	mD3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, NumPrim );
	*/
#endif
}

BFCVoid BFCMesh::SetScale(BFCVector scale)
{
	Win32_unlit_tex_ut_mesh *data = (Win32_unlit_tex_ut_mesh *)GetDataPointer();
	for (BFCUInt i=0;i<NumData;i++)
	{
		data[i].pos.x *= scale.x/Scale.x;
		data[i].pos.y *= scale.y/Scale.y;
		data[i].pos.z *= scale.z/Scale.z;
	}
	DataSet();
	Scale.x = scale.x;Scale.y = scale.y;Scale.z = scale.z;

}

BFCVoid BFCMesh::ScaleIt(BFCFloat scalex, BFCFloat scaley, BFCFloat scalez)
{
	Win32_unlit_tex_ut_mesh *data = (Win32_unlit_tex_ut_mesh *)GetDataPointer();
	for (BFCUInt i=0;i<NumData;i++)
	{
		data[i].pos.x *= scalex;
		data[i].pos.y *= scaley;
		data[i].pos.z *= scalez;
	}
	DataSet();
	Scale.x *= scalex;Scale.y *= scaley;Scale.z *= scalez;
}

BFCVoid BFCMesh::SetScale(BFCFloat scalex, BFCFloat scaley, BFCFloat scalez)
{
	Win32_unlit_tex_ut_mesh *data = (Win32_unlit_tex_ut_mesh *)GetDataPointer();
	for (BFCUInt i=0;i<NumData;i++)
	{
		data[i].pos.x *= scalex/Scale.x;
		data[i].pos.y *= scaley/Scale.y;
		data[i].pos.z *= scalez/Scale.z;
	}
	DataSet();
	Scale.x = scalex;Scale.y = scaley;Scale.z = scalez;
}

BFCVoid BFCMesh::SetFaceColor(BFCUInt tri_n,BFCUInt color)
{
	Win32_unlit_tex_ut_mesh *data = (Win32_unlit_tex_ut_mesh *)GetDataPointer();
	BFCWord *index = (BFCWord *)GetDataPointer2();

	// Set color to triangle
	data[index[tri_n*3]].color = color;
	data[index[tri_n*3+1]].color = color;
	data[index[tri_n*3+2]].color = color;


	DataSet();
	Data2Set();
}

BFCVoid BFCMesh::RotateX(BFCFloat ang)
{
	// Save the pos
	BFCVector pos = M.GetPos();
	// Position the object at origin
	M.SetPos(0.0f,0.0f,0.0f);
	// Rotate
	M.RotateX(ang);
	// Set back the pos
	M.SetPos(pos);
	// Refresh the X rot info var
	RotX += ang;
}

BFCVoid BFCMesh::RotateY(BFCFloat ang)
{
	// Save the pos
	BFCVector pos = M.GetPos();
	// Position the object at origin
	M.SetPos(0.0f,0.0f,0.0f);
	// Rotate
	M.RotateY(ang);
	// Set back the pos
	M.SetPos(pos);
	// Refresh the X rot info var
	RotY += ang;
}

BFCVoid BFCMesh::RotateZ(BFCFloat ang)
{
	// Save the pos
	BFCVector pos = M.GetPos();
	// Position the object at origin
	M.SetPos(0.0f,0.0f,0.0f);
	// Rotate
	M.RotateZ(ang);
	// Set back the pos
	M.SetPos(pos);
	// Refresh the X rot info var
	RotZ += ang;
}




