#include <stdio.h>
#include <windows.h>
#include <D3d8.h>
#include <BFCMath.h>


HINSTANCE mappinst=NULL;
HWND mhwnd=NULL;
int m_x = 1024;
int m_y = 768;
bool mdebug;

struct Vector
{
	float x,y,z;
};


struct Vector3d
{
	Vector pos;			// The untransformed position for the vertex.
    unsigned int  color;			// The vertex color.
};



//************************************************************************
//
//						wndproc
//
//************************************************************************


LRESULT CALLBACK wndproc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg)
	{
		case WM_CLOSE:
			{
				PostQuitMessage(0);
				return 0;
			} break;
		case WM_ACTIVATE:
			{
				if (wParam!=WA_INACTIVE)
				{

				}
			} break;
		case WM_PAINT:
			{
			} break;
		case WM_TIMER:
			{
			} break;
	}
	return (DefWindowProc(hwnd,msg,wParam,lParam));
};

//************************************************************************
//
//						WinMain
//
//************************************************************************

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	WNDCLASS wc;
	MSG msg;
	mappinst=hInstance;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wc.hInstance=hInstance;
	wc.lpfnWndProc=wndproc;
	wc.lpszClassName="BFCEditor";
	wc.lpszMenuName=NULL;
	wc.style=CS_OWNDC | CS_VREDRAW | CS_HREDRAW;

	if(!RegisterClass(&wc)) MessageBox(NULL,"Can't register Window class","ERROR",MB_OK | MB_ICONERROR);
	if (!(mhwnd=CreateWindow("BFCEditor","BFCEditor",WS_OVERLAPPEDWINDOW | WS_VISIBLE,0,0,m_x,m_y,NULL,NULL,hInstance,NULL)))
	{
		MessageBox(NULL,"Can't create Window ","ERROR",MB_OK | MB_ICONERROR);
	}






	IDirect3D8 *D3d;
	D3DDISPLAYMODE D3ddm;
	IDirect3DDevice8 *D3dDevice;
	D3DPRESENT_PARAMETERS D3dpp;


	if( NULL == ( D3d = Direct3DCreate8( D3D_SDK_VERSION ) ) )
	{

		return -1;
	}

	if( FAILED( D3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &D3ddm ) ) )
	{

		return -1;
	}

	ZeroMemory( &D3dpp, sizeof(D3dpp) );
	D3dpp.Windowed   = TRUE;
	D3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3dpp.BackBufferFormat = D3ddm.Format;
	D3dpp.BackBufferWidth = 1024;
	D3dpp.BackBufferHeight = 768;
	// hmmm to check these two...
    D3dpp.EnableAutoDepthStencil = TRUE;
    D3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	D3dpp.Flags = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;



	if( FAILED( D3d->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, mhwnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&D3dpp, &D3dDevice ) ) )
	{

		return -1;
	}


	BFCMatrix proj;
	proj.ProjectionMatrix(bfc_perspec_proj,1.0f,10000.0f,PI*0.3333f,PI*0.3333f); // 0.3333 ~= 1/3
 	D3dDevice->SetTransform(D3DTS_PROJECTION, (D3DMATRIX *)&proj);











	Vector3d *data;
	unsigned int *index_data;
	IDirect3DVertexBuffer8* vb;
	IDirect3DIndexBuffer8 *index;

	D3dDevice->CreateVertexBuffer(sizeof(Vector3d)*3,0,D3DFVF_XYZ,D3DPOOL_DEFAULT,&vb);
	D3dDevice->CreateIndexBuffer(12,0,D3DFMT_INDEX32,D3DPOOL_DEFAULT,&index);

	vb->Lock(0,0,(unsigned char **)&data,0);
	data[0].pos.x = 0.0f;
	data[0].pos.y = 0.0f;
	data[0].pos.z = 10.0f;

	data[1].pos.x = 0.0f;
	data[1].pos.y = 5.0f;
	data[1].pos.z = 10.0f;
	
	data[2].pos.x = 5.0f;
	data[2].pos.y = 5.0f;
	data[2].pos.z = 10.0f;
	vb->Unlock();

	index->Lock(0,12,(unsigned char **)&index_data,0);

	index_data[0] = 0;
	index_data[1] = 1;
	index_data[2] = 2;

	index->Unlock();

	D3dDevice->SetRenderState(D3DRS_CLIPPING , FALSE);
	D3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	while (1){
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)){
			if (msg.message==WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		D3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,255), 1.0f, 0L );
		D3dDevice->BeginScene();
		D3dDevice->SetStreamSource( 0, vb, sizeof(Vector3d) );
		D3dDevice->SetIndices( index, 0 );
		D3dDevice->SetVertexShader( D3DFVF_XYZ);
		D3dDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 3, 0, 1 );
		D3dDevice->EndScene();
		D3dDevice->Present( NULL, NULL, NULL, NULL );
		

	}		



	return(msg.wParam);
}