#include <BFCMath.h>
#include <BFCGraphicEngine.h>

#ifdef _DEBUG
#include <BFCDebug.h>
#endif



//************************************************************************
//
//						BFCVector
//
//************************************************************************





//************************************************************************
//
//						BFCLine
//
//************************************************************************

BFCLine::BFCLine() : LastTValue(0)
{
}

//return t value to insert in the param equations of the line to get x y z where line intersect plane only
BFCFloat BFCLine::GetTPlaneIntersection(const BFCVector &v1, const BFCVector &v2, const BFCVector &v3)
{
	BFCFloat r;
	BFCFloat d;
	BFCVector vt1;
	BFCVector vt2;
	BFCVector n1;
	// Get the tri normal
	vt1 = v2 - v1;
	vt2 = v3 - v1;
	n1 = vt1.CrossProduct(vt2);
	// it's a good idea to normalize
	n1.Normalize();
	// I resolved my equations and I fall in this...    SpectRe
	// What interresting it indicates also the visual solution
	// Calc the vector from the vertex of the tri to the line pos
	vt1 = Pos - v1;
	// Calc the opposite vector of the line, opposite because we must respect the way
	vt2 = Pos - Dir;
	// "Projection" of vt1 on n1
	r = n1.DotProduct(vt1);
	// "Projection" of vt2 on n1
	d = n1.DotProduct(vt2);

	vt2.Normalize();
	BFCFloat f = n1.DotProduct(vt2);
/*
	mDebugLineMesh.CreateDebugLine(5,2,2);
	BFCVector fuck;
	fuck.x = 0.0f;
	fuck.y = 0.0f;
	fuck.z = 0.0f;
	mDebugLineMesh.SetDebugLine(5,&fuck,&vt2,0,0xFF770000);
	mDebugLineMesh.SetDebugLine(5,&Pos,&Dir,1,0xFFFFCCCC);
	mDebugLineMesh.DebugLineSet(5);
*/


	// Here we have infinite or no solution. SpectRe
	// Hope it's ok . SpectRe
	if (fabs(f)<BFCPrecision) return bfc_all_t;
	// we've got t
	return (r/d);
}

BFCBool BFCLine::IsTriIntersection(const BFCVector &v1, const BFCVector &v2, const BFCVector &v3,BFCBool *border, BFCFloat prec)
{
	LastTValue = GetTPlaneIntersection(v1,v2,v3);
	// hmmm should be ok now with this condition
	if (LastTValue==bfc_all_t)
	{
		BFCTriangle tri;
		tri.v1 = v1;
		tri.v2 = v2;
		tri.v3 = v3;
		// we get the distance from the line and the plane of the triangle
		BFCFloat _dist = tri.VectorDist(&Pos);
		if (_dist < BFCPrecision*100.0f)
		{
			if (PointInTri(&Pos,&v1,&v2,&v3) || PointInTri(&Dir,&v1,&v2,&v3))
			{
				if (border) *border = true;
				return true;
			}
		} else
		{
			if (border) *border = false;
			return false;
		}
/*		
		// since we have infinite or no solution we check with this condition. SpectRe
		if (PointInTri(&Pos,&v1,&v2,&v3) && PointInTri(&Dir,&v1,&v2,&v3))
		{
			if (border) *border = true;
			return true;
		}
		else
		{
			if (border) *border = false;
			return false;
		}
*/		
	}
	// Insert t into param equations
	LastPointIntersect.x = Pos.x+(Dir.x - Pos.x)*LastTValue;
	LastPointIntersect.y = Pos.y+(Dir.y - Pos.y)*LastTValue;
	LastPointIntersect.z = Pos.z+(Dir.z - Pos.z)*LastTValue;
	// Check point is in tri
	return PointInTri(&LastPointIntersect,&v1,&v2,&v3,border,prec);
}

