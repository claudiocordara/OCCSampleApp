
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
#include <BRepMesh.hxx>
#include <StlTransfer.hxx>
#include <Geom_BSplineSurface.hxx>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

// COCCSampleAppDoc

IMPLEMENT_DYNCREATE(COCCSampleAppDoc, CDocument)

BEGIN_MESSAGE_MAP(COCCSampleAppDoc, CDocument)
	ON_COMMAND(ID_TEST_ADDSPHERE, &COCCSampleAppDoc::OnTestAddsphere)
	ON_COMMAND(ID_TEST_MESHINGTEST, &COCCSampleAppDoc::OnTestMeshing)
	ON_COMMAND(ID_TEST_ADDNURBSSURFACE, &COCCSampleAppDoc::OnTestAddNurbsSurface)
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


void COCCSampleAppDoc::OnTestMeshing() {
	const Standard_Real aRadius = 10.0;
	const Standard_Real aHeight = 25.0;

	BRepPrimAPI_MakeCylinder aCylinder(aRadius, aHeight);
	TopoDS_Shape aShape = aCylinder.Shape();

	//const Standard_Real aLinearDeflection = 0.01;
	//const Standard_Real anAngularDeflection = 0.5;
	//BRepMesh_IncrementalMesh aMesher(aShape, aLinearDeflection, Standard_False, anAngularDeflection);
	//Handle(StlMesh_Mesh) aShapeMesh = new StlMesh_Mesh;
	//StlTransfer::RetrieveMesh(aShape, aShapeMesh);

	Handle(StlMesh_Mesh) aShapeMesh;
	generateMesh(aShape, 0.01, 0.5, aShapeMesh);

	saveMesh(aShapeMesh, L"F:\\Sviluppo\\OCCSampleApp\\shapeMesh.stl");
}



bool COCCSampleAppDoc::generateMesh(const TopoDS_Shape &_shape, Standard_Real _linearDeflection, Standard_Real _angularDeflection, Handle(StlMesh_Mesh) &_mesh) {
	BRepMesh_IncrementalMesh aMesher(_shape, _linearDeflection, Standard_False, _angularDeflection);
	_mesh = new StlMesh_Mesh;
	StlTransfer::RetrieveMesh(_shape, _mesh);
	return true;
}



bool COCCSampleAppDoc::saveMesh(const Handle(StlMesh_Mesh) &_mesh, std::wstring _fileName) {
	bool ret = false;
	std::ofstream outputFile;
	outputFile.open(_fileName.c_str(), std::ofstream::out | std::ofstream::binary);
	if (outputFile.good() == true) {
		time_t t;
		struct tm t_info;
		time(&t);
		localtime_s(&t_info, &t);
		char timeStr[1024];
		asctime_s(timeStr, &t_info);
		std::string headerStr = "stl_offset  ";
		headerStr += timeStr;
		for (unsigned int i = 0; outputFile.good() && i < 80; i++) {
			if (i < headerStr.length()) {
				outputFile.put(headerStr[i]);
			}
			else {
				outputFile.put('\n');
			}
		}
		if (outputFile.good()) {
			unsigned int triangNum = _mesh->NbTriangles();// numFace();
			outputFile.write((char*)(&triangNum), sizeof(triangNum));
			unsigned short attr = 0;

			for (int domIndex = 1; outputFile.good() && domIndex <= _mesh->NbDomains(); domIndex++) {
				TColgp_SequenceOfXYZ vertexArray = _mesh->Vertices(domIndex);
				StlMesh_SequenceOfMeshTriangle facesArray = _mesh->Triangles(domIndex);
				for (int i = 1; i <= facesArray.Size(); i++) {
					float val = 0.0;

					Standard_Real nX = 0;
					Standard_Real nY = 0;
					Standard_Real nZ = 0;
					Standard_Integer v1Index = 0;
					Standard_Integer v2Index = 0;
					Standard_Integer v3Index = 0;
					(facesArray.Value(i))->GetVertexAndOrientation(v1Index, v2Index, v3Index, nX, nY, nZ);
					val = (float)nX;
					outputFile.write((char*)(&val), sizeof(val));
					val = (float)nY;
					outputFile.write((char*)(&val), sizeof(val));
					val = (float)nZ;
					outputFile.write((char*)(&val), sizeof(val));

					val = (float)(vertexArray.Value(v1Index).X());
					outputFile.write((char*)(&val), sizeof(val));
					val = (float)(vertexArray.Value(v1Index).Y());
					outputFile.write((char*)(&val), sizeof(val));
					val = (float)(vertexArray.Value(v1Index).Z());
					outputFile.write((char*)(&val), sizeof(val));

					val = (float)(vertexArray.Value(v2Index).X());
					outputFile.write((char*)(&val), sizeof(val));
					val = (float)(vertexArray.Value(v2Index).Y());
					outputFile.write((char*)(&val), sizeof(val));
					val = (float)(vertexArray.Value(v2Index).Z());
					outputFile.write((char*)(&val), sizeof(val));

					val = (float)(vertexArray.Value(v3Index).X());
					outputFile.write((char*)(&val), sizeof(val));
					val = (float)(vertexArray.Value(v3Index).Y());
					outputFile.write((char*)(&val), sizeof(val));
					val = (float)(vertexArray.Value(v3Index).Z());
					outputFile.write((char*)(&val), sizeof(val));

					outputFile.write((char*)(&attr), sizeof(attr));
				}
				//for (i = 0; outputFile.good() && i < triangNum; i++) {
				//	float val = 0.0;
				//	for (unsigned int j = 0; outputFile.good() && j < 12; j++) {
				//		if (j < 3) {
				//			val = (float)(mFaces[i]->n[j]);
				//		}
				//		else {
				//			val = (float)((mFaces[i]->vNdx[(j - 3) / 3])->v[(j - 3) % 3]);
				//		}
				//		_fstream.write((char*)(&val), sizeof(val));
				//	}
				//	_fstream.write((char*)(&(mFaces[i]->attr)), sizeof(mFaces[i]->attr));
				//}
			}


			if (outputFile.good()) {
				ret = true;
			}
		}
	}
	outputFile.close();
	return ret;
}


