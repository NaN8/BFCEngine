#ifndef __ED_BFCDIALOG__H
#define __ED_BFCDIALOG__H

#include <BFCBase.h>
#include <BFCUtility.h>
#include <BFCMath.h>
#include <BFCPipeline.h>


extern HWND mhwnd;
extern HINSTANCE mappinst;

enum BFCDialogType
{
	bfc_double_edit = 0
};


class BFCDialog : public BFCVoidNode
{

protected:

	// Here Pipeline system is used also for callback system
	void (BFCPipeline::*P_Func)();	// Temp pointer to function
	BFCPipeline *P_This;			// Pointer the to right pipeline type to be executed


	BFCDialogType Type;
	HWND Hwnd;
	HWND Info;

	// internal static list of all windows created, mainly used in DialogProc. SpectRe
	static BFCVoidList DialogList;

	// Last Dialog who received the message

	//static BFCDialog *LastDialog;

	// Sema to avoid Pseudo multhreading problem

	BFCBool Sema;

	// Constructor

	BFCDialog(BFCDialogType type);

	// overrides method

	virtual BFCVoid ICommand(LPARAM l_param, BFCDialog *from_dialog) { };

public:

	// Destructor

	~BFCDialog();

	// Methods

	BFCVoid Show(BFCBool val);
	BFCVoid SetPos(BFCInt x, BFCInt y);

	// user override method



	//	Get methods

	POINT GetMousePos();
	HWND GetHandle() { return Hwnd; }

	// Set methods

	BFCVoid SetSema(BFCBool val) { Sema = val; }
	BFCVoid SetInfoText(BFCUInt n, BFCChar *text);
	BFCVoid	SetInfoValue(BFCUInt n, BFCFloat val);

	// The dialog callback function. SpectRe
	static BFCInt __stdcall DialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam);

};

//*********************************************************************
//
//
//					BFCEditDialog
//
//
//*********************************************************************


class BFCEditDialog : public BFCDialog
{
	// Dyn array of edit handle
	HWND *EditHandle;
	// Dyn array of static handle
	HWND *StaticHandle;

	BFCUInt NumEdit;

	BFCChar Buff[256];

	// Indicate which edit changed last time
	BFCUInt LastEditChange;	

protected:

	BFCPipeFunc *EditCallBack;	// Can be a Pointer to an edit callback

	// Dyn array of ID
	BFCInt *EditId;
	BFCInt *StaticId;

	// to call after filling above ID array
	BFCVoid CreateHwnd();

	// Find the Edit Id pos

	BFCUInt FindEditIdNumber(BFCInt id);

	virtual BFCVoid ICommand(LPARAM w_param, BFCDialog *from_dialog);

public:

	BFCEditDialog(BFCUInt num_edit, BFCDialogType type);
	~BFCEditDialog();

	// user override or callback settings

	virtual BFCVoid EditChanged(BFCUInt n) { };
	BFCVoid SetEditCallback(BFCPipeFunc *PipeFunc) { EditCallBack = PipeFunc; };

	// Get Methods

	BFCChar *GetEditText(BFCUInt n);
	BFCFloat GetEditValue(BFCUInt n);
	BFCUInt GetLastEditChange() { return LastEditChange; };

	// Set Methods

	BFCVoid SetText(BFCChar *text);
	BFCVoid SetStaticText(BFCUInt n, BFCChar *text);
	BFCVoid SetEditText(BFCUInt n, BFCChar *text);
	BFCVoid SetEditValue(BFCUInt n, BFCFloat val);

};

//*********************************************************************
//
//
//					BFCDoubleEdit
//
//
//*********************************************************************


class BFCDoubleEdit : public BFCEditDialog
{

protected:


public:

	BFCDoubleEdit(BFCChar *title, BFCChar *text1, BFCChar *text2);

};


#endif