
// MFCGraphicsEditorDoc.h : interface of the CMFCGraphicsEditorDoc class
//


#pragma once
#include "Shape.h"
#include "StateMashine.h"
#include <map>
#include <list>
#include <functional>
#include <initializer_list>
#include <memory>

class CMFCGraphicsEditorDoc : public CDocument
{
protected: // create from serialization only
	CMFCGraphicsEditorDoc();
	DECLARE_DYNCREATE(CMFCGraphicsEditorDoc)

// Attributes
public:
	std::list<std::unique_ptr<Shape>> m_figures;
	StateMashine m_stateMashine;
	std::map<StateMashine, std::function<Shape*(CPoint& p1, CPoint& p2)>> _factoryMap;
// Operations
public:
	void fillFactoryMap();
	Shape* getFigureFromFactory(CPoint& p1, CPoint& p2);
	void addANewShapeToList(CPoint& p1,CPoint& p2);
	void stretchShapeUnderCreation(CPoint& p);
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CMFCGraphicsEditorDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
};
