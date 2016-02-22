
// MFCGraphicsEditorDoc.cpp : implementation of the CMFCGraphicsEditorDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCGraphicsEditor.h"
#endif

#include "MFCGraphicsEditorDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCGraphicsEditorDoc

IMPLEMENT_DYNCREATE(CMFCGraphicsEditorDoc, CDocument)

BEGIN_MESSAGE_MAP(CMFCGraphicsEditorDoc, CDocument)
//	ON_COMMAND(ID_RECT_BUTTON, &CMFCGraphicsEditorDoc::OnRectButton)
//	ON_COMMAND(ID_ARROW_BUTTON, &CMFCGraphicsEditorDoc::OnArrowButton)
//	ON_COMMAND(ID_ELLIPSE_BUTTON, &CMFCGraphicsEditorDoc::OnEllipseButton)
//	ON_COMMAND(ID_MODE_BUTTON, &CMFCGraphicsEditorDoc::OnModeButton)
//	ON_COMMAND(ID_TRIANGLE_BUTTON, &CMFCGraphicsEditorDoc::OnTriangleButton)
END_MESSAGE_MAP()


// CMFCGraphicsEditorDoc construction/destruction

CMFCGraphicsEditorDoc::CMFCGraphicsEditorDoc()
{
	// TODO: add one-time construction code here

}

CMFCGraphicsEditorDoc::~CMFCGraphicsEditorDoc()
{
}

BOOL CMFCGraphicsEditorDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	m_stateMashine = StateMashine::InitState;
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	fillFactoryMap();
	return TRUE;
}




// CMFCGraphicsEditorDoc serialization

void CMFCGraphicsEditorDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMFCGraphicsEditorDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMFCGraphicsEditorDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMFCGraphicsEditorDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCGraphicsEditorDoc diagnostics

#ifdef _DEBUG
void CMFCGraphicsEditorDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMFCGraphicsEditorDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

template<typename T>
class FigureFact {
public:
	T* operator() (CPoint& p1, CPoint& p2) {
		return new T(p1, p2);
	}
};


void CMFCGraphicsEditorDoc::fillFactoryMap() {
	_factoryMap[StateMashine::RectCreatute] = FigureFact<Rect>();
	_factoryMap[StateMashine::EllipseCreature] = FigureFact<Ellip>();
	_factoryMap[StateMashine::TriangleCreature] = FigureFact<Triangle>();
	_factoryMap[StateMashine::ArrowCreature] = FigureFact<Arrow>();
}

Shape* CMFCGraphicsEditorDoc::getFigureFromFactory(CPoint& p1, CPoint& p2) {
	return _factoryMap[m_stateMashine](p1, p2);
}

void CMFCGraphicsEditorDoc::addANewShapeToList(CPoint& p1, CPoint& p2) {
	m_figures.push_back( std::unique_ptr<Shape>(getFigureFromFactory(p1, p2))  );
}

void CMFCGraphicsEditorDoc::stretchShapeUnderCreation(CPoint& p2) {
	CArray<CPoint>& points_ref = m_figures.back()->points();
	points_ref[1] = p2;
	//Shape::correctAnchorPoints(points_ref[0],points_ref[1]);
}

