#ifndef __BFCLIGHT__H
#define __BFCLIGHT__H


#include "BFCUtility.h"
#include "BFCMath.h"
#include "D3d8.h"


enum BFCLightType
{
	bfc_point_light = 0,
	bfc_dir_light = 1,		// Directional Light
	bfc_spot_light = 2
};


//************************************************************************
//
//								BFCColor
//
//************************************************************************


struct BFCColor
{
	BFCFloat r;
	BFCFloat g;
	BFCFloat b;
	BFCFloat a;

	BFCColor() : r(0),g(0),b(0),a(0) { }
	BFCColor(BFCFloat _r,BFCFloat _g,BFCFloat _b,BFCFloat _a) : r(_r),g(_g),b(_b),a(_a) { }
};

struct BFCColorByte
{
	union
	{
		struct
		{
			BFCByte r;
			BFCByte g;
			BFCByte b;
			BFCByte a;
		};
		BFCUInt color;
	};

	BFCColorByte() : r(0),g(0),b(0),a(0) { }
	BFCColorByte(BFCUInt col) : color(col) { }
	BFCColorByte(BFCByte _r,BFCByte _g,BFCByte _b,BFCByte _a) : r(_r),g(_g),b(_b),a(_a) { }
};



//************************************************************************
//
//								BFCLight
//
//************************************************************************


class BFCLight : public BFCVoidNode
{
	friend class BFCGraphicEngine;

	static BFCUInt Number;		// Number of light
	BFCBool Enabled;			// Light Enabled ?
	BFCBool Added;				// Light Added in the engine

	BFCLightType Type;
	BFCVector Position;
	BFCVector Direction;
	BFCColor Ambient;
	BFCColor Diffuse;
	BFCColor Specular;
	BFCFloat Range;
	BFCFloat Falloff;	// Better to set it to 1
	BFCFloat Att1;
	BFCFloat Att2;
	BFCFloat Att3;
	BFCFloat Phi;
	BFCFloat Theta;

#ifdef WIN32
	D3DLIGHT8 D3dLight;
#endif

public:

	BFCLight(BFCLightType type);

	BFCVector GetPosition() const { return Position; }
	BFCVector GetDirection() const { return Direction; }
	BFCColor GetAmbient() const { return Ambient; }
	BFCColor GetDiffuse() const { return Diffuse; }
	BFCColor GetSpecular() const { return Specular; }
	BFCFloat GetRange() const { return Range; }
	BFCFloat GetFalloff() const { return Falloff; }
	BFCFloat GetAtt1() const { return Att1; }
	BFCFloat GetAtt2() const { return Att2; }
	BFCFloat GetAtt3() const { return Att3; }
	BFCFloat GetPhi() const { return Phi; }
	BFCFloat GetTheta() const { return Theta; }


	BFCVoid SetPosition(const BFCVector &pos);
	BFCVoid SetDirection(const BFCVector &dir);
	BFCVoid SetAmbient(const BFCColor &ambient);
	BFCVoid SetDiffuse(const BFCColor &diffuse);
	BFCVoid SetSpecular(const BFCColor &specular);
	BFCVoid SetRange(BFCFloat range);
	BFCVoid SetFalloff(BFCFloat falloff);
	BFCVoid SetAtt1(BFCFloat att1);
	BFCVoid SetAtt2(BFCFloat att2);
	BFCVoid SetAtt3(BFCFloat att3);
	BFCVoid SetPhi(BFCFloat phi);
	BFCVoid SetTheta(BFCFloat theta);

	BFCVoid Enable();
	BFCVoid Disable();

};






#endif