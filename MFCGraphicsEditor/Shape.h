#pragma once
#include <algorithm>
#include <utility>

// Shape command target
class Shape : public CObject{
	DECLARE_SERIAL(Shape)
protected:
	CArray<CPoint> _points;
	CPen _pen;
	CBrush _brush;
public:
	Shape(CPoint&,CPoint&,int n_points);
	virtual ~Shape();
	
	static void correctAnchorPoints(CPoint& p1,CPoint& p2);
	virtual void Serialize(CArchive& ar) override;
	virtual void draw(CDC* pDC) {}
	virtual void storeStateAndDraw(CDC* pDC) {
		CPen* pOldPen = pDC->SelectObject(&_pen);
		CBrush* pOldBrush = pDC->SelectObject(&_brush);

		draw(pDC);

		pDC->SelectObject(pOldPen);
		pOldPen->DeleteObject();
		pDC->SelectObject(pOldBrush);
		pOldBrush->DeleteObject();
	}
	virtual bool isInternalPoint(CPoint& p);
	CArray<CPoint>& points() { return _points; }
};


class Rect : public Shape{
	DECLARE_SERIAL(Rect)
public:
	Rect(CPoint&, CPoint&);
	~Rect();
	virtual void draw(CDC* pDC) override;
	virtual void Serialize(CArchive& ar) override;
	
};

class Ellip : public Shape{
	DECLARE_SERIAL(Ellip)
public:
	Ellip(CPoint&, CPoint&);
	~Ellip();
	virtual void draw(CDC* pDC) override;
	virtual void Serialize(CArchive& ar) override;
	
};


class Triangle : public Shape{
	DECLARE_SERIAL(Triangle)
private:
	static const int _size = 3;
public:
	Triangle(CPoint&, CPoint&);
	~Triangle();
	CPoint* fromBoundingRect();
	virtual void draw(CDC* pDC) override ;
	virtual void Serialize(CArchive& ar) override;
	
};


class Arrow : public Shape{
	DECLARE_SERIAL(Arrow)
public:
	Arrow(CPoint&, CPoint&);
	~Arrow();
	virtual void draw(CDC* pDC) override;
	virtual void Serialize(CArchive& ar) override;
	
};