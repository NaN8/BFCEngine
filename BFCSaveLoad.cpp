#include <BFCSaveLoad.h>
#include <BFCManager.h>
#include <Primitives.h>
#include <EdMain.h>



//*************************************************************************
//
//
//						BFCChunk
//
//
//*************************************************************************

BFCVoid BFCChunk::Read(BFCSaveLoad *save_load)
{
	if (save_load->GetVersionLoading() >= 0.1f)
	{
		Read_0_1(save_load);
	}
}


//*************************************************************************
//
//
//						BFCFileChunk
//
//
//*************************************************************************


BFCVoid BFCFileChunk::Write(BFCSaveLoad *save_load)
{
	fwrite(&Version,sizeof(Version),1,save_load->GetActFilePointer());
	fwrite(&DecimalVersion,sizeof(DecimalVersion),1,save_load->GetActFilePointer());
	fwrite(&FileType,sizeof(FileType),1,save_load->GetActFilePointer());
}

BFCVoid BFCFileChunk::Read_0_1(BFCSaveLoad *save_load)
{
	fread(&Version,sizeof(Version),1,save_load->GetActFilePointer());
	fread(&DecimalVersion,sizeof(DecimalVersion),1,save_load->GetActFilePointer());
	fread(&FileType,sizeof(FileType),1,save_load->GetActFilePointer());
	// Here we calculate the version in float
	save_load->VersionLoading = (float)Version + (float)DecimalVersion / BFCMaxVersionNumber;
	save_load->FileTypeLoading = FileType;	
}

//*************************************************************************
//
//
//						BFCViewPortChunk
//
//
//*************************************************************************


BFCVoid BFCViewPortChunk::Write(BFCSaveLoad *save_load)
{
	fwrite(&Num,sizeof(Num),1,save_load->GetActFilePointer());
	fwrite(&SysFrustum,sizeof(SysFrustum),1,save_load->GetActFilePointer());
	fwrite(&ActWindowRect,sizeof(ActWindowRect),1,save_load->GetActFilePointer());
	fwrite(&ActViewPort,sizeof(ActViewPort),1,save_load->GetActFilePointer());
	fwrite(&XRot,sizeof(XRot),1,save_load->GetActFilePointer());
	fwrite(&YRot,sizeof(YRot),1,save_load->GetActFilePointer());
	fwrite(&CamPos,sizeof(CamPos),1,save_load->GetActFilePointer());
}

BFCVoid BFCViewPortChunk::Read_0_1(BFCSaveLoad *save_load)
{
	fread(&Num,sizeof(Num),1,save_load->GetActFilePointer());
	fread(&SysFrustum,sizeof(SysFrustum),1,save_load->GetActFilePointer());
	fread(&ActWindowRect,sizeof(ActWindowRect),1,save_load->GetActFilePointer());
	fread(&ActViewPort,sizeof(ActViewPort),1,save_load->GetActFilePointer());
	fread(&XRot,sizeof(XRot),1,save_load->GetActFilePointer());
	fread(&YRot,sizeof(YRot),1,save_load->GetActFilePointer());
	fread(&CamPos,sizeof(CamPos),1,save_load->GetActFilePointer());
}

BFCVoid BFCViewPortChunk::Fill(BFCViewPort *data)
{
	SysFrustum = data->SysFrustum;
	ActWindowRect = data->ActWindowRect;
	ActViewPort = data->ActViewPort;
	ActViewPort = mMainEditor->ActViewPort;
	XRot = mMainEditor->XRot;
	YRot = mMainEditor->YRot;
	CamPos = mMainEditor->CamPos;
}

BFCVoid BFCViewPortChunk::AddIt(BFCVoid *p)
{
	mGraphicEngine->ViewPort.SysFrustum = SysFrustum;
	mGraphicEngine->ViewPort.ActWindowRect = ActWindowRect;
	mGraphicEngine->ViewPort.ActViewPort = ActViewPort;
	mMainEditor->ActViewPort = ActViewPort;
	mMainEditor->XRot = XRot;
	mMainEditor->YRot = YRot;
	mMainEditor->CamPos = CamPos;
}

//*************************************************************************
//
//
//						BFCViewPortDataChunk
//
//
//*************************************************************************


