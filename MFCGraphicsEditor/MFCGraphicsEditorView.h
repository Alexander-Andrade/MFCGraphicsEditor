
// MFCGraphicsEditorView.h : interface of the CMFCGraphicsEditorView class
//

#pragma once

#include "StateMashine.h"

class CMFCGraphicsEditorView : public CView
{
protected: // create from serialization only
	CMFCGraphicsEditorView();
	DECLARE_DYNCREATE(CMFCGraphicsEditorView)

// Attributes
public:
	CMFCGraphicsEditorDoc* GetDocument() const;
	CDC m_dcMem;
	CBitmap m_bmpMem;
	//for moving
	CPoint memPoint;
// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMFCGraphicsEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRectButton();
	afx_msg void OnArrowButton();
	afx_msg void OnTriangleButton();
	afx_msg void OnEllipseButton();
	afx_msg void OnModeButton();
};

#ifndef _DEBUG  // debug version in MFCGraphicsEditorView.cpp
inline CMFCGraphicsEditorDoc* CMFCGraphicsEditorView::GetDocument() const
   { return reinterpret_cast<CMFCGraphicsEditorDoc*>(m_pDocument); }
#endif

