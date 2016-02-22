// Shape.cpp : implementation file
//

#include "stdafx.h"
#include "MFCGraphicsEditor.h"
#include "Shape.h"


// Shape
Shape::Shape(CPoint& topleft = CPoint(0,0),CPoint& bottomright = CPoint(0,0),int n_points = 2) {
	_points.resize(n_points);
	_points[0] = topleft;
	_points[1] = bottomright;
	_pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	_brush.CreateSolidBrush(RGB(255, 0, 0));
}
IMPLEMENT_SERIAL(Shape, CObject, 2)
Shape::~Shape(){
}

void Shape::Serialize(CArchive& ar) {
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		ar << _points.size();
		for (CPoint& p : _points)
			ar << p;
		//ar << _pen;
		//ar << _brush;
	}
	else {
		int size;
		ar >> size;
		_points.resize(size);
		for (CPoint& p : _points)
			ar >> p;
		//_pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		//_brush.CreateSolidBrush(RGB(255, 0, 0));
	}
}

void Shape::correctAnchorPoints(CPoint& p1, CPoint& p2) {
	if (p1.x > p2.x)
		std::swap(p1.x, p2.x);
	if (p1.y > p2.y)
		std::swap(p1.y, p2.y);
}

void Shape::move(CPoint& delta) {
	for (CPoint& p : _points)
		p += delta;
}

bool Shape::isInternalPoint(CPoint& p) {
	//is internal point of the bounding rect
	std::pair<int, int> x_coords = std::minmax(_points[0].x,_points[1].x);
	std::pair<int, int> y_coord = std::minmax(_points[0].y, _points[1].y);
	return p.x > x_coords.first && p.x < x_coords.second && p.y > y_coord.first && p.y < y_coord.second;
}

CRect Shape::getBoundingRect() {
	// ? may be correctAnchorPoints
	return CRect(_points[0],_points[1]);
}

// Rectangle
Rect::Rect(CPoint& topleft = CPoint(0, 0), CPoint& bottomright = CPoint(0, 0)) : Shape(topleft, bottomright) {
}
IMPLEMENT_SERIAL(Rect, Shape, 2)
Rect::~Rect(){
}

void Rect::draw(CDC* pDC) {
	pDC->Rectangle(CRect(_points[0],_points[1]));
}

void Rect::Serialize(CArchive& ar) {
	Shape::Serialize(ar);
}

// Circle
Ellip::Ellip(CPoint& topleft = CPoint(0, 0), CPoint& bottomright = CPoint(0, 0)) : Shape(topleft, bottomright) {
}
IMPLEMENT_SERIAL(Ellip, Shape, 2)
Ellip::~Ellip() {

}

void Ellip::Serialize(CArchive& ar) {
	Shape::Serialize(ar);
}

void Ellip::draw(CDC* pDC) {
	pDC->Ellipse(CRect(_points[0], _points[1]));
}



//Triangle
Triangle::Triangle(CPoint& topleft = CPoint(0, 0), CPoint& bottomright = CPoint(0, 0)) : Shape(topleft, bottomright) {
}
IMPLEMENT_SERIAL(Triangle, Shape, 2)
Triangle::~Triangle() {
}

void Triangle::Serialize(CArchive& ar) {
	Shape::Serialize(ar);
}

CPoint* ::Triangle::fromBoundingRect() {
	CPoint *points = new CPoint[_size];
	int mean_x = (_points[1].x + _points[0].x) / 2;
	points[0] = CPoint(mean_x,_points[0].y);
	points[1] = _points[1];
	points[2] = CPoint(_points[0].x,_points[1].y);
	return points;
}
void Triangle::draw(CDC* pDC) {
	CPoint* points = fromBoundingRect();
	pDC->Polygon(points,_size);
	delete[] points;
}


//Arrow
Arrow::Arrow(CPoint& backend = CPoint(0, 0), CPoint& frontend = CPoint(0, 0)) : Shape(backend, frontend) ,
_backFig(nullptr),_frontFig(nullptr){
}
IMPLEMENT_SERIAL(Arrow, Shape, 2)
Arrow::~Arrow() {
}

void Arrow::draw(CDC* pDC) {
	if (_backFig == nullptr) {
		pDC->MoveTo(_points[0]);
		pDC->LineTo(_points[1]);
	}
	else {
		pDC->MoveTo(_backFig->getBoundingRect().CenterPoint());
		pDC->LineTo(_frontFig->getBoundingRect().CenterPoint());
	}
	
}

void Arrow::Serialize(CArchive& ar) {
	Shape::Serialize(ar);
	if (ar.IsStoring()) {
		ar << _backFig->points()[0] << _frontFig->points()[1];
	}
	else{
		ar >> _points[0] >> _points[1];
	}
}