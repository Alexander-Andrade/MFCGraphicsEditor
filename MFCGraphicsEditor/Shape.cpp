// Shape.cpp : implementation file
//

#include "stdafx.h"
#include "MFCGraphicsEditor.h"
#include "Shape.h"


// Shape
Shape::Shape(CPoint& topleft = CPoint(0,0),CPoint& bottomright = CPoint(0,0),int n_points = 2) {
	_points.SetSize(n_points);
	_points[0] = topleft;
	_points[1] = bottomright;
	_pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	_brush.CreateSolidBrush(RGB(0, 0, 0));
}
IMPLEMENT_SERIAL(Shape, CObject, 2)
Shape::~Shape(){
}

void Shape::Serialize(CArchive& ar) {
	CObject::Serialize(ar);
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
Arrow::Arrow(CPoint& backend = CPoint(0, 0), CPoint& frontend = CPoint(0, 0)) : Shape(backend, frontend) {
}
IMPLEMENT_SERIAL(Arrow, Shape, 2)
Arrow::~Arrow() {
}

void Arrow::draw(CDC* pDC) {
	return;
}

void Arrow::Serialize(CArchive& ar) {
	Shape::Serialize(ar);
}