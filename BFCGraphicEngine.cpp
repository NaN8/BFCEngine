#include <BFCGraphicEngine.h>
#include <D3dx8math.h>
#include <stdio.h>
#include <BFCManager.h>
#include <Primitives.h>



#ifdef WIN32
	IDirect3DDevice8 *mD3dDevice = NULL;
#endif


//************************************************************************
//
//						BFCViewPort
//
//************************************************************************

BFCViewPort::BFCViewPort() : Num(0), ActViewPort(0)
{

}

BFCVoid BFCViewPort::DefaultInit()
{
	DeleteAllViewPort();
	AddViewPort();

	// Set View Matrix
	BFCMatrix view;
	view.Identity();
	SetView(0,&view);

	// Set Projection matrix

	BFCMatrix proj;
	proj.ProjectionMatrix(bfc_perspec_proj,1.0f,10000.0f,PI*0.3333f,PI*0.3333f); // 0.3333 ~= 1/3
	SetProj(0,&proj,bfc_perspec_proj);	

	// Set Actual Client screen Frustum

	RefreshActSysFrustum();
	SetFrustum(0,&SysFrustum);

	SetActViewPort(0);
}

BFCViewPort::AddViewPort()
{
	View.Add(new BFCViewPortData() );
	Num++;
}

BFCViewPort::DeleteViewPort(BFCUInt n)
{
	View.RemoveDelete(n);
	Num--;
}

BFCViewPort::DeleteAllViewPort()
{
	View.RemoveDeleteAll();
	Num = 0;
}

BFCUInt BFCViewPort::GetNumViewport()
{
	return Num;
}

BFCMatrix *BFCViewPort::GetView(BFCUInt n) const
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	if (data) return &(data->View); else return NULL;
}

BFCMatrix *BFCViewPort::GetProj(BFCUInt n) const
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	if (data) return &(data->Proj); else return NULL;
}

BFCViewType BFCViewPort::GetViewType(BFCUInt n) const
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	if (data) return data->ViewType; else
	{
		BFCAssert(0, "GetViewType called on an inexistant ViewPort");
		return bfc_perspec_view;
	}
}

BFCVector BFCViewPort::GetCameraPos(BFCUInt n) const
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	if (data) return data->CamPos; else
	{
		BFCAssert(0, "GetCameraPos called on an inexistant ViewPort");
		return BFCVector(0.0f,0.0f,0.0f);
	}
}

BFCFloat BFCViewPort::GetXRot(BFCUInt n) const
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	if (data) return data->XRot; else
	{
		BFCAssert(0, "GetXRot called on an inexistant ViewPort");
		return 0.0f;
	}
}

BFCFloat BFCViewPort::GetYRot(BFCUInt n) const
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	if (data) return data->YRot; else
	{
		BFCAssert(0, "GetYRot called on an inexistant ViewPort");
		return 0.0f;
	}
}

BFCVoid BFCViewPort::RefreshActSysFrustum()
{
#ifdef WIN32
	mD3dDevice->GetViewport((D3DVIEWPORT8 *)&SysFrustum);
#endif
}

BFCFrustum *BFCViewPort::GetFrustum(BFCUInt n) const
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	if (data) return &(data->Frustum); else return NULL;
}

BFCFloat BFCViewPort::GetScale(BFCUInt n) const
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	BFCAssert(data,"Trying to get scale on a NULL viewport");
	return data->Scale;
}

BFCProjectionType BFCViewPort::GetType(BFCUInt n) const
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	BFCAssert(data,"Trying to get viewport type on a NULL viewport");
	return data->Type;
}

BFCVoid BFCViewPort::SetScale(BFCUInt n, BFCFloat v)
{
	BFCVector scale;
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	BFCAssert(data,"Trying to set a NULL viewport");
	BFCAssert(v,"Trying to set a scale of 0");
	scale.x = 1/data->Scale; scale.z = scale.y = scale.x;
	data->Proj.SetScale(scale);
	scale.x = v; scale.z = scale.y = scale.x;
	data->Proj.SetScale(scale);
	data->Scale = v;
}

BFCVoid BFCViewPort::SetView(BFCUInt n, const BFCMatrix *m)
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	BFCAssert(data,"Trying to set view on a NULL viewport");
	data->View = *m;
}

BFCVoid BFCViewPort::SetProj(BFCUInt n, const BFCMatrix *m, BFCProjectionType type)
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	BFCAssert(data,"Trying to set proj on a NULL viewport");
	data->Proj = *m;
	data->Type = type;
}

BFCVoid BFCViewPort::SetFrustum(BFCUInt n, const BFCFrustum *s)
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	BFCAssert(data,"Trying to set frustum on a NULL viewport");
	data->Frustum = *s;
}

BFCVoid BFCViewPort::SetWidth(BFCUInt n, BFCDWord v)
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	BFCAssert(data,"Trying to set width on a NULL viewport");
	data->Frustum.width = v;
}

BFCVoid BFCViewPort::SetXPos(BFCUInt n, BFCDWord v)
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	BFCAssert(data,"Trying to set height on a NULL viewport");
	data->Frustum.x = v;
}

BFCVoid BFCViewPort::SetYPos(BFCUInt n, BFCDWord v)
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	BFCAssert(data,"Trying to set YPos on a NULL viewport");
	data->Frustum.y = v;
}

BFCVoid BFCViewPort::SetXRot(BFCUInt n, BFCFloat v)
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	BFCAssert(data,"Trying to set XRot on a NULL viewport");
	data->XRot = v;
}

BFCVoid BFCViewPort::SetYRot(BFCUInt n, BFCFloat v)
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	BFCAssert(data,"Trying to set YRot on a NULL viewport");
	data->YRot = v;
}

BFCVoid BFCViewPort::SetType(BFCUInt n, BFCProjectionType type)
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	BFCAssert(data,"Trying to set viewport type on a NULL viewport");
	data->Type = type;
}

BFCVoid BFCViewPort::SetViewType(BFCUInt n, BFCViewType view_type)
{
	BFCMatrix proj;
	BFCMatrix view;
	// we check if n viewport exist
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	BFCAssert(data,"Trying to set viewport View type on a NULL viewport");
	switch(view_type)
	{
	case bfc_perspec_view:
		{
			// Set View Matrix
			view.Identity();
			SetView(n,&view);

			// Set Projection matrix
			proj.ProjectionMatrix(bfc_perspec_proj,1.0f,10000.0f,PI*0.3333f,PI*0.3333f); // 0.3333 ~= 1/3
			SetProj(n,&proj,bfc_perspec_proj);	
			data->ViewType = bfc_perspec_view;
		} break;
	case bfc_top_view:
		{
			// Set Projection matrix
			proj.ProjectionMatrix(bfc_ortho_proj,1.0f,10000.0f,PI*0.3333f,PI*0.3333f); // 0.3333 ~= 1/3
			SetProj(n,&proj,bfc_ortho_proj);	
			// We set the standard view for this viewport
			SetRotateX(n,-PI/2,true);
			SetCameraPosition(n,0.0f,500.0f,0.0f);
			data->ViewType = bfc_top_view;
		} break;
	case bfc_left_view:
		{
			// Set Projection matrix
			proj.ProjectionMatrix(bfc_ortho_proj,1.0f,10000.0f,PI*0.3333f,PI*0.3333f); // 0.3333 ~= 1/3
			SetProj(n,&proj,bfc_ortho_proj);	
			// We set the standard view for this viewport
			SetRotateY(n,-PI/2,true);
			SetCameraPosition(n,-500.0f,0.0f,0.0f);
			data->ViewType = bfc_left_view;
		} break;
	case bfc_front_view:
		{
			// Set Projection matrix
			proj.ProjectionMatrix(bfc_ortho_proj,1.0f,10000.0f,PI*0.3333f,PI*0.3333f); // 0.3333 ~= 1/3
			SetProj(n,&proj,bfc_ortho_proj);	
			// We set the standard view for this viewport
			SetCameraPosition(n,0.0f,0.0f,-500.0f);
			data->ViewType = bfc_front_view;
		} break;
	}
}

BFCVoid BFCViewPort::SetActViewPort(BFCUInt n)
{
	ActViewPort = n;
	BFCFrustum *fru = GetFrustum(n);
	BFCMatrix *view = GetView(n);
	BFCMatrix *proj = GetProj(n);
#ifdef WIN32
	// ifdef WIN32, BFCFrustum must be like D3DVIEWPORT8 !!!
	if (mD3dDevice->SetViewport( (D3DVIEWPORT8 *)fru )!=D3D_OK)
	{
		BFCAssert(0,"Unable to set the D3D viewport");
	}
	mD3dDevice->SetTransform(D3DTS_VIEW, (D3DMATRIX *)view);
 	mD3dDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)proj);
#endif
}

BFCVoid BFCViewPort::SetRotateX(BFCUInt n,BFCFloat angle,BFCBool apply)
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	// we refresh XRot with the new value
	data->XRot=angle;
	if (apply)
	{
		BFCMatrix tmpview;
		tmpview.Identity();
		tmpview.RotateY( data->YRot );
		tmpview.RotateX( data->XRot );
		// now we set value
		SetView(n,&tmpview);
		SetCameraPosition(n,&(data->CamPos));
	}
}

BFCVoid BFCViewPort::SetRotateY(BFCUInt n,BFCFloat angle, BFCBool apply)
{
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	// we refresh XRot with the new value
	data->YRot=angle;
	if (apply)
	{
		BFCMatrix tmpview;
		tmpview.Identity();
		tmpview.RotateY( data->YRot );
		tmpview.RotateX( data->XRot );
		// now we set value
		SetView(n,&tmpview);
		SetCameraPosition(n,&(data->CamPos));
	}
}

BFCVoid BFCViewPort::SetCameraPosition(BFCUInt n, BFCVector *v)
{
	BFCVector v2;
	// Here we save the "real" cam pos
	BFCViewPortData *data = (BFCViewPortData *) View.FindPtr(n);
	data->CamPos = *v;
	// hmmmm...
	v2.x = -v->x;
	v2.y = -v->y;
	v2.z = -v->z;
	BFCMatrix *view = GetView(n);
	// Transform the vector to take care of the view rotation and then set the pos
	view->SetPos(view->MulVecOnly(v2));
}

BFCVoid BFCViewPort::SetCameraPosition(BFCUInt n, BFCFloat x, BFCFloat y, BFCFloat z)
{
	BFCVector v(x,y,z);
	SetCameraPosition(n, &v);
}

//************************************************************************
//
//						BFCGraphicEngine
//
//************************************************************************


BFCGraphicEngine::BFCGraphicEngine() : BFCBigBase(bfc_graphic_engine), PickedTri(-1), PointSize(1.0f), PickedVert(-1),
									   LastTValue(0.0f)
{
	ChangeViewLoop = new BFCPipeline("Change View Loop");
	mEngineState = &EngineState;
}

BFCGEReturn BFCGraphicEngine::Init(BFCUInt xres,BFCUInt yres)
{
#ifdef WIN32
	if( NULL == ( D3d = Direct3DCreate8( D3D_SDK_VERSION ) ) )
	{
		BFCAssert(1,"Can't create D3d");
		return g_e_error;
	}

	if( FAILED( D3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &D3ddm ) ) )
	{
		BFCAssert(1,"Can't get adapter display mode");
		return g_e_error;
	}

	D3DPRESENT_PARAMETERS D3dpp; 
	ZeroMemory( &D3dpp, sizeof(D3dpp) );
	D3dpp.Windowed   = TRUE;
	D3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3dpp.BackBufferFormat = D3ddm.Format;
	D3dpp.BackBufferWidth = xres;
	D3dpp.BackBufferHeight = yres;
	// hmmm to check these two...
    D3dpp.EnableAutoDepthStencil = TRUE;
    D3dpp.AutoDepthStencilFormat = D3DFMT_D16;
#ifdef BFC_EDITOR
	D3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
#endif



	if( FAILED( D3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mhwnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&D3dpp, &D3dDevice ) ) )
	{
		BFCAssert(1,"Can't create D3d device");
		return g_e_error;
	}
	mD3dDevice = D3dDevice;
#endif

	ViewPort.DefaultInit();

	return g_e_ok;
}

BFCVoid BFCGraphicEngine::SetPointSize(BFCFloat size)
{
	PointSize = size;
#ifdef WIN32
	// Not supported in directx for the moment ?
	//mD3dDevice->SetRenderState(D3DRS_POINTSIZE,(DWORD)size);
	//mD3dDevice->SetRenderState(D3DRS_POINTSIZE_MIN,(DWORD)size);
#endif
}

BFCVoid BFCGraphicEngine::SetAmbient(BFCColorByte *col)
{
#ifdef WIN32
	mD3dDevice->SetRenderState( D3DRS_AMBIENT, (unsigned long)col->color );
#endif

	Ambient = *col;
}

const BFCColorByte *BFCGraphicEngine::GetAmbient(BFCColorByte *col) const
{
	return &Ambient;
}

BFCVoid BFCGraphicEngine::DisableAllTexMesh()
{
	BFCMesh *_mesh = (BFCMesh *)Mesh.GetFirst();
	while (_mesh)
	{
		if (_mesh->GetType() == unlit_tex_ut_i_tri_mesh)
		{
			_mesh->SetEnablePipe(false);
		}
		_mesh = (BFCMesh *)Mesh.GetNext();
	}
}

BFCVoid BFCGraphicEngine::EnableAllTexMesh()
{
	BFCMesh *_mesh = (BFCMesh *)Mesh.GetFirst();
	while (_mesh)
	{
		if (_mesh->GetType() == unlit_tex_ut_i_tri_mesh)
		{
			_mesh->SetEnablePipe(true);
		}
		_mesh = (BFCMesh *)Mesh.GetNext();
	}
}