void COCCSampleAppDoc::OnTestAddNurbsSurface() {
	TColgp_Array2OfPnt cpArray(0, 3, 0, 3);
	cpArray.SetValue(0, 0, gp_Pnt(-6, -6, 0));
	cpArray.SetValue(0, 1, gp_Pnt(-6, -2, 0));
	cpArray.SetValue(0, 2, gp_Pnt(-6, 2, 0));
	cpArray.SetValue(0, 3, gp_Pnt(-6, 6, 0));
	cpArray.SetValue(1, 0, gp_Pnt(-2, -6, 0));
	cpArray.SetValue(1, 1, gp_Pnt(-2, -2, 8));
	cpArray.SetValue(1, 2, gp_Pnt(-2, 2, 8));
	cpArray.SetValue(1, 3, gp_Pnt(-2, 6, 0));
	cpArray.SetValue(2, 0, gp_Pnt(2, -6, 0));
	cpArray.SetValue(2, 1, gp_Pnt(2, -2, 8));
	cpArray.SetValue(2, 2, gp_Pnt(2, 2, 8));
	cpArray.SetValue(2, 3, gp_Pnt(2, 6, 0));
	cpArray.SetValue(3, 0, gp_Pnt(6, -6, 0));
	cpArray.SetValue(3, 1, gp_Pnt(6, -2, 0));
	cpArray.SetValue(3, 2, gp_Pnt(6, 2, 0));
	cpArray.SetValue(3, 3, gp_Pnt(6, 6, 0));

	TColStd_Array1OfReal uKnotsArray(0, 1);
	uKnotsArray.SetValue(0, 0);
	uKnotsArray.SetValue(1, 1);

	TColStd_Array1OfReal vKnotsArray(0, 1);
	vKnotsArray.SetValue(0, 0);
	vKnotsArray.SetValue(1, 1);

	TColStd_Array1OfInteger uMultsArray(0, 1);
	uMultsArray.SetValue(0, 4);
	uMultsArray.SetValue(1, 4);

	TColStd_Array1OfInteger vMultsArray(0, 1);
	vMultsArray.SetValue(0, 4);
	vMultsArray.SetValue(1, 4);

	Handle(Geom_BSplineSurface) nurbsSurf;
	nurbsSurf = new Geom_BSplineSurface(cpArray, uKnotsArray, vKnotsArray, uMultsArray, vMultsArray, 3, 3);
	
	BRepBuilderAPI_MakeFace faceMaker(nurbsSurf, Precision::Confusion());
	TopoDS_Face Nurbsface = faceMaker.Face();

	nurbsShape = new AIS_Shape(Nurbsface);
	if (nurbsShape.IsNull() == false) {

		// Add the new sphere to the graphic context
		myAISContext->SetDisplayMode(nurbsShape, AIS_Shaded);
		myAISContext->Display(nurbsShape);  // Draw the Sphere on the Screen  
		myAISContext->DisplayAll();

		// Update the viewer
		myViewer->InitActiveViews();
		Handle(V3d_View) activeView = myViewer->ActiveView();
		if (activeView.IsNull() == false) {
			activeView->FitAll();           // Focus to the View to the Drawn Shape.  
		}
	}


	Handle(StlMesh_Mesh) aShapeMesh;
	generateMesh(Nurbsface, 0.01, 0.5, aShapeMesh);
	saveMesh(aShapeMesh, L"F:\\Sviluppo\\OCCSampleApp\\nurbsMesh.stl");
}
