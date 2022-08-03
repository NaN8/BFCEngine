#ifndef __BFCGRAPHICENGINE__H
#define __BFCGRAPHICENGINE__H

#ifdef WIN32
#include <D3d8.h>
#endif

#include <BFCUtility.h>
#include <Main.h>
#include <BFCMesh.h>
#include <BFCLight.h>
#include <BFCEngineState.h>


// Return error of graphic engine

enum BFCGEReturn
{
	g_e_error,
	g_e_ok
};

struct BFCFrustum
{
	BFCDWord x;
	BFCDWord y;
	BFCDWord width;
	BFCDWord height;
	BFCFloat near_clip;
	BFCFloat far_clip;

	BFCFrustum() : x(0), y(0), width(0), height(0), near_clip(0), far_clip(1) { }
};

enum BFCViewType
{
	bfc_perspec_view,
	bfc_top_view,
	bfc_front_view,
	bfc_left_view
};

#ifdef WIN32

struct BFCTri
{
	Win32_unlit_tex_ut_mesh v1;
	Win32_unlit_tex_ut_mesh v2;
	Win32_unlit_tex_ut_mesh v3;
};

#endif

//************************************************************************
//
//		BFCTriIntersect. struct used for tri intersection with another tri
//
//************************************************************************

const BFCUInt BFCMaxTriIntersection = 2000;

enum BFCEdge
{
	no_edge = 0,
	v1_v2_edge = 4,		// these number must in relation with number from v1_v2_intersect in BFCIntersectionType
	v2_v3_edge = 5,
	v3_v1_edge = 6
};

enum BFCIntersectionType
{
	no_intersect = 0,
	v1_v2_intersect2 = 1,	// intersect2 means intersection of Tri2 in BFCTriIntersect
	v2_v3_intersect2 = 2,
	v3_v1_intersect2 = 3,
	v1_v2_intersect = 4,
	v2_v3_intersect = 5,
	v3_v1_intersect = 6
};

enum BFCLineIntoMesh
{
	bfc_line_out = 0,
	bfc_line_in = 1,
	bfc_line_int = 2
};

struct BFCIntersectInfo
{
	// tell if it has been connected to a line list
	BFCBool ConnectedToLine;
	BFCVector Intersect;
	// if TLineIntersect is BFCFarValue there was no intersection of the corresponding IntersectType line
	BFCFloat TLineIntersect;
	BFCIntersectionType IntersectType;
	// These contain the pos of the edge that has been intersected
	// Here edge are corner, misuse of the term!!!
	BFCVector Edge1;
	BFCVector Edge2;
	// the oposite edge
	BFCVector OpEdge;
	BFCBool Border;

	BFCIntersectInfo() : IntersectType(no_intersect), TLineIntersect(BFCFarValue), ConnectedToLine(false), Border(false) { }
};

struct BFCTriIntersect
{
	BFCTri Tri1;
	BFCTri Tri2;
	// These are for CreateLineListFromTriInt in BFCEngine
	BFCBool corner1_used;
	BFCBool corner2_used;
	BFCBool corner3_used;
	// tell if corner have been used once in CreateLineListFromTriInt
	BFCBool corner_used_once;

	BFCUInt NumberIntersection;
	// Here are all intersection information
	BFCIntersectInfo IntersectInfo[BFCMaxTriIntersection];
	// line is used to calc intersection
	BFCLine Line;
	// tell the index of the last v1_v2 intersection made in IsTriIntersect
	BFCInt Last_v1_v2_int;
	// t value of the last v1_v2 intersect
	BFCFloat T_v1_v2;


	Init();
	BFCTriIntersect();

	// Reset all intersection info

	BFCVoid Reset();

	// IsTriIntersect fill IntersectType Intersect NumberIntersection and return true if there is an intersection

	BFCBool IsTriIntersect(BFCMesh *actor = NULL);
};

//************************************************************************
//
//						BFCViewPort
//
//************************************************************************

struct BFCViewPortData
{
	friend struct BFCViewPortDataChunk;
	BFCMatrix View;
	BFCMatrix Proj;
	BFCFrustum Frustum;
	BFCFloat Scale;
	// Used as info of absolut rotation
	BFCFloat XRot,YRot;
	// CamPos is effective camera position since in view the pos is there to simulate the camera
	BFCVector CamPos;
	BFCProjectionType Type;
	BFCViewType ViewType;

