#include <BFCMath.h>
#include <BFCUtility.h>
#include <BFCPipeline.h>
#include <BFCManager.h>
#include <stdio.h>
#include <EdMain.h>
#include <BFCSaveLoad.h>


HINSTANCE mappinst=NULL;
HWND mhwnd=NULL;
int m_x = 1024;
int m_y = 768;
bool mdebug;

BFCManager gManager;
MainEditor Editor;
MSG gmsg;
BFCBool StayInDebugVisualize = true;
BFCBool DebugVisualizeCall = false;
BFCBool DebugVisualizeCall2 = false;

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
				gManager.Execute();
			} break;
		case WM_TIMER:
			{
			} break;
		case WM_RBUTTONDOWN:
			{
				BFCInt xPos = LOWORD(lParam);  // horizontal position of cursor 
				BFCInt yPos = HIWORD(lParam);  // vertical position of cursor 
				Editor.RMouseIsDown(xPos, yPos);
			} break;
		case WM_RBUTTONUP:
			{
				BFCInt xPos = LOWORD(lParam);  // horizontal position of cursor 
				BFCInt yPos = HIWORD(lParam);  // vertical position of cursor 
				Editor.RMouseIsUp(xPos, yPos);
			} break;
		case WM_LBUTTONDOWN:
			{
				BFCInt xPos = LOWORD(lParam);  // horizontal position of cursor 
				BFCInt yPos = HIWORD(lParam);  // vertical position of cursor 
				Editor.LMouseIsDown(xPos, yPos);
/*				if (mGraphicEngine->PickMesh(1,xPos,yPos))
				{
					SendMessage(mhwnd,WM_SETTEXT,0,(long)"True");
				} else
				{
					SendMessage(mhwnd,WM_SETTEXT,0,(long)"False");
				}*/
			} break;
		case WM_LBUTTONUP:
			{
				BFCInt xPos = LOWORD(lParam);  // horizontal position of cursor 
				BFCInt yPos = HIWORD(lParam);  // vertical position of cursor 
				Editor.LMouseIsUp(xPos, yPos);
			} break;
		case WM_MOUSEMOVE:
			{
				BFCInt xPos = LOWORD(lParam);  // horizontal position of cursor 
				BFCInt yPos = HIWORD(lParam);  // vertical position of cursor 
				Editor.MouseMoved(xPos,yPos);
			} break;
		case WM_MOUSEWHEEL:
			{
				BFCShort zDelta = (short) HIWORD(wParam);    // wheel rotation
				Editor.MouseWheel(zDelta);
			} break;
		case WM_SIZE:
			{
				m_x = LOWORD(lParam); // width of client area 
				m_y = HIWORD(lParam); // height of client area
				BFCRect rect;
				rect.right = m_x;
				rect.down = m_y;
				mGraphicEngine->ViewPort.SetWindowRect(rect);
			} break;
		case WM_SYSKEYDOWN:
			{
				switch(wParam)
				{
				case 'C':
					{
						Editor.CreateCube();
					} break;
				case 'S':
					{
						Editor.CreateSphere();
					} break;
				case 'P':
					{
						Editor.CreatePyramid();
					} break;
				case 'D':
					{
						StayInDebugVisualize = false;
					} break;
				case 'X':
					{
						if (DebugVisualizeCall)
						{
							DebugVisualizeCall = false;
						}
						else 
						{
							DebugVisualizeCall = true;
						}
					} break;
				case 'J':
					{
						if (DebugVisualizeCall2)
						{
							DebugVisualizeCall2 = false;
						}
						else 
						{
							DebugVisualizeCall2 = true;
						}
					} break;
				}
			} break;
		case WM_KEYDOWN:
			{
				switch(wParam)
				{
				case VK_CONTROL:
					{
						Editor.CtrlIsDown(true);
					} break;
				case VK_HOME:
					{
						Editor.HomeDown();
					} break;
				case VK_END:
					{
						Editor.EndDown();
					} break;
				case VK_PRIOR:
					{
						Editor.PageUp();
					} break;
				case VK_NEXT:
					{
						Editor.PageDown();
					} break;
				case VK_SHIFT:
					{
						Editor.ShiftIsDown(true);
					} break;
				case 'C':
					{
						if (Editor.Keys.GetControl())
						{
							Editor.EditCoord();
						}
					} break;
				case 'P':
					{
						Editor.PIsDown(true);
					} break;
				case 'M':
					{
						Editor.MIsDown(true);
					} break;
				case 'X':
					{
						if (Editor.Keys.GetControl())
						{
							Editor.BoolSubstract();
						} else
						{
							Editor.XIsDown(true);
						}
					} break;
				case 'Y':
					{
						Editor.YIsDown(true);
					} break;
				case 'Z':
					{
						Editor.ZIsDown(true);
					} break;
				case 'O':
					{
						Editor.OIsDown(true);
					} break;
				case 'S':
					{
						Editor.SIsDown(true);
					} break;
				case 'E':
					{
						Editor.EIsDown(true);
					} break;
				case VK_UP:
					{
						Editor.UpArrow();
					} break;
				case VK_DOWN:
					{
						Editor.DownArrow();
					} break;
				case VK_LEFT:
					{
						Editor.LeftArrow();
					} break;
				case VK_RIGHT:
					{
						Editor.RightArrow();
					} break;
				}
			} break;
		case WM_KEYUP:
			{
				switch(wParam)
				{
				case 'P':
					{
						Editor.PIsDown(false);
					} break;
				case 'X':
					{
						Editor.XIsDown(false);
					} break;
				case 'Y':
					{
						Editor.YIsDown(false);
					} break;
				case 'Z':
					{
						Editor.ZIsDown(false);
					} break;
				case 'O':
					{
						Editor.OIsDown(false);
					} break;
				case 'S':
					{
						Editor.SIsDown(false);
					} break;
				case 'E':
					{
						Editor.EIsDown(false);
					} break;
				case VK_CONTROL:
					{
						Editor.CtrlIsDown(false);
					} break;
				case VK_MENU:
					{
						Editor.AltIsDown(false);
					} break;
				
				case VK_SHIFT:
					{
						Editor.ShiftIsDown(false);
					} break;
				}
			} break;
	}
	return (DefWindowProc(hwnd,msg,wParam,lParam));
};