BFCBool BFCLine::IsParallel(BFCLine &line, BFCFloat tol)
{
	BFCVector v1,v2;
	v1 = Dir - Pos;
	v2 = line.Dir - line.Pos;
	v1.Normalize();
	v2.Normalize();
	// put the 2 vector in the "same" way
	if ( (v2.x > 0 && v1.x<0) || (v2.x < 0 && v1.x>0) || (v2.y > 0 && v1.y<0) || (v2.y < 0 && v1.y>0) || (v2.z > 0 && v1.z<0) || (v2.z < 0 && v1.z>0))
	{
		v2.x = -v2.x;
		v2.y = -v2.y;
		v2.z = -v2.z;
	}
	// check if they are nearly parallel
	if (BFCEqual(v1.x,v2.x,tol))
	{
		if (BFCEqual(v1.y,v2.y,tol))
		{
			if (BFCEqual(v1.z,v2.z,tol))
			{
				return true;
			}
		}
	}
	return false;
}

/*
Mainly the formula is this one but we must take care of division by 0
	div = d.y * d2.x - d.x * d2.y;
	t2 = (d.x * p2.y - d.x * p.y + d.y * p.x - d.y * p2.x) / div;
	t = (p2.x + d2.x * t2 - p.x) / d.x;
	// check if these value satisfy the third equation
	if (p.z+d.z*t == p2.z +d2.z*t2) return true else return false;
*/
BFCBool BFCLine::IsLineIntersection(BFCLine &line)
{
	BFCVector p,p2,d,d2;
	p = Pos;
	p2 = line.Pos;
	// calc the line vector
	d = Dir - Pos;
	d2 = line.Dir - line.Pos;
	BFCFloat t,t2;
	BFCFloat div;
	// We must take care of all division by 0 !!!!! Damn

	if ( !EqZero(d.x) )
	{
		div = d.y * d2.x - d.x * d2.y;
		if ( EqZero(div) )
		{
			if ( !EqZero(d.z) )
			{
				div = d.y * d2.z - d.z * d2.y;
				if ( EqZero(div) )
				{
					div = d.x * d2.z - d.z * d2.x;
					// for the moment we don't consider line one on another
					if ( EqZero(div) ) return false;
					t2 = (d.z * p2.x - d.z * p.x + d.x * p.z - d.x * p2.z) / div;
					t = (p2.z + d2.z * t2 - p.z) / d.z;
					LastTValue2 = t2;
					LastTValue = t;
					// check if these value satisfy the third equation
					if ( BFCEqual(p.y+d.y*t,p2.y +d2.y*t2) ) return true; else return false;
				}
				t2 = (d.z * p2.y - d.z * p.y + d.y * p.z - d.y * p2.z) / div;
				t = (p2.z + d2.z * t2 - p.z) / d.z;
				LastTValue2 = t2;
				LastTValue = t;
				// check if these value satisfy the third equation
				if ( BFCEqual(p.x+d.x*t,p2.x +d2.x*t2) ) return true; else return false;
			} else
			{
				div = d.z * d2.y - d.y * d2.z;
				if ( EqZero(div) )
				{
					div = d.z * d2.x - d.z * d2.x;
					// for the moment we don't consider line one on another
					if ( EqZero(div) ) return false;
					t2 = (d.x * p2.z - d.x * p.z + d.z * p.x - d.z * p2.x) / div;
					t = (p2.x + d2.x * t2 - p.x) / d.x;
					LastTValue2 = t2;
					LastTValue = t;
					// check if these value satisfy the third equation
					if ( BFCEqual(p.y+d.y*t,p2.y +d2.y*t2) ) return true; else return false;
				}
				t2 = (d.y * p2.z - d.y * p.z + d.z * p.y - d.z * p2.y) / div;
				t = (p2.y + d2.y * t2 - p.y) / d.y;
				LastTValue2 = t2;
				LastTValue = t;
				// check if these value satisfy the third equation
				if ( BFCEqual(p.x+d.x*t,p2.x +d2.x*t2) ) return true; else return false;
			}
		} else
		{
			t2 = (d.x * p2.y - d.x * p.y + d.y * p.x - d.y * p2.x) / div;
			t = (p2.x + d2.x * t2 - p.x) / d.x;
			LastTValue2 = t2;
			LastTValue = t;
			// check if these value satisfy the third equation
			if ( BFCEqual(p.z+d.z*t,p2.z +d2.z*t2) ) return true; else return false;
		}
	} else
	{
		div = d.x * d2.y - d.y * d2.x;
		if ( EqZero(div) )
		{
			if ( !EqZero(d.y) )
			{
				div = d.z * d2.y - d.y * d2.z;
				if ( EqZero(div) )
				{
					div = d.x * d2.y - d.y * d2.x;
					// for the moment we don't consider line one on another
					if ( EqZero(div) ) return false;
					t2 = (d.y * p2.x - d.y * p.x + d.x * p.y - d.x * p2.y) / div;
					t = (p2.y + d2.y * t2 - p.y) / d.y;
					LastTValue2 = t2;
					LastTValue = t;
					// check if these value satisfy the third equation
					if ( BFCEqual(p.z+d.z*t,p2.z +d2.z*t2) ) return true; else return false;
				}
				t2 = (d.y * p2.z - d.y * p.z + d.z * p.y - d.z * p2.y) / div;
				t = (p2.y + d2.y * t2 - p.y) / d.y;
				LastTValue2 = t2;
				LastTValue = t;
				// check if these value satisfy the third equation
				if ( BFCEqual(p.x+d.x*t,p2.x +d2.x*t2) ) return true; else return false;			
			} else
			{
				div = d.y * d2.z - d.z * d2.y;
				if ( EqZero(div) )
				{
					div = d.x * d2.z - d.x * d2.y;
					// for the moment we don't consider line one on another
					if ( EqZero(div) ) return false;
					t2 = (d.z * p2.x - d.z * p.x + d.x * p.z - d.x * p2.z) / div;
					t = (p2.z + d2.z * t2 - p.z) / d.z;
					LastTValue2 = t2;
					LastTValue = t;
					// check if these value satisfy the third equation
					if ( BFCEqual(p.y+d.y*t,p2.y + d2.y*t2) ) return true; else return false;
				}
				t2 = (d.z * p2.y - d.z * p.y + d.y * p.z - d.y * p2.z) / div;
				t = (p2.z + d2.z * t2 - p.z) / d.z;
				LastTValue2 = t2;
				LastTValue = t;
				// check if these value satisfy the third equation
				if ( BFCEqual(p.x+d.x*t,p2.x + d2.x*t2) ) return true; else return false;
			}
		} else
		{
			if ( !EqZero(d.y) )
			{
				t2 = (d.y * p2.x - d.y * p.x + d.x * p.y - d.x * p2.y) / div;
				t = (p2.y + d2.y * t2 - p.y) / d.y;
				LastTValue2 = t2;
				LastTValue = t;
				// check if these value satisfy the third equation
				if ( BFCEqual(p.z+d.z*t,p2.z +d2.z*t2) ) return true; else return false;
			} else
			{
				if ( EqZero(d.z) ) return false;
				div = d.x * d2.z - d.z * d2.x;
				if ( EqZero(div) )
				{
					div = d.y * d2.z - d.z * d2.y;
					// for the moment we don't consider line one on another
					if ( EqZero(div) ) return false;
					t2 = (d.z * p2.y - d.z * p.y + d.y * p.z - d.y * p2.z) / div;
					t = (p2.z + d2.z * t2 - p.z) / d.z;
					LastTValue2 = t2;
					LastTValue = t;
					// check if these value satisfy the third equation
					if ( BFCEqual(p.x+d.x*t,p2.x +d2.x*t2) ) return true; else return false;
				}
				t2 = (d.z * p2.z - d.z * p.x + d.x * p.z - d.x * p2.z) / div;
				t = (p2.z + d2.z * t2 - p.z) / d.z;
				LastTValue2 = t2;
				LastTValue = t;
				// check if these value satisfy the third equation
				if ( BFCEqual(p.y+d.y*t,p2.y +d2.y*t2) ) return true; else return false;
			}
		}
	}
}