BFCVoid BFCViewPortDataChunk::Write(BFCSaveLoad *save_load)
{
	fwrite(&View,sizeof(View),1,save_load->GetActFilePointer());
	fwrite(&Proj,sizeof(Proj),1,save_load->GetActFilePointer());
	fwrite(&Frustum,sizeof(Frustum),1,save_load->GetActFilePointer());
	fwrite(&Scale,sizeof(Scale),1,save_load->GetActFilePointer());
	fwrite(&XRot,sizeof(XRot),1,save_load->GetActFilePointer());
	fwrite(&YRot,sizeof(YRot),1,save_load->GetActFilePointer());
	fwrite(&CamPos,sizeof(CamPos),1,save_load->GetActFilePointer());
	fwrite(&ProjectionType,sizeof(ProjectionType),1,save_load->GetActFilePointer());
	fwrite(&ViewType,sizeof(ViewType),1,save_load->GetActFilePointer());
}

BFCVoid BFCViewPortDataChunk::Read_0_1(BFCSaveLoad *save_load)
{
	fread(&View,sizeof(View),1,save_load->GetActFilePointer());
	fread(&Proj,sizeof(Proj),1,save_load->GetActFilePointer());
	fread(&Frustum,sizeof(Frustum),1,save_load->GetActFilePointer());
	fread(&Scale,sizeof(Scale),1,save_load->GetActFilePointer());
	fread(&XRot,sizeof(XRot),1,save_load->GetActFilePointer());
	fread(&YRot,sizeof(YRot),1,save_load->GetActFilePointer());
	fread(&CamPos,sizeof(CamPos),1,save_load->GetActFilePointer());
	fread(&ProjectionType,sizeof(ProjectionType),1,save_load->GetActFilePointer());
	fread(&ViewType,sizeof(ViewType),1,save_load->GetActFilePointer());
}

BFCVoid BFCViewPortDataChunk::Fill(BFCViewPortData *data)
{
	View = data->View;
	Proj = data->Proj;
	Frustum = data->Frustum;
	Scale = data->Scale;
	XRot = data->XRot;
	YRot = data->YRot;
	CamPos = data->CamPos;
	ProjectionType = data->Type;
	ViewType = data->ViewType;
}

BFCVoid BFCViewPortDataChunk::AddIt(BFCVoid *p)
{
	BFCUInt view_number = mGraphicEngine->ViewPort.GetNumViewport();
	mGraphicEngine->ViewPort.AddViewPort();
	BFCViewPortData *data = (BFCViewPortData *) mGraphicEngine->ViewPort.View.FindPtr(view_number);
	data->View = View;
	data->Proj = Proj;
	data->Frustum = Frustum;
	data->XRot = XRot;
	data->YRot = YRot;
	data->CamPos = CamPos;
	data->Type = ProjectionType;
	data->ViewType = ViewType;
	data->Scale = Scale;
}


//*************************************************************************
//
//
//						BFCTextureChunk
//
//
//*************************************************************************


BFCVoid BFCTextureChunk::Write(BFCSaveLoad *save_load)
{
	fwrite(&Width,sizeof(Width),1,save_load->GetActFilePointer());
	fwrite(&Height,sizeof(Height),1,save_load->GetActFilePointer());
	fwrite(&FileName,sizeof(FileName),1,save_load->GetActFilePointer());
	fwrite(&DataType,sizeof(DataType),1,save_load->GetActFilePointer());
	fwrite(&ID,sizeof(ID),1,save_load->GetActFilePointer());
	fwrite(&DataSize,sizeof(DataSize),1,save_load->GetActFilePointer());
}

BFCVoid BFCTextureChunk::Read_0_1(BFCSaveLoad *save_load)
{
	fread(&Width,sizeof(Width),1,save_load->GetActFilePointer());
	fread(&Height,sizeof(Height),1,save_load->GetActFilePointer());
	fread(&FileName,sizeof(FileName),1,save_load->GetActFilePointer());
	fread(&DataType,sizeof(DataType),1,save_load->GetActFilePointer());
	fread(&ID,sizeof(ID),1,save_load->GetActFilePointer());
	fread(&DataSize,sizeof(DataSize),1,save_load->GetActFilePointer());
}

BFCVoid BFCTextureChunk::Fill(BFCTextureData *data)
{
	Width = data->Width;
	Height = data->Height;
	strcpy(FileName,data->FileName);
	DataType = data->DataType;
	ID = data->ActID;
	DataSize = Width * Height * 4;
}