//************************************************************************************
//
//						DebugVisualize Let us see the graphic scene while debugging
//
//************************************************************************************


BFCVoid DebugVisualize()
{
	if (DebugVisualizeCall)
	{
		while (StayInDebugVisualize){
			if (PeekMessage(&gmsg,NULL,0,0,PM_REMOVE)){
				if (gmsg.message==WM_QUIT) break;
				TranslateMessage(&gmsg);
				DispatchMessage(&gmsg);
			}
			
			gManager.Execute();
		}
		StayInDebugVisualize = true;
	}
}

BFCVoid DebugVisualize2()
{
	if (DebugVisualizeCall2)
	{
		while (StayInDebugVisualize){
			if (PeekMessage(&gmsg,NULL,0,0,PM_REMOVE)){
				if (gmsg.message==WM_QUIT) break;
				TranslateMessage(&gmsg);
				DispatchMessage(&gmsg);
			}
			
			gManager.Execute();
		}
		StayInDebugVisualize = true;
	}
}

//************************************************************************
//
//						WinMain
//
//************************************************************************

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	WNDCLASS wc;
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

	gManager.Init(m_x,m_y);






	Editor.Init();
/*
	BFCMesh *test;
	test = new BFCMesh(unlit_tex_ut_i_tri_mesh,"Test");
	test->SetNumData(3);
	test->SetNumData2(3);
	Win32_unlit_tex_ut_mesh *data = (Win32_unlit_tex_ut_mesh *)test->GetDataPointer();
	BFCDWord *data2 = (BFCDWord *)test->GetDataPointer2();
	data[0].pos.x = -0.5f;data[0].pos.y = 0.0f;data[0].pos.z = 1.5f;data[0].color=0xFFFFFFFF;
	data[1].pos.x = 0.0f;data[1].pos.y = 1.5f;data[1].pos.z = -1.5f;data[1].color=0xFFFFFFFF;
	data[2].pos.x = 1.5f;data[2].pos.y = -0.5f;data[2].pos.z = 1.5f;data[2].color=0xFFFFFFFF;

	BFCVector tmp,tmp2,calc;
	tmp = data[1].pos-data[0].pos;
	tmp2 = data[2].pos-data[0].pos;
	data[0].normal = tmp.CrossProduct(tmp2);
	data[0].normal.Normalize();

	tmp = data[2].pos-data[1].pos;
	tmp2 = data[0].pos-data[1].pos;
	data[1].normal = tmp.CrossProduct(tmp2);
	data[1].normal.Normalize();

	tmp = data[0].pos-data[2].pos;
	tmp2 = data[1].pos-data[2].pos;
	data[2].normal = tmp.CrossProduct(tmp2);
	data[2].normal.Normalize();


	data[0].u =0.0f;data[0].v =0.0f;
	data[1].u =1.0f;data[1].v =0.0f;
	data[2].u =1.0f;data[2].v =1.0f;

	test->Tex->LoadRawFile("fer.raw");

	data2[0] = 1;
	data2[1] = 2;
	data2[2] = 3;
*/

	//BFCISphere *cube = new BFCISphere(20.0f,20.0f,20.0f,20);
