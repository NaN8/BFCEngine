#include <BFCTexture.h>
#include <stdio.h>
#include <string.h>
#include <BFCGraphicEngine.h>
#include <BFCManager.h>
#include <D3Dx8.h>



//************************************************************************
//
//						BFCTextureData
//
//************************************************************************



BFCReturn BFCTextureData::CreateData(BFCInt id,BFCChar *file_name, BFCTexture *tex,BFCUInt width,BFCUInt height,BFCChar *data)
{
	ActID = id;
	Width = width;
	Height = height;
	DataType = tex->Type;
	strcpy(FileName,file_name);

#ifdef WIN32

	// Create texture in memory
	mD3dDevice->CreateTexture(Width,Height,0,0,tex->D3dFormat,D3DPOOL_SYSTEMMEM,&MemTex);
	// Copy texture Data
	D3DLOCKED_RECT rect;
	MemTex->LockRect(0,&rect,NULL,0);
	memcpy(rect.pBits,data,width*height*4);
	MemTex->UnlockRect(0);
	// Create video tex and copy data to it
	mD3dDevice->CreateTexture(Width,Height,0,0,tex->D3dFormat,D3DPOOL_DEFAULT,&VideoTex);
	mD3dDevice->UpdateTexture(MemTex,VideoTex); // param: source, dest !!!

#endif

	tex->MemoryUsed += width*height*4;

	return bfc_ok;
}

BFCChar *BFCTextureData::GetDataPointer()
{
	D3DLOCKED_RECT rect;
	MemTex->LockRect(0,&rect,NULL,0);
	return (BFCChar *)rect.pBits;
}

BFCReturn BFCTextureData::DataSet()
{
	MemTex->UnlockRect(0);
	return bfc_ok;
}


//************************************************************************
//
//						BFCTexture
//
//************************************************************************


BFCUInt BFCTexture::MemoryUsed = 0;
BFCVoidList BFCTexture::TextureData;
BFCUInt BFCTexture::MaxMemory = 8388608;


BFCTexture::BFCTexture(BFCTextureType type) : ID(-1), MyData(NULL),Type(type)	// -1 no texture loaded
{
	switch (Type)
	{
	case bfc_rgba_tex:
		{
#ifdef WIN32
			D3dFormat = D3DFMT_A8R8G8B8;
#endif
		} break;
	}
}

BFCReturn BFCTexture::LoadRawFile(BFCChar *name)
{
	FILE *fp;
	BFCUInt size;
	BFCUInt res;	// resolution
	BFCChar *data;	// pointer to loaded data
	BFCChar *data2;	// pointer to transformed data
	BFCTextureData *tex_data;

	// check file size and resolution
	fp = fopen(name,"rb");
	fseek(fp,0,SEEK_END);
	size = ftell(fp);
	fseek(fp,0,SEEK_SET);
	res = size/3;
	res = (BFCUInt)sqrt(res);

	data = new BFCChar[size];
	data2 = new BFCChar[res*res*4];
	fread(data,1,size,fp);

	// convert from rgb to rgba if we need it

	BFCUInt i=0;
	BFCUInt j=0;
	while (i<size)
	{
		data2[j] = data[i];
		data2[j+1] = data[i+1];
		data2[j+2] = data[i+2];
		data2[j+3] = (char)255;
		i+=3;
		j+=4;
	}

	
	// Check texture validity and assign memory for the texture

	ID = mManager->CalcKindCRC(data,size);
	// Check if the texture already exist in the memory
	BFCTextureData *_check_data = (BFCTextureData *)TextureData.GetFirst();
	BFCBool _data_exist = false;
	while (_check_data)
	{
		if (_check_data->ActID == ID)
		{
			_data_exist = true;
			break;
		}
		_check_data = (BFCTextureData *)TextureData.GetNext();
	}
	if (_data_exist)
	{
		// assign already existant texture data to texture
		MyData = _check_data;
	} else
	{
		// check string length
		if (strlen(name)<TEX_FILE_MAX)
		{
			tex_data = new BFCTextureData();
			// Add the texture to the list
			TextureData.Add(tex_data);
			MyData = tex_data;
			// Assign memory for the texture
			tex_data->CreateData( ID, name, this,res,res,data2 );
		} else
		{
			BFCAssert(0,"Texture file name too long");
			return bfc_arg_too_long;
		}
	}

	delete [] data;
	delete [] data2;
	fclose(fp);

	return bfc_ok;
}

BFCTextureData *BFCTexture::Find(BFCInt ID)
{
	// Check if the texture already exist in the memory
	BFCTextureData *_check_data = (BFCTextureData *)TextureData.GetFirst();
	while (_check_data)
	{
		if (_check_data->ActID == ID)
		{
			return _check_data;
		}
		_check_data = (BFCTextureData *)TextureData.GetNext();
	}
	return NULL;
}