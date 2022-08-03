#include "BFCLight.h"
#include "BFCGraphicEngine.h"



//************************************************************************
//
//								BFCLight
//
//************************************************************************


BFCUInt BFCLight::Number = 0;


BFCLight::BFCLight(BFCLightType type) : Type(type), Enabled(false), Added(false)
{
#ifdef WIN32
	ZeroMemory(&D3dLight, sizeof(D3DLIGHT8));
	switch(type)
	{
	case bfc_point_light:
		{
			D3dLight.Type = D3DLIGHT_POINT;
		} break;
	case bfc_dir_light:
		{
			D3dLight.Type = D3DLIGHT_DIRECTIONAL;
		} break;
	case bfc_spot_light:
		{
			D3dLight.Type = D3DLIGHT_SPOT;
		} break;
	}
#endif
}


BFCVoid BFCLight::SetPosition(const BFCVector &pos)
{
	Position = pos;
}


BFCVoid BFCLight::SetDirection(const BFCVector &dir)
{
	Direction = dir;
#ifdef WIN32
	D3dLight.Direction.x = dir.x;
	D3dLight.Direction.y = dir.y;
	D3dLight.Direction.z = dir.z;
	if (Added) mD3dDevice->SetLight(MyID, &D3dLight);
#endif
}


BFCVoid BFCLight::SetAmbient(const BFCColor &ambient)
{
	Ambient = ambient;
}


BFCVoid BFCLight::SetDiffuse(const BFCColor &diffuse)
{
	Diffuse = diffuse;
#ifdef WIN32
	D3dLight.Diffuse.r = diffuse.r;
	D3dLight.Diffuse.g = diffuse.g;
	D3dLight.Diffuse.b = diffuse.b;
	D3dLight.Diffuse.a = diffuse.a;
	if (Added) mD3dDevice->SetLight(MyID, &D3dLight);
#endif
}


BFCVoid BFCLight::SetSpecular(const BFCColor &specular)
{
	Specular = specular;
}


BFCVoid BFCLight::SetRange(BFCFloat range)
{
	Range = range;
#ifdef WIN32
	D3dLight.Range = range;
	if (Added) mD3dDevice->SetLight(MyID, &D3dLight);
#endif
}


BFCVoid BFCLight::SetFalloff(BFCFloat falloff)
{
	Falloff = falloff;
}


BFCVoid BFCLight::SetAtt1(BFCFloat att1)
{
	Att1 = att1;
}


BFCVoid BFCLight::SetAtt2(BFCFloat att2)
{
	Att2 = att2; 
}


BFCVoid BFCLight::SetAtt3(BFCFloat att3)
{
	Att3 = att3;
}


BFCVoid BFCLight::SetPhi(BFCFloat phi)
{
	Phi = phi;
}


BFCVoid BFCLight::SetTheta(BFCFloat theta)
{
	Theta = theta;
}

BFCVoid BFCLight::Enable()
{
	Enabled = true;
#ifdef WIN32
	if (Added) mD3dDevice->LightEnable(MyID,TRUE);
#endif
}

BFCVoid BFCLight::Disable()
{
	Enabled = false;
#ifdef WIN32
	if (Added) mD3dDevice->LightEnable(MyID,FALSE);
#endif
}
