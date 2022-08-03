#ifndef __BFCMATH__H
#define __BFCMATH__H

#include <BFCUtility.h>



enum BFCProjectionType
{
	bfc_ortho_proj = 0,
	bfc_perspec_proj = 1
};

const BFCFloat bfc_all_t = -2000000.0f;

// Implementation in header file because vc++ doesn't respect inline function in cpp file very well


//************************************************************************
//
//						BFCVector
//
//************************************************************************


struct BFCVector : public BFCShortBase
{
	BFCFloat x;
	BFCFloat y;
	BFCFloat z;

	// Constructor

	BFCVector(BFCFloat __x,BFCFloat __y,BFCFloat __z) : x(__x),y(__y),z(__z) { };
	BFCVector() : x(0),y(0),z(0) { };

	// Operator

	BFCVector operator+(const BFCVector &a) const
	{
		return BFCVector(x+a.x, y+a.y, z+a.z);
	}
	BFCVector operator-(const BFCVector &a) const
	{
		return BFCVector(x-a.x, y-a.y, z-a.z);
	}
	BFCVector operator*(const BFCVector &a) const
	{
		return BFCVector(x*a.x, y*a.y, z*a.z);
	}
	BFCVector operator*(const BFCFloat a) const
	{
		return BFCVector(x*a, y*a, z*a);
	}
	BFCVector operator/(const BFCVector &a) const
	{
		return BFCVector(x/a.x, y/a.y, z/a.z);
	}
	BFCVector operator/(const BFCFloat a) const
	{
		return BFCVector(x/a, y/a, z/a);
	}
	BFCVoid Zero()
	{
		x = 0; y = 0; z = 0;
	}
	BFCFloat DotProduct(const BFCVector &a) const
	{
		return (x*a.x+y*a.y+z*a.z);
	}
	BFCVector CrossProduct(const BFCVector &a) const
	{
		return BFCVector(y*a.z-z*a.y, z*a.x-x*a.z, x*a.y-y*a.x);
	}
	BFCFloat Length() const
	{
		return (BFCFloat)(sqrt(x*x+y*y+z*z));
	}
	BFCBool IsNear(const BFCVector &v,BFCFloat tol = BFCPrecision) const
	{
		BFCFloat dist;
		BFCVector _vec = *this - v;
		// Instead of calling length we just call DotProd on it self and we compare with tol * tol so we don't need to use Sqrt
		dist = _vec.DotProduct(_vec);
		if (dist <= tol * tol) return true; else return false;
	}
	BFCVector Normal(const BFCVector &v,const BFCVector &v2) const
	{
		BFCVector tmp;
		tmp = v - *this;
		tmp = tmp.CrossProduct(v2 - *this);
		tmp.Normalize();
		return tmp;
	}
	BFCVoid Normalize()
	{
		BFCFloat i = 1.0f/Length();
		x*=i;y*=i;z*=i;
	}
};

// Remember always a plane is defined by all the vector p that have the cross product = 0 with a normal of the plane . SpectRe

struct BFCTriangle
{
	BFCVector v1,v2,v3;
	BFCVector n1;

	BFCVoid CalcNormal()
	{
		n1 = (v2-v1).CrossProduct(v3-v1);
		n1.Normalize();
	}

	BFCFloat VectorDist(BFCVector *v)
	{
		// we make a "projection" of "v" on "n1" SpectRe
		CalcNormal();
		return (BFCFloat) ( fabs(n1.DotProduct(*v-v1)) );
	}
};

//************************************************************************
//
//						BFCMatrix
//
//************************************************************************


struct BFCMatrix : public BFCShortBase
{
	union 
	{
		struct 
		{
			BFCFloat        _00, _01, _02, _03;
			BFCFloat        _10, _11, _12, _13;
			BFCFloat        _20, _21, _22, _23;
			BFCFloat        _30, _31, _32, _33;

		};
		BFCFloat m[4][4];
	};	

	// Constructors

	BFCMatrix() { Identity(); };

	BFCMatrix(	BFCFloat __00, BFCFloat __01, BFCFloat __02, BFCFloat __03,
				BFCFloat __10, BFCFloat __11, BFCFloat __12, BFCFloat __13,
				BFCFloat __20, BFCFloat __21, BFCFloat __22, BFCFloat __23,
				BFCFloat __30, BFCFloat __31, BFCFloat __32, BFCFloat __33 ) :
				_00(__00), _01(__01), _02(__02), _03(__03),
				_10(__10), _11(__11), _12(__12), _13(__13),
				_20(__20), _21(__21), _22(__22), _23(__23),
				_30(__30), _31(__31), _32(__32), _33(__33) 
				{ };


