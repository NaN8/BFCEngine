#ifndef __BFCMAIN__H
#define __BFCMAIN__H




extern HINSTANCE mappinst;
extern HWND mhwnd;
extern int m_x;
extern int m_y;
extern bool mdebug;
extern BFCBool DebugVisualizeCall;
extern BFCBool DebugVisualizeCall2;
extern LRESULT CALLBACK wndproc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam);
extern BFCVoid DebugVisualize();
extern BFCVoid DebugVisualize2();




#endif
