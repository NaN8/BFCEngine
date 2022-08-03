#ifndef __ED_PRIMITIVES__H
#define __ED_PRIMITIVES__H

#include <BFCManager.h>

// Type of index prim, angle can be not shared to avoid problem such as perfect mapping not possible

enum BFCTypePrimGen
{
	bfc_angle_shared,
	bfc_angle_not_shared
};


//************************************************************************
//
//						BFCIPrimitive ( Indexed Primitive)
//
//************************************************************************


class BFCIPrimitive : public BFCMesh
{

protected:

	BFCTypePrimGen TypeGen;
	BFCWord *Index;
	Win32_unlit_tex_ut_mesh *Vert;
	// Point data and Index data
	Win32_lit_ut_mesh *PointData;
	BFCWord *PointIndex;

	// The editing line mesh
	BFCMesh *LineMesh;

	BFCUInt PointColor;	// Color of the prim
	friend class MainEditor;

public:

	BFCIPrimitive(BFCFloat scalex,BFCFloat scaley,BFCFloat scalez,BFCUInt NumData,BFCUInt NumData2,BFCChar *name, BFCTypePrimGen type_gen = bfc_angle_shared);
	~BFCIPrimitive();
	BFCVoid CalcNormal();	// note calc normal is already called in FinishCon
	FinishCon();	// Call FinishCon to finish the construction of the primitive
	BFCVoid DrawSquare(BFCUInt *mem,BFCUInt x,BFCUInt y,BFCUInt color,BFCUInt pitch=0,BFCUInt size = 1);
	BFCVoid DrawPoint();	// Draw Points method
	BFCVoid DrawLine();		// Draw Line method

	BFCVoid DettachTriangle();
	// Color for all the mesh
	BFCVoid SetPointColor(BFCUInt col) { PointColor = col; }
	// Color for a vertex only
	BFCVoid SetPointColor(BFCUInt col,BFCUInt vertn) { PointData[PointIndex[vertn]].color = col; }
	// this is an absolute scale
	BFCVoid SetScale(BFCVector scale);
	// this is a relative scale
	BFCVoid ScaleIt(BFCFloat x, BFCFloat y, BFCFloat z);
};

//************************************************************************
//
//						BFCPrimitive
//
//************************************************************************


class BFCPrimitive : public BFCMesh
{

protected:

	Win32_unlit_tex_ut_mesh *Vert;
	// Point data
	Win32_lit_ut_mesh *PointData;

	BFCUInt PointColor;	// Color of the prim
	friend class MainEditor;		

public:

	BFCPrimitive(BFCFloat scalex,BFCFloat scaley,BFCFloat scalez,BFCUInt NumData,BFCChar *name);
	~BFCPrimitive();
	BFCVoid CalcNormal();	// note calc normal is already called in FinishCon
	FinishCon();	// Call FinishCon to finish the construction of the primitive
	BFCVoid DrawSquare(BFCUInt *mem,BFCUInt x,BFCUInt y,BFCUInt color,BFCUInt pitch=0,BFCUInt size = 1);
	BFCVoid DrawPoint();	// Draw Points method

	BFCVoid SetPointColor(BFCUInt col) { PointColor = col; }
	BFCVoid SetPointColor(BFCUInt col,BFCUInt vertn) { PointData[vertn].color = col; }
	BFCVoid SetScale(BFCVector scale);
};


//************************************************************************
//
//			BFCICube ( Indexed Cube : not perfect for mapping )
//
//************************************************************************


class BFCICube : public BFCIPrimitive
{

public:

	BFCICube(BFCFloat scalex,BFCFloat scaley,BFCFloat scalez, BFCTypePrimGen type_gen = bfc_angle_shared);

};

//************************************************************************
//
//			BFCIPyramid 
//
//************************************************************************


class BFCIPyramid : public BFCIPrimitive
{

public:

	BFCIPyramid(BFCFloat scalex,BFCFloat scaley,BFCFloat scalez, BFCTypePrimGen type_gen = bfc_angle_shared);

};

//************************************************************************
//
//			BFCISphere ( Indexed Sphere )
//
//************************************************************************

enum BFCMappingSphere
{
	bfc_linear_map = 0,
	bfc_double_map = 1
};

class BFCISphere : public BFCIPrimitive
{

	// Detail definition
	BFCUInt Def;
	BFCMappingSphere MapStyle;

public:

	BFCISphere(BFCFloat scalex,BFCFloat scaley,BFCFloat scalez, BFCUInt def);
	BFCVoid SetMappingStyle(BFCMappingSphere map);

};

//************************************************************************
//
//						BFCCube
//
//************************************************************************


class BFCCube : public BFCPrimitive
{

public:

	BFCCube(BFCFloat scalex,BFCFloat scaley,BFCFloat scalez);

};


//************************************************************************
//
//						BFCGrid
//
//************************************************************************


class BFCGrid : public BFCMesh
{

	BFCInt Size;
	BFCInt Def;

public:

	BFCGrid(BFCInt def, BFCInt size);
	BFCVoid SetGridForView();

	BFCInt GetSize() { return Size; };
	BFCInt GetDef() { return Def; };


};



#endif