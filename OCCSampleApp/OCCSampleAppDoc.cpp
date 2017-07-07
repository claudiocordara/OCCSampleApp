
// OCCSampleAppDoc.cpp : implementation of the COCCSampleAppDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "OCCSampleApp.h"
#endif

#include "OCCSampleAppDoc.h"

#include <propkey.h>

#include <BRepBuilderAPI.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRep_Tool.hxx>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

// COCCSampleAppDoc

IMPLEMENT_DYNCREATE(COCCSampleAppDoc, CDocument)

BEGIN_MESSAGE_MAP(COCCSampleAppDoc, CDocument)
	ON_COMMAND(ID_TEST_ADDSPHERE, &COCCSampleAppDoc::OnTestAddsphere)
	ON_COMMAND(ID_TEST_MESHINGTEST, &COCCSampleAppDoc::OnTestMeshing)
END_MESSAGE_MAP()


// COCCSampleAppDoc construction/destruction

COCCSampleAppDoc::COCCSampleAppDoc()
{
	// TODO: add one-time construction code here
	Handle(Graphic3d_GraphicDriver) theGraphicDriver = ((COCCSampleAppApp*)AfxGetApp())->GetGraphicDriver();

	// 3D viewer intialization
	TCollection_ExtendedString aNameOfViewer("Visu3d");
	myViewer = new V3d_Viewer(theGraphicDriver, aNameOfViewer.ToExtString());
	myViewer->SetDefaultLights();
	myViewer->SetLightOn();
	myViewer->SetDefaultBackgroundColor(Quantity_NOC_BLACK);

	// Graphic context initialization
	myAISContext = new AIS_InteractiveContext(myViewer);
	myAISContext->SetDisplayMode(AIS_Shaded);
	myAISContext->SetAutomaticHilight(Standard_False);
}

COCCSampleAppDoc::~COCCSampleAppDoc()
{
}

BOOL COCCSampleAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// COCCSampleAppDoc serialization

void COCCSampleAppDoc::Serialize(CArchive& ar)
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
void COCCSampleAppDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
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
void COCCSampleAppDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void COCCSampleAppDoc::SetSearchContent(const CString& value)
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

// COCCSampleAppDoc diagnostics

#ifdef _DEBUG
void COCCSampleAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COCCSampleAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COCCSampleAppDoc commands


/*!
Create a sphere with a given radius and add it to the graphic context

\param Radius
Sphere radius
*/
void COCCSampleAppDoc::AddSphere(double Radius) {

	// Create a sphere object
	BRepPrimAPI_MakeSphere mkSphere(Radius);
	TopoDS_Shape Sphere = mkSphere.Shape();
	sphereShape = new AIS_Shape(Sphere);


	if (sphereShape.IsNull() == false) {

		// Add the new sphere to the graphic context
		myAISContext->SetDisplayMode(sphereShape, AIS_Shaded);
		myAISContext->Display(sphereShape);  // Draw the Sphere on the Screen  
		myAISContext->DisplayAll();

		// Update the viewer
		myViewer->InitActiveViews();
		Handle(V3d_View) activeView = myViewer->ActiveView();
		if (activeView.IsNull() == false) {
			activeView->FitAll();           // Focus to the View to the Drawn Shape.  
		}
	}
}


void COCCSampleAppDoc::OnTestAddsphere() {
	// TODO: aggiungere qui il codice del gestore di comandi
	AddSphere(10);
}


//def simple_mesh() :
//#
//	# Create the shape
//#
//	shape = BRepPrimAPI_MakeBox(200, 200, 200).Shape()
//	theBox = BRepPrimAPI_MakeBox(200, 60, 60).Shape()
//	theSphere = BRepPrimAPI_MakeSphere(gp_Pnt(100, 20, 20), 80).Shape()
//	shape = BRepAlgoAPI_Fuse(theSphere, theBox).Shape()
//#
//	# Mesh the shape
//#
//	BRepMesh_IncrementalMesh(shape, 0.8)
//	builder = BRep_Builder()
//	comp = TopoDS_Compound()
//	builder.MakeCompound(comp)
//
//	bt = BRep_Tool()
//	ex = TopExp_Explorer(shape, TopAbs_FACE)
//	while ex.More() :
//		face = topods_Face(ex.Current())
//		location = TopLoc_Location()
//		facing = (bt.Triangulation(face, location)).GetObject()
//		tab = facing.Nodes()
//		tri = facing.Triangles()
//		for i in range(1, facing.NbTriangles() + 1) :
//			trian = tri.Value(i)
//			index1, index2, index3 = trian.Get()
//			for j in range(1, 4) :
//				if j == 1 :
//					m = index1
//					n = index2
//					elif j == 2 :
//					n = index3
//					elif j == 3 :
//					m = index2
//					me = BRepBuilderAPI_MakeEdge(tab.Value(m), tab.Value(n))
//					if me.IsDone() :
//						builder.Add(comp, me.Edge())
//						ex.Next()
//						display.EraseAll()
//						display.DisplayShape(shape)
//						display.DisplayShape(comp, update = True)

void COCCSampleAppDoc::OnTestMeshing() {
	const Standard_Real aRadius = 10.0;
	const Standard_Real aHeight = 25.0;

	BRepPrimAPI_MakeCylinder aCylinder(aRadius, aHeight);
	TopoDS_Shape aShape = aCylinder.Shape();
	const Standard_Real aLinearDeflection = 0.01;
	const Standard_Real anAngularDeflection = 0.5;
	BRepMesh_IncrementalMesh aMesh(aShape, aLinearDeflection, Standard_False, anAngularDeflection);
	aMesh.Perform();
}