BFCVoid BFCGraphicEngine::Execute()
{
	// Execute Draw Pipeline for every viewport
	for (BFCUInt i=0;i<ViewPort.Num;i++)
	{
		// Set corresponding viewport
		ViewPort.SetActViewPort(i);

		// Execute Change View Loop level pipelines

		ChangeViewLoop->Execute();

		mD3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0L );
		mD3dDevice->BeginScene();

		// Execute Draw pipeline of all normal meshes
		BFCMesh *_mesh = (BFCMesh *)Mesh.GetFirst();
		while (_mesh)
		{
			_mesh->Execute();
			_mesh = (BFCMesh *)Mesh.GetNext();
		}

		// Execute Draw pipeline of all interface meshes
		BFCMesh *_imesh = (BFCMesh *)InterfaceMesh.GetFirst();
		while (_imesh)
		{
			_imesh->Execute();
			_imesh = (BFCMesh *)InterfaceMesh.GetNext();
		}
		mD3dDevice->EndScene();
		
	}
	mD3dDevice->Present( NULL, NULL, NULL, NULL );
	
}

BFCGEReturn BFCGraphicEngine::AddMesh(BFCMesh *mesh)
{
	// Test of vert or index data are locked and unlock if the condition meet
	if (mesh->Locked)
	{
#ifdef WIN32
		mesh->Vb->Unlock();
#endif
		mesh->Locked = false;
	}
	// if locked unlock index now
	if (mesh->Locked2)
	{
#ifdef WIN32
		mesh->Index->Unlock();
#endif
		mesh->Locked2 = false;
	}
	// Add the mesh to the right list
	switch (mesh->Type2)
	{
	case interface_mesh:
		{
			InterfaceMesh.Add(NULL,mesh);
		} break;
	default:
		{
			Mesh.Add(NULL,mesh);
		} break;
	}
	return g_e_ok;
}

BFCGEReturn BFCGraphicEngine::RemoveMesh(BFCMesh *mesh, BFCBool delete_it)
{
	switch (mesh->Type2)
	{
	case interface_mesh:
		{
			InterfaceMesh.Remove(mesh, delete_it);
		} break;
	default:
		{
			Mesh.Remove(mesh, delete_it);
		} break;
	}
	return g_e_ok;
}

BFCGEReturn BFCGraphicEngine::AddLight(BFCLight *light)
{
	Light.Add(light);
	light->Number++;
	light->Added = true;
#ifdef WIN32
	mD3dDevice->SetLight(light->MyID, &(light->D3dLight) );
#endif
	return g_e_ok;
}

BFCGEReturn BFCGraphicEngine::RemoveLight(BFCLight *light)
{
	if ( Light.Remove(light) ) light->Number--; else
	{
		BFCAssert(0,"Trying to remove a light not in the engine list");
		return g_e_error;
	}
	light->Added = false;
	return g_e_ok;
}

BFCGEReturn BFCGraphicEngine::Close()
{
#ifdef WIN32
	if (D3dDevice!=NULL)
	{
		D3dDevice->Release();
	}
	if (D3d!=NULL)
	{
		D3d->Release();
	}
#endif

	return g_e_ok;
}

// Transform value taking care of resolution change
BFCVoid BFCGraphicEngine::TransWithRes(BFCInt *x,BFCInt *y)
{
	*x = (int)(( (float)*x / (float)ViewPort.GetActWindowRect().right) * (float)ViewPort.GetSysFrustum().width);
	*y = (int)(( (float)*y / (float)ViewPort.GetActWindowRect().down) * (float)ViewPort.GetSysFrustum().height);
}

BFCVoid BFCGraphicEngine::CameraToWorld(BFCMesh *mesh, BFCInt vp, BFCVector *v)
{
	// Get the view matrix
	BFCMatrix view = *ViewPort.GetView(vp);
	// Get view pos
	BFCVector pos = view.GetPos();
	// Get the inverse of the view matrix
	view.Inverse();
	// Check SetCameraPos we need to take care of the right visual pos.
	// The inverse is not perfect right now so we complete it "manually" SpectRe
	pos = view.MulVecOnly(pos);
	// transform *v to world space
	*v = view.MulVecOnly(*v);
	*v = *v - pos;
}

BFCVoid BFCGraphicEngine::WorldToLocal(BFCMatrix *mat, BFCVector *v)
{
	// local copy of the world matrix
	BFCMatrix _mat = *mat;
	// Get world pos
	BFCVector pos = _mat.GetPos();
	// Get the inverse of the world matrix
	_mat.Inverse();
	// The inverse is not perfect right now so we complete it "manually" SpectRe
	pos = _mat.MulVecOnly(pos);
	// transform *v to world space
	*v = _mat.MulVecOnly(*v);
	*v = *v - pos;
}

BFCVoid BFCGraphicEngine::CameraToLocal(BFCMesh *mesh, BFCInt vp, BFCVector *v)
{
	// Get the view matrix
	BFCMatrix view = *ViewPort.GetView(vp);
	// Get view pos
	BFCVector pos = view.GetPos();
	// Get the inverse of the view matrix
	view.Inverse();
	// Check SetCameraPos we need to take care of the right visual pos.
	// The inverse is not perfect right now so we complete it "manually" SpectRe
	pos = view.MulVecOnly(pos);
	// transform *v to world space
	*v = view.MulVecOnly(*v);

	BFCMatrix mat = mesh->M;
	mat.Inverse();
	// Get the object pos that will be transformed
	BFCVector trans_pos = mat.GetPos() ;
	// transform the mesh pos
	// The inverse is not perfect right now so we complete it "manually" SpectRe
	trans_pos = mat.MulVecOnly(trans_pos);
	// substract the transformed pos ( the camera pos in local object space)
	trans_pos = trans_pos - mat.MulVecOnly(pos);
	// transform *v with the inverse matrix mesh
	*v = mat.MulVecOnly(*v);

	// Add the transformed vector pos
	*v = *v + trans_pos;
}

BFCFloat BFCGraphicEngine::GetCameraZPosFromLocal(BFCMesh *mesh, BFCInt vp, BFCVector &point)
{

	BFCMatrix view = *ViewPort.GetView(vp);
	// Transform to world
	BFCVector trans = mesh->M * point;
	// Transform to camera space
	trans = view * trans;
	return trans.z;
}

// Return a line going from picked pos in local camera space

BFCLine BFCGraphicEngine::GetCameraLineFromPos(BFCInt vp,BFCInt x, BFCInt y)
{
	// First of all transform x and y to the right value for corresponding viewport. 
	// Why ? I don't know why I should do that for directx?!! Seems directx keep initial windows viewport setting...
	// ... Maybe of back buffer width and height   :/   SpectRe
	// This down here is awfull!!!!
	TransWithRes(&x,&y);

	BFCVector pick;	// pick screen vector
	BFCFrustum *fru = ViewPort.GetFrustum(vp);
	BFCMatrix *proj = ViewPort.GetProj(vp);
	BFCLine line; // pick ray line

	// transform x and y for the viewport
	x -= fru->x;
	y -= fru->y;
	// Get the pick vector in screen space
	pick.x = ( (2.0f * (float)x) / (fru->width) -1.0f ) / proj->_00;
	pick.y = -( (2.0f * (float)y) / (fru->height) -1.0f ) / proj->_11;
	pick.z = 1.0f;

	// Check if viewport is ortho
	if (ViewPort.GetType(vp) == bfc_ortho_proj)
	{
		// if yes set line pos
		line.Pos = pick;
		line.Pos.z = 0.0f;
	}
	line.Dir = pick;
	return line;
}

// Return a line going from picked pos in local world of mesh.

BFCLine BFCGraphicEngine::GetLocalLineFromPos(BFCMesh *mesh, BFCInt vp,BFCInt x, BFCInt y)
{
	BFCVector v;
	BFCLine line; // pick ray line

	line = GetCameraLineFromPos(vp,x,y);

	// Get the view matrix
	BFCMatrix view = *ViewPort.GetView(vp);
	// Set line pos with inverse view pos
	v = view.GetPos();
	// Get the inverse of the view matrix
	view.Inverse();
	// This trans is needed to avoid to trans all tri check also SetCameraPos we need to take care of the right visual pos
	// The inverse is not perfect right now so we complete it "manually"
	v = view.MulVecOnly(v);
	// transform line to world space
	line.Dir = view.MulVecOnly(line.Dir);
	// actually this line is usefull only for ortho since in perspec Pos = (0,0,0)
	line.Pos = view.MulVecOnly(line.Pos);

	BFCMatrix mat = mesh->M;
	mat.Inverse();
	// Get the object pos that will be transformed for the line
	BFCVector trans_pos = mat.GetPos() ;
	// transform the vector pos for the line
	// The inverse is not perfect right now so we complete it "manually" SpectRe
	trans_pos = mat.MulVecOnly(trans_pos);
	// substract the transformed v ( the camera pos in local object space)
	trans_pos = trans_pos - mat.MulVecOnly(v);
	// transform line with the inverse instead of all tri of prim
	line.Dir = mat.MulVecOnly(line.Dir);
	line.Pos = mat.MulVecOnly(line.Pos);

	// Add the transformed vector to the line
	line.Pos = line.Pos + trans_pos;
	line.Dir = line.Dir + trans_pos;

	return line;
}


BFCMesh *BFCGraphicEngine::PickVert(BFCInt vp,BFCInt x, BFCInt y)
{
	BFCFloat smallest_t = BFCFarValue;
	BFCMesh *nearest_mesh = NULL;
	BFCMesh *mesh = (BFCMesh *)Mesh.GetFirst();
	BFCVoid *data = NULL;
	BFCVoid *data2 = NULL;
	BFCLine local_line;	// transformed line to "local" coord of mesh
	BFCVector trans_pos;	// transformed position of the line to be like there was an object rotation
	// GO trough all mesh
	while (mesh)
	{
		// Checking depends of mesh Type
		switch (mesh->Type)
		{
		case unlit_tex_ut_tri_mesh:
			{
				// Cast to the right data type
				Win32_unlit_tex_ut_mesh *format =  (Win32_unlit_tex_ut_mesh *) data;
				
				// we get the pick line
				local_line = GetLocalLineFromPos(mesh,vp,x,y);

				// format index
				BFCUInt i = 0;
				// Now go trough all data and check
				while (i<mesh->NumData)
				{
					// Is there an intersection in this triangle of this mesh ?
					if (local_line.IsNearVert(format[i].pos,1.0f))
					{
						if (local_line.LastTValue<smallest_t && local_line.LastTValue>0)
						{
							PickedVert = i;
							smallest_t = local_line.LastTValue;
							LastTValue = smallest_t;
							nearest_mesh = mesh;
						}
					}
					i++;
				}
			} break;
		case unlit_tex_ut_i_tri_mesh:
			{
				data = mesh->GetDataPointer();
				data2 = mesh->GetDataPointer2();

				// Cast to the right data type
				Win32_unlit_tex_ut_mesh *format =  (Win32_unlit_tex_ut_mesh *) data;
				BFCWord *index = (BFCWord *) data2;

				// we get the pick line
				local_line = GetLocalLineFromPos(mesh,vp,x,y);

				// format index
				BFCUInt i = 0;
				// Now go trough all data and check
				while (i<mesh->NumData2)
				{
					// Is the line passing near the vert
					if (local_line.IsNearVert(format[index[i]].pos,0.0f))
					{
						if (local_line.LastTValue<smallest_t && local_line.LastTValue>0)
						{
							PickedVert = i;
							smallest_t = local_line.LastTValue;
							LastTValue = smallest_t;
							nearest_mesh = mesh;
						}
					}
					i++;
				}
			} break;
		}
		mesh = (BFCMesh *)Mesh.GetNext();
	}
	return nearest_mesh;
}

BFCMesh *BFCGraphicEngine::PickMesh(BFCInt vp, BFCInt x, BFCInt y)
{
	BFCFloat smallest_t = BFCFarValue;
	BFCMesh *nearest_mesh = NULL;
	BFCMesh *mesh = (BFCMesh *)Mesh.GetFirst();
	BFCVoid *data = NULL;
	BFCVoid *data2 = NULL;
	BFCLine local_line;	// transformed line to "local" coord of mesh
	BFCVector trans_pos;	// transformed position of the line to be like there was an object rotation
	// GO trough all mesh
	while (mesh)
	{
		// Checking depends of mesh Type
		switch (mesh->Type)
		{
		case unlit_tex_ut_tri_mesh:
			{
				// Cast to the right data type
				Win32_unlit_tex_ut_mesh *format =  (Win32_unlit_tex_ut_mesh *) data;
				// primitive index
				BFCUInt n_pri = 0;

				// we get the pick line
				local_line = GetLocalLineFromPos(mesh,vp,x,y);

				// format index
				BFCUInt i = 0;
				// Now go trough all data and check
				while (n_pri<mesh->NumPrim)
				{
					// Is there an intersection in this triangle of this mesh ?
					if (local_line.IsTriIntersection(format[i].pos,format[i+1].pos,format[i+2].pos))
					{
						if (local_line.LastTValue >= 0)
						{
							if (local_line.LastTValue<smallest_t)
							{
								PickedPoint = local_line.LastPointIntersect;
								PickedTri = n_pri;
								smallest_t = local_line.LastTValue;
								nearest_mesh = mesh;
							}
						}
					}
					n_pri++;
					i+=3;	// in this case 3 data per primitive
				}
			} break;
		case unlit_tex_ut_i_tri_mesh:
			{
				data = mesh->GetDataPointer();
				data2 = mesh->GetDataPointer2();
				// Cast to the right data type
				Win32_unlit_tex_ut_mesh *format =  (Win32_unlit_tex_ut_mesh *) data;
				BFCWord *index = (BFCWord *) data2;
				// primitive index
				BFCUInt n_pri = 0;

				// we get the pick line
				local_line = GetLocalLineFromPos(mesh,vp,x,y);

				// format index
				BFCUInt i = 0;
				// Now go trough all data and check
				while (n_pri<mesh->NumPrim)
				{
					// Is there an intersection in this triangle of this mesh ?
					if (local_line.IsTriIntersection(format[index[i]].pos,format[index[i+1]].pos,format[index[i+2]].pos))
					{
						if (local_line.LastTValue >= 0)
						{
							if (local_line.LastTValue<smallest_t)
							{
								PickedPoint = local_line.LastPointIntersect;
								PickedTri = n_pri;
								smallest_t = local_line.LastTValue;
								nearest_mesh = mesh;
							}
						}
					}
					n_pri++;
					i+=3;	// in this case 3 data per primitive
				}
				mesh->DataSet();
				mesh->Data2Set();
			} break;
		}
		mesh = (BFCMesh *)Mesh.GetNext();
	}
	return nearest_mesh;
}