	BFCViewPortData() :  XRot(0.0f), YRot(0.0f), Scale(1.0f), Type(bfc_perspec_proj), ViewType(bfc_perspec_view)  { }
};

struct BFCRect
{
	BFCUInt left;
	BFCUInt right;
	BFCUInt top;
	BFCUInt down;

	BFCRect() : left(0), right(0), top(0), down(0) { }
};

class BFCViewPort : public BFCMediumBase
{
	friend class BFCGraphicEngine;
	friend struct BFCViewPortChunk;
	friend struct BFCViewPortDataChunk;
	friend class BFCSaveLoad;
	// List of pointer to BFCViewPortData
	BFCVoidList View;
	BFCUInt Num;
	// System Frustum is the initial frustum. I use this becasue ViewPort in directx is not clear
	BFCFrustum SysFrustum;
	BFCRect ActWindowRect;
	BFCUInt ActViewPort;

	// Methods

	// Refresh SysFrustum with the actual system frustum
	BFCVoid RefreshActSysFrustum();

public:

	BFCViewPort();

	AddViewPort();
	DeleteViewPort(BFCUInt n);
	DeleteAllViewPort();
	BFCVoid DefaultInit();
	BFCUInt GetNumViewport();
	BFCMatrix *GetView(BFCUInt n) const;
	BFCMatrix *GetProj(BFCUInt n) const;
	BFCProjectionType GetType(BFCUInt n) const;
	BFCFrustum *GetFrustum(BFCUInt n) const;
	BFCFloat GetScale(BFCUInt n) const;
	BFCVector GetCameraPos(BFCUInt n) const;
	BFCFloat GetXRot(BFCUInt n) const;
	BFCFloat GetYRot(BFCUInt n) const;
	BFCViewType GetViewType(BFCUInt n) const;
	BFCUInt GetActViewPort() const { return ActViewPort; };
	BFCFrustum GetSysFrustum() const { return SysFrustum; }
	BFCRect GetActWindowRect() const { return ActWindowRect; }

	BFCVoid SetView(BFCUInt n, const BFCMatrix *m);
	BFCVoid SetProj(BFCUInt n, const BFCMatrix *m, BFCProjectionType type);
	BFCVoid SetFrustum(BFCUInt n, const BFCFrustum *s);
	BFCVoid SetScale(BFCUInt n, BFCFloat v);
	BFCVoid SetType(BFCUInt n, BFCProjectionType type);
	BFCVoid SetXPos(BFCUInt n, BFCDWord v);
	BFCVoid SetYPos(BFCUInt n, BFCDWord v);
	BFCVoid SetXRot(BFCUInt n, BFCFloat v);
	BFCVoid SetYRot(BFCUInt n, BFCFloat v);
	BFCVoid SetWidth(BFCUInt n, BFCDWord v);
	BFCVoid SetHeight(BFCUInt n, BFCDWord v);
	BFCVoid SetWindowRect(BFCRect &rect) { ActWindowRect = rect; }
	BFCVoid SetActViewPort(BFCUInt n);
	// These two tell the camera to rotate. note : they are absolut not relative and they have an order. Check the code. SpectRe
	BFCVoid SetRotateX(BFCUInt n,BFCFloat angle, BFCBool apply = true);
	BFCVoid SetRotateY(BFCUInt n,BFCFloat angle, BFCBool apply = true);
	BFCVoid SetCameraPosition(BFCUInt n, BFCVector *v);
	BFCVoid SetCameraPosition(BFCUInt n, BFCFloat x, BFCFloat y, BFCFloat z);
	BFCVoid SetViewType(BFCUInt n, BFCViewType view_type);
};

//************************************************************************
//
//						BFCGraphicEngine
//
//************************************************************************

class BFCGraphicEngine : public BFCBigBase
{

#ifdef WIN32
	IDirect3D8 *D3d;
	D3DDISPLAYMODE D3ddm;
	IDirect3DDevice8 *D3dDevice;
#endif

	BFCVoidTreeList Mesh;			// Mesh tree list
	BFCVoidTreeList InterfaceMesh;	// Interface Mesh tree list. To be drawn last
	BFCVoidList DrawList;			// List of element that are drawn and not renderized
	BFCVoidList Light;				// Light list
	BFCColorByte Ambient;
	BFCFloat PointSize;

	friend class BFCMesh;
	friend class BFCDynamicMeshMan;
	friend class BFCLight;
	friend class BFCEngineState;
	friend class BFCSaveLoad;