BFCBool BFCLine::IntLineList(BFCVoidList *line_list)
{
	BFCLine line;
	BFCVector *l1,*l2;
	l1 = (BFCVector *) line_list->GetFirstPtr();
	if (l1) l2 = (BFCVector *) line_list->GetNextPtr();
	while (l1)
	{
		line.Pos = *l1;
		line.Dir = *l2;


/*		BFCChar text[256];

if (DebugVisualizeCall2)
{
	Win32_lit_ut_mesh *ddata = (Win32_lit_ut_mesh *)mDebugMesh->GetDataPointer();
	ddata[0].pos = *l1;
	ddata[0].color = 0xFF00FF00;

	ddata[1].pos = *l2;
	ddata[1].color = 0xFF00FF00;

	ddata[2].pos = Pos;
	ddata[2].color = 0xFFFF00FF;

	ddata[3].pos = Dir;
	ddata[3].color = 0xFFFF00FF;
	mDebugMesh->DataSet();

	
		sprintf(text,"Testing");
		SendMessage(mhwnd,WM_SETTEXT,0,(long)text);
		DebugVisualize();

}
*/

		if (line.IsLineIntersection(*this))
		{
			if (line.LastTValue>BFCPrecision && line.LastTValue<1.0f-BFCPrecision 
				&& line.LastTValue2>BFCPrecision && line.LastTValue2<1.0f-BFCPrecision)
			{
/*if (DebugVisualizeCall2)
{

				sprintf(text,"Intersection");
				SendMessage(mhwnd,WM_SETTEXT,0,(long)text);
				DebugVisualize();
}*/
				return true;
			}
		}
/*if (DebugVisualizeCall2)
{

		sprintf(text,"No Intersection");
		SendMessage(mhwnd,WM_SETTEXT,0,(long)text);
		DebugVisualize();
}*/
		l1 = (BFCVector *) line_list->GetNextPtr();
		if (l1)
		{
			l2 = (BFCVector *) line_list->GetNextPtr();
		}
	}
	return false;
}