// Line to mesh is not finished !!!!!!!!!!!!!

BFCMesh *BFCGraphicEngine::LineToMesh(BFCLine *line)
{
	BFCMesh *mesh = (BFCMesh *)Mesh.GetFirst();
	BFCVoid *data = NULL;
	BFCLine local_line;	// transformed line to "local" coord of mesh
	BFCVector trans_pos;	// transformed position of the line to be like there was an object rotation
	// GO trough all mesh
	while (mesh)
	{
		data = mesh->Data;
		// Checking depends of mesh Type
		if (mesh->Type==unlit_tex_ut_tri_mesh)
		{
			// Cast to the right data type
			Win32_unlit_tex_ut_mesh *format =  (Win32_unlit_tex_ut_mesh *) data;
			// primitive index
			BFCUInt n_pri = 0;
			// format index
			BFCUInt i = 0;
			// mesh matrix
			BFCMatrix mat = mesh->M;
			mat.Inverse();
			// Get the object pos that will be transformed for the line
			trans_pos = mat.GetPos();
			// transform the vector pos for the line
			trans_pos = mat.MulVecOnly(trans_pos);
			// transform line with the inverse instead of all tri of prim
			local_line.Dir = mat.MulVecOnly(line->Dir);
			local_line.Pos = mat.MulVecOnly(line->Pos);
			// Add the transformed vector to the line
			local_line.Pos = local_line.Pos + trans_pos;
			local_line.Dir = local_line.Dir + trans_pos;

			// Now go trough all data and check
			while (n_pri<mesh->NumPrim)
			{
				// Is there an intersection in this triangle of this mesh ?
				if (local_line.IsTriIntersection(format[i].pos,format[i+1].pos,format[i+2].pos))
				{
					if (local_line.LastTValue >= 0)
					{

						PickedPoint = local_line.LastPointIntersect;
						PickedTri = n_pri;
						return mesh;
					}
				}
				n_pri++;
				i+=3;	// in this case 3 data per primitive
			}
		}
		mesh = (BFCMesh *)Mesh.GetNext();
	}
	return NULL;	// no intersection
}

// MeshIntTri return true if the mesh is intersecting the tri, it return true when the tri is completly inside and touching all the mesh.
// return true when the tri is completly inside

BFCBool BFCGraphicEngine::MeshIntTri(BFCMesh *mesh, BFCTri *tri)
{
	BFCBool on_border = false;
	BFCLine line;
	// here we get mesh data pointers
	Win32_unlit_tex_ut_mesh *data = (Win32_unlit_tex_ut_mesh *) mesh->GetDataPointer();
	BFCWord *index = (BFCWord *) mesh->GetDataPointer2();
	// tell if there was an intersection of a line number n
	BFCBool intersection_line_1 = false;
	BFCBool intersection_line_2 = false;
	BFCBool intersection_line_3 = false;

	// the t value of the last intersection with line n
	BFCFloat intersection_t_1;
	BFCFloat intersection_t_2;
	BFCFloat intersection_t_3;

//	mDebugLineMesh.CreateDebugLine(4,4);

	BFCUInt i=0;
	while (i<mesh->GetNumData2())
	{	
		BFCTri _tri;
		// Transform and copy pos to world
		_tri.v1.pos = mesh->M * data[index[i]].pos;
		_tri.v2.pos = mesh->M * data[index[i+1]].pos;
		_tri.v3.pos = mesh->M * data[index[i+2]].pos;

		// Check first edge into tri

		line.Pos = _tri.v1.pos;
		line.Dir = _tri.v2.pos;
/*
		mDebugLineMesh.SetDebugLine(4,&tri.v1.pos,&tri.v2.pos,0,0xFF881138);
		mDebugLineMesh.SetDebugLine(4,&tri.v2.pos,&tri.v3.pos,1,0xFF881138);
		mDebugLineMesh.SetDebugLine(4,&tri.v3.pos,&tri.v1.pos,2,0xFF881138);

		mDebugLineMesh.SetDebugLine(4,&_tri.v1.pos,&_tri.v2.pos,3,0xFF001138);
		mDebugLineMesh.DebugLineSet(4);
		DebugVisualize();
*/
		// If there is an intersection
		if (line.IsTriIntersection(tri->v1.pos,tri->v2.pos,tri->v3.pos,&on_border,50.0f))
		{
			// if it's on border we don't consider as an intersection
			if (!on_border)
			{
				// These line are for tolerance regulation in relation with the length of the line
				// They still need to be corrolate with "prec" parameter of this function
				BFCFloat _length = (line.Dir - line.Pos).Length();
				BFCFloat _tol;
				if (_length>=0.2f)
				{
					_tol = 0.1f / _length;
				} else
				{
					_tol = 0.5f;
				}

				// we check if the line is in the tri
				if (line.LastTValue<1.0f - _tol && line.LastTValue>0.0f + _tol)
				{
					// if yes set that there was an intersection with this line
					intersection_line_1 = true;
					intersection_t_1 = line.LastTValue;
				}
			}
		}

		on_border = false;

		// Check second edge into tri

		line.Pos = _tri.v2.pos;
		line.Dir = _tri.v3.pos;

		// If there is an intersection
		if (line.IsTriIntersection(tri->v1.pos,tri->v2.pos,tri->v3.pos,&on_border,50.0f))
		{
			// if it's on border we don't consider as an intersection
			if (!on_border)
			{
				// These line are for tolerance regulation in relation with the length of the line
				// They still need to be corrolate with "prec" parameter of this function
				BFCFloat _length = (line.Dir - line.Pos).Length();
				BFCFloat _tol;
				if (_length>=0.2f)
				{
					_tol = 0.1f / _length;
				} else
				{
					_tol = 0.5f;
				}

				// we check if the line is in the tri
				if (line.LastTValue<1.0f - _tol && line.LastTValue>0.0f + _tol)
				{
					// if yes set that there was an intersection with this line
					intersection_line_2 = true;
					intersection_t_2 = line.LastTValue;
				}
			}
		}

		on_border = false;

		// Check third edge into tri

		line.Pos = _tri.v3.pos;
		line.Dir = _tri.v1.pos;

		// If there is an intersection
		if (line.IsTriIntersection(tri->v1.pos,tri->v2.pos,tri->v3.pos,&on_border,50.0f))
		{
			// if it's on border we don't consider as an intersection
			if (!on_border)
			{
				// These line are for tolerance regulation in relation with the length of the line
				// They still need to be corrolate with "prec" parameter of this function
				BFCFloat _length = (line.Dir - line.Pos).Length();
				BFCFloat _tol;
				if (_length>=0.2f)
				{
					_tol = 0.1f / _length;
				} else
				{
					_tol = 0.5f;
				}
				// we check if the line is in the tri
				if (line.LastTValue<1.0f - _tol && line.LastTValue>0.0f + _tol)
				{
					// if yes set that there was an intersection with this line
					intersection_line_3 = true;
					intersection_t_3 = line.LastTValue;
				}
			}
		}

		i+=3;
	}

	mesh->DataSet();
	mesh->Data2Set();

	// This the last check to see if the tri is maybe inside or line of mesh are not intersecting the tri but the tri intersect the mesh

	line.Pos = tri->v1.pos;
	line.Dir = tri->v2.pos;
	BFCLineIntoMesh _tmp = LineInsideMesh(mesh,&line,true,1.0f);
	line.Pos = tri->v2.pos;
	line.Dir = tri->v3.pos;
	BFCLineIntoMesh _tmp2 = LineInsideMesh(mesh,&line,true,1.0f);
	line.Pos = tri->v3.pos;
	line.Dir = tri->v1.pos;
	BFCLineIntoMesh _tmp3 = LineInsideMesh(mesh,&line,true,1.0f);

	if (_tmp == bfc_line_in)
	{
		if (_tmp2 == bfc_line_in)
		{
			if (_tmp3 == bfc_line_in) return true;
		}
	}

	if (_tmp == bfc_line_int) return true;
	if (_tmp2 == bfc_line_int) return true;
	if (_tmp3 == bfc_line_int) return true;

	// if there was an intersection
	if (intersection_line_1 || intersection_line_2 || intersection_line_3)
	{
		return true;
	}
	return false;
}


BFCLineIntoMesh BFCGraphicEngine::LineInsideMesh(BFCMesh *mesh, BFCLine *line, BFCBool border_inside, BFCFloat prec)
{
	// this is for intersection in angle, it's for not counting twice the same line state change ( for ex: from in-out )
	//BFCBool TOneValueExist = false;
	// here we get mesh data pointers
	Win32_unlit_tex_ut_mesh *data = (Win32_unlit_tex_ut_mesh *) mesh->GetDataPointer();
	BFCWord *index = (BFCWord *) mesh->GetDataPointer2();	
	BFCTri tri;
	// we make a local copy of the line;
	BFCLine local_line = *line;
	BFCChar text[50];
	BFCChar text2[50];
	sprintf(text,"Intersec fuck ");
	sprintf(text2,"No Intersec fuck !!");
	mDebugLineMesh.CreateDebugLine(4,4,2);
	mDebugLineMesh.SetDebugLine(4,&local_line.Pos,&local_line.Dir,0,0xFF555555);
	// number of intersection in one way
	BFCUInt num_oneway_int;
	// list of intersection used to avoid counting twice an intersection in the same place
	BFCVoidList int_list;
	// tells if there is multiple intersection in one point
	BFCBool mult_int_point;
	// tell if the line has been rotated die to a true in mult_int_point
	BFCBool line_rotated = false;
	BFCBool line_back_rotated = false;
	BFCMatrix rot_m;
	// rotation around wich axis...
	BFCInt wich_rot = 0;
	rot_m.Identity();

check_col:

	num_oneway_int = 0;
	mult_int_point = false;

	BFCUInt i=0;
	while (i<mesh->GetNumData2())
	{		
		// Transform and copy pos to world
		tri.v1.pos = mesh->M * data[index[i]].pos;
		tri.v2.pos = mesh->M * data[index[i+1]].pos;
		tri.v3.pos = mesh->M * data[index[i+2]].pos;

		mDebugLineMesh.SetDebugLine(4,&tri.v1.pos,&tri.v2.pos,1,0xFF10EEFF);
		mDebugLineMesh.SetDebugLine(4,&tri.v2.pos,&tri.v3.pos,2,0xFF10EEFF);
		mDebugLineMesh.SetDebugLine(4,&tri.v3.pos,&tri.v1.pos,3,0xFF10EEFF);



		// If there is an intersection
		if (local_line.IsTriIntersection(tri.v1.pos,tri.v2.pos,tri.v3.pos,NULL,prec))
		{
			

			mDebugLineMesh.DebugLineSet(4,text);
			// if the line is parallel and on the edge and inside the tri return bfc_line_in
			if (local_line.LastTValue == bfc_all_t)
			{
				// of cuz only if border are considered inside
				if (border_inside)
				{
					//sprintf(text,"In");
					//SendMessage(mhwnd,WM_SETTEXT,0,(long)text);
					//DebugVisualize2();
					if (PointInTri(&local_line.Pos,&tri.v1.pos,&tri.v2.pos,&tri.v3.pos, NULL, 10.0f))
					{
						if (PointInTri(&local_line.Dir,&tri.v1.pos,&tri.v2.pos,&tri.v3.pos, NULL, 10.0f))
						{
							return bfc_line_in;
						} else
						{
							//return bfc_line_int;
						}
					} else
					{
						//return bfc_line_int;
					}
				}
			}
			// These line are for tolerance regulation in relation with the length of the line
			// They still need to be corrolate with "prec" parameter of this function
			BFCFloat _length = (local_line.Dir - local_line.Pos).Length();
			BFCFloat _tol;
			if (_length>=0.2f)
			{
				_tol = 0.1f / _length;
			} else
			{
				_tol = 0.5f;
			}
			if (line_rotated)
			{
				_tol = 0.1f;
			}
			// we check if the line is in the tri
			if (local_line.LastTValue<1.0f-_tol && local_line.LastTValue>0.0f+_tol)
			{
				// if yes return that there is an intersection.
				//sprintf(text,"Int");
				//SendMessage(mhwnd,WM_SETTEXT,0,(long)text);
				//DebugVisualize2();
				// we take care of "in line" intersection only if a line has not been rotated.
				if (!line_rotated)
				{
					return bfc_line_int;
				} else
				{
					goto add_col_check;		
				}
			}

			// count number of intersection in one way and direction
			if (local_line.LastTValue>=1.0f-_tol)
			{
add_col_check:
				BFCVoid *_int = int_list.GetFirstPtr();
				// look if there is an intersection already at this position
				while (_int)
				{
					if ( BFCEqual(local_line.LastTValue,*(BFCFloat *)_int,1.0f) )
					{
						mult_int_point = true;
					}
					_int = int_list.GetNextPtr();
				}
				num_oneway_int++;
				BFCFloat *_add = new BFCFloat;
				*_add = local_line.LastTValue;
				int_list.Add((BFCVoid *)_add);
			}	

		//sprintf(text,"Out");
		//SendMessage(mhwnd,WM_SETTEXT,0,(long)text);
		} else
		{
			mDebugLineMesh.DebugLineSet(4,text2);
		}
		i+=3;
	}

	// empty int_list
	int_list.RemoveDeleteAll();

	// if there is multiple intersections in one point
	if (mult_int_point)
	{
		if (line_back_rotated) return bfc_line_out;

		// we rotate a little bit the line and we check all again ( what ugly uhh!!! )
		switch (wich_rot)
		{
		case 0:
			{
				rot_m.RotateX(BFCPI / 24);
				wich_rot = 1;
			} break;
		case 1:
			{
				rot_m.RotateY(BFCPI / 24);
				wich_rot = 2;
			} break;
		case 2:
			{
				rot_m.RotateZ(BFCPI / 24);
				wich_rot = 0;
			} break;

		}
		BFCVector _tmp = local_line.Dir - local_line.Pos;
		_tmp = rot_m * _tmp;
		local_line.Dir = _tmp + local_line.Pos;
		line_rotated = true;
		goto check_col;
	} else
	{
		// if the line is considered out we try to rotate the line in the other way. (we are maybe on the edge of the mesh).
		if (line_rotated && !line_back_rotated)
		{
			// if the line is out maybe it's because the line before the rotation was on border of the mesh
			if (num_oneway_int % 2 == 0)
			{
				rot_m.Identity();
				// we rotate a little bit the line and we check all again ( what ugly uhh!!! )
				rot_m.RotateX(-BFCPI / 12);
		
				BFCVector _tmp = local_line.Dir - local_line.Pos;
				_tmp = rot_m * _tmp;
				local_line.Dir = _tmp + local_line.Pos;
				line_rotated = true;
				line_back_rotated = true;
				goto check_col;				
			}
		}
	}

	mesh->DataSet();
	mesh->Data2Set();

	if (num_oneway_int % 2 == 0)
	{
		//sprintf(text,"Out");
		//SendMessage(mhwnd,WM_SETTEXT,0,(long)text);
		//DebugVisualize2();
		return bfc_line_out;
	} else
	{
		//sprintf(text,"In");
		//SendMessage(mhwnd,WM_SETTEXT,0,(long)text);
		//DebugVisualize2();
		return bfc_line_in;
	}
}

