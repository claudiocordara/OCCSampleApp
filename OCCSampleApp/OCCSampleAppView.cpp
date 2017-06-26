
// OCCSampleAppView.cpp : implementation of the COCCSampleAppView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OCCSampleApp.h"
#endif

#include "OCCSampleAppDoc.h"
#include "OCCSampleAppView.h"

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif


// COCCSampleAppView

IMPLEMENT_DYNCREATE(COCCSampleAppView, CView)

BEGIN_MESSAGE_MAP(COCCSampleAppView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// COCCSampleAppView construction/destruction

COCCSampleAppView::COCCSampleAppView()
{
	// TODO: add construction code here

}

COCCSampleAppView::~COCCSampleAppView()
{
}

BOOL COCCSampleAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COCCSampleAppView drawing

void COCCSampleAppView::OnDraw(CDC* /*pDC*/)
{
	COCCSampleAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	myView->MustBeResized();
	myView->Update();
}


// COCCSampleAppView printing

BOOL COCCSampleAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COCCSampleAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COCCSampleAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// COCCSampleAppView diagnostics

#ifdef _DEBUG
void COCCSampleAppView::AssertValid() const
{
	CView::AssertValid();
}

void COCCSampleAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COCCSampleAppDoc* COCCSampleAppView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COCCSampleAppDoc)));
	return (COCCSampleAppDoc*)m_pDocument;
}
#endif //_DEBUG


// COCCSampleAppView message handlers


void COCCSampleAppView::OnInitialUpdate() {
	// Create the new OCC view
	myView = GetDocument()->GetViewer()->CreateView();

	if (myView.IsNull() == false) {
		myView->SetShadingModel(V3d_GOURAUD);

		// Link the graphic driver to the MFC view window, creating a new WNT_Window instance
		Handle(Graphic3d_GraphicDriver) theGraphicDriver = ((COCCSampleAppApp*)AfxGetApp())->GetGraphicDriver();
		Aspect_Handle aWindowHandle = (Aspect_Handle)GetSafeHwnd();
		Handle(WNT_Window) aWntWindow = new WNT_Window(GetSafeHwnd());

		// Link the OCC view to the WNT_Window instance
		myView->SetWindow(aWntWindow);
		if (!aWntWindow->IsMapped()) {
			aWntWindow->Map();
		}

		// Get the size of WNT_Window 
		Standard_Integer w = 100;
		Standard_Integer h = 100;
		aWntWindow->Size(w, h);

		// Resize the MFC view window according to WNT_Window size
		::PostMessage(GetSafeHwnd(), WM_SIZE, SIZE_RESTORED, w + h * 65536);

		// Fit the OCC view in its window
		myView->FitAll();

		// Visualize the axis in the bottom left corner
		myView->ZBufferTriedronSetup(Quantity_NOC_RED, Quantity_NOC_GREEN, Quantity_NOC_BLUE1, 0.8, 0.05, 12);
		myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_WHITE, 0.2, V3d_WIREFRAME/*V3d_ZBUFFER*/);
	}
}



void COCCSampleAppView::OnLButtonDown(UINT nFlags, CPoint point) {
	if (myView.IsNull() == false) {
		myView->StartRotation(point.x, point.y);
	}
}


void COCCSampleAppView::OnMouseMove(UINT nFlags, CPoint point) {
	CView::OnMouseMove(nFlags, point);
	if (nFlags && MK_LBUTTON && myView.IsNull() == false) {
		myView->Rotation(point.x, point.y);
	}
}