BFCBool BFCLine::GetNearestPoint(const BFCVector &vert)
{
	// LastTValue will be the t to insert in param equation of the line to find the nearest point
	BFCVector line;
	// calc the line vector
	line = Dir - Pos;
	// I Resolved equations for finding nearest point of the line from the vert and I fall in this. SpectRe
	// Hint: I used the dot product to fall in this commented solution
	BFCFloat div;
	//div = line.x * Dir.x + line.y * Dir.y + line.z * Dir.z - line.x * Pos.x - line.y * Pos.y - line.z * Pos.z;
	//t = (line.x * vert.x + line.y * vert.y + line.z * vert.z - line.x * Pos.x - line.y * Pos.y - line.z * Pos.z) / div;
	// Now the compact and maybe also visual solution is:
	div = line.DotProduct(Dir) - line.DotProduct(Pos);
	// I don't care about infinity solution
	if (EqZero(div))
	{
		return false;
	}
	LastTValue = ( line.DotProduct(vert) - line.DotProduct(Pos) ) / div;
	// we use LastPointIntersect as nearest point variable
	// now we insert the t
	LastPointIntersect = Pos + line * LastTValue;
	return true;
}

// tol is tolerance number by wich we can judge the line is near the vert
BFCBool BFCLine::IsNearVert(const BFCVector &vert, BFCFloat tol)
{
	// Calc nearest point
	if (GetNearestPoint(vert))
	{
		// if ok, now we check distance
		BFCFloat dist;
		BFCVector _vec = LastPointIntersect - vert;
		// Instead of calling length we just call DotProd on it self and we compare with tol * tol so we don't need to use Sqrt
		dist = _vec.DotProduct(_vec);
		// tol = 0.0f tell to calculate the tol value with the distance
		if (tol == 0.0f)
		{
			tol = LastTValue * 0.01f;
		}
		if (dist <= tol * tol) return true; else return false;
	} else return false;
}

BFCVector BFCLine::GetTPos(BFCFloat t)
{
	return BFCVector(Pos + (Dir-Pos) * t);
}

BFCFloat BFCLine::GetTFromZ(BFCFloat z)
{
	BFCAssert(Dir.z,"Can't call GetTFromZ with Dir.z = 0 !");
	return ( (z - Pos.z) / Dir.z );
}

//========================================================================
//
//						Global Functions
//
//========================================================================

// To algorithtm is in brief this:
// First we check if the point is in the rectangle going from the edge
// The point can be inside the tri and not inside one of the rectangle because of "open angle"
// Then we find the nearest edge of the point
// Then we trace line from the point and from nearest point on oposite edge to the edge found
// we check the length to see if the point is under the op edge
// In certain cse we must find open angle, because we don't control if it's under open angle
// it seems a little bit spaghetti code!! It's because I use roll up of code to optimize.  SpectRe
// Anyway there is still a lot of optimisation to do 