	BFCInt PickedTri;
	BFCVector PickedPoint; // PickedPoint is local respect to the mesh
	BFCInt PickedVert;
	// It's last T value used in methods like PickVert etc... ( in line intersections ) SpectRe
	BFCFloat LastTValue;

public:

	// Public Data

	BFCViewPort ViewPort;
	// Engine state variable
	BFCEngineState EngineState;
	// Pipeline of the graphic engine

	BFCPipeline *ChangeViewLoop; // Change View Loop level pipelines

	// Construtor

	BFCGraphicEngine();

	// Method

	BFCGEReturn Init(BFCUInt xres,BFCUInt yres);
	// Execute to be called in a while cicle
	BFCVoid Execute();
	BFCGEReturn Close();

	BFCVoid DisableAllTexMesh();
	BFCVoid EnableAllTexMesh();

	BFCGEReturn AddMesh(BFCMesh *mesh);
	BFCGEReturn RemoveMesh(BFCMesh *mesh,BFCBool delete_it = true);

	BFCGEReturn AddLight(BFCLight *light);
	BFCGEReturn RemoveLight(BFCLight *light);

	BFCVoid SetAmbient(BFCColorByte *col);
	BFCVoid SetPointSize(BFCFloat size);
	const BFCColorByte *GetAmbient(BFCColorByte *col) const;
	BFCFloat GetPointSize() const { return PointSize; }
	BFCVoidTreeList *GetMeshTree() { return &Mesh; }

	// Pick Methods

	BFCMesh *PickMesh(BFCInt vp,BFCInt x, BFCInt y);	// vp = viewport number and fîll PickedTri and PickedPoint if return is not null
	BFCMesh *PickVert(BFCInt vp,BFCInt x, BFCInt y);	// vp = viewport number and fill PickedVert return is not null
	const BFCVector *GetLocalPickedPoint() const { return &PickedPoint; }
	BFCUInt GetPickedTri() { return PickedTri; }
	BFCInt GetPickedVert() { return PickedVert; }
	BFCFloat GetLastTValue() { return LastTValue; }

	// ******** Here are usefull function ************

	// border_inside tell if we consider a line on border inside or outside
	BFCLineIntoMesh LineInsideMesh(BFCMesh *mesh, BFCLine *line, BFCBool border_inside = true, BFCFloat prec = 1.0f);
	// tell if the mesh is intersecting the tri
	BFCBool MeshIntTri(BFCMesh *mesh, BFCTri *tri);
	BFCMesh *LineToMesh(BFCLine *line);	// Line to mesh is not finished !!!!!!!!
	BFCVoid WorldToLocal(BFCMatrix *mat, BFCVector *v);
	BFCVoid CameraToLocal(BFCMesh *mesh, BFCInt vp, BFCVector *v);
	BFCVoid CameraToWorld(BFCMesh *mesh, BFCInt vp, BFCVector *v);
	BFCFloat GetCameraZPosFromLocal(BFCMesh *mesh, BFCInt vp, BFCVector &point);	// Get the zpos in camera space
	BFCLine GetCameraLineFromPos(BFCInt vp,BFCInt x, BFCInt y); // Return a line going from picked pos in local camera space
	BFCLine GetLocalLineFromPos(BFCMesh *mesh, BFCInt vp,BFCInt x, BFCInt y); // Return a line going from picked pos in local world of mesh.

	// Transform value taking care of resolution change
	BFCVoid TransWithRes(BFCInt *x,BFCInt *y);



	// Here are morphing function and some function they use.

	// return true if filled v1 and v2 with pointer value of the 2 vector with the v1 and v2 value)
	BFCBool GetPointerFromValue(BFCVector **v1,BFCVector **v2,BFCVoidList *list, BFCFloat prec_factor = 10.0f);
	// It clean the list from parrallel line touching. it has some bug ( not finished )
	BFCVoid PurgeLineList(BFCVoidList *line_list);
	// Create a line list from the tri intersection info. Mainly used in BoolSubsctract. Return true if something has been added, created
	BFCBool CreateLineListFromTriInt(BFCMesh *mesh, BFCTriIntersect *info, BFCVoidList *line_list);
	BFCVoid FillTriListFromLineList(BFCMesh *victim,BFCMesh *actor, BFCVoidList *tri_list, BFCVoidList *line_list);
	BFCVoid BoolSubstract(BFCMesh *victim,BFCMesh *actor);

};

#ifdef WIN32
	extern IDirect3DDevice8 *mD3dDevice;
#endif


#endif