	BFCVoid Identity()
	{
		m[0][0] = 1.0f;m[0][1] = 0.0f;m[0][2] = 0.0f;m[0][3] = 0.0f;
		m[1][0] = 0.0f;m[1][1] = 1.0f;m[1][2] = 0.0f;m[1][3] = 0.0f;
		m[2][0] = 0.0f;m[2][1] = 0.0f;m[2][2] = 1.0f;m[2][3] = 0.0f;
		m[3][0] = 0.0f;m[3][1] = 0.0f;m[3][2] = 0.0f;m[3][3] = 1.0f;
	}

	BFCVoid Zero()
	{
		m[0][0] = 0.0f;m[0][1] = 0.0f;m[0][2] = 0.0f;m[0][3] = 0.0f;
		m[1][0] = 0.0f;m[1][1] = 0.0f;m[1][2] = 0.0f;m[1][3] = 0.0f;
		m[2][0] = 0.0f;m[2][1] = 0.0f;m[2][2] = 0.0f;m[2][3] = 0.0f;
		m[3][0] = 0.0f;m[3][1] = 0.0f;m[3][2] = 0.0f;m[3][3] = 0.0f;
	}

	BFCVoid Inverse()
	{
		BFCFloat swap = _01;
		_01 = _10;
		_10 = swap;

		swap = _02;
		_02 = _20;
		_20 = swap;

		swap = _12;
		_12 = _21;
		_21 = swap;

		_30 = -_30;
		_31 = -_31;
		_32 = -_32;
	}

	BFCVoid ProjectionMatrix(BFCProjectionType type, BFCFloat near_clip, BFCFloat far_clip, BFCFloat h_fov, BFCFloat v_fov, BFCFloat scale = 1.0f)
	{
		BFCFloat h,w;
		Zero();
		if (type==bfc_perspec_proj)
		{
			// Here we "Normalize" x,y,z components for the screen and the z buffer
			w = (BFCFloat)(1.0f/tan(h_fov*0.5f));  // 1/tan(x) == cot(x)
			h = (BFCFloat)(1.0f/tan(v_fov*0.5f));   // 1/tan(x) == cot(x)
			m[0][0] = w;
			m[1][1] = h;
			m[2][2] = far_clip/(far_clip - near_clip);
			m[3][2] = -m[2][2]*near_clip;
			m[2][3] = 1.0f;
		} else
		{
			// Here we "Normalize" x,y,z components for the screen and the z buffer
			w = (BFCFloat)(1.0f/tan(h_fov*0.5f)) * scale;  // 1/tan(x) == cot(x)
			h = (BFCFloat)(1.0f/tan(v_fov*0.5f)) * scale;   // 1/tan(x) == cot(x)
			m[0][0] = w / 100.0f;
			m[1][1] = h / 100.0f;
			m[2][2] = 1/(far_clip - near_clip);
			m[3][2] = -m[2][2]*near_clip;
			m[3][3] = 1.0f;
		}
	}

	// Matrix mul directx like

	BFCMatrix operator*(const BFCMatrix &_m) const
	{
		return BFCMatrix( _m.m[0][0]*m[0][0]+_m.m[1][0]*m[0][1]+_m.m[2][0]*m[0][2]+_m.m[3][0]*m[0][3],
						  _m.m[0][1]*m[0][0]+_m.m[1][1]*m[0][1]+_m.m[2][1]*m[0][2]+_m.m[3][1]*m[0][3],
						  _m.m[0][2]*m[0][0]+_m.m[1][2]*m[0][1]+_m.m[2][2]*m[0][2]+_m.m[3][2]*m[0][3],
						  _m.m[0][3]*m[0][0]+_m.m[1][3]*m[0][1]+_m.m[2][3]*m[0][2]+_m.m[3][3]*m[0][3],

						  _m.m[0][0]*m[1][0]+_m.m[1][0]*m[1][1]+_m.m[2][0]*m[1][2]+_m.m[3][0]*m[1][3],
						  _m.m[0][1]*m[1][0]+_m.m[1][1]*m[1][1]+_m.m[2][1]*m[1][2]+_m.m[3][1]*m[1][3],
						  _m.m[0][2]*m[1][0]+_m.m[1][2]*m[1][1]+_m.m[2][2]*m[1][2]+_m.m[3][2]*m[1][3],
						  _m.m[0][3]*m[1][0]+_m.m[1][3]*m[1][1]+_m.m[2][3]*m[1][2]+_m.m[3][3]*m[1][3],

						  _m.m[0][0]*m[2][0]+_m.m[1][0]*m[2][1]+_m.m[2][0]*m[2][2]+_m.m[3][0]*m[2][3],
						  _m.m[0][1]*m[2][0]+_m.m[1][1]*m[2][1]+_m.m[2][1]*m[2][2]+_m.m[3][1]*m[2][3],
						  _m.m[0][2]*m[2][0]+_m.m[1][2]*m[2][1]+_m.m[2][2]*m[2][2]+_m.m[3][2]*m[2][3],
						  _m.m[0][3]*m[2][0]+_m.m[1][3]*m[2][1]+_m.m[2][3]*m[2][2]+_m.m[3][3]*m[2][3],

						  _m.m[0][0]*m[3][0]+_m.m[1][0]*m[3][1]+_m.m[2][0]*m[3][2]+_m.m[3][0]*m[3][3],
						  _m.m[0][1]*m[3][0]+_m.m[1][1]*m[3][1]+_m.m[2][1]*m[3][2]+_m.m[3][1]*m[3][3],
						  _m.m[0][2]*m[3][0]+_m.m[1][2]*m[3][1]+_m.m[2][2]*m[3][2]+_m.m[3][2]*m[3][3],
						  _m.m[0][3]*m[3][0]+_m.m[1][3]*m[3][1]+_m.m[2][3]*m[3][2]+_m.m[3][3]*m[3][3] );
	}