/*
	BFCICube *cube = new BFCICube(12.0f,12.0f,42.0f,bfc_angle_not_shared);
	BFCTexture *tex = new BFCTexture(bfc_rgba_tex);
	tex->LoadRawFile("Fer.raw");
	cube->InsertTexture(tex,0);
*/
//	BFCCube *cube2 = new BFCCube(42.0f,42.0f,42.0f);
//	cube2->Tex->LoadRawFile("wood.raw");

/*
D3DMATERIAL8 mtrl;
ZeroMemory( &mtrl, sizeof(D3DMATERIAL8) );
mtrl.Diffuse.r = 1.0f;
mtrl.Diffuse.g = 1.0f;
mtrl.Diffuse.b = 1.0f;
mtrl.Diffuse.a = 1.0f;
mD3dDevice->SetMaterial( &mtrl );
*/



	BFCLight *light = new BFCLight(bfc_dir_light);
	light->SetDiffuse(BFCColor(1.0f,1.0f,1.0f,0.0f));
	light->SetDirection(BFCVector(0.0f,0.0f,1.0f));
	mGraphicEngine->AddLight(light);
	light->Enable();

	//mGraphicEngine->AddMesh(test);
/*
	BFCVector f1,f2,f3;
	f1.x = -8.11f;
	f1.y = -8.11f;
	f1.z = 0.15f;

	f2.x = -11.20f;
	f2.y = -0.64f;
	f2.z = 0.15f;

	f3 = f2-f1;

	BFCVector z,n1;
	z.Zero();
	n1.x = -0.67f;
	n1.y = -0.28f;
	n1.z = -0.67f;

	mDebugLineMesh.CreateDebugLine(8,2,1);
	mDebugLineMesh.SetDebugLine(8,&z,&f3,0,0xFF555555);
	mDebugLineMesh.SetDebugLine(8,&z,&n1,1,0xFF00FFFF);
	mDebugLineMesh.DebugLineSet(8,"ah");
*/

	BFCMatrix view;
	BFCFloat ang = 0.0f;

	BFCFloat pos = 1.0f;
	BFCFloat inc = 0.01f;

//	cube->M.SetPos(0.0f,0.0f,102.0f);
	while (1){
		if (PeekMessage(&gmsg,NULL,0,0,PM_REMOVE)){
			if (gmsg.message==WM_QUIT) break;
			TranslateMessage(&gmsg);
			DispatchMessage(&gmsg);
		}

	//view.SetRotateX(PI/2.0f);
	//view.SetPos(0.0f,6.0f,0.0f);
//	mGraphicEngine->ViewPort.SetView(0,&view);
	
//	mGraphicEngine->ViewPort.SetCameraPosition(0,0.0f,0.0f,0.0f);
	
	pos+=inc;
	if (pos>10.0f || pos<1.0f)
	{
		inc=-inc;
		//pos+=2*inc;
	}

		//cube->M.RotateX(PI/2000);

//		cube2->M.SetPos(BFCVector(0.0f,0.0f,152.0f));
//		cube2->M.SetPos(BFCVector(4.0f,0.0f,20.0f));

		
		gManager.Execute();
		ang+=PI/2000;
	}		

	gManager.Close();
	mMainEditor->Close();

	return(gmsg.wParam);
}