BFCBool BFCGraphicEngine::CreateLineListFromTriInt(BFCMesh *mesh, BFCTriIntersect *tri, BFCVoidList *line_list)
{
	BFCUInt i;
	BFCIntersectInfo *act_point = NULL;
	// tell if something has been added. This is for return type
	BFCBool some_added = false;
	// edge tell in wich edge we are
	BFCEdge edge = no_edge;
	// indicate the last corner we passed trough. ( Edge1 or Edge2 )   ( Edge is a corner because of the misuse)
	BFCUInt last_corner;
	// tell if we found a connected point
	BFCBool found = false;
	BFCUInt first_line_index;
	// we set back corner as not used. Because they can be used several time but only once in CreateLineListFromTriInt
	tri->corner1_used = false;
	tri->corner2_used = false;
	tri->corner3_used = false;

	mDebugLineMesh.CreateDebugLine(0,1);

	// get the first free line

	i=0;
	while (i<tri->NumberIntersection)
	{
		if (!tri->IntersectInfo[i].ConnectedToLine)
		{
			found = true;
			break;
		}
		i++;
	}

	first_line_index = i;
	
	// we add the first line

	if (found)
	{
		// check if the point is not already connected or added
		if (!tri->IntersectInfo[i].ConnectedToLine)
		{
			line_list->Add(&tri->IntersectInfo[i].Intersect);
			// Don't set it as connected maybe we'll need it later
			//tri->IntersectInfo[i].ConnectedToLine = true;
			mDebugLineMesh.SetDebugLine(0,&tri->IntersectInfo[i].Intersect,NULL,0,0xFFFF2040);
			act_point = &tri->IntersectInfo[i];
		}
		i++;
		if (!tri->IntersectInfo[i].ConnectedToLine)
		{
			line_list->Add(&tri->IntersectInfo[i].Intersect);
			// Set it as connected
			tri->IntersectInfo[i].ConnectedToLine = true;
			mDebugLineMesh.SetDebugLine(0,NULL,&tri->IntersectInfo[i].Intersect,0,0xFFFF2040);
			mDebugLineMesh.DebugLineSet(0);
			act_point = &tri->IntersectInfo[i];
		}		
		i++;
	}

	// Now we try to find a connected point
	
	while (found)
	{
		found = false;

		// First we try victim edge intersection. ( since it has priority on others intersection )

		// First we check if it's this intesection type or if its a intersection near on the border
		if (act_point->IntersectType >= v1_v2_intersect)
		{
			// this one is used to save the nearest point on the line from the act_point on the edge
			BFCInt near_line = -1;
			// this will contain the smallest t param value difference between act_point and a point on the edge
			BFCFloat smallest_t_diff = BFCFarValue;
			// Try to find nearest point on victim edge
			i=0;
			while (i<tri->NumberIntersection)
			{
				// check if the point is not already connected
				if (!tri->IntersectInfo[i].ConnectedToLine)
				{
					switch (edge)
					{
					case no_edge:	// we are no "fully" on the edge we are touching it
						{
							// check if it's the same intersection type
							if (act_point->IntersectType == tri->IntersectInfo[i].IntersectType)
							{
								// we check if the t diff is smaller
								BFCFloat _t = (BFCFloat)fabs(act_point->TLineIntersect - tri->IntersectInfo[i].TLineIntersect);
								if (smallest_t_diff>_t)
								{
									// check if line is inside
									BFCLine _line;
									_line.Pos = act_point->Intersect;
									_line.Dir = tri->IntersectInfo[i].Intersect;
									// if the line is not inside we keep it
									if (!LineInsideMesh(mesh,&_line, false))
									{
										smallest_t_diff = _t;
										near_line = i;
									}
								}
							}
						} break;
					default:
						{
							// check if it's the same intersection type (on the same edge)
							if (tri->IntersectInfo[i].IntersectType == edge)
							{
								if (last_corner == 1)
								{
									// we check if the t diff is smaller
									BFCFloat _t = (BFCFloat)fabs(tri->IntersectInfo[i].TLineIntersect);
									if (smallest_t_diff>_t)
									{
										// check if line is inside
										BFCLine _line;
										_line.Pos = tri->IntersectInfo[i].Edge1;
										_line.Dir = tri->IntersectInfo[i].Intersect;
										// if the line is not inside we keep it
										if (!LineInsideMesh(mesh,&_line, false))
										{
											smallest_t_diff = _t;
											near_line = i;
										}
									}
								} else
								{
									// we check if the t diff is smaller
									BFCFloat _t = (BFCFloat)fabs(1 - tri->IntersectInfo[i].TLineIntersect);
									if (smallest_t_diff>_t)
									{
										// check if line is inside
										BFCLine _line;
										_line.Pos = tri->IntersectInfo[i].Edge2;
										_line.Dir = tri->IntersectInfo[i].Intersect;
										// if the line is not inside we keep it
										if (!LineInsideMesh(mesh,&_line, false))
										{
											smallest_t_diff = _t;
											near_line = i;
										}
									}
								}
							}
						}
					}
				}
				i++;
			}
			// if near_line exist we add it
			if (near_line != -1)
			{
				// we add the new line
				// but first check if we are on an edge
				if (edge == no_edge)
				{
					line_list->Add(&act_point->Intersect);
				} else
				{
					if (last_corner == 1)
					{
						switch (edge)
						{
						case v1_v2_edge:
							{
								line_list->Add(&tri->Tri1.v1.pos);
								mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v1.pos,NULL,0,0xFFFF2040);
							} break;
						case v2_v3_edge:
							{
								line_list->Add(&tri->Tri1.v2.pos);
								mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v2.pos,NULL,0,0xFFFF2040);
							} break;
						case v3_v1_edge:
							{
								line_list->Add(&tri->Tri1.v3.pos);
								mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v3.pos,NULL,0,0xFFFF2040);
							} break;
						}
					} else
					{
						switch (edge)
						{
						case v1_v2_edge:
							{
								line_list->Add(&tri->Tri1.v2.pos);
								mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v2.pos,NULL,0,0xFFFF2040);
							} break;
						case v2_v3_edge:
							{
								line_list->Add(&tri->Tri1.v3.pos);
								mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v3.pos,NULL,0,0xFFFF2040);
							} break;
						case v3_v1_edge:
							{
								line_list->Add(&tri->Tri1.v1.pos);
								mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v1.pos,NULL,0,0xFFFF2040);
							} break;
						}						
					}
				}
				line_list->Add(&tri->IntersectInfo[near_line].Intersect);
				mDebugLineMesh.SetDebugLine(0,NULL,&tri->IntersectInfo[near_line].Intersect,0,0xFFFF2040);
				mDebugLineMesh.DebugLineSet(0);

				// yes we add the second line found connected
				// we add also the second point ( since the result of intersection give us a line)
				// we check wich one
				if (near_line%2==0)
				{
					// we check if next one is not connected
					if (!tri->IntersectInfo[near_line+1].ConnectedToLine)
					{
						line_list->Add(&tri->IntersectInfo[near_line].Intersect);
						// set the point as connected
						tri->IntersectInfo[near_line].ConnectedToLine = true;
						// it's the next one
						line_list->Add(&tri->IntersectInfo[near_line+1].Intersect);
						mDebugLineMesh.SetDebugLine(0,&tri->IntersectInfo[near_line].Intersect,&tri->IntersectInfo[near_line+1].Intersect,0,0xFFFF2040);
						mDebugLineMesh.DebugLineSet(0);
						tri->IntersectInfo[near_line+1].ConnectedToLine = true;
						// refresh act_point index
						act_point = &tri->IntersectInfo[near_line+1];
					} else
					{
						// set anyway actual point as connected
						tri->IntersectInfo[near_line].ConnectedToLine = true;
					}
				} else
				{
					// we check if next one is not connected
					if (!tri->IntersectInfo[near_line-1].ConnectedToLine)
					{
						line_list->Add(&tri->IntersectInfo[near_line].Intersect);
						// set the point as connected
						tri->IntersectInfo[near_line].ConnectedToLine = true;
						// it's the previous one
						line_list->Add(&tri->IntersectInfo[near_line-1].Intersect);
						mDebugLineMesh.SetDebugLine(0,&tri->IntersectInfo[near_line].Intersect,&tri->IntersectInfo[near_line-1].Intersect,0,0xFFFF2040);
						mDebugLineMesh.DebugLineSet(0);
						tri->IntersectInfo[near_line-1].ConnectedToLine = true;
						// refresh act_point index
						act_point = &tri->IntersectInfo[near_line-1];
					} else
					{
						// set anyway actual point as connected
						tri->IntersectInfo[near_line].ConnectedToLine = true;
					}
				}
				edge = no_edge;
				found = true;
				// set i back to 0
				i = 0;
			} else
			{
				// here we didn't find anything so we need to connect it to the corner
				// first check if we are on edge or touching it than check wich corner is inside

				BFCLine _line;
				switch (edge)
				{
				case no_edge:
					{
						switch (act_point->IntersectType)
						{
						case v1_v2_intersect:
							{
								_line.Pos = act_point->Intersect;
								_line.Dir = tri->Tri1.v1.pos;
								// if the line is not inside we keep it
								if (!LineInsideMesh(mesh,&_line, false))
								{
									if (!tri->corner1_used)
									{
										tri->corner1_used = true;
										tri->corner_used_once = true;
										found = true;
										line_list->Add(&act_point->Intersect);
										line_list->Add(&tri->Tri1.v1.pos);
										mDebugLineMesh.SetDebugLine(0,&act_point->Intersect,&tri->Tri1.v1.pos,0,0xFFFF2040);
										mDebugLineMesh.DebugLineSet(0);
										last_corner = 2;
										edge = v3_v1_edge;
									}
								} else
								{
									_line.Pos = act_point->Intersect;
									_line.Dir = tri->Tri1.v2.pos;
									// if the line is not inside we keep it
									if (!LineInsideMesh(mesh,&_line, false) && !tri->corner2_used)
									{
										// otherwise the other side should be inside
										tri->corner2_used = true;
										tri->corner_used_once = true;
										found = true;
										line_list->Add(&act_point->Intersect);
										line_list->Add(&tri->Tri1.v2.pos);
										mDebugLineMesh.SetDebugLine(0,&act_point->Intersect,&tri->Tri1.v2.pos,0,0xFFFF2040);
										mDebugLineMesh.DebugLineSet(0);
										last_corner = 1;
										edge = v2_v3_edge;
									}
								}
							} break;
						case v2_v3_intersect:
							{
								_line.Pos = act_point->Intersect;
								_line.Dir = tri->Tri1.v2.pos;
								// if the line is not inside we keep it
								if (!LineInsideMesh(mesh,&_line, false))
								{
									if (!tri->corner2_used)
									{
										tri->corner2_used = true;
										tri->corner_used_once = true;
										found = true;
										line_list->Add(&act_point->Intersect);
										line_list->Add(&tri->Tri1.v2.pos);
										mDebugLineMesh.SetDebugLine(0,&act_point->Intersect,&tri->Tri1.v2.pos,0,0xFFFF2040);
										mDebugLineMesh.DebugLineSet(0);
										last_corner = 2;
										edge = v1_v2_edge;
									}
								} else
								{
									_line.Pos = act_point->Intersect;
									_line.Dir = tri->Tri1.v3.pos;
									// if the line is not inside we keep it
									if (!LineInsideMesh(mesh,&_line, false) && !tri->corner3_used)
									{								
										// otherwise the other side should be inside
										tri->corner3_used = true;
										tri->corner_used_once = true;
										found = true;
										line_list->Add(&act_point->Intersect);
										line_list->Add(&tri->Tri1.v3.pos);
										mDebugLineMesh.SetDebugLine(0,&act_point->Intersect,&tri->Tri1.v3.pos,0,0xFFFF2040);
										mDebugLineMesh.DebugLineSet(0);
										last_corner = 1;
										edge = v3_v1_edge;
									}
								}
							} break;
						case v3_v1_intersect:
							{
								_line.Pos = act_point->Intersect;
								_line.Dir = tri->Tri1.v3.pos;
								// if the line is not inside we keep it
								if (!LineInsideMesh(mesh,&_line, false))
								{
									if (!tri->corner3_used)
									{
										tri->corner3_used = true;
										tri->corner_used_once = true;
										found = true;
										line_list->Add(&act_point->Intersect);
										line_list->Add(&tri->Tri1.v3.pos);
										mDebugLineMesh.SetDebugLine(0,&act_point->Intersect,&tri->Tri1.v3.pos,0,0xFFFF2040);
										mDebugLineMesh.DebugLineSet(0);
										last_corner = 2;
										edge = v2_v3_edge;
									}
								} else
								{
									_line.Pos = act_point->Intersect;
									_line.Dir = tri->Tri1.v1.pos;
									// if the line is not inside we keep it
									if (!LineInsideMesh(mesh,&_line, false) && !tri->corner1_used)
									{
										// otherwise the other side should be inside
										tri->corner1_used = true;
										tri->corner_used_once = true;
										found = true;								
										line_list->Add(&act_point->Intersect);
										line_list->Add(&tri->Tri1.v1.pos);
										mDebugLineMesh.SetDebugLine(0,&act_point->Intersect,&tri->Tri1.v1.pos,0,0xFFFF2040);
										mDebugLineMesh.DebugLineSet(0);
										last_corner = 1;
										edge = v1_v2_edge;
									}
								}
							} break;
						}
					} break;
				case v1_v2_edge:
					{
						if (last_corner == 1)
						{
							if (!tri->corner2_used)
							{
								_line.Pos = tri->Tri1.v1.pos;
								_line.Dir = tri->Tri1.v2.pos;
								// if the line is not inside we keep it
								if (!LineInsideMesh(mesh,&_line, false))
								{
									tri->corner2_used = true;
									tri->corner_used_once = true;
									found = true;
									line_list->Add(&tri->Tri1.v1.pos);
									line_list->Add(&tri->Tri1.v2.pos);
									mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v1.pos,&tri->Tri1.v2.pos,0,0xFFFF2040);
									mDebugLineMesh.DebugLineSet(0);
									last_corner = 1;
									edge = v2_v3_edge;
								}
							}
						} else
						{
							if (!tri->corner1_used)
							{
								_line.Pos = tri->Tri1.v2.pos;
								_line.Dir = tri->Tri1.v1.pos;
								// if the line is not inside we keep it
								if (!LineInsideMesh(mesh,&_line, false))
								{
									tri->corner1_used = true;
									tri->corner_used_once = true;
									found = true;
									line_list->Add(&tri->Tri1.v2.pos);
									line_list->Add(&tri->Tri1.v1.pos);
									mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v2.pos,&tri->Tri1.v1.pos,0,0xFFFF2040);
									mDebugLineMesh.DebugLineSet(0);
									last_corner = 2;
									edge = v3_v1_edge;
								}
							}
						}
					} break;
				case v2_v3_edge:
					{
						if (last_corner == 1)
						{
							if (!tri->corner3_used)
							{
								_line.Pos = tri->Tri1.v2.pos;
								_line.Dir = tri->Tri1.v3.pos;
								// if the line is not inside we keep it
								if (!LineInsideMesh(mesh,&_line, false))
								{
									tri->corner3_used = true;
									tri->corner_used_once = true;
									found = true;
									line_list->Add(&tri->Tri1.v2.pos);
									line_list->Add(&tri->Tri1.v3.pos);
									mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v2.pos,&tri->Tri1.v3.pos,0,0xFFFF2040);
									mDebugLineMesh.DebugLineSet(0);
									last_corner = 1;
									edge = v3_v1_edge;
								}
							}
						} else
						{
							if (!tri->corner2_used)
							{
								_line.Pos = tri->Tri1.v3.pos;
								_line.Dir = tri->Tri1.v2.pos;
								// if the line is not inside we keep it
								if (!LineInsideMesh(mesh,&_line, false))
								{
									tri->corner2_used = true;
									tri->corner_used_once = true;
									found = true;
									line_list->Add(&tri->Tri1.v3.pos);
									line_list->Add(&tri->Tri1.v2.pos);
									mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v3.pos,&tri->Tri1.v2.pos,0,0xFFFF2040);
									mDebugLineMesh.DebugLineSet(0);
									last_corner = 2;
									edge = v1_v2_edge;
								}
							}
						}
					} break;
				case v3_v1_edge:
					{
						if (last_corner == 1)
						{
							if (!tri->corner1_used)
							{
								_line.Pos = tri->Tri1.v3.pos;
								_line.Dir = tri->Tri1.v1.pos;
								// if the line is not inside we keep it
								if (!LineInsideMesh(mesh,&_line, false))
								{
									tri->corner1_used = true;
									tri->corner_used_once = true;
									found = true;
									line_list->Add(&tri->Tri1.v3.pos);
									line_list->Add(&tri->Tri1.v1.pos);
									mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v3.pos,&tri->Tri1.v1.pos,0,0xFFFF2040);
									mDebugLineMesh.DebugLineSet(0);
									last_corner = 1;
									edge = v1_v2_edge;
								}
							}
						} else
						{
							if (!tri->corner3_used)
							{
								_line.Pos = tri->Tri1.v1.pos;
								_line.Dir = tri->Tri1.v3.pos;
								// if the line is not inside we keep it
								if (!LineInsideMesh(mesh,&_line, false))
								{
									tri->corner3_used = true;
									tri->corner_used_once = true;
									found = true;
									line_list->Add(&tri->Tri1.v1.pos);
									line_list->Add(&tri->Tri1.v3.pos);
									mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v1.pos,&tri->Tri1.v3.pos,0,0xFFFF2040);
									mDebugLineMesh.DebugLineSet(0);
									last_corner = 2;
									edge = v2_v3_edge;
								}
							}
						}
					} break;
				}
			}
		}

		if (found)
		{
			some_added = true;
			continue;
		}

		// Here we didn't find any victim edge connected to the actual point. So we try to find a normal connected point


		i = 0;
		while (i<tri->NumberIntersection)
		{
			// check if the point is not already connected or added
			if (!tri->IntersectInfo[i].ConnectedToLine)
			{
				BFCBool add_second_line = false;
				switch (edge)
				{
				case no_edge:
					{
						// check if the point can be connected
						if (tri->IntersectInfo[i].Intersect.IsNear(act_point->Intersect,BFCPrecision*30.0f))
						{
							// if yes we set we can try to add the line
							add_second_line = true;
						} break;
					}
				case v1_v2_edge:
					{
						if (last_corner == 1)
						{
							// check if the point can be connected
							if (tri->IntersectInfo[i].Intersect.IsNear(tri->Tri1.v1.pos,BFCPrecision*30.0f))
							{
								// if yes we add the line
								line_list->Add(&tri->Tri1.v1.pos);
								line_list->Add(&tri->IntersectInfo[i].Intersect);
								mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v1.pos,&tri->IntersectInfo[i].Intersect,0,0xFFFF2040);
								mDebugLineMesh.DebugLineSet(0);
								// we set we can try to add the second line
								add_second_line = true;
								found = true;
							} break;
						} else
						{
							// check if the point can be connected
							if (tri->IntersectInfo[i].Intersect.IsNear(tri->Tri1.v2.pos,BFCPrecision*30.0f))
							{
								// if yes we add the line
								line_list->Add(&tri->Tri1.v2.pos);
								line_list->Add(&tri->IntersectInfo[i].Intersect);
								mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v2.pos,&tri->IntersectInfo[i].Intersect,0,0xFFFF2040);
								mDebugLineMesh.DebugLineSet(0);
								// we set we can try to add the second line
								add_second_line = true;
								found = true;
							} break;						
						}
					}
				case v2_v3_edge:
					{
						if (last_corner == 1)
						{
							// check if the point can be connected
							if (tri->IntersectInfo[i].Intersect.IsNear(tri->Tri1.v2.pos,BFCPrecision*30.0f))
							{
								// if yes we add the line
								line_list->Add(&tri->Tri2.v1.pos);
								line_list->Add(&tri->IntersectInfo[i].Intersect);
								mDebugLineMesh.SetDebugLine(0,&tri->Tri2.v1.pos,&tri->IntersectInfo[i].Intersect,0,0xFFFF2040);
								mDebugLineMesh.DebugLineSet(0);
								// we set we can try to add the second line
								add_second_line = true;
								found = true;
							} break;
						} else
						{
							// check if the point can be connected
							if (tri->IntersectInfo[i].Intersect.IsNear(tri->Tri1.v3.pos,BFCPrecision*30.0f))
							{
								// if yes we add the line
								line_list->Add(&tri->Tri1.v3.pos);
								line_list->Add(&tri->IntersectInfo[i].Intersect);
								mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v3.pos,&tri->IntersectInfo[i].Intersect,0,0xFFFF2040);
								mDebugLineMesh.DebugLineSet(0);
								// we set we can try to add the second line
								add_second_line = true;
								found = true;
							} break;						
						}
					}
				case v3_v1_edge:
					{
						if (last_corner == 1)
						{
							// check if the point can be connected
							if (tri->IntersectInfo[i].Intersect.IsNear(tri->Tri1.v3.pos,BFCPrecision*30.0f))
							{
								// if yes we add the line
								line_list->Add(&tri->Tri1.v3.pos);
								line_list->Add(&tri->IntersectInfo[i].Intersect);
								mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v3.pos,&tri->IntersectInfo[i].Intersect,0,0xFFFF2040);
								mDebugLineMesh.DebugLineSet(0);
								// we set we can try to add the second line
								add_second_line = true;
								found = true;
							} break;
						} else
						{
							// check if the point can be connected
							if (tri->IntersectInfo[i].Intersect.IsNear(tri->Tri1.v1.pos,BFCPrecision*30.0f))
							{
								// if yes we add the line
								line_list->Add(&tri->Tri1.v1.pos);
								line_list->Add(&tri->IntersectInfo[i].Intersect);
								mDebugLineMesh.SetDebugLine(0,&tri->Tri1.v1.pos,&tri->IntersectInfo[i].Intersect,0,0xFFFF2040);
								mDebugLineMesh.DebugLineSet(0);
								// we set we can try to add the second line
								add_second_line = true;
								found = true;
							} break;						
						}
					}
				}
				if (add_second_line)
				{
					// we try to add also the second point ( since the result of intersection give us a line)
					// we check wich one
					if (i%2==0)
					{
						// it's the next one
						// last check if next point is not connected to line
						if (!tri->IntersectInfo[i+1].ConnectedToLine)
						{
							line_list->Add(&tri->IntersectInfo[i].Intersect);
							// set the point as connected
							tri->IntersectInfo[i].ConnectedToLine = true;
							line_list->Add(&tri->IntersectInfo[i+1].Intersect);
							mDebugLineMesh.SetDebugLine(0,&tri->IntersectInfo[i].Intersect,&tri->IntersectInfo[i+1].Intersect,0,0xFFFF2040);
							mDebugLineMesh.DebugLineSet(0);
							tri->IntersectInfo[i+1].ConnectedToLine = true;
							// refresh act_point index
							act_point = &tri->IntersectInfo[i+1];
							found = true;
						}
					} else
					{
						// it's the previous one
						// last check if previous point is not connected to line
						if (!tri->IntersectInfo[i-1].ConnectedToLine)
						{
							line_list->Add(&tri->IntersectInfo[i].Intersect);
							// set the point as connected
							tri->IntersectInfo[i].ConnectedToLine = true;
							line_list->Add(&tri->IntersectInfo[i-1].Intersect);
							mDebugLineMesh.SetDebugLine(0,&tri->IntersectInfo[i].Intersect,&tri->IntersectInfo[i-1].Intersect,0,0xFFFF2040);
							mDebugLineMesh.DebugLineSet(0);
							tri->IntersectInfo[i-1].ConnectedToLine = true;
							// refresh act_point index
							act_point = &tri->IntersectInfo[i-1];
							found = true;
						}
					}
				}
			}
			if (found) break;
			i++;
		}

		if (found)
		{
			some_added = true;
		} else
		{
			// we didn't found anything we try to add the tri

			// check if corner have never been used
			if (!tri->corner_used_once)
			{
				// we check if the 3 lines are not inside the mesh
				BFCLine _line;
				_line.Pos = tri->Tri1.v1.pos;
				_line.Dir = tri->Tri1.v2.pos;
				if (!LineInsideMesh(mesh,&_line, false))
				{
					_line.Pos = tri->Tri1.v2.pos;
					_line.Dir = tri->Tri1.v3.pos;
					if (!LineInsideMesh(mesh,&_line, false))
					{
						_line.Pos = tri->Tri1.v3.pos;
						_line.Dir = tri->Tri1.v1.pos;
						if (!LineInsideMesh(mesh,&_line, false))
						{
							// we add the tri
							line_list->Add(&tri->Tri1.v1);
							line_list->Add(&tri->Tri1.v2);

							line_list->Add(&tri->Tri1.v2);
							line_list->Add(&tri->Tri1.v3);

							line_list->Add(&tri->Tri1.v3);
							line_list->Add(&tri->Tri1.v1);
							tri->corner1_used = true;
						}
					}
				}				
			} else
			{
			}
		}
	}
	// at the end we set first line index (first free IntersectInfo) as connected
	tri->IntersectInfo[first_line_index].ConnectedToLine = true;
	mDebugLineMesh.Disable(0);
	return some_added;
}

