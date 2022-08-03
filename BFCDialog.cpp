#include <BFCDialog.h>
#include <resource.h>
#include <main.h>
#include <resource.h>




//*********************************************************************
//
//
//					BFCDialog
//
//
//*********************************************************************

BFCVoidList BFCDialog::DialogList;


BFCDialog::BFCDialog(BFCDialogType type) : Type(type), Hwnd(NULL), P_Func(NULL), P_This(NULL), Sema(true), Info(NULL)
{
	switch (type)
	{
	case bfc_double_edit:
		{
			Hwnd = CreateDialog(mappinst,MAKEINTRESOURCE(IDD_DOUBLE_EDIT),mhwnd,DialogProc);
		} break;
	}
	DialogList.Add(this);
	// Set pointer to my Hwnd so that DialogProc can find me later
	SetPtr(Hwnd);
	// Set focus back to the main window
	SetFocus(mhwnd);
	Info = GetDlgItem(Hwnd,IDC_INFO);
}

BFCDialog::~BFCDialog()
{
	// Here we don't need to delete twice so false in remove SpectRe
	DialogList.Remove(this,false);
	EndDialog(Hwnd, 0);
}

BFCInt __stdcall BFCDialog::DialogProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	// Find the windows BFCDialog instance
	BFCDialog *dialog = (BFCDialog *)DialogList.Find(hwnd);
	switch (Message)
	{
	case WM_INITDIALOG:
		{
			// Can't put init dialog here because inpossible to know the instance at creation time, SpectRe
			// return 1: Set focus to the first control
			return 1;	
		} break;
	case WM_CLOSE:
		{
			dialog->Show(false);
			return 1;
		} break;
    case WM_COMMAND:
		{
			dialog->ICommand(wParam, dialog);
			return 1;
		}
	}
	return 0;
}

BFCVoid BFCDialog::Show(BFCBool val)
{
	ShowWindow(Hwnd,val);
}

POINT BFCDialog::GetMousePos()
{
	POINT p;
	GetCursorPos(&p);
	return p;
}

BFCVoid BFCDialog::SetPos(BFCInt x, BFCInt y)
{
	RECT rect;
	GetWindowRect(Hwnd,&rect);
	SetWindowPos(Hwnd,0,x,y,rect.right,rect.bottom,SWP_NOZORDER | SWP_NOACTIVATE);
}

BFCVoid BFCDialog::SetInfoText(BFCUInt n, BFCChar *text)
{
	SendMessage(Info,WM_SETTEXT,0,(LPARAM)text);
}

BFCVoid BFCDialog::SetInfoValue(BFCUInt n, BFCFloat val)
{
	BFCChar *p_char = Float2Char(val);
	SendMessage(Info,WM_SETTEXT,0,(LPARAM)p_char);
}

//*********************************************************************
//
//
//					BFCEditDialog
//
//
//*********************************************************************


BFCEditDialog::BFCEditDialog(BFCUInt num_edit, BFCDialogType type) : BFCDialog(type), EditCallBack(NULL), LastEditChange(0xFFFFFFFF)
{
	BFCUInt i;
	NumEdit = num_edit;
	// Create edit handle array
	EditHandle = new HWND[num_edit];
	// Create static handle array
	StaticHandle = new HWND[num_edit];
	// Create ID array
	EditId = new BFCInt[num_edit];
	StaticId = new BFCInt[num_edit];
	// Init values to zero
	for (i=0;i<num_edit;i++)
	{
		EditHandle[i] = NULL;
		EditId[i] = 0;
		StaticHandle[i] = NULL;
		StaticId[i] = 0;
	}
}

BFCEditDialog::~BFCEditDialog()
{
	if (EditHandle) delete [] EditHandle;
	if (StaticHandle) delete [] StaticHandle;
	if (EditId) delete [] EditId;
	if (StaticId) delete [] StaticId;
}

BFCVoid BFCEditDialog::CreateHwnd()
{
	BFCUInt i;
	for (i=0;i<NumEdit;i++)
	{
		if (EditId[i]!=0) EditHandle[i] = GetDlgItem(Hwnd,EditId[i]); else BFCAssert(0,"Forgot to set an EditID");
		if (StaticId[i]!=0) StaticHandle[i] = GetDlgItem(Hwnd,StaticId[i]); else BFCAssert(0,"Forgot to set an StaticID");
	}
}

// Get Methods

BFCChar *BFCEditDialog::GetEditText(BFCUInt n)
{
	SendMessage(EditHandle[n],WM_GETTEXT,256,(LPARAM)Buff);
	return Buff;
}

BFCFloat BFCEditDialog::GetEditValue(BFCUInt n)
{
	SendMessage(EditHandle[n],WM_GETTEXT,256,(LPARAM)Buff);
	BFCFloat _value = (BFCFloat)atof(Buff);
	return _value;
}

// Set Methods

BFCVoid BFCEditDialog::SetText(BFCChar *text)
{
	SendMessage(Hwnd,WM_SETTEXT,0,(LPARAM)text);
}

BFCVoid BFCEditDialog::SetStaticText(BFCUInt n, BFCChar *text)
{
	SendMessage(StaticHandle[n],WM_SETTEXT,0,(LPARAM)text);
}

BFCVoid BFCEditDialog::SetEditText(BFCUInt n, BFCChar *text)
{
	SendMessage(EditHandle[n],WM_SETTEXT,0,(LPARAM)text);
}

BFCVoid BFCEditDialog::SetEditValue(BFCUInt n, BFCFloat val)
{
	BFCChar *p_char = Float2Char(val);
	SendMessage(EditHandle[n],WM_SETTEXT,0,(LPARAM)p_char);
}

BFCUInt BFCEditDialog::FindEditIdNumber(BFCInt id)
{
	BFCUInt i = 0;
	while (i < NumEdit)
	{
		if ( EditId[i] == id ) return i;
		i++;
	}
	// return invalid
	return 0xFFFFFFFF;
}

BFCVoid BFCEditDialog::ICommand(LPARAM w_param, BFCDialog *from_dialog)
{
	// we cast to edit dialog
	BFCEditDialog *edit_dialog = (BFCEditDialog *)from_dialog;
	switch (LOWORD(w_param))
	{
	case IDC_OK:
		{
			from_dialog->Show(false);
		} break;
	default:
		{
			if (Sema)
			{
				// if a callback func has been set call it 
				if (EditCallBack)
				{
					P_Func = EditCallBack->pipe_func;
					P_This = EditCallBack->ThisPtr;
					(P_This->*P_Func)();
				}
				// other values are edit changed
				LastEditChange = FindEditIdNumber(LOWORD(w_param));
				edit_dialog->EditChanged( LastEditChange );
			}
		} break;
	}
}

//*********************************************************************
//
//
//					BFCDoubleEdit
//
//
//*********************************************************************


BFCDoubleEdit::BFCDoubleEdit(BFCChar *title, BFCChar *text1, BFCChar *text2) :	BFCEditDialog(2,bfc_double_edit)
{
	// Set the ID

	StaticId[0] = IDC_STATIC1;
	StaticId[1] = IDC_STATIC2;

	EditId[0] = IDC_EDIT1;
	EditId[1] = IDC_EDIT2;

	CreateHwnd();

	SetStaticText(0,text1);
	SetStaticText(1,text2);
	SetText(title);
}