BFCVoid BFCTextureChunk::AddIt(BFCVoid *p)
{
	BFCChar *data;
	BFCSaveLoad *save_load = (BFCSaveLoad *) p;
	// Alloc data array
	data = new BFCChar[DataSize];
	// Read tex data from file
	fread(data,DataSize,1,save_load->GetActFilePointer());

	BFCTexture *tex = new BFCTexture(DataType);
	tex->ID = ID;
	
	BFCTextureData *tex_data = new BFCTextureData();
	// Add the texture to the list
	tex->TextureData.Add(tex_data);
	tex->MyData = tex_data;
	// Assign memory for the texture
	tex_data->CreateData( ID, FileName, tex,Width,Height,data);


	// we don't need the texture for the moment, we just need the data, so we can delete it
	delete tex;
	delete [] data;
}

//*************************************************************************
//
//
//						BFCMeshTexIndexChunk
//
//
//*************************************************************************

BFCVoid BFCMeshTexIndexChunk::Write(BFCSaveLoad *save_load)
{
	fwrite(&Index,sizeof(Index),1,save_load->GetActFilePointer());
	fwrite(&LastIndex,sizeof(LastIndex),1,save_load->GetActFilePointer());
	fwrite(&TexID,sizeof(TexID),1,save_load->GetActFilePointer());
}

BFCVoid BFCMeshTexIndexChunk::Read_0_1(BFCSaveLoad *save_load)
{
	fread(&Index,sizeof(Index),1,save_load->GetActFilePointer());
	fread(&LastIndex,sizeof(LastIndex),1,save_load->GetActFilePointer());
	fread(&TexID,sizeof(TexID),1,save_load->GetActFilePointer());
}

BFCVoid BFCMeshTexIndexChunk::Fill(BFCTextureIndex *data)
{
	Index = data->Index;
	LastIndex = data->LastIndex;
	TexID = data->Tex->ID;
}

BFCVoid BFCMeshTexIndexChunk::AddIt(BFCVoid *p)
{
	BFCMesh *mesh = (BFCMesh *) p;
	// we find the texure data
	BFCTextureData *tex_data = BFCTexture::Find(TexID);
	BFCAssert(tex_data,"Unexpected error. \nMesh index a texture missing");
	// we create a new texture pointer
	BFCTexture *tex = new BFCTexture(tex_data->DataType);
	// we fill it
	tex->ID = TexID;
	tex->MyData = tex_data;
	// and we add it
	mesh->InsertTexture(tex,Index,LastIndex);
}

//*************************************************************************
//
//
//						BFCMeshChunk
//
//
//*************************************************************************

BFCVoid BFCMeshChunk::Write(BFCSaveLoad *save_load)
{
	fwrite(&MeshType,sizeof(MeshType),1,save_load->GetActFilePointer());
	fwrite(&NumData,sizeof(NumData),1,save_load->GetActFilePointer());
	fwrite(&NumData2,sizeof(NumData2),1,save_load->GetActFilePointer());
	fwrite(&Scale,sizeof(Scale),1,save_load->GetActFilePointer());
	fwrite(&RotX,sizeof(RotX),1,save_load->GetActFilePointer());
	fwrite(&RotY,sizeof(RotY),1,save_load->GetActFilePointer());
	fwrite(&RotZ,sizeof(RotZ),1,save_load->GetActFilePointer());
	fwrite(&M,sizeof(M),1,save_load->GetActFilePointer());
	fwrite(Name,sizeof(Name),1,save_load->GetActFilePointer());
	fwrite(&NumTex,sizeof(NumTex),1,save_load->GetActFilePointer());
}

BFCVoid BFCMeshChunk::Read_0_1(BFCSaveLoad *save_load)
{
	fread(&MeshType,sizeof(MeshType),1,save_load->GetActFilePointer());
	fread(&NumData,sizeof(NumData),1,save_load->GetActFilePointer());
	fread(&NumData2,sizeof(NumData2),1,save_load->GetActFilePointer());
	fread(&Scale,sizeof(Scale),1,save_load->GetActFilePointer());
	fread(&RotX,sizeof(RotX),1,save_load->GetActFilePointer());
	fread(&RotY,sizeof(RotY),1,save_load->GetActFilePointer());
	fread(&RotZ,sizeof(RotZ),1,save_load->GetActFilePointer());
	fread(&M,sizeof(M),1,save_load->GetActFilePointer());
	fread(Name,sizeof(Name),1,save_load->GetActFilePointer());
	fread(&NumTex,sizeof(NumTex),1,save_load->GetActFilePointer());
}