BFCBool BFCGraphicEngine::GetPointerFromValue(BFCVector **v1,BFCVector **v2,BFCVoidList *list, BFCFloat prec_factor)
{
	BFCVector *add;
	BFCVector *add2;
	BFCVector *v = (BFCVector *)list->GetFirstPtr();
	while (v)
	{
		add = NULL;
		add2 = NULL;
		if (v->IsNear(**v1,BFCPrecision*prec_factor))
		{
			add = v;
		}
		v = (BFCVector *)list->GetNextPtr();
		if (v == NULL) break;
		if (add)
		{
			if (v->IsNear(**v2,BFCPrecision*prec_factor))
			{
				add2 = v;
				*v1 = add;
				*v2 = add2;
				return true;
			}
		}
		v = (BFCVector *)list->GetNextPtr();
	}
	return false;
}


// Purge Line list has some bug ( not finished )

BFCVoid BFCGraphicEngine::PurgeLineList(BFCVoidList *line_list)
{
	BFCLine line1,line2;
	BFCVector *pos = (BFCVector *)line_list->GetFirstPtr();
	BFCVector *dir = (BFCVector *)line_list->GetNextPtr();
	// number of time to call next to get to the pos
	BFCUInt check_pos = 0;
	BFCBool found = false;
	// we go through all the list
	while (pos && dir)
	{
		// we copy the line found in line1
		line1.Pos = *pos; 
		line1.Dir = *dir;
		BFCVector *_find1;
		BFCVector *_find2;
		if (!found)
		{
			check_pos += 1;
			pos = (BFCVector *)line_list->GetFirstPtr();
			dir = (BFCVector *)line_list->GetNextPtr();
			for (BFCUInt i=0;i<check_pos;i++)
			{
				pos = (BFCVector *)line_list->GetNextPtr();
				dir = (BFCVector *)line_list->GetNextPtr();
			}
			_find1 = (BFCVector *)line_list->GetNextPtr();
			_find2 = (BFCVector *)line_list->GetNextPtr();
		} else
		{
			pos = (BFCVector *)line_list->GetFirstPtr();
			dir = (BFCVector *)line_list->GetNextPtr();
			check_pos = 0;
			_find1 = (BFCVector *)line_list->GetNextPtr();
			_find2 = (BFCVector *)line_list->GetNextPtr();
		}
		found = false;
		// while found another line check if it's parallel to line1
		while (_find1 && _find2)
		{
			// we copy the line found in line2
			line2.Pos = *_find1;
			line2.Dir = *_find2;
			if (line1.IsParallel(line2,10.0f))
			{
				// we check if one end touch the other
				if (BFCEqual(line1.Pos.x,line2.Pos.x,10.0f) && BFCEqual(line1.Pos.y,line2.Pos.y,10.0f) && BFCEqual(line1.Pos.z,line2.Pos.z,10.0f))
				{
					// we remove the two line
					line_list->RemoveNear(pos,dir);
					line_list->RemoveNear(_find1,_find2);
					// we add the new one
					line_list->Add(dir);
					line_list->Add(_find2);
					found = true;
					break;
				} else
				{
					// if no, we check other ends
					if (BFCEqual(line1.Pos.x,line2.Dir.x,10.0f) && BFCEqual(line1.Pos.y,line2.Dir.y,10.0f) && BFCEqual(line1.Pos.z,line2.Dir.z,10.0f))
					{
						// we remove the two line
						line_list->RemoveNear(pos,dir);
						line_list->RemoveNear(_find1,_find2);
						// we add the new one
						line_list->Add(dir);
						line_list->Add(_find1);
						found = true;
						break;
					} else
					{
						// if no, we check other ends
						if (BFCEqual(line1.Dir.x,line2.Dir.x,10.0f) && BFCEqual(line1.Dir.y,line2.Dir.y,10.0f) && BFCEqual(line1.Dir.z,line2.Dir.z,10.0f))
						{
							// we remove the two line
							line_list->RemoveNear(pos,dir);
							line_list->RemoveNear(_find1,_find2);
							// we add the new one
							line_list->Add(pos);
							line_list->Add(_find1);
							found = true;
							break;
						} else
						{
							// if no, we check other ends
							if (BFCEqual(line1.Dir.x,line2.Pos.x,10.0f) && BFCEqual(line1.Dir.y,line2.Pos.y,10.0f) && BFCEqual(line1.Dir.z,line2.Pos.z,10.0f))
							{
								// we remove the two line
								line_list->RemoveNear(pos,dir);
								line_list->RemoveNear(_find1,_find2);
								// we add the new one
								line_list->Add(pos);
								line_list->Add(_find2);
								found = true;
								break;
							}
						}
					}
				}
			}
			_find1 = (BFCVector *)line_list->GetNextPtr();
			_find2 = (BFCVector *)line_list->GetNextPtr();
		}
	}
}

