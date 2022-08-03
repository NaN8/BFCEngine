#ifndef __ED_KEYMANAGER__H
#define __ED_KEYMANAGER__H

#include <BFCBase.h>


enum BFCKey
{
	bfc_none = 0,
	bfc_ctrl,
	bfc_alt,
	bfc_shift,
	bfc_x,
	bfc_y,
	bfc_z,
	bfc_m,
	bfc_e,
	bfc_c,
	bfc_o,
	bfc_p,
	bfc_s,
	bfc_home,
	bfc_end,
	bfc_prev,
	bfc_next,
	bfc_lmousedown,
	bfc_mmousedown,
	bfc_rmousedown
};


class BFCKeyManager
{

	// Keys mouse info

	BFCBool AltDown;
	BFCBool CtrlDown;
	BFCBool ShiftDown;
	BFCBool XDown;
	BFCBool YDown;
	BFCBool ZDown;
	BFCBool EDown;
	BFCBool MDown;
	BFCBool ODown;
	BFCBool PDown;
	BFCBool RDown;
	BFCBool SDown;
	BFCBool PrevDown;
	BFCBool NextDown;
	BFCBool HomeDown;
	BFCBool EndDown;


	BFCBool LMouseDown;
	BFCBool MMouseDown;
	BFCBool RMouseDown;

	// List of key pressed recently
	BFCKey KeyPressed[4];

public:

	// Constructor

	BFCKeyManager();

	BFCVoid CtrlIsDown(BFCBool val) { CtrlDown = val; AddNewKey(bfc_ctrl); }
	BFCVoid AltIsDown(BFCBool val) { AltDown = val; AddNewKey(bfc_alt); }
	BFCVoid ShiftIsDown(BFCBool val) { ShiftDown = val; AddNewKey(bfc_shift); }
	BFCVoid XIsDown(BFCBool val) { XDown = val; AddNewKey(bfc_x); }
	BFCVoid YIsDown(BFCBool val) { YDown = val; AddNewKey(bfc_y); }
	BFCVoid ZIsDown(BFCBool val) { ZDown = val; AddNewKey(bfc_z); }
	BFCVoid OIsDown(BFCBool val) { ODown = val; AddNewKey(bfc_o); }
	BFCVoid PIsDown(BFCBool val) { PDown = val; AddNewKey(bfc_p); }
	BFCVoid SIsDown(BFCBool val) { SDown = val; AddNewKey(bfc_s); }
	BFCVoid LMouseIsDown(BFCBool val) { LMouseDown = val; AddNewKey(bfc_lmousedown); }
	BFCVoid MMouseIsDown(BFCBool val) { MMouseDown = val; AddNewKey(bfc_mmousedown); }
	BFCVoid RMouseIsDown(BFCBool val) { RMouseDown = val; AddNewKey(bfc_rmousedown); }
	BFCVoid MIsDown(BFCBool val) { MDown = val; AddNewKey(bfc_m); }
	BFCVoid EIsDown(BFCBool val) { EDown = val; AddNewKey(bfc_e); }
	BFCVoid HomeIsDown(BFCBool val) { HomeDown = val; AddNewKey(bfc_home); }
	BFCVoid EndIsDown(BFCBool val) { EndDown = val; AddNewKey(bfc_end); }
	BFCVoid PrevIsDown(BFCBool val) { PrevDown = val; AddNewKey(bfc_prev); }
	BFCVoid NextIsDown(BFCBool val) { NextDown = val; AddNewKey(bfc_next); }

	// Get Methods

	BFCBool GetControl() { return CtrlDown; }
	BFCBool GetAlt() { return AltDown; }
	BFCBool GetShift() { return ShiftDown; }
	BFCBool GetX() { return XDown; }
	BFCBool GetY() { return YDown; }
	BFCBool GetZ() { return ZDown; }
	BFCBool GetHome() { return HomeDown; }
	BFCBool GetEnd() { return EndDown; }
	BFCBool GetPrev() { return PrevDown; }
	BFCBool GetNext() { return NextDown; }
	BFCBool GetE() { return EDown; }
	BFCBool GetM() { return MDown; }
	BFCBool GetR() { return RDown; }
	BFCBool GetS() { return SDown; }
	BFCBool GetO() { return ODown; }
	BFCBool GetLMouse() { return LMouseDown; }
	BFCBool GetMMouse() { return MMouseDown; }
	BFCBool GetRMouse() { return RMouseDown; }

	// Add new key in the key pressed list

	AddNewKey(BFCKey key);
	BFCVoid Flush();

};



#endif