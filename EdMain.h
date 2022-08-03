#ifndef __ED_MAIN__H
#define __ED_MAIN__H

#include <BFCBase.h>
#include <Primitives.h>
#include <BFCDialog.h>
#include <BFCKeyManager.h>
#include <EditorState.h>



//************************************************************************
//
//						MainEditor
//
//************************************************************************

// This class handle main editor stuff

class MainEditor : public BFCPipeline
{

	friend struct BFCViewPortChunk;
	friend struct BFCViewPortDataChunk;

	// Tmp var mainly used for comm between methods

	BFCMatrix Tmpview;	// used for settings view
	Win32_unlit_tex_ut_mesh *VertData;
	BFCWord *IndexData;

	// Editing Function var and mode

	BFCEditorState EdState;

	// Keys mouse info

	BFCInt MouseOutPosX;	// Used for managing the mouse when it's out. SpectRe
	BFCInt MouseOutPosY;	// Used for managing the mouse when it's out. SpectRe
	BFCInt LastMDownX;	// last mouse down in x
	BFCInt LastMDownY;	// last mouse down in y

	// Actual view info

	BFCFloat XRot,YRot;
	BFCVector CamPos;
	BFCInt ActViewPort;

	BFCFloat MoveInc;
	BFCVector VTmp;
	BFCMatrix MTmp;

	BFCInt XRes,YRes;
	HDC DesktopHDC;

	BFCMesh *LastSelectMesh;	// last selected mesh
	BFCMesh *PreLastSelectMesh;	// previous of last selected mesh
	BFCInt LastSelectVert;		// last selected vert
	BFCInt LastSelectFace;		// last selected face
	BFCTexture *SaveTexture;	// to save the selected texture (since for the moment we use a texture for selection)
	Win32_unlit_tex_ut_mesh TmpMeshData; // used to store tmp data value
	BFCInt NumberEqualVertex;

	BFCPrimitive *LastPrim;
	BFCIPrimitive *LastPrimI;
	// It's last T value used in PickVert
	BFCFloat LastTValue;

	BFCGrid *Grid;
	BFCTexture *SelectTex;
	
	// Dialog var

	BFCDoubleEdit *CoordDialog;

	// EditingVar

	BFCVoidList VertexMoveList;

	// Editing functions

	BFCVoid FillVertexToMove();
	BFCVoid ObjectMove(BFCInt mx,BFCInt my);
	BFCVoid VertexMove(BFCInt mx,BFCInt my);
	BFCVoid SelectVert(BFCInt mx,BFCInt my);
	BFCVoid SelectObject(BFCInt mx,BFCInt my);
	BFCVoid SelectFace(BFCInt mx,BFCInt my);
	BFCVoid Deselect();

	// Pipeline function

	BFCVoid InitPipeline();
	BFCVoid ChangeViewLoop();


public:

	// Construtor

	MainEditor();

	// Keys and mouse info

	BFCKeyManager Keys;

	// Message method

	BFCVoid Message(BFCChar *msg);

	// Init and creation

	BFCVoid Init();
	BFCVoid InitView();
	BFCVoid AddBorders();
	BFCVoid InitDialog();

	// Destruction

	// !!! Call close when app close otherwise you'll get bad error !!! SpectRe
	BFCVoid Close();

	// keys mouse info method

	BFCVoid SetAllKey(BFCBool val);
	BFCVoid MouseIsDown(BFCInt mx, BFCInt my);
	BFCVoid LMouseIsDown(BFCInt mx, BFCInt my);
	BFCVoid LMouseIsUp(BFCInt mx, BFCInt my);
	BFCVoid RMouseIsDown(BFCInt mx, BFCInt my);
	BFCVoid RMouseIsUp(BFCInt mx, BFCInt my);
	BFCVoid MouseMoved(BFCInt mx, BFCInt my);
	BFCVoid MouseWheel(BFCShort val);
	BFCVoid AltIsDown(BFCBool val);
	BFCVoid CtrlIsDown(BFCBool val);
	BFCVoid ShiftIsDown(BFCBool val);
	BFCVoid MIsDown(BFCBool val);
	BFCVoid XIsDown(BFCBool val);
	BFCVoid YIsDown(BFCBool val);
	BFCVoid ZIsDown(BFCBool val);
	BFCVoid EIsDown(BFCBool val);
	BFCVoid PIsDown(BFCBool val);
	BFCVoid OIsDown(BFCBool val);
	BFCVoid SIsDown(BFCBool val);
	BFCVoid HomeDown();
	BFCVoid EndDown();
	BFCVoid PageDown();
	BFCVoid PageUp();
	BFCVoid UpArrow();
	BFCVoid DownArrow();
	BFCVoid LeftArrow();
	BFCVoid RightArrow();

	BFCVoid MouseOutScreen(BFCInt &mx, BFCInt &my);

	// Primitives creation methods

	BFCVoid CreateCube();
	BFCVoid CreateSphere();
	BFCVoid CreatePyramid();
	BFCVoid LoadTexture();
	BFCVoid EditCoord();

	// Editing functions

	BFCVoid BoolSubstract();

	// Give the equal vertex number n of LastSelectVert return -1 if no equal vertex found

	BFCInt GetEqualVertex(BFCInt n, BFCVector pos);

	// Dialog method here

	BFCVoid CoordChanged();
	BFCVoid RefreshCoord();

	// View method

	BFCVoid CheckChangeView(BFCInt mx, BFCInt my);
	BFCVoid RefreshViewInfo();

	// Help method

	// Stick point to grid
	BFCVoid StickPoint(BFCMesh *mesh, BFCVector *vector);

	// Get methods here

	BFCUInt GetActViewPort() { return ActViewPort; }
	BFCMesh *GetSelectedMesh() { return LastSelectMesh; }

	// Set methods here

	BFCVoid SetSelectedFaceColor(BFCUInt color);

	// Use VTmp to change the cam pos
	BFCVoid ChangeCamPos();
	BFCVoid RefreshCam();

	// saving loading methods

	BFCVoid Save();
	BFCVoid Load();
	// remove only the primitive or meshes of the scene not interface stuff or whatever
	BFCVoid RemoveAllMeshScene();
	// Clear all the scene
	BFCVoid EmptyScene();
};

extern MainEditor *mMainEditor;



#endif __ED_MAIN__H