BFCVoid BFCGraphicEngine::FillTriListFromLineList(BFCMesh *victim,BFCMesh *actor, BFCVoidList *tri_list, BFCVoidList *line_list)
{

	BFCChar text[256];

	mDebugLineMesh.CreateDebugLine(5,240);

	BFCVector *v = (BFCVector *) line_list->GetFirstPtr();
	BFCVector *v2 = (BFCVector *) line_list->GetNextPtr();
	BFCUInt i=0;
	while (i<line_list->GetNumber())
	{
		mDebugLineMesh.SetDebugLine(5,v,v2,i,0xFF000000);
		v = (BFCVector *) line_list->GetNextPtr();
		v2 = (BFCVector *) line_list->GetNextPtr();
		i+=2;
	}
	i=line_list->GetNumber();
	while (i<240)
	{
		mDebugLineMesh.SetDebugLine(5,&BFCVector (0.0f,0.0f,0.0f),&BFCVector (0.0f,0.0f,0.0f),i,0xFF000000);
		i++;
	}


	BFCVoidList added_line_list;
	// we want the node to be put always at the end
	added_line_list.SetRespectMyId(false);
	BFCLine line;
	BFCVector tmp;
	// edge 1 , side a and b
	BFCVector *e1a,*e1b;
	// find used to find to nearest inside edge
	BFCVector *find = NULL;
	// near_edge used to save the nearest edge.
	BFCVector *near_edge = NULL;
	BFCFloat near_dist = BFCFarValue;
	// tell if it's connected by an edge

	e1a = (BFCVector *)line_list->GetFirstPtr();
	e1b = (BFCVector *)line_list->GetNextPtr();
	find = (BFCVector *)line_list->GetNextPtr();

	// while there is still 2 elements in the list

	while (e1b)
	{
		near_edge = NULL;
		near_dist = BFCFarValue;
		// we try to find a point nearest as possible inside ( normally on border ) of the victim mesh
		while (find)
		{
			// check if last_find is not directly connected to the first edge
			if (e1a->IsNear(*find, BFCPrecision*10.0f) || e1b->IsNear(*find, BFCPrecision*10.0f))
			{	
			} else
			{


				mDebugLineMesh.SetDebugLine(5,e1b,find,110,0xFF00FF00);
				mDebugLineMesh.SetDebugLine(5,e1a,find,111,0xFFFF00FF);
				mDebugLineMesh.SetDebugLine(5,e1a,e1b,112,0xFFFFFF00);
				sprintf(text,"Testing");
				mDebugLineMesh.DebugLineSet(5,text);
				sprintf(text,"dist bigger ");
				SendMessage(mhwnd,WM_SETTEXT,0,(long)text);


				// check with the point
				line.Pos = *e1b;
				line.Dir = *find;
				// we check if it's a nearest value before lineinsidemesh for optimisation
				tmp = line.Dir - line.Pos;
				BFCFloat _dist = tmp.DotProduct(tmp);
				if (_dist < near_dist)
				{
					sprintf(text,"no pass");
					SendMessage(mhwnd,WM_SETTEXT,0,(long)text);

					// check if it's inside and it's not crossing through an already added line
					if (LineInsideMesh(victim,&line, true) == bfc_line_in && !line.IntLineList(&added_line_list) )
					{
						sprintf(text,"1st pass");
						SendMessage(mhwnd,WM_SETTEXT,0,(long)text);

						// then we check if the second edge is inside
						line.Pos = *e1a;
						// check if it's completly into the victim and it's not crossing through an already added line
						if (LineInsideMesh(victim,&line, true) == bfc_line_in && !line.IntLineList(&added_line_list) )
						{
							sprintf(text,"2nd pass");
							SendMessage(mhwnd,WM_SETTEXT,0,(long)text);

							BFCTri _tri;
							_tri.v1.pos = *e1a;
							_tri.v2.pos = *e1b;
							_tri.v3.pos = *find;

							// we need to check also if the actor mesh is not intersecting the tri
							if (!MeshIntTri(actor,&_tri))
							{
								sprintf(text,"Mesh Int tri pass");
								SendMessage(mhwnd,WM_SETTEXT,0,(long)text);

								near_dist = _dist;
								near_edge = find;
							}
						}				
					}
				}
				mDebugLineMesh.DebugLineSet(5,text);
			}
			find = (BFCVector *)line_list->GetNextPtr();
		}



		// Now we add the new formed tri ( if one has been made )
		if (near_edge)
		{
			BFCTri *_tri;
			_tri = new BFCTri();
			_tri->v1.pos = *e1a;
			_tri->v2.pos = *e1b;
			_tri->v3.pos = *near_edge;
			tri_list->Add(_tri);

			mDebugLineMesh.SetDebugLine(5,e1a,e1b,110,0xFF000000);
			mDebugLineMesh.SetDebugLine(5,e1b,near_edge,111,0xFF000000);
			mDebugLineMesh.SetDebugLine(5,near_edge,e1a,112,0xFF000000);
			mDebugLineMesh.DebugLineSet(5,"Testing remove");

			// we remove the edge used (e1a e1b)
			line_list->RemoveNear(e1a,e1b);
			// we check if we must remove or add some additional edge in the edge list
			BFCVector *_tmp = e1b;
			BFCVector *_tmp2 = near_edge;
			// if it exist in the list remove it otherwise add it
			// 50.0 is a high value but should be ok for a pair
			if (GetPointerFromValue(&_tmp,&_tmp2,line_list,10.0f))
			{
				line_list->RemoveNear(_tmp,_tmp2);
			} else
			{
				// check in the other way
				if (GetPointerFromValue(&_tmp2,&_tmp,line_list,10.0f))
				{
					line_list->RemoveNear(_tmp2,_tmp);
				} else
				{
					line_list->Add(_tmp);
					line_list->Add(_tmp2);
					added_line_list.Add(_tmp);
					added_line_list.Add(_tmp2);
				}
			}
			// we do the same for the last edge
			_tmp = near_edge;
			_tmp2 = e1a;
			// if it exist in the list remove it otherwise add it
			if (GetPointerFromValue(&_tmp,&_tmp2,line_list,10.0f))
			{
				line_list->RemoveNear(_tmp,_tmp2);
			} else
			{
				if (GetPointerFromValue(&_tmp2,&_tmp,line_list,10.0f))
				{
					line_list->RemoveNear(_tmp2,_tmp);
				} else
				{
					line_list->Add(_tmp);
					line_list->Add(_tmp2);
					added_line_list.Add(_tmp);
					added_line_list.Add(_tmp2);
				}
			}
		} else
		{
			// if we didn't find anything we just remove it anyway.
			// Should be here because the line can't be used to form a tri (think of the 2 pyramids one another the other at same height)
			line_list->RemoveNear(e1a,e1b);
		}
		// set pointer back to NULL. To check then if the 3 sequence still exist
		find = NULL;
		e1b = NULL;
		e1a = (BFCVector *)line_list->GetFirstPtr();
		if (e1a) e1b = (BFCVector *)line_list->GetNextPtr();
		if (e1b) find = (BFCVector *)line_list->GetNextPtr();

		sprintf(text,"Number: %i",line_list->GetNumber());
		SendMessage(mhwnd,WM_SETTEXT,0,(long)text);
		DebugVisualize();

		if (!find)
		{
			// if there is still one edge ( can happen ) we try to use it with a point in added_line_list (since line_list is empty)
			if (e1b && e1a)
			{
				near_edge = NULL;
				near_dist = BFCFarValue;
				// we use get first ptr 2 to not interfere later with IntLineList()
				find = (BFCVector *)added_line_list.GetFirstPtr2();
				// we try to find a point nearest as possible inside ( normally on border ) of the victim mesh
				while (find)
				{
					// check if last_find is not directly connected to the first edge
					if (e1a->IsNear(*find, BFCPrecision*10.0f) || e1b->IsNear(*find, BFCPrecision*10.0f))
					{	
					} else
					{


						mDebugLineMesh.SetDebugLine(5,e1b,find,110,0xFF00FF00);
						mDebugLineMesh.SetDebugLine(5,e1a,find,111,0xFFFF00FF);
						mDebugLineMesh.SetDebugLine(5,e1a,e1b,112,0xFFFFFF00);
						sprintf(text,"Testing");
						mDebugLineMesh.DebugLineSet(5,text);
						sprintf(text,"dist bigger ");
						SendMessage(mhwnd,WM_SETTEXT,0,(long)text);


						// check with the point
						line.Pos = *e1b;
						line.Dir = *find;
						// we check if it's a nearest value before lineinsidemesh for optimisation
						tmp = line.Dir - line.Pos;
						BFCFloat _dist = tmp.DotProduct(tmp);
						if (_dist < near_dist)
						{
							sprintf(text,"no pass");
							SendMessage(mhwnd,WM_SETTEXT,0,(long)text);

							// check if it's inside and it's not crossing through an already added line
							if (LineInsideMesh(victim,&line, true) == bfc_line_in && !line.IntLineList(&added_line_list) )
							{
								sprintf(text,"1st pass");
								SendMessage(mhwnd,WM_SETTEXT,0,(long)text);

								// then we check if the second edge is inside
								line.Pos = *e1a;
								// check if it's completly into the victim and it's not crossing through an already added line
								if (LineInsideMesh(victim,&line, true) == bfc_line_in && !line.IntLineList(&added_line_list) )
								{
									sprintf(text,"2nd pass");
									SendMessage(mhwnd,WM_SETTEXT,0,(long)text);

									BFCTri _tri;
									_tri.v1.pos = *e1a;
									_tri.v2.pos = *e1b;
									_tri.v3.pos = *find;

									// we need to check also if the actor mesh is not intersecting the tri
									if (!MeshIntTri(actor,&_tri))
									{
										sprintf(text,"Mesh Int tri pass");
										SendMessage(mhwnd,WM_SETTEXT,0,(long)text);

										near_dist = _dist;
										near_edge = find;
									}
								}				
							}
						}
						mDebugLineMesh.DebugLineSet(5,text);
					}
					find = (BFCVector *)added_line_list.GetNextPtr2();
				}
				// if somthing found we add it
				if (near_edge)
				{
					BFCTri *_tri;
					_tri = new BFCTri();
					_tri->v1.pos = *e1a;
					_tri->v2.pos = *e1b;
					_tri->v3.pos = *near_edge;
					tri_list->Add(_tri);
				}
			}
			break;
		}
	}
	// we empty the added_line_list
	added_line_list.RemoveAll();
}