	BFCVector operator*(const BFCVector &v) const
	{
		return BFCVector(	m[0][0]*v.x+m[1][0]*v.y+m[2][0]*v.z+m[3][0],m[0][1]*v.x+m[1][1]*v.y+m[2][1]*v.z+m[3][1],
							m[0][2]*v.x+m[1][2]*v.y+m[2][2]*v.z+m[3][2]);
	}

	// Multiply only the vector, does not add the pos

	BFCVector MulVecOnly(const BFCVector &v) const
	{
		return BFCVector(	m[0][0]*v.x+m[1][0]*v.y+m[2][0]*v.z,m[0][1]*v.x+m[1][1]*v.y+m[2][1]*v.z,
							m[0][2]*v.x+m[1][2]*v.y+m[2][2]*v.z);
	}

	BFCVoid RotateX(BFCFloat ang)
	{
		*this = (*this) *
			BFCMatrix(1.0f, 0.0f               , 0.0f              , 0.0f,
		              0.0f, (BFCFloat)cos(ang) , (BFCFloat)sin(ang), 0.0f,
		              0.0f, (BFCFloat)-sin(ang), (BFCFloat)cos(ang), 0.0f,
		              0.0f, 0.0f               , 0.0f              , 1.0f);
	}

	BFCVoid RotateY(BFCFloat ang)
	{
		*this = (*this) *
			BFCMatrix((BFCFloat)cos(ang), 0.0f, (BFCFloat)-sin(ang), 0.0f,
		              0.0f              , 1.0f, 0.0f               , 0.0f,
		              (BFCFloat)sin(ang), 0.0f, (BFCFloat)cos(ang) , 0.0f,
		              0.0f              , 0.0f, 0.0f               , 1.0f);
	}

	BFCVoid RotateZ(BFCFloat ang)
	{
		*this = *this *
			BFCMatrix((BFCFloat)cos(ang), (BFCFloat)-sin(ang), 0.0f, 0.0f,
		              (BFCFloat)sin(ang), (BFCFloat)cos(ang) , 0.0f, 0.0f,
		              0.0f              , 0.0f               , 1.0f, 0.0f,
		              0.0f              , 0.0f               , 0.0f, 1.0f);
	}

	// note: Set Rotate reset the matrix

	BFCVoid SetRotateX(BFCFloat ang)
	{
		m[0][0] = 1.0f;	m[0][1] = 0.0f;					m[0][2] = 0.0f;					m[0][3] = 0.0f;
		m[1][0] = 0.0f;	m[1][1] = (BFCFloat)cos(ang);	m[1][2] = (BFCFloat)sin(ang);	m[1][3] = 0.0f;
		m[2][0] = 0.0f;	m[2][1] = (BFCFloat)-sin(ang);	m[2][2] = (BFCFloat)cos(ang);	m[2][3] = 0.0f;
		m[3][0] = 0.0f;	m[3][1] = 0.0f;					m[3][2] = 0.0f;					m[3][3] = 1.0f;
	}

	// note: Set Rotate reset the matrix

	BFCVoid SetRotateY(BFCFloat ang)
	{
		m[0][0] = (BFCFloat)cos(ang);	m[0][1] = 0.0f;	m[0][2] = (BFCFloat)-sin(ang);	m[0][3] = 0.0f;
		m[1][0] = 0.0f;					m[1][1] = 1.0f;	m[1][2] = 0.0f;					m[1][3] = 0.0f;
		m[2][0] = (BFCFloat)sin(ang);	m[2][1] = 0.0f;	m[2][2] = (BFCFloat)cos(ang);	m[2][3] = 0.0f;
		m[3][0] = 0.0f;					m[3][1] = 0.0f;	m[3][2] = 0.0f;					m[3][3] = 1.0f;
	}

