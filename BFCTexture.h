#ifndef __BFCTEXTURE__H
#define __BFCTEXTURE__H

#include <BFCUtility.h>
#include <D3d8.h>


const BFCUInt TEX_FILE_MAX = 256;

class BFCTexture;  // Forward declaration

enum BFCTextureType
{
	bfc_rgba_tex = 0,
};

class BFCTextureData : public BFCVoidNode
{
	friend class BFCMesh;
	friend class BFCTexture;
	friend struct BFCTextureChunk;
	friend struct BFCMeshTexIndexChunk;

	BFCInt  ActID;		// Actual texture CRC loaded
	BFCUInt Width;
	BFCUInt Height;
	BFCChar FileName[TEX_FILE_MAX];		// Texture File (in memory or disk ) Name loaded
#ifdef WIN32
	IDirect3DTexture8 *MemTex;			//Texture in main memory
	IDirect3DTexture8 *VideoTex;		//Texture in video memory
#endif
	BFCTextureType DataType;

public:

	BFCTextureData() : Width(0), Height(0), ActID(-1) // -1 no texture loaded
	{
#ifdef WIN32
		MemTex = NULL;
		VideoTex = NULL;
#endif
	};

	BFCReturn CreateData(BFCInt id,BFCChar *file_name, BFCTexture *tex,BFCUInt width,BFCUInt height,BFCChar *data);
	BFCChar *GetDataPointer();
	BFCReturn DataSet();
};


//************************************************************************
//
//						BFCTexture
//
//************************************************************************

class BFCTexture : public BFCMediumBase
{
	friend class BFCMesh;
	friend class BFCTextureData;
	friend struct BFCTextureChunk;
	friend struct BFCMeshTexIndexChunk;

	static BFCVoidList TextureData;		// This is the list of all texture data (not clipped)
	static BFCUInt MemoryUsed;			// This is the memory used by all texture
	static BFCUInt MaxMemory;			// The maximum video memory all texture can have
	BFCTextureData *MyData;				// Pointer to texture data
	BFCInt ID;							// The texture ID
	BFCTextureType Type;

#ifdef WIN32
	D3DFORMAT D3dFormat;
#endif


public:

	BFCTexture(BFCTextureType type);
	BFCReturn LoadRawFile(BFCChar *name);		// Raw file must always be rgb and square
	BFCVoid SetMaxMemory(BFCInt v) { MaxMemory = v; }
	BFCBool TextureLoaded() { if (ID == -1) return false; else return true; }
	static BFCVoidList *GetTextureDataList() { return &TextureData; }
	static BFCTextureData *Find(BFCInt ID);

};






#endif