BFCVoid BFCMeshChunk::Fill(BFCMesh *data)
{
	MeshType = data->Type;
	NumData = data->NumData;
	NumData2 = data->NumData2;
	Scale = data->Scale;
	RotX = data->RotX;
	RotY = data->RotY;
	RotZ = data->RotZ;
	M  = data->M;
	strcpy(Name,data->Name);
	NumTex = data->Tex.GetNumber();
}

BFCVoid BFCMeshChunk::AddIt(BFCVoid *p)
{
	BFCSaveLoad *save_load = (BFCSaveLoad *) p;
	// Now we read data1
	Win32_unlit_tex_ut_mesh *data = new Win32_unlit_tex_ut_mesh[NumData];
	fread(data,sizeof(Win32_unlit_tex_ut_mesh),NumData,save_load->GetActFilePointer());
	// and data2
	BFCWord *index = new BFCWord[NumData2];
	fread(index,sizeof(BFCWord),NumData2,save_load->GetActFilePointer());

	// Now we add and create the mesh or primitive

	BFCIPrimitive *mesh = new BFCIPrimitive(Scale.x,Scale.y,Scale.z,NumData,NumData2,Name, bfc_angle_shared);
	Win32_unlit_tex_ut_mesh *new_data = (Win32_unlit_tex_ut_mesh *) mesh->GetDataPointer();
	BFCWord *new_index = (BFCWord *) mesh->GetDataPointer2();

	// we fill data
	memcpy(new_data,data,NumData * sizeof(Win32_unlit_tex_ut_mesh));
	memcpy(new_index,index,NumData2 * sizeof(BFCWord));
	mesh->Scale = Scale;
	mesh->RotX = RotX;
	mesh->RotY = RotY;
	mesh->RotZ = RotZ;
	mesh->M = M;


	mesh->DataSet();
	mesh->Data2Set();

	delete [] data;
	delete [] index;

	mesh->FinishCon();

	// here we insert texture data

	BFCMeshTexIndexChunk index_chunk;
	// Now we read texture data
	for (BFCUInt i=0;i<NumTex;i++)
	{
		index_chunk.Read(save_load);
		index_chunk.AddIt(mesh);
	}

}


//*************************************************************************
//
//
//						BFCSaveLoad
//
//
//*************************************************************************


BFCSaveLoad::BFCSaveLoad() : ActFilePointer(NULL), VersionLoading(1.0f)
{

}

BFCReturn BFCSaveLoad::OpenSave(BFCChar *name)
{
	if (!ActFilePointer)
	{
		ActFilePointer = fopen(name,"wb");
		return bfc_ok;
	} else return bfc_in_use;
}

BFCReturn BFCSaveLoad::CloseSave()
{
	if (ActFilePointer)
	{
		fclose(ActFilePointer);
		ActFilePointer = NULL;
		return bfc_ok;
	} else return bfc_already_in_the_state;
}

BFCReturn BFCSaveLoad::OpenLoad(BFCChar *name)
{
	if (!ActFilePointer)
	{
		ActFilePointer = fopen(name,"rb");
		// check if file exist
		if (!ActFilePointer)
		{
			return bfc_not_found;
		}
		return bfc_ok;
	} else return bfc_in_use;
}

BFCReturn BFCSaveLoad::CloseLoad()
{
	if (ActFilePointer)
	{
		fclose(ActFilePointer);
		ActFilePointer = NULL;
		return bfc_ok;
	} else return bfc_already_in_the_state;
}

BFCReturn BFCSaveLoad::Load(BFCChar *name)
{
	BFCChunkType chunk_type;

	BFCReturn ret = OpenLoad(name);
	if (ret != bfc_ok) return ret;

	// loop until there is something to read or there is an error

	while (1)
	{
		if ( fread(&chunk_type,sizeof(chunk_type),1,ActFilePointer) < 1 )
		{
			// check if we are at the end or if there was an error
			if ( feof(ActFilePointer) ) break; else return bfc_read_error; 
		} else
		{
			// here we read the corresponding chunk
			switch (chunk_type)
			{
			case bfc_file_chunk:
				{
					BFCFileChunk file_chunk;
					file_chunk.Read(this);
					file_chunk.AddIt(NULL);
				} break;
			case bfc_texture_chunk:
				{
					BFCTextureChunk tex_chunk;
					tex_chunk.Read(this);
					tex_chunk.AddIt(this);
				} break;
			case bfc_viewport_chunk:
				{
					BFCViewPortChunk viewport;
					viewport.Read(this);
					viewport.AddIt(this);
				} break;
			case bfc_viewport_data_chunk:
				{
					BFCViewPortDataChunk viewport_data;
					viewport_data.Read(this);
					viewport_data.AddIt(this);
				} break;
			case bfc_mesh_chunk:
				{
					BFCMeshChunk mesh_chunk;
					mesh_chunk.Read(this);
					mesh_chunk.AddIt(this);
				} break;
			}
		}
	}

	CloseLoad();

	return bfc_ok;
}