BFCBool PointInTri(const BFCVector *p,const BFCVector *a,const BFCVector *b,const BFCVector *c,BFCBool *border, BFCFloat prec)
{
	// ab first edge
	// bc second edge
	// ca third edge
	BFCVector near_on_ab;
	BFCVector near_on_bc;
	BFCVector near_on_ca;
	BFCFloat dist_from_ab = BFCFarValue;
	BFCFloat dist_from_bc = BFCFarValue;
	BFCFloat dist_from_ca = BFCFarValue;
	// dist will be smallest distance from an edge
	BFCFloat dist = BFCFarValue;
	BFCFloat tmp;
	BFCVector vec_tmp;
	BFCLine line;
	BFCBool in_ab;
	BFCBool in_bc;
	BFCBool in_ca;
	BFCBool tri_open_angle = false;
	// nearest edge of the point
	BFCInt nearest_edge;

	line.Pos = *a;
	line.Dir = *b;
	line.GetNearestPoint(*p);
	near_on_ab = line.LastPointIntersect;
	if (line.LastTValue>1.0f+BFCPrecision || line.LastTValue<0.0f-BFCPrecision)
	{
		// Here point is not in triangle going from the edge ab
		in_ab = false;
		tri_open_angle = true;
	} else
	{
		dist_from_ab = (*p - near_on_ab).Length();
		in_ab = true;
		dist = dist_from_ab;
		nearest_edge = 1;
	}

	line.Pos = *b;
	line.Dir = *c;
	line.GetNearestPoint(*p);
	near_on_bc = line.LastPointIntersect;
	if (line.LastTValue>1.0f+BFCPrecision || line.LastTValue<0.0f-BFCPrecision)
	{
		// Here point is not in triangle going from the edge bc
		in_bc = false;
		// point is not in two rectangles going from edges so return false
		if (tri_open_angle) return false;
		tri_open_angle = true;
	} else
	{
		dist_from_bc = (*p - near_on_bc).Length();
		in_bc = true;
		if (dist_from_bc < dist)
		{
			dist = dist_from_bc;
			nearest_edge = 2;
		}
	}

	line.Pos = *c;
	line.Dir = *a;
	line.GetNearestPoint(*p);
	near_on_ca = line.LastPointIntersect;
	if (line.LastTValue>1.0f+BFCPrecision || line.LastTValue<0.0f-BFCPrecision)
	{
		// Here point is not in triangle going from the edge ca
		in_ca = false;
		// point is not in two rectangles going from edges so return false
		if (tri_open_angle) return false;
		tri_open_angle = true;
	} else
	{
		in_ca = true;
		dist_from_ca = (*p - near_on_ca).Length();
		if (dist_from_ca < dist)
		{
			dist = dist_from_ca;
			nearest_edge = 3;
		}
	}

	if (tri_open_angle)
	{
		// Here we need to control point under only one op edge
		switch(nearest_edge)
		{
		case 1:
			{
				// ab edge case
				// we check with which line we must control since it's an "open angle" triangle
				if (in_bc)
				{
					line.Pos = *b;
					line.Dir = *c;
					line.GetNearestPoint(near_on_ab);
					tmp = (near_on_ab-line.LastPointIntersect).Length();
					if ( fabs(dist_from_bc - tmp) > BFCPrecision * prec )
					if (dist_from_bc > tmp) return false;
					if (border)
					{
						if ( fabs(dist_from_bc - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
				} else
				{
					line.Pos = *c;
					line.Dir = *a;
					line.GetNearestPoint(near_on_ab);
					tmp = (near_on_ab-line.LastPointIntersect).Length();
					if ( fabs(dist_from_ca - tmp) > BFCPrecision * prec )
					if (dist_from_ca > tmp) return false;
					if (border)
					{
						if ( fabs(dist_from_ca - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
				}
				return true;
			}break;
		case 2:
			{
				// bc edge case
				// we check with which line we must control since it's an "open angle" triangle
				if (in_ca)
				{
					line.Pos = *c;
					line.Dir = *a;
					line.GetNearestPoint(near_on_bc);
					tmp = (near_on_bc-line.LastPointIntersect).Length();
					if ( fabs(dist_from_ca - tmp) > BFCPrecision * prec )
					if (dist_from_ca > tmp) return false;
					if (border)
					{
						if ( fabs(dist_from_ca - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
				} else
				{
					line.Pos = *a;
					line.Dir = *b;
					line.GetNearestPoint(near_on_bc);
					tmp = (near_on_bc-line.LastPointIntersect).Length();
					if ( fabs(dist_from_ab - tmp) > BFCPrecision * prec )
					if (dist_from_ab > tmp) return false;
					if (border)
					{
						if ( fabs(dist_from_ab - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
				}
				return true;
			}break;
		case 3:
			{
				// ca edge case
				// we check with which line we must control since it's an "open angle" triangle
				if (in_ab)
				{
					line.Pos = *a;
					line.Dir = *b;
					line.GetNearestPoint(near_on_ca);
					tmp = (near_on_ca-line.LastPointIntersect).Length();
					if ( fabs(dist_from_ab - tmp) > BFCPrecision * prec )
					if (dist_from_ab > tmp) return false;
					if (border)
					{
						if ( fabs(dist_from_ab - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
				} else
				{
					line.Pos = *b;
					line.Dir = *c;
					line.GetNearestPoint(near_on_ca);
					tmp = (near_on_ca-line.LastPointIntersect).Length();
					if ( fabs(dist_from_bc - tmp) > BFCPrecision * prec )
					if (dist_from_bc > tmp) return false;
					if (border)
					{
						if ( fabs(dist_from_bc - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
				}
				return true;
			}break;
		}
	} else
	{
		// Here we need to control point under two op edge normally ( check the code there are exception!! )
		switch(nearest_edge)
		{
		case 1:
			{
				// ab edge case
				// First we check if it's not an "open angle"
				line.Pos = *b;
				line.Dir = *c;
				vec_tmp = line.GetTPos(BFCPrecision * prec);
				line.Pos = *a;
				line.Dir = *b;
				line.GetNearestPoint(vec_tmp);
				if (line.LastTValue<1.0f && line.LastTValue>0.0f)
				{
					// it's not an "open angle"
					line.Pos = *b;
					line.Dir = *c;
					line.GetNearestPoint(near_on_ab);
					tmp = (near_on_ab-line.LastPointIntersect).Length();
					if ( fabs(dist_from_bc - tmp) > BFCPrecision * prec )
					if (dist_from_bc > tmp) return false;
					if (border)
					{
						if ( fabs(dist_from_bc - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
				} else
				{
					// Check the other edge
					line.Pos = *c;
					line.Dir = *a;
					line.GetNearestPoint(near_on_ab);
					tmp = (near_on_ab-line.LastPointIntersect).Length();
					if ( fabs(dist_from_ca - tmp) > BFCPrecision * prec )
					if (dist_from_ca > tmp) return false;
					if (border)
					{
						if ( fabs(dist_from_ca - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
					return true;
				}
				// Check the other edge
				// First we check if it's not an "open angle"
				line.Pos = *c;
				line.Dir = *a;
				vec_tmp = line.GetTPos(BFCPrecision * prec);
				line.Pos = *a;
				line.Dir = *b;
				line.GetNearestPoint(vec_tmp);
				if (line.LastTValue<1.0f && line.LastTValue>0.0f)
				{
					// it's not an "open angle"
					line.Pos = *c;
					line.Dir = *a;
					line.GetNearestPoint(near_on_ab);
					tmp = (near_on_ab-line.LastPointIntersect).Length();
					if ( fabs(dist_from_ca - tmp) > BFCPrecision * prec )
					if (dist_from_ca > tmp) return false;
					if (border && *border == false)
					{
						if ( fabs(dist_from_ca - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
				}
				// it's ok it's under the line
				return true;
			}break;
		case 2:
			{
				// bc edge case
				// First we check if it's not an "open angle"
				line.Pos = *c;
				line.Dir = *a;
				vec_tmp = line.GetTPos(BFCPrecision * prec);
				line.Pos = *b;
				line.Dir = *c;
				line.GetNearestPoint(vec_tmp);
				if (line.LastTValue<1.0f && line.LastTValue>0.0f)
				{
					// it's not an "open angle"
					line.Pos = *c;
					line.Dir = *a;
					line.GetNearestPoint(near_on_bc);
					tmp = (near_on_bc-line.LastPointIntersect).Length();
					if ( fabs(dist_from_ca - tmp) > BFCPrecision * prec )
					if (dist_from_ca > tmp) return false;
					if (border)
					{
						if ( fabs(dist_from_ca - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
				} else
				{
					// Check the other edge
					line.Pos = *a;
					line.Dir = *b;
					line.GetNearestPoint(near_on_bc);
					tmp = (near_on_bc-line.LastPointIntersect).Length();
					if ( fabs(dist_from_ab - tmp) > BFCPrecision * prec )
					if (dist_from_ab > tmp) return false;
					if (border)
					{
						if ( fabs(dist_from_ab - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
					return true;
				}

				// Check the other edge
				// First we check if it's not an "open angle"
				line.Pos = *a;
				line.Dir = *b;
				vec_tmp = line.GetTPos(BFCPrecision * prec);
				line.Pos = *b;
				line.Dir = *c;
				line.GetNearestPoint(vec_tmp);
				if (line.LastTValue<1.0f && line.LastTValue>0.0f)
				{
					// it's not an "open angle"
					line.Pos = *a;
					line.Dir = *b;
					line.GetNearestPoint(near_on_bc);
					tmp = (near_on_bc-line.LastPointIntersect).Length();
					if ( fabs(dist_from_ab - tmp) > BFCPrecision * prec )
					if (dist_from_ab > tmp) return false;
					if (border && *border == false)
					{
						if ( fabs(dist_from_ab - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
				}

				// it's ok it's under the line
				return true;
			}break;
		case 3:
			{
				// ca edge case
				// First we check if it's not an "open angle"
				line.Pos = *a;
				line.Dir = *b;
				vec_tmp = line.GetTPos(BFCPrecision * prec);
				line.Pos = *c;
				line.Dir = *a;
				line.GetNearestPoint(vec_tmp);
				if (line.LastTValue<1.0f && line.LastTValue>0.0f)
				{
					// it's not an "open angle"
					line.Pos = *a;
					line.Dir = *b;
					line.GetNearestPoint(near_on_ca);
					tmp = (near_on_ca-line.LastPointIntersect).Length();
					if ( fabs(dist_from_ab - tmp) > BFCPrecision * prec )
					if (dist_from_ab > tmp) return false;
					if (border)
					{
						if ( fabs(dist_from_ab - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
				} else
				{
					// Check the other edge
					line.Pos = *b;
					line.Dir = *c;
					line.GetNearestPoint(near_on_ca);
					tmp = (near_on_ca-line.LastPointIntersect).Length();
					if ( fabs(dist_from_bc - tmp) > BFCPrecision * prec )
					if (dist_from_bc > tmp) return false;
					if (border && *border == false)
					{
						if ( fabs(dist_from_bc - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
					return true;
				}
				// Check the other edge
				// First we check if it's not an "open angle"
				line.Pos = *b;
				line.Dir = *c;
				vec_tmp = line.GetTPos(BFCPrecision * prec);
				line.Pos = *c;
				line.Dir = *a;
				line.GetNearestPoint(vec_tmp);
				if (line.LastTValue<1.0f && line.LastTValue>0.0f)
				{				
					line.Pos = *b;
					line.Dir = *c;
					line.GetNearestPoint(near_on_ca);
					tmp = (near_on_ca-line.LastPointIntersect).Length();
					if ( fabs(dist_from_bc - tmp) > BFCPrecision * prec )
					if (dist_from_bc > tmp) return false;
					if (border)
					{
						if ( fabs(dist_from_bc - tmp) < BFCPrecision * prec ) *border = true; else *border = false;
					}
				}
				// it's ok it's under the line
				return true;
			}break;
		}
	}

	return false;
};

BFCBool EqZero(BFCFloat v)
{
	return (fabs(v) < BFCPrecision);
}

BFCBool BFCEqual(BFCFloat v,BFCFloat v2, BFCFloat prec_fac)
{
	return ( fabs(v2 - v) < BFCPrecision * prec_fac );
}
