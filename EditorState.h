#ifndef __ED_MODE__H
#define __ED_MODE__H

#include <BFCBase.h>


enum BFCEditingMode
{
	bfc_normal_mode = 0,
	bfc_face_mode = 1,
	bfc_ed_max_mode = 2
};


//************************************************************************
//
//						BFCEditingMode
//
//************************************************************************


class BFCEditorState
{

	BFCEditingMode EdMode;
	BFCBool StickGrid;
	// Get all point in the same place
	BFCBool GetPoint;

public:

	BFCEditorState();

	BFCBool GetGetPoint() { return GetPoint; }
	BFCBool GetStickGrid() { return StickGrid; }
	BFCVoid SetStickGrid(BFCBool val) { StickGrid = val; }
	BFCEditingMode GetEdMode() { return EdMode; }
	BFCVoid SetEdMode(BFCEditingMode val) { EdMode = val; }
	BFCVoid SetGetPoint(BFCBool val) { GetPoint = val; }


};











#endif