BFCVoid BFCGraphicEngine::BoolSubstract(BFCMesh *victim,BFCMesh *actor)
{
	// tell if we must use lin_int because there is an intersection at the same point
	BFCBool use_line_int = false;
	// list of intersection used to avoid counting twice an intersection in the same place
	BFCVoidList int_list;

	BFCVoidList new_tri_list;
	// we want the node to be put always at the end
	new_tri_list.SetRespectMyId(false);
	// list of all intersection between one victim tri with all tri of actor
	BFCVoidList tmp_intersect_list;
	// we want the node to be put always at the end
	tmp_intersect_list.SetRespectMyId(false);
	BFCVoidList line_list;
	// we want the node to be put always at the end
	line_list.SetRespectMyId(false);
	BFCTriIntersect *tri_int = NULL;
	BFCUInt number_newdata = 0;
	BFCInt last_v1_v2_int;


	BFCIPrimitive *prim = (BFCIPrimitive *) victim;
	BFCIPrimitive *prim2 = (BFCIPrimitive *) actor;
	prim->ScaleIt(4.0f,4.0f,4.0f);
	BFCVector pos = victim->M.GetPos();
	pos = pos * 4.0f;
	victim->M.SetPos(pos);
	prim2->ScaleIt(4.0f,4.0f,4.0f);
	pos = actor->M.GetPos();
	pos = pos * 4.0f;
	actor->M.SetPos(pos);


	// here we get victim data pointers
	Win32_unlit_tex_ut_mesh *v_data = (Win32_unlit_tex_ut_mesh *) victim->GetDataPointer();
	BFCWord *v_index = (BFCWord *) victim->GetDataPointer2();
	// here we get actor data pointers
	Win32_unlit_tex_ut_mesh *a_data = (Win32_unlit_tex_ut_mesh *) actor->GetDataPointer();
	BFCWord *a_index = (BFCWord *) actor->GetDataPointer2();

	tri_int = new BFCTriIntersect();

mDebugLineMesh.CreateDebugLine(3,6);

	// we go trough all triangles

	BFCUInt v = 0;
	while (v<victim->GetNumData2())
	{
		use_line_int = false;
		last_v1_v2_int = -1;
		tri_int->Reset();
		// fill all victim data. we need all data for victim
		tri_int->Tri1.v1 = v_data[v_index[v]];
		tri_int->Tri1.v2 = v_data[v_index[v+1]];
		tri_int->Tri1.v3 = v_data[v_index[v+2]];

		// transform pos to world

		tri_int->Tri1.v1.pos = victim->M * tri_int->Tri1.v1.pos;
		tri_int->Tri1.v2.pos = victim->M * tri_int->Tri1.v2.pos;
		tri_int->Tri1.v3.pos = victim->M * tri_int->Tri1.v3.pos;

		// Here we check collision between all actor triangles

		BFCUInt a = 0;
		BFCUInt number_right_intersect1 = 0;

		while (a<actor->GetNumData2())
		{
			// Transform and copy pos to world
			tri_int->Tri2.v1.pos = actor->M * a_data[a_index[a]].pos;
			tri_int->Tri2.v2.pos = actor->M * a_data[a_index[a+1]].pos;
			tri_int->Tri2.v3.pos = actor->M * a_data[a_index[a+2]].pos;


	mDebugLineMesh.SetDebugLine(3,&tri_int->Tri1.v1.pos,&tri_int->Tri1.v2.pos,0,0xFF00FF00);
	mDebugLineMesh.SetDebugLine(3,&tri_int->Tri1.v2.pos,&tri_int->Tri1.v3.pos,1,0xFF00FF00);
	mDebugLineMesh.SetDebugLine(3,&tri_int->Tri1.v3.pos,&tri_int->Tri1.v1.pos,2,0xFF00FF00);
	mDebugLineMesh.SetDebugLine(3,&tri_int->Tri2.v1.pos,&tri_int->Tri2.v2.pos,3,0xFF005500);
	mDebugLineMesh.SetDebugLine(3,&tri_int->Tri2.v2.pos,&tri_int->Tri2.v3.pos,4,0xFF005500);
	mDebugLineMesh.SetDebugLine(3,&tri_int->Tri2.v3.pos,&tri_int->Tri2.v1.pos,5,0xFF005500);
	
	BFCChar text[256];
	sprintf(text,"No");
	

			// if there is an intersection
			if (tri_int->IsTriIntersect())
			{
				sprintf(text,"Yes %i",tri_int->NumberIntersection);
				// Here we know the tri is between inside and outside. SpectRe
			} else
			{
				// Here maybe the tri is completly inside or outside. SpectRe
				// Check number of right intersections
				// We don't need to check all the lines and side (we use v1_v2), just one line and side are sufficient. SpectRe
				// Check if there was a new v1_v2 int
				if (tri_int->Last_v1_v2_int>last_v1_v2_int)
				{
					last_v1_v2_int = tri_int->Last_v1_v2_int;
					if (tri_int->T_v1_v2>=1.0f)
					{
						number_right_intersect1++;
						BFCVoid *_int = int_list.GetFirstPtr();
						// look if there is an intersection already at this position
						while (_int)
						{
							if ( BFCEqual(tri_int->T_v1_v2,*(BFCFloat *)_int,1.0f) )
							{
								use_line_int = true;
							}
							_int = int_list.GetNextPtr();
						}
						BFCFloat *_add = new BFCFloat;
						*_add = tri_int->T_v1_v2;
						int_list.Add((BFCVoid *)_add);
					}
				}				
			}
			a+=3;
	SendMessage(mhwnd,WM_SETTEXT,0,(long)text);
	mDebugLineMesh.DebugLineSet(3);
		}


		// Now we gonna add triangle(s) to the list

		// = 1 should not exist

		if (tri_int->NumberIntersection < 2)
		{
			// we check if we must use line_int method or use number_right_intersect
			if (use_line_int)
			{
				BFCLine _line;
				_line.Pos = tri_int->Tri1.v1.pos;
				_line.Dir = tri_int->Tri1.v2.pos;
				if (!LineInsideMesh(actor,&_line))
				{
					// Here the tri is outside so we add it to the list
					BFCTri *_tri = new BFCTri();
					*_tri = tri_int->Tri1;
					new_tri_list.Add(_tri);
				} else
				{
					// Here the tri is inside so we add nothing
				}
			} else
			{
				// Here the tri is inside or outside so we check it
				if (number_right_intersect1 % 2 == 0)
				{
		BFCChar text[256];
		sprintf(text,"Tri outside");
		SendMessage(mhwnd,WM_SETTEXT,0,(long)text);
		DebugVisualize();

					// Here the tri is outside so we add it to the list
					BFCTri *_tri = new BFCTri();
					*_tri = tri_int->Tri1;
					new_tri_list.Add(_tri);
				} else
				{
					// Here the tri is inside so we add nothing
				}
			}
		} else
		{
			// here we add new formed triangles
			// First we create the line list
			while (CreateLineListFromTriInt(actor,tri_int,&line_list))
			{
			}
			FillTriListFromLineList(victim,actor,&new_tri_list,&line_list);
			line_list.RemoveAll();

		}

		v+=3;
		// empty int_list
		int_list.RemoveDeleteAll();
	}
	number_newdata = new_tri_list.GetNumber();
	actor->DataSet();
	actor->Data2Set();
	victim->DataSet();
	victim->Data2Set();

	// now we can generate the new "cutted" mesh

	// here we set victim data pointers

	victim->DynamicMan->SetNumData(number_newdata*3);
	v_data = (Win32_unlit_tex_ut_mesh *) victim->DynamicMan->GetDataPointer();
	victim->DynamicMan->SetNumData2(number_newdata*3);
	v_index = (BFCWord *) victim->DynamicMan->GetDataPointer2();

	BFCTri *_tri = (BFCTri *)new_tri_list.GetFirstPtr();
	BFCUInt _i = 0;
	while (_tri)
	{
		// Here we fill first tri
		v_data[_i] = _tri->v1;
		// Transfrom back to local
		mGraphicEngine->WorldToLocal(&victim->M,&v_data[_i].pos);
		v_index[_i] = _i;
		_i++;
		v_data[_i] = _tri->v2;
		// Transfrom back to local
		mGraphicEngine->WorldToLocal(&victim->M,&v_data[_i].pos);
		v_index[_i] = _i;
		_i++;
		v_data[_i] = _tri->v3;
		// Transfrom back to local
		mGraphicEngine->WorldToLocal(&victim->M,&v_data[_i].pos);
		v_index[_i] = _i;
		_i++;
		
		_tri = (BFCTri *)new_tri_list.GetNextPtr();
	}
	//new_tri_list.RemoveDeleteAll();
	victim->DynamicMan->SetNewData();
}

//************************************************************************
//
//		BFCTriIntersect. struct used for tri intersection with another tri
//
//************************************************************************


BFCTriIntersect::Init()
{
	Reset();
}

BFCTriIntersect::BFCTriIntersect()
{
	Init();
}

BFCVoid BFCTriIntersect::Reset()
{
	// list of intersection used to avoid counting twice an intersection in the same place
	BFCVoidList int_list;
	// Reset of intersections
	NumberIntersection = 0;
	Last_v1_v2_int = -1;
	T_v1_v2 = BFCFarValue;
	corner1_used = false;
	corner2_used = false;
	corner3_used = false;
	corner_used_once = false;
	// BFCFarValue means no intersection
	for (BFCInt i=0;i<BFCMaxTriIntersection;i++)
	{
		IntersectInfo[i].TLineIntersect = BFCFarValue;
		IntersectInfo[i].Intersect.Zero();
		IntersectInfo[i].Edge1.Zero();
		IntersectInfo[i].Edge2.Zero();
		IntersectInfo[i].IntersectType = no_intersect;
		IntersectInfo[i].ConnectedToLine = false;
		IntersectInfo[i].Border = false;
	}	
}

