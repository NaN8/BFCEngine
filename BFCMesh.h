#ifndef __BFCMESH__H
#define __BFCMESH__H

#ifdef WIN32
#include <D3d8.h>
#endif

#include <BFCUtility.h>
#include <BFCMath.h>
#include <BFCPipeline.h>
#include <BFCTexture.h>
#include <BFCLight.h>


class BFCMesh;	// forward declaration


enum BFCMeshType
{
	lit_t_tri_mesh = 0,				// lit transformed triangle mesh
	lit_ut_tri_mesh = 1,			// lit untransformed triangle mesh
	unlit_ut_tri_mesh = 2,			// unlit untransformed triangle mesh
	unlit_tex_ut_tri_mesh = 3,		// unlit textured untransformed triangle mesh
	unlit_tex_ut_i_tri_mesh = 4,	// unlit textured untransformed indexed triangle mesh
	lit_t_line_mesh = 5,			// lit transformed line mesh
	lit_ut_line_mesh = 6,			// lit untransformed line mesh
	lit_ut_i_line_mesh = 7,			// lit untransformed indexed line mesh
	lit_ut_point_mesh = 8			// lit untransformed point mesh
};

enum BFCMeshType2
{
	normal_mesh = 0,
	interface_mesh = 1,
};

// Return error of mesh methods

enum BFCGMReturn
{
	g_m_error,
	g_m_already_locked,
	g_m_already_unlocked,
	g_m_ok
};

//************************************************************************
//
//			List of Mesh data Structure, can be very different between platform
//
//************************************************************************


#ifdef WIN32

const BFCDWord D3D_LIT_T = (D3DFVF_XYZRHW|D3DFVF_DIFFUSE);
struct Win32_lit_t_mesh
{
    BFCVector pos; 			// The transformed position for the vertex.
	BFCFloat rhw;
    BFCUInt  color;			// The vertex color.
};

const BFCDWord D3D_LIT_UT = (D3DFVF_XYZ|D3DFVF_DIFFUSE);
struct Win32_lit_ut_mesh
{
    BFCVector pos;			// The untransformed position for the vertex.
    BFCUInt  color;			// The vertex color.
};

const BFCDWord D3D_UNLIT_UT = (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE);
struct Win32_unlit_ut_mesh
{
    BFCVector pos;			// The untransformed position for the vertex.
	BFCVector normal;		// The normal of the vertex
    BFCUInt   color;		// The vertex color.
};

const BFCDWord D3D_UNLIT_TEX_UT = (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1);
struct Win32_unlit_tex_ut_mesh
{
    BFCVector pos;			// The untransformed position for the vertex.
	BFCVector normal;		// The normal of the vertex
    BFCUInt   color;		// The vertex color.
	BFCFloat u,v;			// Texture coord
};

//  Here is the structure (vertex buffer descriptor) for the table (check in cpp file for info for the table)

struct vb_desc
{
	BFCDWord format;				// the flexible vertex format id
	size_t size;					// size of data unit
	BFCVoid (BFCMesh::*method)();	// pointer to the method
	BFCUInt NumDataPerPrim;			// Number of Data per primitive
};  

#endif


class BFCMaterial
{

public:

	BFCColor Ambient;
	BFCColor Diffuse;
	BFCColor Specular;

	BFCMaterial() {};

};

//********************************************
//
//			Struct BFCTextureIndex
//
//********************************************


struct BFCTextureIndex
{
	BFCTexture *Tex;
	// First index used
	BFCUInt Index;
	// Last index used
	BFCUInt LastIndex;
	// Number of prim used
	//BFCUInt Number;

	BFCTextureIndex() : Tex(NULL), Index(0), LastIndex(0)/*, Number(0)*/ {}
};


//************************************************************************
//
//			DynamicMeshMan : Class to manage dynamic mesh
//
//************************************************************************


class BFCDynamicMeshMan
{

	BFCVoid *Data;			//	Usually Vertex,Normal etc...
	BFCVoid *Data2;			//  Usually Index...
	BFCUInt	NumPrim;		// Number of primitives to be drawn
	BFCBool Locked;			// Is the mesh data locked	
	BFCBool Locked2;		// Is the mesh data 2 locked
	BFCUInt NumData;
	BFCUInt NumData2;
	BFCMesh *MyMesh;
	

#ifdef WIN32
	IDirect3DVertexBuffer8 *Vb;   // Vertex Buffer ( Normally created in GetDataPointer )
	IDirect3DIndexBuffer8 *Index; // Index Buffer ( Normally created in GetDataPointer2 )
#endif

public:

	// p Pointer to the mesh to manage
	BFCDynamicMeshMan(BFCMesh *p);
	BFCVoid CleanData();
	BFCVoid CleanData2();
	BFCVoid SetNumData(BFCUInt n) { CleanData(); NumData = n; }	// One of the first thing we must do is setting number of data (usualy vertex)
	BFCVoid SetNumData2(BFCUInt n) { CleanData2(); NumData2 = n; }
	BFCVoid *GetDataPointer();				        // Then we can get data pointer to fill it. Use this only for filling data!!
	BFCVoid *GetDataPointer2();				        // Then we can get data pointer 2 ( if we need it also. Normally it's index data )
	BFCGMReturn DataSet();							// Tells that data has been filled and is ready ( usually vertex data)
	BFCGMReturn Data2Set();							// Tells that data 2 has been filled and is ready ( usually index data)

