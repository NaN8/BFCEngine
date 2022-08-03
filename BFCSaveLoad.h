#ifndef __BFCSAVELOAD__H
#define __BFCSAVELOAD__H

#include <BFCUtility.h>
#include <BFCGraphicEngine.h>


//==========================================================================================
//
//					Don't forget to use as less as posible method when you set data,
//					since you don't know what the method can do.
//					If a "SetOnly" method is available you can use it without problem
//
//
//
//==========================================================================================




const BFCFloat BFCMaxVersionNumber = 10000.0f;
const BFCFloat BFCActualSceneVersion = 0.1f;
const BFCUInt BFCSceneType = 55;

//***************************************************************************************
//
//					!! Don't forget to write chunk type before any chunk !!
//
//***************************************************************************************


// forward declaration

class BFCSaveLoad;

enum BFCChunkType
{
	bfc_file_chunk = 0,
	bfc_viewport_chunk,
	bfc_viewport_data_chunk,
	bfc_mesh_chunk,
	bfc_mesh_tex_index_chunk,
	bfc_texture_chunk
};

// Remember struct or class with pure virtual function cannot be of size 0. SpectRe

struct BFCChunk
{
	virtual BFCVoid Write(BFCSaveLoad *save_load) = 0;
	virtual BFCVoid Read(BFCSaveLoad *save_load);
	virtual BFCVoid Read_0_1(BFCSaveLoad *save_load) = 0;
	virtual BFCVoid AddIt(BFCVoid *p) = 0;
};

struct BFCFileChunk : BFCChunk
{
	// Version number
	BFCUInt Version;
	// Version number after the point
	BFCUInt DecimalVersion;
	BFCUInt FileType;

	// Methods

	BFCVoid Write(BFCSaveLoad *save_load);
	// Read version 0.1
	BFCVoid Read_0_1(BFCSaveLoad *save_load);
	BFCVoid AddIt(BFCVoid *p) { }
};

struct BFCViewPortChunk : BFCChunk
{
	BFCUInt Num;
	// System Frustum is the initial frustum. I use this becasue ViewPort in directx is not clear
	BFCFrustum SysFrustum;
	BFCRect ActWindowRect;
	// Actual view info
	BFCUInt ActViewPort;
	BFCFloat XRot,YRot;
	BFCVector CamPos;


	// Methods

	BFCVoid Write(BFCSaveLoad *save_load);
	// Read version 0.1
	BFCVoid Read_0_1(BFCSaveLoad *save_load);
	BFCVoid Fill(BFCViewPort *data);
	BFCVoid AddIt(BFCVoid *p);
};

struct BFCViewPortDataChunk : BFCChunk
{
	BFCMatrix View;
	BFCMatrix Proj;
	BFCFrustum Frustum;
	BFCFloat Scale;
	// Used as info of absolut rotation
	BFCFloat XRot,YRot;
	// CamPos is effective camera position since in view the pos is there to simulate the camera
	BFCVector CamPos;
	BFCProjectionType ProjectionType;
	BFCViewType ViewType;

	// Methods

	BFCVoid Write(BFCSaveLoad *save_load);
	// Read version 0.1
	BFCVoid Read_0_1(BFCSaveLoad *save_load);
	BFCVoid Fill(BFCViewPortData *data);
	BFCVoid AddIt(BFCVoid *p);
};

struct BFCMeshChunk : BFCChunk
{
	BFCMeshType MeshType;
	BFCUInt NumData;
	BFCUInt NumData2;
	// Scaling of the mesh
	BFCVector Scale;
	// Rotation info
	BFCFloat RotX;
	BFCFloat RotY;
	BFCFloat RotZ;
	BFCMatrix M;
	BFCChar Name[32];
	//*********************
	BFCUInt NumTex;

	// Methods

	BFCVoid Write(BFCSaveLoad *save_load);
	// Read version 0.1
	BFCVoid Read_0_1(BFCSaveLoad *save_load);
	BFCVoid Fill(BFCMesh *data);
	BFCVoid AddIt(BFCVoid *p);
};

struct BFCMeshTexIndexChunk : BFCChunk
{
	// First index used
	BFCUInt Index;
	// Last index used
	BFCUInt LastIndex;
	// The texture ID
	BFCInt TexID;

	// Methods

	BFCVoid Write(BFCSaveLoad *save_load);
	// Read version 0.1
	BFCVoid Read_0_1(BFCSaveLoad *save_load);
	BFCVoid Fill(BFCTextureIndex *data);
	BFCVoid AddIt(BFCVoid *p);
};

// Don't forget to write texture data after this chunk

struct BFCTextureChunk : BFCChunk
{
	BFCUInt Width;
	BFCUInt Height;
	BFCChar FileName[TEX_FILE_MAX];		// Texture File Name
	BFCTextureType DataType;
	BFCInt ID;							// The texture ID (CRC)
	// Num of byte of the texture data following this chunk
	BFCUInt DataSize;

	// Methods

	BFCVoid Write(BFCSaveLoad *save_load);
	// Read version 0.1
	BFCVoid Read_0_1(BFCSaveLoad *save_load);
	BFCVoid Fill(BFCTextureData *data);
	BFCVoid AddIt(BFCVoid *p);
};

//*************************************************************************
//
//
//						BFCSaveLoad
//
//
//*************************************************************************



class BFCSaveLoad
{

	BFCFloat VersionLoading;
	BFCUInt FileTypeLoading;
	FILE *ActFilePointer;
	friend struct BFCFileChunk;

	BFCReturn OpenSave(BFCChar *name);
	BFCReturn CloseSave();
	BFCReturn OpenLoad(BFCChar *name);
	BFCReturn CloseLoad();

public:

	BFCSaveLoad();

	BFCReturn Load(BFCChar *name);
	BFCReturn WriteChunkType(BFCChunkType type, BFCSaveLoad *save_load);
	BFCReturn WriteScene(BFCChar *name);
	

	FILE *GetActFilePointer() { return ActFilePointer; }
	BFCFloat GetVersionLoading() { return VersionLoading; }
	BFCUInt GetFileTypeLoading() { return FileTypeLoading; }
};





#endif