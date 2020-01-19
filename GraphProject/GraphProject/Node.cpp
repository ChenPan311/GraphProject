#include <iostream>
#include "Node.h"
using namespace std;

int Node::count = 0;

// Node definitions 
 Node::Node(const CPoint &p, const COLORREF &c = RGB(255, 255, 255)) :point(p), borderColor(c) { 
	 id = ++count;
}

 // nodeRectangle definitions
nodeRectangle::nodeRectangle(const CPoint &p, const COLORREF &c) :Node(p, c) {

}


void nodeRectangle::Draw(CDC * dc) { // Rectangle drawing
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, borderColor);
	dc->SelectObject(&pen);
	dc->Rectangle(point.x - SHAPE_AREA, point.y - SHAPE_AREA,
		point.x + SHAPE_AREA, point.y + SHAPE_AREA);
}

IMPLEMENT_SERIAL(nodeRectangle, CObject, 1)
void nodeRectangle::Serialize(CArchive &ar) {
	CObject::Serialize(ar);
	int type = 1;
	if (ar.IsStoring()) {
		ar << type << point << borderColor << id;
	}
	else {
		ar >> point >> borderColor >> id;
	}
}

bool nodeRectangle::PtInShape(const CPoint &p) {
	CRect rect;
	rect.SetRect(point.x - SHAPE_AREA, point.y - SHAPE_AREA,
		point.x + SHAPE_AREA, point.y + SHAPE_AREA);

	return rect.PtInRect(p);
}

// nodeEllipse definitions
nodeEllipse::nodeEllipse(const CPoint &p, const COLORREF &c) :Node(p, c) {

}

void nodeEllipse::Draw(CDC * dc) { // Ellipse drawing
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, borderColor);
	dc->SelectObject(&pen);
	dc->Ellipse(point.x - SHAPE_AREA, point.y - SHAPE_AREA,
		point.x + SHAPE_AREA, point.y + SHAPE_AREA);
}


IMPLEMENT_SERIAL(nodeEllipse, CObject, 1)
void nodeEllipse::Serialize(CArchive &ar) {
	CObject::Serialize(ar);
	int type = 0;
	if (ar.IsStoring()) {
		ar << type << point << borderColor << id;
	}
	else {
		ar >> point >> borderColor >> id;
	}
}

bool nodeEllipse::PtInShape(const CPoint &p) {
	double result = (p.x - point.x)*(p.x - point.x) + (p.y - point.y)*(p.y - point.y); // (x - a)^2 + (y - b)^2

	return result <= (SHAPE_AREA*SHAPE_AREA); // if point if smaller than radious^2 then the point is inside of the circle 
}

// nodeTriangle definitions
nodeTriangle::nodeTriangle(const CPoint &p, const COLORREF &c) :Node(p, c) {

}

void nodeTriangle::Draw(CDC * dc) { // triangle drawing
	CPen pen;
	CPoint arr[3];
	arr[0] = CPoint(point.x - SHAPE_AREA, point.y + SHAPE_AREA);
	arr[1] = CPoint(point.x + SHAPE_AREA, point.y + SHAPE_AREA);
	arr[2] = CPoint(point.x, point.y - SHAPE_AREA);
	pen.CreatePen(PS_SOLID, 2, borderColor);
	dc->SelectObject(&pen);
	dc->Polygon(arr, 3);
}

double areaOfTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	double area = (x1*(y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0;
	return area > 0 ? area : (-1) * area;
}

bool nodeTriangle::PtInShape(const CPoint &p) {
	double triangleArea = areaOfTriangle(point.x, point.y - SHAPE_AREA, 
		point.x - SHAPE_AREA, point.y + SHAPE_AREA,
		point.x + SHAPE_AREA, point.y + SHAPE_AREA);
	double PAB = areaOfTriangle(p.x, p.y,
		point.x, point.y - SHAPE_AREA,
		point.x - SHAPE_AREA, point.y + SHAPE_AREA);
	double PBC = areaOfTriangle(p.x, p.y,
		point.x - SHAPE_AREA, point.y + SHAPE_AREA,
		point.x + SHAPE_AREA, point.y + SHAPE_AREA);
	double PAC = areaOfTriangle(p.x, p.y,
		point.x, point.y - SHAPE_AREA,
		point.x + SHAPE_AREA, point.y + SHAPE_AREA);

	return triangleArea == PAB + PBC + PAC;
}

IMPLEMENT_SERIAL(nodeTriangle, CObject, 1)
void nodeTriangle::Serialize(CArchive& ar) {
	CObject::Serialize(ar);
	int type = 2;
	if (ar.IsStoring()) {
		ar << type << point << borderColor << id;
	}
	else {
		ar >> point >> borderColor >> id;
	}
}