	BFCVoid SetNewData();

};


//************************************************************************
//
//						BFCMesh
//
//************************************************************************


class BFCMesh : public BFCVoidTreeNode, public BFCPipeline
{

protected:

	BFCMeshType Type;
	BFCMeshType2 Type2;
	BFCVoid *Data;			//	Usually Vertex,Normal etc...
	BFCVoid *Data2;			//  Usually Index...
	BFCUInt	NumPrim;		// Number of primitives to be drawn
	BFCBool Locked;			// Is the mesh data locked	
	BFCBool Locked2;		// Is the mesh data 2 locked
	BFCBool Enabled;

	BFCMaterial Material;

#ifdef WIN32
	IDirect3DVertexBuffer8 *Vb;   // Vertex Buffer ( Normally created in GetDataPointer )
	IDirect3DIndexBuffer8 *Index; // Index Buffer ( Normally created in GetDataPointer2 )
#endif

	friend class BFCGraphicEngine;
	friend class BFCDynamicMeshMan;
	friend class BFCSaveLoad;
	friend struct BFCMeshChunk;

	BFCUInt NumData;
	BFCUInt NumData2;
	BFCVector Scale;		// Scaling of the mesh

		// Rotation info

	BFCFloat RotX;
	BFCFloat RotY;
	BFCFloat RotZ;

	
	BFCVoidList Tex;			// List of texture with index of the mesh

public:

	BFCMatrix M;
	// Used to changed dynamicly the mesh
	BFCDynamicMeshMan *DynamicMan;

	// Destructor

	~BFCMesh();

	// Constructor
	// BFCMesh mesh constructor normally need type of mesh
	BFCMesh(BFCMeshType type,BFCChar *name);

	// Method

	// index is the vert or index number
	// For an editor InsertTexture return one of the texture replaced
	BFCTexture *InsertTexture(BFCTexture *tex,BFCUInt index, BFCUInt lastindex = 0);
	BFCVoid CleanData();
	BFCVoid CleanData2();
	BFCVoid SetNumData(BFCUInt n) { CleanData(); NumData = n; }	// One of the first thing we must do is setting number of data (usualy vertex)
	BFCVoid SetNumData2(BFCUInt n) { CleanData2(); NumData2 = n; }
	BFCVoid *GetDataPointer();				        // Then we can get data pointer to fill it. Use this only for filling data!!
	BFCVoid *GetDataPointer2();				        // Then we can get data pointer 2 ( if we need it also. Normally it's index data )
	BFCGMReturn DataSet();							// Tells that data has been filled and is ready ( usually vertex data)
	BFCGMReturn Data2Set();							// Tells that data 2 has been filled and is ready ( usually index data)


	BFCBool GetLocked() { return Locked; } const
	BFCBool GetLocked2() { return Locked2; } const
	BFCUInt GetNumData() { return NumData; } const
	BFCUInt GetNumData2() { return NumData2; } const
	BFCMeshType GetType() { return Type; }
	BFCVector GetScale()  { return Scale; }
	Win32_unlit_tex_ut_mesh GetMeshData(BFCUInt n);
	Win32_lit_ut_mesh GetMeshLitData(BFCUInt n);

	BFCVoid SetMeshData(BFCUInt n, Win32_unlit_tex_ut_mesh *data);
	BFCVoid SetMeshLitData(BFCUInt n, Win32_lit_ut_mesh *data);
	BFCVoid ScaleIt(BFCFloat scalex, BFCFloat scaley, BFCFloat scalez);
	BFCVoid SetScale(BFCFloat scalex, BFCFloat scaley, BFCFloat scalez);
	BFCVoid SetScale(BFCVector scale);
	BFCVoid SetType2(BFCMeshType2 type) { Type2 = type; }
	BFCVoid SetFaceColor(BFCUInt tri_n,BFCUInt color);

	BFCVoid Enable() { Enabled = true; }
	BFCVoid Disable() { Enabled = false; }

	// Rotation method

	BFCVoid RotateX(BFCFloat ang);
	BFCVoid RotateY(BFCFloat ang);
	BFCVoid RotateZ(BFCFloat ang);

	//Here are some display routine code

	BFCVoid CommonLightSetting();
	BFCVoid CommonTexSetting(BFCTexture *tex);
	BFCVoid CommonNonTexLightSetting();

	BFCVoid	DrawLitTTriMesh();
	BFCVoid	DrawLitUTriMesh();
	BFCVoid	DrawUnLitUTriMesh();
	BFCVoid	DrawUnLitTexUTriMesh();
	BFCVoid	DrawUnLitTexUITriMesh();
	BFCVoid	DrawLitTLineMesh();
	BFCVoid	DrawLitUTLineMesh();
	BFCVoid	DrawLitUTILineMesh();
	BFCVoid	DrawLitUTPointMesh();

};









#endif