BFCReturn BFCSaveLoad::WriteChunkType(BFCChunkType type, BFCSaveLoad *save_load)
{
	fwrite(&type,sizeof(BFCChunkType),1,save_load->GetActFilePointer());
	return bfc_ok;
}

BFCReturn BFCSaveLoad::WriteScene(BFCChar *name)
{
	BFCReturn ret = OpenSave(name);
	if (ret != bfc_ok) return ret;

	WriteChunkType(bfc_file_chunk, this);
	BFCFileChunk _file_chunk;
	_file_chunk.FileType = BFCSceneType;
	_file_chunk.Version = (BFCUInt) floor(BFCActualSceneVersion);
	_file_chunk.DecimalVersion = (BFCUInt) (( BFCActualSceneVersion - floor(BFCActualSceneVersion) ) * BFCMaxVersionNumber);
	_file_chunk.Write(this);


	// Save Viewport Info

	WriteChunkType(bfc_viewport_chunk, this);
	BFCViewPortChunk _viewport_chunk;
	_viewport_chunk.Fill(&mGraphicEngine->ViewPort);
	_viewport_chunk.Write(this);

	BFCViewPortDataChunk _viewport_data_chunk;
	BFCViewPortData *_viewport_data = (BFCViewPortData *) mGraphicEngine->ViewPort.View.GetFirstPtr();
	// Save all viewport data view info
	while (_viewport_data)
	{
		_viewport_data_chunk.Fill(_viewport_data);
		WriteChunkType(bfc_viewport_data_chunk, this);
		_viewport_data_chunk.Write(this);
		_viewport_data = (BFCViewPortData *) mGraphicEngine->ViewPort.View.GetNextPtr();
	}

	// Save all texture

	// tex is used to read all texture
	BFCTextureChunk _tex_chunk;
	BFCTextureData *_check_data = (BFCTextureData *)BFCTexture::GetTextureDataList()->GetFirst();
	while (_check_data)
	{
		// Here we write the data header of the texture
		_tex_chunk.Fill(_check_data);
		WriteChunkType(bfc_texture_chunk, this);
		_tex_chunk.Write(this);

		// Here we write the data of the texture
		BFCChar *_data = _check_data->GetDataPointer();
		fwrite(_data,1,_tex_chunk.Width * _tex_chunk.Height * 4,ActFilePointer);
		_check_data->DataSet();

		_check_data = (BFCTextureData *)BFCTexture::GetTextureDataList()->GetNext();
	}

	// Save mesh

	BFCMesh *_mesh = (BFCMesh *)mGraphicEngine->Mesh.GetFirst();
	BFCMeshChunk mesh_chunk;
	while (_mesh)
	{
		// for the moment we save only this kind of mesh
		if (_mesh->GetType() == unlit_tex_ut_i_tri_mesh)
		{
			mesh_chunk.Fill(_mesh);
			WriteChunkType(bfc_mesh_chunk, this);
			mesh_chunk.Write(this);
			// Here we write data1 and data2
			Win32_unlit_tex_ut_mesh *data = (Win32_unlit_tex_ut_mesh *) _mesh->GetDataPointer();
			BFCWord *index = (BFCWord *) _mesh->GetDataPointer2();
			fwrite(data,sizeof(Win32_unlit_tex_ut_mesh),mesh_chunk.NumData,ActFilePointer);
			fwrite(index,sizeof(BFCWord),mesh_chunk.NumData2,ActFilePointer);
			_mesh->DataSet();
			_mesh->Data2Set();

			// Save texture index
			BFCMeshTexIndexChunk _tex_index_chunk;
			BFCTextureIndex *_tex_index = (BFCTextureIndex *) _mesh->Tex.GetFirstPtr();
			while (_tex_index)
			{
				_tex_index_chunk.Fill(_tex_index);
				_tex_index_chunk.Write(this);
				_tex_index = (BFCTextureIndex *) _mesh->Tex.GetNextPtr();
			}
		}
		_mesh = (BFCMesh *)mGraphicEngine->Mesh.GetNext();
	}

	CloseSave();

	return bfc_ok;
}
