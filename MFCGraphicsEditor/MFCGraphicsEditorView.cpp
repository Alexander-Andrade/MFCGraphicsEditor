
// MFCGraphicsEditorView.cpp : implementation of the CMFCGraphicsEditorView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCGraphicsEditor.h"
#endif

#include "MFCGraphicsEditorDoc.h"
#include "MFCGraphicsEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCGraphicsEditorView

IMPLEMENT_DYNCREATE(CMFCGraphicsEditorView, CView)

BEGIN_MESSAGE_MAP(CMFCGraphicsEditorView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCGraphicsEditorView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_RECT_BUTTON, &CMFCGraphicsEditorView::OnRectButton)
	ON_COMMAND(ID_ARROW_BUTTON, &CMFCGraphicsEditorView::OnArrowButton)
	ON_COMMAND(ID_TRIANGLE_BUTTON, &CMFCGraphicsEditorView::OnTriangleButton)
	ON_COMMAND(ID_ELLIPSE_BUTTON, &CMFCGraphicsEditorView::OnEllipseButton)
	ON_COMMAND(ID_MODE_BUTTON, &CMFCGraphicsEditorView::OnModeButton)
END_MESSAGE_MAP()

// CMFCGraphicsEditorView construction/destruction

CMFCGraphicsEditorView::CMFCGraphicsEditorView()
{
	// TODO: add construction code here

}

CMFCGraphicsEditorView::~CMFCGraphicsEditorView()
{
}

BOOL CMFCGraphicsEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMFCGraphicsEditorView drawing

void CMFCGraphicsEditorView::OnDraw(CDC* pDC)
{
	CMFCGraphicsEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	if ( ! pDoc->m_figures.empty()) {
		CRect rect;
		GetClientRect(&rect);
		m_dcMem.FillSolidRect(rect, RGB(255, 255, 255));

		for(std::unique_ptr<Shape>& sh_ptr : pDoc->m_figures)
			sh_ptr->storeStateAndDraw(&m_dcMem);

		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcMem, 0, 0, SRCCOPY);
	}
}


// CMFCGraphicsEditorView printing


void CMFCGraphicsEditorView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCGraphicsEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMFCGraphicsEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMFCGraphicsEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMFCGraphicsEditorView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMFCGraphicsEditorView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCGraphicsEditorView diagnostics

#ifdef _DEBUG
void CMFCGraphicsEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCGraphicsEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCGraphicsEditorDoc* CMFCGraphicsEditorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCGraphicsEditorDoc)));
	return (CMFCGraphicsEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCGraphicsEditorView message handlers


int CMFCGraphicsEditorView::OnCreate(LPCREATESTRUCT lpCreateStruct){
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CClientDC dc(this);
	int x = ::GetSystemMetrics(SM_CXSCREEN);
	int y = ::GetSystemMetrics(SM_CYSCREEN);
	m_dcMem.CreateCompatibleDC(&dc);
	m_bmpMem.CreateCompatibleBitmap(&dc, x, y);
	m_dcMem.SelectObject(&m_bmpMem);
	return 0;
}


void CMFCGraphicsEditorView::OnLButtonDown(UINT nFlags, CPoint point){
	// TODO: Add your message handler code here and/or call default
	CMFCGraphicsEditorDoc* doc =  GetDocument();
	//create new figure
	if (nFlags == MK_LBUTTON && doc->m_stateMashine != StateMashine::Motion && doc->m_stateMashine != StateMashine::InitState)
		doc->addANewShapeToList(point, point);
	//try to move existing figures
	else if (nFlags == MK_LBUTTON && doc->m_stateMashine == StateMashine::Motion)
		memPoint = point;

	CView::OnLButtonDown(nFlags, point);
}


BOOL CMFCGraphicsEditorView::OnEraseBkgnd(CDC* pDC){
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}


void CMFCGraphicsEditorView::OnLButtonUp(UINT nFlags, CPoint point){
	//complete arrow creation
	CMFCGraphicsEditorDoc* doc = GetDocument();
	if (doc->m_stateMashine == StateMashine::ArrowCreature)
		doc->finishArrowCreation(point);


	CView::OnLButtonUp(nFlags, point);
}


void CMFCGraphicsEditorView::OnMouseMove(UINT nFlags, CPoint point)
{
	CMFCGraphicsEditorDoc* doc = GetDocument();
	// continue of figure creation
	if (nFlags == MK_LBUTTON && doc->m_stateMashine != StateMashine::Motion && doc->m_stateMashine != StateMashine::InitState)
		doc->stretchShapeUnderCreation(point);
	//shift all the object pixels on the delta value
	else if (nFlags == MK_LBUTTON && doc->m_stateMashine == StateMashine::Motion) {
		CPoint delta = point - memPoint;
		Shape* fig = doc->getFigUnderThePoint(point);
		if(fig != nullptr)
			fig->move(delta);
		memPoint = point;
	}


	doc->SetModifiedFlag(TRUE);
	Invalidate();
	CView::OnMouseMove(nFlags, point);
}


void CMFCGraphicsEditorView::OnRectButton()
{
	CMFCGraphicsEditorDoc* doc = GetDocument();
	doc->m_stateMashine = StateMashine::RectCreatute;
}


void CMFCGraphicsEditorView::OnArrowButton()
{
	CMFCGraphicsEditorDoc* doc = GetDocument();
	doc->m_stateMashine = StateMashine::ArrowCreature;
}


void CMFCGraphicsEditorView::OnTriangleButton()
{
	CMFCGraphicsEditorDoc* doc = GetDocument();
	doc->m_stateMashine = StateMashine::TriangleCreature;
}


void CMFCGraphicsEditorView::OnEllipseButton()
{
	CMFCGraphicsEditorDoc* doc = GetDocument();
	doc->m_stateMashine = StateMashine::EllipseCreature;
}


void CMFCGraphicsEditorView::OnModeButton()
{
	CMFCGraphicsEditorDoc* doc = GetDocument();
	//toggle mode
	if (doc->m_stateMashine != StateMashine::Motion)
		doc->m_stateMashine = StateMashine::Motion;
	else
		doc->m_stateMashine = StateMashine::InitState;
}