	// note: Set Rotate reset the matrix

	BFCVoid SetRotateZ(BFCFloat ang)
	{
		m[0][0] = (BFCFloat)cos(ang);	m[0][1] = (BFCFloat)-sin(ang);	m[0][2] = 0.0f;	m[0][3] = 0.0f;
		m[1][0] = (BFCFloat)sin(ang);	m[1][1] = (BFCFloat)cos(ang);	m[1][2] = 0.0f;	m[1][3] = 0.0f;
		m[2][0] = 0.0f;					m[2][1] = 0.0f;					m[2][2] = 1.0f;	m[2][3] = 0.0f;
		m[3][0] = 0.0f;					m[3][1] = 0.0f;					m[3][2] = 0.0f;	m[3][3] = 1.0f;
	}

	BFCVoid SetRight(const BFCVector &v)
	{
		m[0][0] = v.x;
		m[0][1] = v.y;
		m[0][2] = v.z;
	}
	BFCVoid SetUp(const BFCVector &v)
	{
		m[1][0] = v.x;
		m[1][1] = v.y;
		m[1][2] = v.z;
	}
	BFCVoid SetAt(const BFCVector &v)
	{
		m[2][0] = v.x;
		m[2][1] = v.y;
		m[2][2] = v.z;
	}
	BFCVoid SetPos(const BFCVector &v)
	{
		m[3][0] = v.x;
		m[3][1] = v.y;
		m[3][2] = v.z;
	}
	BFCVoid SetPos(BFCFloat x,BFCFloat y,BFCFloat z)
	{
		m[3][0] = x;
		m[3][1] = y;
		m[3][2] = z;
	}

	BFCVoid SetScale(const BFCVector &v)
	{
		m[0][0] *= v.x;
		m[1][1] *= v.y;
		m[2][2] *= v.z;
	}
	BFCVoid SetScale(BFCFloat x,BFCFloat y,BFCFloat z)
	{
		m[0][0] *= x;
		m[1][1] *= y;
		m[2][2] *= z;
	}

	BFCVector GetRight() const 
	{
		return BFCVector(m[0][0],m[0][1],m[0][2]);
	}
	BFCVector GetUp() const 
	{
		return BFCVector(m[1][0],m[1][1],m[1][2]);
	}
	BFCVector GetAt() const 
	{
		return BFCVector(m[2][0],m[2][1],m[2][2]);
	}
	BFCVector GetPos() const 
	{
		return BFCVector(m[3][0],m[3][1],m[3][2]);
	}
};


//************************************************************************
//
//						BFCLine
//
//************************************************************************

struct BFCLine : public BFCShortBase
{

	BFCVector Pos;
	BFCVector Dir;					// !!! This Dir is not relative to the pos. It should be called not Dir but Head... !!! SpectRe
	BFCVector LastPointIntersect;	// Last point of intersection
	BFCFloat LastTValue;
	BFCFloat LastTValue2;

	// Constructor

	BFCLine();

	//return t value to insert in the param equations of the line to get x y z where line intersect plane only
	BFCFloat GetTPlaneIntersection(const BFCVector &v1, const BFCVector &v2, const BFCVector &v3);
	BFCBool IsTriIntersection(const BFCVector &v1, const BFCVector &v2, const BFCVector &v3,BFCBool *border = NULL,BFCFloat prec = 1.0f);
	// tol is tolerance number by wich we can judge the line is near the vert
	BFCBool IsNearVert(const BFCVector &vert, BFCFloat tol);
	// we use LastPointIntersect as nearest point variable. return false if nearest point is infinite
	BFCBool GetNearestPoint(const BFCVector &vert);
	// Get vector pos with a t value
	BFCVector GetTPos(BFCFloat t);
	// Get t from z pos
	BFCFloat GetTFromZ(BFCFloat z);
	BFCBool IsLineIntersection(BFCLine &line);
	BFCBool IsParallel(BFCLine &line, BFCFloat tol = 1.0f);
	// tell if the line is intersecting a line in list
	BFCBool IntLineList(BFCVoidList *line_list);

};

//========================================================================
//
//						Global Functions
//
//========================================================================

// if border is passed it will be true if point is on border otherwise false

BFCBool PointInTri(const BFCVector *p,const BFCVector *a,const BFCVector *b,const BFCVector *c,BFCBool *border = NULL,BFCFloat prec = 1.0f);

// Equal zero ?

BFCBool EqZero(BFCFloat v);

BFCBool BFCEqual(BFCFloat v,BFCFloat v2, BFCFloat prec_fac = 1.0f);



#endif