BFCBool BFCTriIntersect::IsTriIntersect(BFCMesh *actor)
{
	BFCBool remove = false;
	BFCInt num_inc = 0;
	BFCBool border;
	BFCBool int_made = false;
	// We First check first edge-line of tri
	Line.Pos = Tri2.v1.pos;
	Line.Dir = Tri2.v2.pos;
	border = false;
	BFCBool on_border_once = false;
	BFCBool on_border_once2 = false;


	// If there is an intersection
	if (Line.IsTriIntersection(Tri1.v1.pos,Tri1.v2.pos,Tri1.v3.pos,&border))
	{
		// we check also if the line is in the tri
		if (Line.LastTValue<1.0f+BFCPrecision && Line.LastTValue>0.0f-BFCPrecision)
		{
			if (border && Line.LastTValue == bfc_all_t)
			{
				// we don't add intersection on all edge
				if (on_border_once)
				{
					// the last one and the next one we add, now we remove them
					NumberIntersection-=2;
				}
				on_border_once = true;
			}
			IntersectInfo[NumberIntersection].TLineIntersect = Line.LastTValue;
			IntersectInfo[NumberIntersection].Intersect = Line.LastPointIntersect;
			IntersectInfo[NumberIntersection].IntersectType = v1_v2_intersect2;
			IntersectInfo[NumberIntersection].Edge1 = Line.Pos;
			IntersectInfo[NumberIntersection].Edge2 = Line.Dir;
			IntersectInfo[NumberIntersection].OpEdge = Tri2.v3.pos;
			IntersectInfo[NumberIntersection].Border = border;
			NumberIntersection++;
			int_made = true;
			num_inc++;
		}
	}

	// We First check second edge-line of tri
	Line.Pos = Tri2.v2.pos;
	Line.Dir = Tri2.v3.pos;
	border = false;

	// If there is an intersection
	if (Line.IsTriIntersection(Tri1.v1.pos,Tri1.v2.pos,Tri1.v3.pos,&border))
	{
		// we check also if the line is in the tri
		if (Line.LastTValue<1.0f+BFCPrecision && Line.LastTValue>0.0f-BFCPrecision)
		{
			// we check if there was not already this intersection with the actual triangle
			BFCBool _exist = false;
			for (BFCInt i=1;i<=num_inc;i++)
			{
				if (IntersectInfo[NumberIntersection-i].Intersect.IsNear(Line.LastPointIntersect,BFCPrecision*10.0f))
				{
					_exist = true;
				}
			}
			if (!_exist)
			{
				if (border && Line.LastTValue == bfc_all_t)
				{
					// we don't add intersection on all edge
					if (on_border_once)
					{
						// the last one and the next one we add, now we remove them
						NumberIntersection-=2;
					}
					on_border_once = true;
				}
				IntersectInfo[NumberIntersection].TLineIntersect = Line.LastTValue;
				IntersectInfo[NumberIntersection].Intersect = Line.LastPointIntersect;
				IntersectInfo[NumberIntersection].IntersectType = v2_v3_intersect2;
				IntersectInfo[NumberIntersection].Edge1 = Line.Pos;
				IntersectInfo[NumberIntersection].Edge2 = Line.Dir;
				IntersectInfo[NumberIntersection].OpEdge = Tri2.v1.pos;
				IntersectInfo[NumberIntersection].Border = border;
				NumberIntersection++;
				int_made = true;
				num_inc++;
			}
		}
	}

	// We First check third edge-line of tri
	Line.Pos = Tri2.v3.pos;
	Line.Dir = Tri2.v1.pos;
	border = false;
	remove = false;


	// If there is an intersection
	if (Line.IsTriIntersection(Tri1.v1.pos,Tri1.v2.pos,Tri1.v3.pos,&border))
	{
		// we check also if the line is in the tri
		if (Line.LastTValue<1.0f+BFCPrecision && Line.LastTValue>0.0f-BFCPrecision)
		{
			// we check if there was not already this intersection with the actual triangle
			BFCBool _exist = false;
			for (BFCInt i=1;i<=num_inc;i++)
			{
				if (IntersectInfo[NumberIntersection-i].Intersect.IsNear(Line.LastPointIntersect,BFCPrecision*10.0f))
				{
					_exist = true;
				}
			}
			if (!_exist)
			{
				if (border && Line.LastTValue == bfc_all_t)
				{
					// we don't add intersection on all edge
					if (on_border_once)
					{
						// the last one and the next one we add, now we remove them
						remove = true;
					}
					on_border_once = true;
				}
				IntersectInfo[NumberIntersection].TLineIntersect = Line.LastTValue;
				IntersectInfo[NumberIntersection].Intersect = Line.LastPointIntersect;
				IntersectInfo[NumberIntersection].IntersectType = v3_v1_intersect2;
				IntersectInfo[NumberIntersection].Edge1 = Line.Pos;
				IntersectInfo[NumberIntersection].Edge2 = Line.Dir;
				IntersectInfo[NumberIntersection].OpEdge = Tri2.v2.pos;
				IntersectInfo[NumberIntersection].Border = border;
				NumberIntersection++;
				int_made = true;
				num_inc++;
				if (remove)
				{
					NumberIntersection-=2;
					num_inc-=2;
					int_made = false;
				}
				// we can't have more than 2 intersections between 2 triangle
				if (num_inc>2)
				{
					NumberIntersection-=1;
					num_inc-=1;
				}
			}
		}
	}	

	// Here we check the second tri with the first one


	// We First check first edge-line of tri
	Line.Pos = Tri1.v1.pos;
	Line.Dir = Tri1.v2.pos;
	border = false;
	remove = false;

	//---------------------------------------------------
	// This for is to transform a point near the line to a vx_vy_intersect type ( point on edge type )

	for (BFCInt i=1;i<=num_inc;i++)
	{
		// we get the nearest point
		Line.GetNearestPoint(IntersectInfo[NumberIntersection-i].Intersect);
		// Now we check if the point is not on the edge
		if (IntersectInfo[NumberIntersection-i].Intersect.IsNear(Line.LastPointIntersect,BFCPrecision*10.0f) )
		{
			IntersectInfo[NumberIntersection-i].TLineIntersect = Line.LastTValue;
			IntersectInfo[NumberIntersection-i].IntersectType = v1_v2_intersect;
			IntersectInfo[NumberIntersection-i].Edge1 = Line.Pos;
			IntersectInfo[NumberIntersection-i].Edge2 = Line.Dir;
			IntersectInfo[NumberIntersection-i].OpEdge = Tri1.v3.pos;
		}
	}

	//---------------------------------------------------

	// If there is an intersection
	if (Line.IsTriIntersection(Tri2.v1.pos,Tri2.v2.pos,Tri2.v3.pos,&border))
	{
		// we check also if the line is in the tri
		if (Line.LastTValue<1.0f+BFCPrecision && Line.LastTValue>0.0f-BFCPrecision)
		{
			// we check if there was not already this intersection with the actual triangle
			BFCBool _exist = false;
			for (BFCInt i=1;i<=num_inc;i++)
			{
				if (IntersectInfo[NumberIntersection-i].Intersect.IsNear(Line.LastPointIntersect,BFCPrecision*10.0f) )
				{
					// if there is a border intersection equal this one we set it as this one 
					if (IntersectInfo[NumberIntersection-i].Border)
					{
						IntersectInfo[NumberIntersection-i].TLineIntersect = Line.LastTValue;
						IntersectInfo[NumberIntersection-i].IntersectType = v1_v2_intersect;
						IntersectInfo[NumberIntersection-i].Edge1 = Line.Pos;
						IntersectInfo[NumberIntersection-i].Edge2 = Line.Dir;
						IntersectInfo[NumberIntersection-i].OpEdge = Tri1.v3.pos;
					}
					_exist = true;
				}
			}
			if (!_exist)
			{
				if (border && Line.LastTValue == bfc_all_t)
				{
					// we don't add intersection on all edge
					if (on_border_once2)
					{
						// the last one and the next one we add, now we remove them
						remove = true;
					}
					on_border_once2 = true;
				}
				IntersectInfo[NumberIntersection].TLineIntersect = Line.LastTValue;
				IntersectInfo[NumberIntersection].Intersect = Line.LastPointIntersect;
				IntersectInfo[NumberIntersection].IntersectType = v1_v2_intersect;
				IntersectInfo[NumberIntersection].Edge1 = Line.Pos;
				IntersectInfo[NumberIntersection].Edge2 = Line.Dir;
				IntersectInfo[NumberIntersection].OpEdge = Tri1.v3.pos;
				IntersectInfo[NumberIntersection].Border = border;
				NumberIntersection++;
				int_made = true;
				num_inc++;
				if (remove)
				{
					NumberIntersection-=2;
					num_inc-=2;
					int_made = false;
				}
				// we can't have more than 2 intersections between 2 triangle
				if (num_inc>2)
				{
					NumberIntersection-=1;
					num_inc-=1;
				}
			}
		}
		Last_v1_v2_int++;
		T_v1_v2 = Line.LastTValue;
	}

	// We First check second edge-line of tri
	Line.Pos = Tri1.v2.pos;
	Line.Dir = Tri1.v3.pos;
	border = false;
	remove = false;

	//---------------------------------------------------
	// This for is to transform a point near the line to a vx_vy_intersect type ( point on edge type )

	for (i=1;i<=num_inc;i++)
	{
		// we get the nearest point
		Line.GetNearestPoint(IntersectInfo[NumberIntersection-i].Intersect);
		// Now we check if the point is not on the edge
		if (IntersectInfo[NumberIntersection-i].Intersect.IsNear(Line.LastPointIntersect,BFCPrecision*10.0f) )
		{
			IntersectInfo[NumberIntersection-i].TLineIntersect = Line.LastTValue;
			IntersectInfo[NumberIntersection-i].IntersectType = v2_v3_intersect;
			IntersectInfo[NumberIntersection-i].Edge1 = Line.Pos;
			IntersectInfo[NumberIntersection-i].Edge2 = Line.Dir;
			IntersectInfo[NumberIntersection-i].OpEdge = Tri1.v1.pos;
		}
	}

	//---------------------------------------------------

	// If there is an intersection
	if (Line.IsTriIntersection(Tri2.v1.pos,Tri2.v2.pos,Tri2.v3.pos,&border))
	{
		// we check also if the line is in the tri
		if (Line.LastTValue<1.0f+BFCPrecision && Line.LastTValue>0.0f-BFCPrecision)
		{
			// we check if there was not already this intersection with the actual triangle
			BFCBool _exist = false;
			for (BFCInt i=1;i<=num_inc;i++)
			{
				if (IntersectInfo[NumberIntersection-i].Intersect.IsNear(Line.LastPointIntersect,BFCPrecision*10.0f) )
				{
					// if there is a border intersection equal this one we set it as this one 
					if (IntersectInfo[NumberIntersection-i].Border)
					{
						IntersectInfo[NumberIntersection-i].TLineIntersect = Line.LastTValue;
						IntersectInfo[NumberIntersection-i].IntersectType = v2_v3_intersect;
						IntersectInfo[NumberIntersection-i].Edge1 = Line.Pos;
						IntersectInfo[NumberIntersection-i].Edge2 = Line.Dir;
						IntersectInfo[NumberIntersection-i].OpEdge = Tri1.v1.pos;
					}
					_exist = true;
				}
			}
			if (!_exist)
			{
				if (border && Line.LastTValue == bfc_all_t)
				{
					// we don't add intersection on all edge
					if (on_border_once2)
					{
						// the last one and the next one we add, now we remove them
						remove = true;
					}
					on_border_once2 = true;
				}
				IntersectInfo[NumberIntersection].TLineIntersect = Line.LastTValue;
				IntersectInfo[NumberIntersection].Intersect = Line.LastPointIntersect;
				IntersectInfo[NumberIntersection].IntersectType = v2_v3_intersect;
				IntersectInfo[NumberIntersection].Edge1 = Line.Pos;
				IntersectInfo[NumberIntersection].Edge2 = Line.Dir;
				IntersectInfo[NumberIntersection].OpEdge = Tri1.v1.pos;
				IntersectInfo[NumberIntersection].Border = border;
				NumberIntersection++;
				int_made = true;
				num_inc++;
				if (remove)
				{
					NumberIntersection-=2;
					num_inc-=2;
					int_made = false;
				}
				// we can't have more than 2 intersections between 2 triangle
				if (num_inc>2)
				{
					NumberIntersection-=1;
					num_inc-=1;
				}
			}
		}
	}

	// We First check third edge-line of tri
	Line.Pos = Tri1.v3.pos;
	Line.Dir = Tri1.v1.pos;
	border = false;
	remove = false;

	//---------------------------------------------------
	// This for is to transform a point near the line to a vx_vy_intersect type ( point on edge type )

	for (i=1;i<=num_inc;i++)
	{
		// we get the nearest point
		Line.GetNearestPoint(IntersectInfo[NumberIntersection-i].Intersect);
		// Now we check if the point is not on the edge
		if (IntersectInfo[NumberIntersection-i].Intersect.IsNear(Line.LastPointIntersect,BFCPrecision*10.0f) )
		{
			IntersectInfo[NumberIntersection-i].TLineIntersect = Line.LastTValue;
			IntersectInfo[NumberIntersection-i].IntersectType = v3_v1_intersect;
			IntersectInfo[NumberIntersection-i].Edge1 = Line.Pos;
			IntersectInfo[NumberIntersection-i].Edge2 = Line.Dir;
			IntersectInfo[NumberIntersection-i].OpEdge = Tri1.v2.pos;
		}
	}

	//---------------------------------------------------

	// If there is an intersection
	if (Line.IsTriIntersection(Tri2.v1.pos,Tri2.v2.pos,Tri2.v3.pos,&border))
	{
		// we check also if the line is in the tri
		if (Line.LastTValue<1.0f+BFCPrecision && Line.LastTValue>0.0f-BFCPrecision)
		{
			// we check if there was not already this intersection with the actual triangle
			BFCBool _exist = false;
			for (BFCInt i=1;i<=num_inc;i++)
			{
				if (IntersectInfo[NumberIntersection-i].Intersect.IsNear(Line.LastPointIntersect,BFCPrecision*10.0f))
				{
					// if there is a border intersection equal this one we set it as this one 
					if (IntersectInfo[NumberIntersection-i].Border)
					{
						IntersectInfo[NumberIntersection-i].TLineIntersect = Line.LastTValue;
						IntersectInfo[NumberIntersection-i].IntersectType = v3_v1_intersect;
						IntersectInfo[NumberIntersection-i].Edge1 = Line.Pos;
						IntersectInfo[NumberIntersection-i].Edge2 = Line.Dir;
						IntersectInfo[NumberIntersection-i].OpEdge = Tri1.v2.pos;
					}
					_exist = true;
				}
			}
			if (!_exist)
			{
				if (border && Line.LastTValue == bfc_all_t)
				{
					// we don't add intersection on all edge
					if (on_border_once2)
					{
						// the last one and the next one we add, now we remove them
						remove = true;
					}
					on_border_once2 = true;
				}
				IntersectInfo[NumberIntersection].TLineIntersect = Line.LastTValue;
				IntersectInfo[NumberIntersection].Intersect = Line.LastPointIntersect;
				IntersectInfo[NumberIntersection].IntersectType = v3_v1_intersect;
				IntersectInfo[NumberIntersection].Edge1 = Line.Pos;
				IntersectInfo[NumberIntersection].Edge2 = Line.Dir;
				IntersectInfo[NumberIntersection].OpEdge = Tri1.v2.pos;
				IntersectInfo[NumberIntersection].Border = border;
				NumberIntersection++;
				int_made = true;
				num_inc++;
				if (remove)
				{
					NumberIntersection-=2;
					num_inc-=2;
					int_made = false;
				}
				// we can't have more than 2 intersections between 2 triangle
				if (num_inc>2)
				{
					NumberIntersection-=1;
					num_inc-=1;
				}
			}
		}
	}

//******************************************************************
//
//			Now we do some last check
//
//******************************************************************

	// we can't have only one intersection

	if (num_inc==1)
	{
		NumberIntersection-=1;
		num_inc-=1;
		int_made = false;
	}

	// if the two intersections are nearly the same we don't consider it since the tri is touching only with the corner

	if (num_inc == 2)
	{
		if (IntersectInfo[NumberIntersection-1].Intersect.IsNear(IntersectInfo[NumberIntersection-2].Intersect,BFCPrecision*10.0f))
		{
			NumberIntersection-=2;
			num_inc-=2;
			int_made = false;
		}

		// also check if the couple doesn't already exist in the array due to conjunction between 2 triangles

		// but first check if there are at least more than 2 intersections
		if (NumberIntersection>2)
		{
			// save wich one is equal
			BFCInt wich_eq = 0;
			BFCUInt i = 0;
			// go until the end but ignore the last 2 of cuzz
			while (i<NumberIntersection-2)
			{
				wich_eq = -1;
				if ( (IntersectInfo[i].Intersect).IsNear(IntersectInfo[NumberIntersection-1].Intersect,BFCPrecision*10.0f))
				{
					wich_eq = 0;
				}
				if ( (IntersectInfo[i].Intersect).IsNear(IntersectInfo[NumberIntersection-2].Intersect,BFCPrecision*10.0f))
				{
					wich_eq = 1;
				}

				i++;

				// one was equal ?

				if (wich_eq>=0)
				{
					if (wich_eq == 0)
					{
						if ( (IntersectInfo[i].Intersect).IsNear(IntersectInfo[NumberIntersection-2].Intersect,BFCPrecision*10.0f))
						{
							// this is to tell that there are 2 equal value
							wich_eq = 2;		
						}
					} else
					{
						if ( (IntersectInfo[i].Intersect).IsNear(IntersectInfo[NumberIntersection-1].Intersect,BFCPrecision*10.0f))
						{
							// this is to tell that there are 2 equal value
							wich_eq = 2;		
						}
					}
				}
				i++;
				// if there are 2 equal then remove this last tri intersection
				if (wich_eq == 2)
				{
					NumberIntersection-=2;
					num_inc-=2;
					int_made = false;
					break;
				}
			}
		}
	}


	return int_made;
}
