
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
#include <GeomConvert.hxx>
#include <BRepBuilderAPI_NurbsConvert.hxx>
#include <BRepLib_FindSurface.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <Geom2d_BSplineCurve.hxx>
#include <Geom2dConvert.hxx>
#include <BRepCheck_Analyzer.hxx>
#include <ShapeFix_Face.hxx>
#include <Geom2d_Line.hxx>

//#ifdef _DEBUG
//#define new DEBUG_NEW
//#endif

// COCCSampleAppDoc

IMPLEMENT_DYNCREATE(COCCSampleAppDoc, CDocument)

BEGIN_MESSAGE_MAP(COCCSampleAppDoc, CDocument)
	ON_COMMAND(ID_TEST_ADDSPHERE, &COCCSampleAppDoc::OnTestAddsphere)
	ON_COMMAND(ID_TEST_MESHINGTEST, &COCCSampleAppDoc::OnTestMeshing)
	ON_COMMAND(ID_TEST_ADDNURBSSURFACE, &COCCSampleAppDoc::OnTestAddNurbsSurface)
	ON_COMMAND(ID_TEST_ADDRATIONALNURBSSURFACE, &COCCSampleAppDoc::OnTestAddRationalNurbsSurface)
	ON_COMMAND(ID_TEST_NURBSDUMP, &COCCSampleAppDoc::OnTestNurbsDump)
	ON_COMMAND(ID_TEST_ADDBOXWITHHOLE, &COCCSampleAppDoc::OnTestAddBoxWithHole)
	ON_COMMAND(ID_TEST_ADDCIRCLEWITHHOLE, &COCCSampleAppDoc::OnTestAddCircleWithHole)
	ON_COMMAND(ID_TEST_ADDTRIMMEDNURBSSURFACE, &COCCSampleAppDoc::OnTestAddTrimmedNurbsSurface)
	ON_COMMAND(ID_TEST_ADDDUMPEDNURBSSURFACE, &COCCSampleAppDoc::OnTestAddDumpedNurbsSurface)
	ON_COMMAND(ID_TEST_ADDCONE, &COCCSampleAppDoc::OnTestAddCone)
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


void COCCSampleAppDoc::AddCone() {
	BRepPrimAPI_MakeCone mkCone(10, 0, 10);
	TopoDS_Shape Cone = mkCone.Shape();
	coneShape = new AIS_Shape(Cone);

	if (coneShape.IsNull() == false) {

		// Add the new sphere to the graphic context
		myAISContext->SetDisplayMode(coneShape, AIS_Shaded);
		myAISContext->Display(coneShape);  // Draw the Sphere on the Screen  
		myAISContext->DisplayAll();

		// Update the viewer
		myViewer->InitActiveViews();
		Handle(V3d_View) activeView = myViewer->ActiveView();
		if (activeView.IsNull() == false) {
			activeView->FitAll();           // Focus to the View to the Drawn Shape.  
		}
	}
}


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
	TColgp_Array2OfPnt cpArray(1, 2, 1, 2);
	cpArray.SetValue(1, 1, gp_Pnt(-50, -50, 0));
	cpArray.SetValue(1, 2, gp_Pnt(-50, 50, 0));
	cpArray.SetValue(2, 1, gp_Pnt(50, -50, 0));
	cpArray.SetValue(2, 2, gp_Pnt(50, 50, 0));
	TColStd_Array1OfReal uKnotsArray(1, 2);
	uKnotsArray.SetValue(1, -50);
	uKnotsArray.SetValue(2, 50);
	TColStd_Array1OfInteger uMultsArray(1, 2);
	uMultsArray.SetValue(1, 2);
	uMultsArray.SetValue(2, 2);
	TColStd_Array1OfReal vKnotsArray(1, 2);
	vKnotsArray.SetValue(1, -50);
	vKnotsArray.SetValue(2, 50);
	TColStd_Array1OfInteger vMultsArray(1, 2);
	vMultsArray.SetValue(1, 2);
	vMultsArray.SetValue(2, 2);
	Handle(Geom_BSplineSurface) nurbsSurf;
	nurbsSurf = new Geom_BSplineSurface(cpArray, uKnotsArray, vKnotsArray, uMultsArray, vMultsArray, 1, 1, Standard_False, Standard_False);

	//TColgp_Array2OfPnt cpArray(0, 3, 0, 3);
	//cpArray.SetValue(0, 0, gp_Pnt(-6, -6, 0));
	//cpArray.SetValue(0, 1, gp_Pnt(-6, -2, 0));
	//cpArray.SetValue(0, 2, gp_Pnt(-6, 2, 0));
	//cpArray.SetValue(0, 3, gp_Pnt(-6, 6, 0));
	//cpArray.SetValue(1, 0, gp_Pnt(-2, -6, 0));
	//cpArray.SetValue(1, 1, gp_Pnt(-2, -2, 8));
	//cpArray.SetValue(1, 2, gp_Pnt(-2, 2, 8));
	//cpArray.SetValue(1, 3, gp_Pnt(-2, 6, 0));
	//cpArray.SetValue(2, 0, gp_Pnt(2, -6, 0));
	//cpArray.SetValue(2, 1, gp_Pnt(2, -2, 8));
	//cpArray.SetValue(2, 2, gp_Pnt(2, 2, 8));
	//cpArray.SetValue(2, 3, gp_Pnt(2, 6, 0));
	//cpArray.SetValue(3, 0, gp_Pnt(6, -6, 0));
	//cpArray.SetValue(3, 1, gp_Pnt(6, -2, 0));
	//cpArray.SetValue(3, 2, gp_Pnt(6, 2, 0));
	//cpArray.SetValue(3, 3, gp_Pnt(6, 6, 0));

	//TColStd_Array1OfReal uKnotsArray(0, 1);
	//uKnotsArray.SetValue(0, 0);
	//uKnotsArray.SetValue(1, 1);

	//TColStd_Array1OfReal vKnotsArray(0, 1);
	//vKnotsArray.SetValue(0, 0);
	//vKnotsArray.SetValue(1, 1);

	//TColStd_Array1OfInteger uMultsArray(0, 1);
	//uMultsArray.SetValue(0, 4);
	//uMultsArray.SetValue(1, 4);

	//TColStd_Array1OfInteger vMultsArray(0, 1);
	//vMultsArray.SetValue(0, 4);
	//vMultsArray.SetValue(1, 4);

	//Handle(Geom_BSplineSurface) nurbsSurf;
	//nurbsSurf = new Geom_BSplineSurface(cpArray, uKnotsArray, vKnotsArray, uMultsArray, vMultsArray, 3, 3);
	
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
	saveMesh(aShapeMesh, L"C:\\Sviluppo\\OCCSampleApp\\nurbsMesh.stl");
}


void COCCSampleAppDoc::OnTestAddRationalNurbsSurface() {
	TColgp_Array2OfPnt cpArray(1, 7, 1, 2);
	TColStd_Array2OfReal wghArray(1, 7, 1, 2);
	cpArray.SetValue(1, 1, gp_Pnt(10, 0, 0));
	cpArray.SetValue(1, 2, gp_Pnt(10, 0, 25));
	cpArray.SetValue(2, 1, gp_Pnt(10, 17.3205, 0));
	cpArray.SetValue(2, 2, gp_Pnt(10, 17.3205, 25));
	cpArray.SetValue(3, 1, gp_Pnt(-5, 8.66025, 0));
	cpArray.SetValue(3, 2, gp_Pnt(-5, 8.66025, 25));
	cpArray.SetValue(4, 1, gp_Pnt(-20, 2.44929e-15, 0));
	cpArray.SetValue(4, 2, gp_Pnt(-20, 2.44929e-15, 25));
	cpArray.SetValue(5, 1, gp_Pnt(-5, -8.66025, 0));
	cpArray.SetValue(5, 2, gp_Pnt(-5, -8.66025, 25));
	cpArray.SetValue(6, 1, gp_Pnt(10, -17.3205, 0));
	cpArray.SetValue(6, 2, gp_Pnt(10, -17.3205, 25));
	cpArray.SetValue(7, 1, gp_Pnt(10, -2.44929e-15, 0));
	cpArray.SetValue(7, 2, gp_Pnt(10, -2.44929e-15, 25));
	wghArray.SetValue(1, 1, 1);
	wghArray.SetValue(1, 2, 1);
	wghArray.SetValue(2, 1, 0.5);
	wghArray.SetValue(2, 2, 0.5);
	wghArray.SetValue(3, 1, 1);
	wghArray.SetValue(3, 2, 1);
	wghArray.SetValue(4, 1, 0.5);
	wghArray.SetValue(4, 2, 0.5);
	wghArray.SetValue(5, 1, 1);
	wghArray.SetValue(5, 2, 1);
	wghArray.SetValue(6, 1, 0.5);
	wghArray.SetValue(6, 2, 0.5);
	wghArray.SetValue(7, 1, 1);
	wghArray.SetValue(7, 2, 1);
	TColStd_Array1OfReal uKnotsArray(1, 4);
	uKnotsArray.SetValue(1, 0);
	uKnotsArray.SetValue(2, 2.0944);
	uKnotsArray.SetValue(3, 4.18879);
	uKnotsArray.SetValue(4, 6.28319);
	TColStd_Array1OfInteger uMultsArray(1, 4);
	uMultsArray.SetValue(1, 3);
	uMultsArray.SetValue(2, 2);
	uMultsArray.SetValue(3, 2);
	uMultsArray.SetValue(4, 3);
	TColStd_Array1OfReal vKnotsArray(1, 2);
	vKnotsArray.SetValue(1, 0);
	vKnotsArray.SetValue(2, 25);
	TColStd_Array1OfInteger vMultsArray(1, 2);
	vMultsArray.SetValue(1, 2);
	vMultsArray.SetValue(2, 2);
	Handle(Geom_BSplineSurface) nurbsSurf;
	nurbsSurf = new Geom_BSplineSurface(cpArray, wghArray, uKnotsArray, vKnotsArray, uMultsArray, vMultsArray, 2, 1, Standard_False, Standard_False);

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
	saveMesh(aShapeMesh, L"F:\\Sviluppo\\OCCSampleApp\\nurbsRatMesh.stl");

	//vCP.clear();
	//std::vector<double> uKnotArray;
	//uKnotArray.push_back(0); 
	//uKnotArray.push_back(0); 
	//uKnotArray.push_back(0); 
	//uKnotArray.push_back(15.7863); 
	//uKnotArray.push_back(15.7863); 
	//uKnotArray.push_back(31.5726); 
	//uKnotArray.push_back(31.5726); 
	//uKnotArray.push_back(47.3589); 
	//uKnotArray.push_back(47.3589); 
	//uKnotArray.push_back(63.1452); 
	//uKnotArray.push_back(63.1452); 
	//uKnotArray.push_back(63.1452);
	//std::vector<double> vKnotArray;
	//vKnotArray.push_back(0); 
	//vKnotArray.push_back(0); 
	//vKnotArray.push_back(33); 
	//vKnotArray.push_back(33);
	//T3DNurbsSurface *nurbsSurface = new T3DNurbsSurface(mBaseNode, nextId(), surfCP, uKnotArray, vKnotArray, 2, 1);
	//nurbsSurface->rational(true);

}


bool COCCSampleAppDoc::dumpNurbs(TopoDS_Shape _shape) {
	std::ofstream outputFile;
	outputFile.open("C:\\temp\\OCCNurbsDump.txt", std::ofstream::out | std::ofstream::binary);
	if (outputFile.good() == true) {
		
		int srfCounter = 0;
		int wireCounter = 0;
		int edgeCounter = 0;
		int lastEdgeIndex = 0;
		int lastWireIndex = 0;

		TopAbs_ShapeEnum shapeEnum = _shape.ShapeType();
		TopExp_Explorer shapeExpl(_shape, TopAbs_FACE);
		while (shapeExpl.More() == Standard_True) {
			TopoDS_Shape aShapeFace = shapeExpl.Current();

			TopoDS_Face aFace = TopoDS::Face(aShapeFace);
			BRepBuilderAPI_NurbsConvert nurbs(aFace);
			Handle(Geom_Surface) geom_Extrusion = BRepLib_FindSurface(nurbs.Shape()).Surface();
			Handle(Geom_BSplineSurface) geombspline_Extrusion = GeomConvert::SurfaceToBSplineSurface(geom_Extrusion);

			TColgp_Array2OfPnt cpArray = geombspline_Extrusion->Poles();
			TColStd_Array1OfReal uKnotsArray = geombspline_Extrusion->UKnots();
			TColStd_Array1OfReal vKnotsArray = geombspline_Extrusion->VKnots();
			TColStd_Array1OfInteger uMultsArray = geombspline_Extrusion->UMultiplicities();
			TColStd_Array1OfInteger vMultsArray = geombspline_Extrusion->VMultiplicities();
			const TColStd_Array2OfReal *wghArray = geombspline_Extrusion->Weights();
			Standard_Integer uDegree = geombspline_Extrusion->UDegree();
			Standard_Integer vDegree = geombspline_Extrusion->VDegree();
			Standard_Boolean isUPeriodic = geombspline_Extrusion->IsUPeriodic();
			Standard_Boolean isVPeriodic = geombspline_Extrusion->IsVPeriodic();

			outputFile << "TColgp_Array2OfPnt cpArray" << srfCounter << "(" << cpArray.LowerRow() << "," << cpArray.UpperRow() << "," << cpArray.LowerCol() << "," << cpArray.UpperCol() << ");" << std::endl;
			if (wghArray != NULL) {
				outputFile << "TColStd_Array2OfReal wghArray" << srfCounter << "(" << wghArray->LowerRow() << "," << wghArray->UpperRow() << "," << wghArray->LowerCol() << "," << wghArray->UpperCol() << ");" << std::endl;
			}
			for (int i = cpArray.LowerRow(); i <= cpArray.UpperRow(); i++) {
				for (int j = cpArray.LowerCol(); j <= cpArray.UpperCol(); j++) {
					outputFile << "cpArray" << srfCounter << ".SetValue(" << i << "," << j << ",gp_Pnt(" << cpArray.Value(i, j).X() << "," << cpArray.Value(i, j).Y() << "," << cpArray.Value(i, j).Z() << "));" << std::endl;
				}
			}
			if (wghArray != NULL) {
				for (int i = wghArray->LowerRow(); i <= wghArray->UpperRow(); i++) {
					for (int j = wghArray->LowerCol(); j <= wghArray->UpperCol(); j++) {
						outputFile << "wghArray" << srfCounter << ".SetValue(" << i << "," << j << "," << wghArray->Value(i, j) << ");" << std::endl;
					}
				}
			}

			outputFile << "TColStd_Array1OfReal uKnotsArray" << srfCounter << "(" << uKnotsArray.Lower() << "," << uKnotsArray.Upper() << ");" << std::endl;
			for (int i = uKnotsArray.Lower(); i <= uKnotsArray.Upper(); i++) {
				outputFile << "uKnotsArray" << srfCounter << ".SetValue(" << i << "," << uKnotsArray.Value(i) << ");" << std::endl;
			}
			outputFile << "TColStd_Array1OfInteger uMultsArray" << srfCounter << "(" << uMultsArray.Lower() << "," << uMultsArray.Upper() << ");" << std::endl;
			for (int i = uMultsArray.Lower(); i <= uMultsArray.Upper(); i++) {
				outputFile << "uMultsArray" << srfCounter << ".SetValue(" << i << "," << uMultsArray.Value(i) << ");" << std::endl;
			}


			outputFile << "TColStd_Array1OfReal vKnotsArray" << srfCounter << "(" << vKnotsArray.Lower() << "," << vKnotsArray.Upper() << ");" << std::endl;
			for (int i = vKnotsArray.Lower(); i <= vKnotsArray.Upper(); i++) {
				outputFile << "vKnotsArray" << srfCounter << ".SetValue(" << i << "," << vKnotsArray.Value(i) << ");" << std::endl;
			}
			outputFile << "TColStd_Array1OfInteger vMultsArray" << srfCounter << "(" << vMultsArray.Lower() << "," << vMultsArray.Upper() << ");" << std::endl;
			for (int i = vMultsArray.Lower(); i <= vMultsArray.Upper(); i++) {
				outputFile << "vMultsArray" << srfCounter << ".SetValue(" << i << "," << vMultsArray.Value(i) << ");" << std::endl;
			}

			outputFile << "Handle(Geom_BSplineSurface) nurbsSurf" << srfCounter << ";" << std::endl;

			//outputFile << "nurbsSurf = new Geom_BSplineSurface(cpArray, " << (wghArray != NULL ? "wghArray, " : "") << "uKnotsArray, vKnotsArray, uMultsArray, vMultsArray, " << uDegree << "," << vDegree << "," << (isUPeriodic == Standard_True ? "Standard_True" : "Standard_False") << "," << (isVPeriodic == Standard_True ? "Standard_True" : "Standard_False") << ");" << std::endl;

			if (wghArray != NULL) {
				outputFile << "nurbsSurf" << srfCounter << " = new Geom_BSplineSurface(cpArray" << srfCounter << ", " << "wghArray" << srfCounter << ", uKnotsArray" << srfCounter << ", vKnotsArray" << srfCounter << ", uMultsArray" << srfCounter << ", vMultsArray" << srfCounter << ", " << uDegree << "," << vDegree << "," << (isUPeriodic == Standard_True ? "Standard_True" : "Standard_False") << "," << (isVPeriodic == Standard_True ? "Standard_True" : "Standard_False") << ");" << std::endl;
			}
			else {
				outputFile << "nurbsSurf" << srfCounter << " = new Geom_BSplineSurface(cpArray" << srfCounter << ", " << "" << srfCounter << ", uKnotsArray" << srfCounter << ", vKnotsArray" << srfCounter << ", uMultsArray" << srfCounter << ", vMultsArray" << srfCounter << ", " << uDegree << "," << vDegree << "," << (isUPeriodic == Standard_True ? "Standard_True" : "Standard_False") << "," << (isVPeriodic == Standard_True ? "Standard_True" : "Standard_False") << ");" << std::endl;
			}

			outputFile << std::endl << std::endl << std::endl;


			TopExp_Explorer wireExpl(_shape, TopAbs_WIRE);
			TopoDS_Shape aShapeWire = wireExpl.Current();
			TopoDS_Wire aWire = TopoDS::Wire(aShapeWire);

			while (wireExpl.More() == Standard_True) {
				TopExp_Explorer edgeExpl(aWire, TopAbs_EDGE);
				while (edgeExpl.More() == Standard_True) {
					TopoDS_Shape aShapeEdge = edgeExpl.Current();
					TopoDS_Edge aEdge = TopoDS::Edge(aShapeEdge);

					Standard_Real aFirst, aLast, aPFirst, aPLast;
					//Handle(Geom_Curve) aCurve3d = BRep_Tool::Curve(aEdge, aFirst, aLast);
					Handle(Geom2d_Curve) aPCurve = BRep_Tool::CurveOnSurface(aEdge, aFace, aPFirst, aPLast);

					//Geom2d_TrimmedCurve aTrimmedCurve(aPCurve, aPFirst, aPLast);
					Handle(Geom2d_TrimmedCurve) aTrimmedCurve = new Geom2d_TrimmedCurve(aPCurve, aPFirst, aPLast);

					Handle(Geom2d_BSplineCurve) nurbsCurve;

					if (aPCurve->IsKind(Geom2d_Line::get_type_descriptor()) == Standard_False) {
						nurbsCurve = Geom2dConvert::CurveToBSplineCurve(aPCurve, Convert_RationalC1);
					}
					else {
						nurbsCurve = Geom2dConvert::CurveToBSplineCurve(aTrimmedCurve, Convert_RationalC1);
					}
					//Handle(Geom2d_BSplineCurve) nurbsCurve = Geom2dConvert::CurveToBSplineCurve(aPCurve, Convert_RationalC1);

					TColgp_Array1OfPnt2d cpArrayTrimCurve = nurbsCurve->Poles();
					const TColStd_Array1OfReal *wghArrayTrimCurve = nurbsCurve->Weights();
					TColStd_Array1OfReal knotsArrayTrimCurve = nurbsCurve->Knots();
					TColStd_Array1OfInteger multsArrayTrimCurve = nurbsCurve->Multiplicities();

					outputFile << "TColgp_Array1OfPnt2d cpArrayTrimCurve" << edgeCounter << "(" << cpArrayTrimCurve.Lower() << "," << cpArrayTrimCurve.Upper() << ");" << std::endl;
					if (wghArrayTrimCurve != NULL) {
						outputFile << "TColStd_Array1OfReal wghArrayTrimCurve" << edgeCounter << "(" << wghArrayTrimCurve->Lower() << "," << wghArrayTrimCurve->Upper() << ");" << std::endl;
					}
					for (int i = cpArrayTrimCurve.Lower(); i <= cpArrayTrimCurve.Upper(); i++) {
						outputFile << "cpArrayTrimCurve" << edgeCounter << ".SetValue(" << i << ",gp_Pnt2d(" << cpArrayTrimCurve.Value(i).X() << "," << cpArrayTrimCurve.Value(i).Y() << "));" << std::endl;
					}
					if (wghArrayTrimCurve != NULL) {
						for (int i = wghArrayTrimCurve->Lower(); i <= wghArrayTrimCurve->Upper(); i++) {
							outputFile << "wghArrayTrimCurve" << edgeCounter << ".SetValue(" << i << "," << wghArrayTrimCurve->Value(i) << ");" << std::endl;
						}
					}
					outputFile << "TColStd_Array1OfReal knotsArrayTrimCurve" << edgeCounter << "(" << knotsArrayTrimCurve.Lower() << "," << knotsArrayTrimCurve.Upper() << ");" << std::endl;
					for (int i = knotsArrayTrimCurve.Lower(); i <= knotsArrayTrimCurve.Upper(); i++) {
						outputFile << "knotsArrayTrimCurve" << edgeCounter << ".SetValue(" << i << "," << knotsArrayTrimCurve.Value(i) << ");" << std::endl;
					}
					outputFile << "TColStd_Array1OfInteger multsArrayTrimCurve" << edgeCounter << "(" << multsArrayTrimCurve.Lower() << "," << multsArrayTrimCurve.Upper() << ");" << std::endl;
					for (int i = multsArrayTrimCurve.Lower(); i <= multsArrayTrimCurve.Upper(); i++) {
						outputFile << "multsArrayTrimCurve" << edgeCounter << ".SetValue(" << i << "," << multsArrayTrimCurve.Value(i) << ");" << std::endl;
					}
					outputFile << "Handle(Geom2d_BSplineCurve) trimCurve" << edgeCounter << ";" << std::endl;
					outputFile << "trimCurve" << edgeCounter << " = new Geom2d_BSplineCurve(cpArrayTrimCurve" << edgeCounter << ",";
					if (wghArrayTrimCurve != NULL) {
						outputFile << "wghArrayTrimCurve" << edgeCounter << ",";
					}
					outputFile << "knotsArrayTrimCurve" << edgeCounter << ",multsArrayTrimCurve" << edgeCounter << "," << nurbsCurve->Degree() << "," << (nurbsCurve->IsPeriodic() == Standard_True ? "Standard_True" : "Standard_False") << ");" << std::endl;


					outputFile << "BRepBuilderAPI_MakeEdge makeEdge" << edgeCounter << "(trimCurve" << edgeCounter << ", nurbsSurf);" << std::endl;
					outputFile << "TopoDS_Edge edge" << edgeCounter << "= makeEdge" << edgeCounter << ".Edge();" << std::endl;
					//}

					edgeExpl.Next();
					edgeCounter++;
				}

				outputFile << "BRepBuilderAPI_MakeWire wireMaker" << wireCounter << ";" << std::endl;
				for (int ndx = lastEdgeIndex; ndx < edgeCounter; ndx++) {
					outputFile << "wireMaker" << wireCounter << ".Add(edge" << ndx << ");" << std::endl;
				}
				outputFile << "TopoDS_Wire wire" << wireCounter << "= wireMaker" << wireCounter << ".Wire();" << std::endl;

				lastEdgeIndex = edgeCounter;

				wireExpl.Next();
				wireCounter++;
			}

			outputFile << "BRepBuilderAPI_MakeFace faceMaker" << srfCounter << "(nurbsSurf" << srfCounter << ", wire" << lastWireIndex << ", false);" << std::endl;
			for (int ndx = lastWireIndex + 1; ndx < wireCounter; ndx++) {
				outputFile << "faceMaker" << srfCounter << ".Add(wire" << ndx << ");" << std::endl;
			}

			outputFile << std::endl << std::endl;

			lastWireIndex = wireCounter;

			srfCounter++;
			shapeExpl.Next();
		}
	}

	outputFile.close();

	return true;
}


void COCCSampleAppDoc::OnTestNurbsDump() {
	const Standard_Real aRadius = 10.0;
	const Standard_Real aHeight = 25.0;

	BRepPrimAPI_MakeCylinder aCylinder(aRadius, aHeight);
	TopoDS_Shape aShape = aCylinder.Shape();

	dumpNurbs(aShape);
}


void COCCSampleAppDoc::OnTestAddBoxWithHole() {
	gp_Pnt lowerLeftCornerOfBox(-50.0, -50.0, 0.0);
	BRepPrimAPI_MakeBox boxMaker(lowerLeftCornerOfBox, 100, 100, 50);
	TopoDS_Shape box = boxMaker.Shape();

	BRepPrimAPI_MakeCylinder cylinderMaker(25.0, 50.0);
	TopoDS_Shape cylinder = cylinderMaker.Shape();

	//Cut the cylinder out from the box
	BRepAlgoAPI_Cut cutMaker(box, cylinder);
	TopoDS_Shape boxWithHole = cutMaker.Shape();
	boxWithHoleShape = new AIS_Shape(boxWithHole);

	if (boxWithHoleShape.IsNull() == false) {

		// Add the new sphere to the graphic context
		myAISContext->SetDisplayMode(boxWithHoleShape, AIS_Shaded);
		myAISContext->Display(boxWithHoleShape);  // Draw the Sphere on the Screen  
		myAISContext->DisplayAll();

		// Update the viewer
		myViewer->InitActiveViews();
		Handle(V3d_View) activeView = myViewer->ActiveView();
		if (activeView.IsNull() == false) {
			activeView->FitAll();           // Focus to the View to the Drawn Shape.  
		}
	}
}


void COCCSampleAppDoc::OnTestAddCircleWithHole() {
	gp_Circ circ(gp::XOY(), 50);
	TopoDS_Edge edge = BRepBuilderAPI_MakeEdge(circ);
	TopoDS_Wire wire = BRepBuilderAPI_MakeWire(edge);
	TopoDS_Face face = BRepBuilderAPI_MakeFace(wire, Standard_True);

	BRepPrimAPI_MakeCylinder cylinderMaker(25.0, 50.0);
	TopoDS_Shape cylinder = cylinderMaker.Shape();

	BRepAlgoAPI_Cut cutMaker(face, cylinder);
	TopoDS_Shape circleWithHole = cutMaker.Shape();

	dumpNurbs(circleWithHole);



	//Standard_Boolean CImportExport::SaveBREP(CString             aFileName,
	//	const TopoDS_Shape& aShape)
	//{
	//	std::filebuf aFileBuf;
	//	std::ostream aStream(&aFileBuf);
	//	if (!aFileBuf.open(aFileName, ios::out))
	//	{
	//		return Standard_False;
	//	}

	//	BRepTools::Write(aShape, aStream);
	//	return Standard_True;
	//}

	std::filebuf aFileBuf;
	std::ostream aStream(&aFileBuf);
	if (aFileBuf.open("F:\\Sviluppo\\OCCSampleApp\\OCCNurbsDump.brep", ios::out)) {
		BRepTools::Write(circleWithHole, aStream);
	}
	aFileBuf.close();

	circleWithHoleShape = new AIS_Shape(circleWithHole);

	if (circleWithHoleShape.IsNull() == false) {

		// Add the new sphere to the graphic context
		myAISContext->SetDisplayMode(circleWithHoleShape, AIS_Shaded);
		myAISContext->Display(circleWithHoleShape);  // Draw the Sphere on the Screen  
		myAISContext->DisplayAll();

		// Update the viewer
		myViewer->InitActiveViews();
		Handle(V3d_View) activeView = myViewer->ActiveView();
		if (activeView.IsNull() == false) {
			activeView->FitAll();           // Focus to the View to the Drawn Shape.  
		}
	}
}


void COCCSampleAppDoc::OnTestAddTrimmedNurbsSurface() {
	TColgp_Array2OfPnt cpArray(1, 2, 1, 2);
	cpArray.SetValue(1, 1, gp_Pnt(-50, -50, 0));
	cpArray.SetValue(1, 2, gp_Pnt(-50, 50, 0));
	cpArray.SetValue(2, 1, gp_Pnt(50, -50, 0));

	cpArray.SetValue(2, 2, gp_Pnt(50, 50, 0));
	TColStd_Array1OfReal uKnotsArray(1, 2);
	uKnotsArray.SetValue(1, -50);
	uKnotsArray.SetValue(2, 50);
	TColStd_Array1OfInteger uMultsArray(1, 2);
	uMultsArray.SetValue(1, 2);
	uMultsArray.SetValue(2, 2);
	TColStd_Array1OfReal vKnotsArray(1, 2);
	vKnotsArray.SetValue(1, -50);
	vKnotsArray.SetValue(2, 50);
	TColStd_Array1OfInteger vMultsArray(1, 2);
	vMultsArray.SetValue(1, 2);
	vMultsArray.SetValue(2, 2);
	Handle(Geom_BSplineSurface) nurbsSurf;
	nurbsSurf = new Geom_BSplineSurface(cpArray, uKnotsArray, vKnotsArray, uMultsArray, vMultsArray, 1, 1, Standard_False, Standard_False);

	TColgp_Array1OfPnt2d cpArrayTrimCurve0(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve0(1, 12);
	cpArrayTrimCurve0.SetValue(1, gp_Pnt2d(50, -19.635));
	cpArrayTrimCurve0.SetValue(2, gp_Pnt2d(50, 19.635));
	cpArrayTrimCurve0.SetValue(3, gp_Pnt2d(39.7355, 37.7257));
	cpArrayTrimCurve0.SetValue(4, gp_Pnt2d(21.803, 50));
	cpArrayTrimCurve0.SetValue(5, gp_Pnt2d(-21.803, 50));
	cpArrayTrimCurve0.SetValue(6, gp_Pnt2d(-39.7355, 37.7257));
	cpArrayTrimCurve0.SetValue(7, gp_Pnt2d(-50, 19.635));
	cpArrayTrimCurve0.SetValue(8, gp_Pnt2d(-50, -19.635));
	cpArrayTrimCurve0.SetValue(9, gp_Pnt2d(-39.7355, -37.7257));
	cpArrayTrimCurve0.SetValue(10, gp_Pnt2d(-21.803, -50));
	cpArrayTrimCurve0.SetValue(11, gp_Pnt2d(21.803, -50));
	cpArrayTrimCurve0.SetValue(12, gp_Pnt2d(39.7355, -37.7257));
	wghArrayTrimCurve0.SetValue(1, 2);
	wghArrayTrimCurve0.SetValue(2, 2);
	wghArrayTrimCurve0.SetValue(3, 2.00318);
	wghArrayTrimCurve0.SetValue(4, 1.93961);
	wghArrayTrimCurve0.SetValue(5, 1.93961);
	wghArrayTrimCurve0.SetValue(6, 2.00318);
	wghArrayTrimCurve0.SetValue(7, 2);
	wghArrayTrimCurve0.SetValue(8, 2);
	wghArrayTrimCurve0.SetValue(9, 2.00318);
	wghArrayTrimCurve0.SetValue(10, 1.93961);
	wghArrayTrimCurve0.SetValue(11, 1.93961);
	wghArrayTrimCurve0.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve0(1, 5);
	knotsArrayTrimCurve0.SetValue(1, 0);
	knotsArrayTrimCurve0.SetValue(2, 1.5708);
	knotsArrayTrimCurve0.SetValue(3, 3.14159);
	knotsArrayTrimCurve0.SetValue(4, 4.71239);
	knotsArrayTrimCurve0.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve0(1, 5);
	multsArrayTrimCurve0.SetValue(1, 3);
	multsArrayTrimCurve0.SetValue(2, 3);
	multsArrayTrimCurve0.SetValue(3, 3);
	multsArrayTrimCurve0.SetValue(4, 3);
	multsArrayTrimCurve0.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve0;
	trimCurve0 = new Geom2d_BSplineCurve(cpArrayTrimCurve0, wghArrayTrimCurve0, knotsArrayTrimCurve0, multsArrayTrimCurve0, 4, Standard_True);
	TColgp_Array1OfPnt2d cpArrayTrimCurve1(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve1(1, 12);
	cpArrayTrimCurve1.SetValue(1, gp_Pnt2d(25, -9.81748));
	cpArrayTrimCurve1.SetValue(2, gp_Pnt2d(25, 9.81748));
	cpArrayTrimCurve1.SetValue(3, gp_Pnt2d(19.8677, 18.8629));
	cpArrayTrimCurve1.SetValue(4, gp_Pnt2d(10.9015, 25));
	cpArrayTrimCurve1.SetValue(5, gp_Pnt2d(-10.9015, 25));
	cpArrayTrimCurve1.SetValue(6, gp_Pnt2d(-19.8677, 18.8629));
	cpArrayTrimCurve1.SetValue(7, gp_Pnt2d(-25, 9.81748));
	cpArrayTrimCurve1.SetValue(8, gp_Pnt2d(-25, -9.81748));
	cpArrayTrimCurve1.SetValue(9, gp_Pnt2d(-19.8677, -18.8629));
	cpArrayTrimCurve1.SetValue(10, gp_Pnt2d(-10.9015, -25));
	cpArrayTrimCurve1.SetValue(11, gp_Pnt2d(10.9015, -25));
	cpArrayTrimCurve1.SetValue(12, gp_Pnt2d(19.8677, -18.8629));
	wghArrayTrimCurve1.SetValue(1, 2);
	wghArrayTrimCurve1.SetValue(2, 2);
	wghArrayTrimCurve1.SetValue(3, 2.00318);
	wghArrayTrimCurve1.SetValue(4, 1.93961);
	wghArrayTrimCurve1.SetValue(5, 1.93961);
	wghArrayTrimCurve1.SetValue(6, 2.00318);
	wghArrayTrimCurve1.SetValue(7, 2);
	wghArrayTrimCurve1.SetValue(8, 2);
	wghArrayTrimCurve1.SetValue(9, 2.00318);
	wghArrayTrimCurve1.SetValue(10, 1.93961);
	wghArrayTrimCurve1.SetValue(11, 1.93961);
	wghArrayTrimCurve1.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve1(1, 5);
	knotsArrayTrimCurve1.SetValue(1, 0);
	knotsArrayTrimCurve1.SetValue(2, 1.5708);
	knotsArrayTrimCurve1.SetValue(3, 3.14159);
	knotsArrayTrimCurve1.SetValue(4, 4.71239);
	knotsArrayTrimCurve1.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve1(1, 5);
	multsArrayTrimCurve1.SetValue(1, 3);
	multsArrayTrimCurve1.SetValue(2, 3);
	multsArrayTrimCurve1.SetValue(3, 3);
	multsArrayTrimCurve1.SetValue(4, 3);
	multsArrayTrimCurve1.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve1;
	trimCurve1 = new Geom2d_BSplineCurve(cpArrayTrimCurve1, wghArrayTrimCurve1, knotsArrayTrimCurve1, multsArrayTrimCurve1, 4, Standard_True);


	// Create edges
	BRepBuilderAPI_MakeEdge makeEdge0(trimCurve0, nurbsSurf);
	TopoDS_Edge edge0 = makeEdge0.Edge();
	BRepBuilderAPI_MakeEdge makeEdge1(trimCurve1, nurbsSurf);
	TopoDS_Edge edge1 = makeEdge1.Edge();

	// Create wires
	BRepBuilderAPI_MakeWire wireMaker0;
	wireMaker0.Add(edge0);
	BRepBuilderAPI_MakeWire wireMaker1;
	wireMaker1.Add(edge1);
	TopoDS_Wire wire0 = wireMaker0.Wire();
	TopoDS_Wire wire1 = wireMaker1.Wire();


	//Standard_Boolean isWire0Closed = wire0.Closed();
	//Standard_Boolean isWire1Closed = wire1.Closed();
	// Create face and add wires
	//BRepBuilderAPI_MakeFace faceMaker(nurbsSurf, Precision::Confusion());
	//faceMaker.Add(wire0);
	//faceMaker.Add(wire1);

	BRepBuilderAPI_MakeFace faceMaker(nurbsSurf, wire0, false);
	faceMaker.Add(TopoDS::Wire(wire1.Reversed()));


	// Fix to recover 3D curves
	ShapeFix_Face fix(faceMaker.Face());
	fix.Perform();
	
	// Get the generated face
	//TopoDS_Face Nurbsface = faceMaker.Face();
	TopoDS_Face Nurbsface = fix.Face();
	
	//ShapeFix_Face faceFix(Nurbsface);
	//faceFix.FixIntersectingWires();
	//faceFix.FixMissingSeam();
	//faceFix.FixOrientation();
	//faceFix.Perform();
	//TopoDS_Face builtFace = faceFix.Face();

	trimmedNurbsShape = new AIS_Shape(Nurbsface);
	if (trimmedNurbsShape.IsNull() == false) {

		// Add the trimmed nurbs to the graphic context
		myAISContext->SetDisplayMode(trimmedNurbsShape, AIS_Shaded);
		myAISContext->Display(trimmedNurbsShape);  // Draw the face on the Screen  
		myAISContext->DisplayAll();

		// Update the viewer
		myViewer->InitActiveViews();
		Handle(V3d_View) activeView = myViewer->ActiveView();
		if (activeView.IsNull() == false) {
			activeView->FitAll();           // Focus to the View to the Drawn face.  
		}
	}

	// Dump in brep format
	std::filebuf aFileBuf;
	std::ostream aStream(&aFileBuf);
	if (aFileBuf.open("F:\\Sviluppo\\OCCSampleApp\\trimmedNurbsShape.brep", ios::out)) {
		BRepTools::Write(Nurbsface, aStream);
	}
	aFileBuf.close();

	// Dump in stl format
	Handle(StlMesh_Mesh) aShapeMesh;
	generateMesh(Nurbsface, 0.01, 0.5, aShapeMesh);
	saveMesh(aShapeMesh, L"F:\\Sviluppo\\OCCSampleApp\\nurbsTrimMesh.stl");
}


void COCCSampleAppDoc::OnTestAddDumpedNurbsSurface() {
	TColgp_Array2OfPnt cpArray0(1, 7, 1, 2);
	TColStd_Array2OfReal wghArray0(1, 7, 1, 2);
	cpArray0.SetValue(1, 1, gp_Pnt(10, 0, 0));
	cpArray0.SetValue(1, 2, gp_Pnt(10, 0, 25));
	cpArray0.SetValue(2, 1, gp_Pnt(10, 17.3205, 0));
	cpArray0.SetValue(2, 2, gp_Pnt(10, 17.3205, 25));
	cpArray0.SetValue(3, 1, gp_Pnt(-5, 8.66025, 0));
	cpArray0.SetValue(3, 2, gp_Pnt(-5, 8.66025, 25));
	cpArray0.SetValue(4, 1, gp_Pnt(-20, 2.44929e-15, 0));
	cpArray0.SetValue(4, 2, gp_Pnt(-20, 2.44929e-15, 25));
	cpArray0.SetValue(5, 1, gp_Pnt(-5, -8.66025, 0));
	cpArray0.SetValue(5, 2, gp_Pnt(-5, -8.66025, 25));
	cpArray0.SetValue(6, 1, gp_Pnt(10, -17.3205, 0));
	cpArray0.SetValue(6, 2, gp_Pnt(10, -17.3205, 25));
	cpArray0.SetValue(7, 1, gp_Pnt(10, -2.44929e-15, 0));
	cpArray0.SetValue(7, 2, gp_Pnt(10, -2.44929e-15, 25));
	wghArray0.SetValue(1, 1, 1);
	wghArray0.SetValue(1, 2, 1);
	wghArray0.SetValue(2, 1, 0.5);
	wghArray0.SetValue(2, 2, 0.5);
	wghArray0.SetValue(3, 1, 1);
	wghArray0.SetValue(3, 2, 1);
	wghArray0.SetValue(4, 1, 0.5);
	wghArray0.SetValue(4, 2, 0.5);
	wghArray0.SetValue(5, 1, 1);
	wghArray0.SetValue(5, 2, 1);
	wghArray0.SetValue(6, 1, 0.5);
	wghArray0.SetValue(6, 2, 0.5);
	wghArray0.SetValue(7, 1, 1);
	wghArray0.SetValue(7, 2, 1);
	TColStd_Array1OfReal uKnotsArray0(1, 4);
	uKnotsArray0.SetValue(1, 0);
	uKnotsArray0.SetValue(2, 2.0944);
	uKnotsArray0.SetValue(3, 4.18879);
	uKnotsArray0.SetValue(4, 6.28319);
	TColStd_Array1OfInteger uMultsArray0(1, 4);
	uMultsArray0.SetValue(1, 3);
	uMultsArray0.SetValue(2, 2);
	uMultsArray0.SetValue(3, 2);
	uMultsArray0.SetValue(4, 3);
	TColStd_Array1OfReal vKnotsArray0(1, 2);
	vKnotsArray0.SetValue(1, 0);
	vKnotsArray0.SetValue(2, 25);
	TColStd_Array1OfInteger vMultsArray0(1, 2);
	vMultsArray0.SetValue(1, 2);
	vMultsArray0.SetValue(2, 2);
	Handle(Geom_BSplineSurface) nurbsSurf0;
	nurbsSurf0 = new Geom_BSplineSurface(cpArray0, wghArray0, uKnotsArray0, vKnotsArray0, uMultsArray0, vMultsArray0, 2, 1, Standard_False, Standard_False);



	TColgp_Array1OfPnt2d cpArrayTrimCurve0(1, 2);
	cpArrayTrimCurve0.SetValue(1, gp_Pnt2d(0, 25));
	cpArrayTrimCurve0.SetValue(2, gp_Pnt2d(6.28319, 25));
	TColStd_Array1OfReal knotsArrayTrimCurve0(1, 2);
	knotsArrayTrimCurve0.SetValue(1, 0);
	knotsArrayTrimCurve0.SetValue(2, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve0(1, 2);
	multsArrayTrimCurve0.SetValue(1, 2);
	multsArrayTrimCurve0.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve0;
	trimCurve0 = new Geom2d_BSplineCurve(cpArrayTrimCurve0, knotsArrayTrimCurve0, multsArrayTrimCurve0, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge0(trimCurve0, nurbsSurf0);
	TopoDS_Edge edge0 = makeEdge0.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve1(1, 2);
	cpArrayTrimCurve1.SetValue(1, gp_Pnt2d(6.28319, 0));
	cpArrayTrimCurve1.SetValue(2, gp_Pnt2d(6.28319, 25));
	TColStd_Array1OfReal knotsArrayTrimCurve1(1, 2);
	knotsArrayTrimCurve1.SetValue(1, 0);
	knotsArrayTrimCurve1.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve1(1, 2);
	multsArrayTrimCurve1.SetValue(1, 2);
	multsArrayTrimCurve1.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve1;
	trimCurve1 = new Geom2d_BSplineCurve(cpArrayTrimCurve1, knotsArrayTrimCurve1, multsArrayTrimCurve1, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge1(trimCurve1, nurbsSurf0);
	TopoDS_Edge edge1 = makeEdge1.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve2(1, 2);
	cpArrayTrimCurve2.SetValue(1, gp_Pnt2d(0, 0));
	cpArrayTrimCurve2.SetValue(2, gp_Pnt2d(6.28319, 0));
	TColStd_Array1OfReal knotsArrayTrimCurve2(1, 2);
	knotsArrayTrimCurve2.SetValue(1, 0);
	knotsArrayTrimCurve2.SetValue(2, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve2(1, 2);
	multsArrayTrimCurve2.SetValue(1, 2);
	multsArrayTrimCurve2.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve2;
	trimCurve2 = new Geom2d_BSplineCurve(cpArrayTrimCurve2, knotsArrayTrimCurve2, multsArrayTrimCurve2, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge2(trimCurve2, nurbsSurf0);
	TopoDS_Edge edge2 = makeEdge2.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve3(1, 2);
	cpArrayTrimCurve3.SetValue(1, gp_Pnt2d(0, 0));
	cpArrayTrimCurve3.SetValue(2, gp_Pnt2d(0, 25));
	TColStd_Array1OfReal knotsArrayTrimCurve3(1, 2);
	knotsArrayTrimCurve3.SetValue(1, 0);
	knotsArrayTrimCurve3.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve3(1, 2);
	multsArrayTrimCurve3.SetValue(1, 2);
	multsArrayTrimCurve3.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve3;
	trimCurve3 = new Geom2d_BSplineCurve(cpArrayTrimCurve3, knotsArrayTrimCurve3, multsArrayTrimCurve3, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge3(trimCurve3, nurbsSurf0);
	TopoDS_Edge edge3 = makeEdge3.Edge();
	BRepBuilderAPI_MakeWire wireMaker0;
	wireMaker0.Add(edge0);
	wireMaker0.Add(edge1);
	wireMaker0.Add(edge2);
	wireMaker0.Add(edge3);
	TopoDS_Wire wire0 = wireMaker0.Wire();
	TColgp_Array1OfPnt2d cpArrayTrimCurve4(1, 2);
	cpArrayTrimCurve4.SetValue(1, gp_Pnt2d(0, 25));
	cpArrayTrimCurve4.SetValue(2, gp_Pnt2d(6.28319, 25));
	TColStd_Array1OfReal knotsArrayTrimCurve4(1, 2);
	knotsArrayTrimCurve4.SetValue(1, 0);
	knotsArrayTrimCurve4.SetValue(2, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve4(1, 2);
	multsArrayTrimCurve4.SetValue(1, 2);
	multsArrayTrimCurve4.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve4;
	trimCurve4 = new Geom2d_BSplineCurve(cpArrayTrimCurve4, knotsArrayTrimCurve4, multsArrayTrimCurve4, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge4(trimCurve4, nurbsSurf0);
	TopoDS_Edge edge4 = makeEdge4.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve5(1, 2);
	cpArrayTrimCurve5.SetValue(1, gp_Pnt2d(6.28319, 0));
	cpArrayTrimCurve5.SetValue(2, gp_Pnt2d(6.28319, 25));
	TColStd_Array1OfReal knotsArrayTrimCurve5(1, 2);
	knotsArrayTrimCurve5.SetValue(1, 0);
	knotsArrayTrimCurve5.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve5(1, 2);
	multsArrayTrimCurve5.SetValue(1, 2);
	multsArrayTrimCurve5.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve5;
	trimCurve5 = new Geom2d_BSplineCurve(cpArrayTrimCurve5, knotsArrayTrimCurve5, multsArrayTrimCurve5, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge5(trimCurve5, nurbsSurf0);
	TopoDS_Edge edge5 = makeEdge5.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve6(1, 2);
	cpArrayTrimCurve6.SetValue(1, gp_Pnt2d(0, 0));
	cpArrayTrimCurve6.SetValue(2, gp_Pnt2d(6.28319, 0));
	TColStd_Array1OfReal knotsArrayTrimCurve6(1, 2);
	knotsArrayTrimCurve6.SetValue(1, 0);
	knotsArrayTrimCurve6.SetValue(2, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve6(1, 2);
	multsArrayTrimCurve6.SetValue(1, 2);
	multsArrayTrimCurve6.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve6;
	trimCurve6 = new Geom2d_BSplineCurve(cpArrayTrimCurve6, knotsArrayTrimCurve6, multsArrayTrimCurve6, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge6(trimCurve6, nurbsSurf0);
	TopoDS_Edge edge6 = makeEdge6.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve7(1, 2);
	cpArrayTrimCurve7.SetValue(1, gp_Pnt2d(0, 0));
	cpArrayTrimCurve7.SetValue(2, gp_Pnt2d(0, 25));
	TColStd_Array1OfReal knotsArrayTrimCurve7(1, 2);
	knotsArrayTrimCurve7.SetValue(1, 0);
	knotsArrayTrimCurve7.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve7(1, 2);
	multsArrayTrimCurve7.SetValue(1, 2);
	multsArrayTrimCurve7.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve7;
	trimCurve7 = new Geom2d_BSplineCurve(cpArrayTrimCurve7, knotsArrayTrimCurve7, multsArrayTrimCurve7, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge7(trimCurve7, nurbsSurf0);
	TopoDS_Edge edge7 = makeEdge7.Edge();
	BRepBuilderAPI_MakeWire wireMaker1;
	wireMaker1.Add(edge4);
	wireMaker1.Add(edge5);
	wireMaker1.Add(edge6);
	wireMaker1.Add(edge7);
	TopoDS_Wire wire1 = wireMaker1.Wire();
	TColgp_Array1OfPnt2d cpArrayTrimCurve8(1, 2);
	cpArrayTrimCurve8.SetValue(1, gp_Pnt2d(0, 25));
	cpArrayTrimCurve8.SetValue(2, gp_Pnt2d(6.28319, 25));
	TColStd_Array1OfReal knotsArrayTrimCurve8(1, 2);
	knotsArrayTrimCurve8.SetValue(1, 0);
	knotsArrayTrimCurve8.SetValue(2, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve8(1, 2);
	multsArrayTrimCurve8.SetValue(1, 2);
	multsArrayTrimCurve8.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve8;
	trimCurve8 = new Geom2d_BSplineCurve(cpArrayTrimCurve8, knotsArrayTrimCurve8, multsArrayTrimCurve8, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge8(trimCurve8, nurbsSurf0);
	TopoDS_Edge edge8 = makeEdge8.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve9(1, 2);
	cpArrayTrimCurve9.SetValue(1, gp_Pnt2d(6.28319, 0));
	cpArrayTrimCurve9.SetValue(2, gp_Pnt2d(6.28319, 25));
	TColStd_Array1OfReal knotsArrayTrimCurve9(1, 2);
	knotsArrayTrimCurve9.SetValue(1, 0);
	knotsArrayTrimCurve9.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve9(1, 2);
	multsArrayTrimCurve9.SetValue(1, 2);
	multsArrayTrimCurve9.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve9;
	trimCurve9 = new Geom2d_BSplineCurve(cpArrayTrimCurve9, knotsArrayTrimCurve9, multsArrayTrimCurve9, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge9(trimCurve9, nurbsSurf0);
	TopoDS_Edge edge9 = makeEdge9.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve10(1, 2);
	cpArrayTrimCurve10.SetValue(1, gp_Pnt2d(0, 0));
	cpArrayTrimCurve10.SetValue(2, gp_Pnt2d(6.28319, 0));
	TColStd_Array1OfReal knotsArrayTrimCurve10(1, 2);
	knotsArrayTrimCurve10.SetValue(1, 0);
	knotsArrayTrimCurve10.SetValue(2, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve10(1, 2);
	multsArrayTrimCurve10.SetValue(1, 2);
	multsArrayTrimCurve10.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve10;
	trimCurve10 = new Geom2d_BSplineCurve(cpArrayTrimCurve10, knotsArrayTrimCurve10, multsArrayTrimCurve10, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge10(trimCurve10, nurbsSurf0);
	TopoDS_Edge edge10 = makeEdge10.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve11(1, 2);
	cpArrayTrimCurve11.SetValue(1, gp_Pnt2d(0, 0));
	cpArrayTrimCurve11.SetValue(2, gp_Pnt2d(0, 25));
	TColStd_Array1OfReal knotsArrayTrimCurve11(1, 2);
	knotsArrayTrimCurve11.SetValue(1, 0);
	knotsArrayTrimCurve11.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve11(1, 2);
	multsArrayTrimCurve11.SetValue(1, 2);
	multsArrayTrimCurve11.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve11;
	trimCurve11 = new Geom2d_BSplineCurve(cpArrayTrimCurve11, knotsArrayTrimCurve11, multsArrayTrimCurve11, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge11(trimCurve11, nurbsSurf0);
	TopoDS_Edge edge11 = makeEdge11.Edge();
	BRepBuilderAPI_MakeWire wireMaker2;
	wireMaker2.Add(edge8);
	wireMaker2.Add(edge9);
	wireMaker2.Add(edge10);
	wireMaker2.Add(edge11);
	TopoDS_Wire wire2 = wireMaker2.Wire();
	BRepBuilderAPI_MakeFace faceMaker0(nurbsSurf0, wire0, false);
	faceMaker0.Add(wire1);
	faceMaker0.Add(wire2);


	TColgp_Array2OfPnt cpArray1(1, 2, 1, 2);
	cpArray1.SetValue(1, 1, gp_Pnt(-10, -10, 25));
	cpArray1.SetValue(1, 2, gp_Pnt(-10, 10, 25));
	cpArray1.SetValue(2, 1, gp_Pnt(10, -10, 25));
	cpArray1.SetValue(2, 2, gp_Pnt(10, 10, 25));
	TColStd_Array1OfReal uKnotsArray1(1, 2);
	uKnotsArray1.SetValue(1, -10);
	uKnotsArray1.SetValue(2, 10);
	TColStd_Array1OfInteger uMultsArray1(1, 2);
	uMultsArray1.SetValue(1, 2);
	uMultsArray1.SetValue(2, 2);
	TColStd_Array1OfReal vKnotsArray1(1, 2);
	vKnotsArray1.SetValue(1, -10);
	vKnotsArray1.SetValue(2, 10);
	TColStd_Array1OfInteger vMultsArray1(1, 2);
	vMultsArray1.SetValue(1, 2);
	vMultsArray1.SetValue(2, 2);
	Handle(Geom_BSplineSurface) nurbsSurf1;
	nurbsSurf1 = new Geom_BSplineSurface(cpArray1, uKnotsArray1, vKnotsArray1, uMultsArray1, vMultsArray1, 1, 1, Standard_False, Standard_False);



	TColgp_Array1OfPnt2d cpArrayTrimCurve12(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve12(1, 12);
	cpArrayTrimCurve12.SetValue(1, gp_Pnt2d(10, -3.92699));
	cpArrayTrimCurve12.SetValue(2, gp_Pnt2d(10, 3.92699));
	cpArrayTrimCurve12.SetValue(3, gp_Pnt2d(7.9471, 7.54514));
	cpArrayTrimCurve12.SetValue(4, gp_Pnt2d(4.3606, 10));
	cpArrayTrimCurve12.SetValue(5, gp_Pnt2d(-4.3606, 10));
	cpArrayTrimCurve12.SetValue(6, gp_Pnt2d(-7.9471, 7.54514));
	cpArrayTrimCurve12.SetValue(7, gp_Pnt2d(-10, 3.92699));
	cpArrayTrimCurve12.SetValue(8, gp_Pnt2d(-10, -3.92699));
	cpArrayTrimCurve12.SetValue(9, gp_Pnt2d(-7.9471, -7.54514));
	cpArrayTrimCurve12.SetValue(10, gp_Pnt2d(-4.3606, -10));
	cpArrayTrimCurve12.SetValue(11, gp_Pnt2d(4.3606, -10));
	cpArrayTrimCurve12.SetValue(12, gp_Pnt2d(7.9471, -7.54514));
	wghArrayTrimCurve12.SetValue(1, 2);
	wghArrayTrimCurve12.SetValue(2, 2);
	wghArrayTrimCurve12.SetValue(3, 2.00318);
	wghArrayTrimCurve12.SetValue(4, 1.93961);
	wghArrayTrimCurve12.SetValue(5, 1.93961);
	wghArrayTrimCurve12.SetValue(6, 2.00318);
	wghArrayTrimCurve12.SetValue(7, 2);
	wghArrayTrimCurve12.SetValue(8, 2);
	wghArrayTrimCurve12.SetValue(9, 2.00318);
	wghArrayTrimCurve12.SetValue(10, 1.93961);
	wghArrayTrimCurve12.SetValue(11, 1.93961);
	wghArrayTrimCurve12.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve12(1, 5);
	knotsArrayTrimCurve12.SetValue(1, 0);
	knotsArrayTrimCurve12.SetValue(2, 1.5708);
	knotsArrayTrimCurve12.SetValue(3, 3.14159);
	knotsArrayTrimCurve12.SetValue(4, 4.71239);
	knotsArrayTrimCurve12.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve12(1, 5);
	multsArrayTrimCurve12.SetValue(1, 3);
	multsArrayTrimCurve12.SetValue(2, 3);
	multsArrayTrimCurve12.SetValue(3, 3);
	multsArrayTrimCurve12.SetValue(4, 3);
	multsArrayTrimCurve12.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve12;
	trimCurve12 = new Geom2d_BSplineCurve(cpArrayTrimCurve12, wghArrayTrimCurve12, knotsArrayTrimCurve12, multsArrayTrimCurve12, 4, Standard_True);
	BRepBuilderAPI_MakeEdge makeEdge12(trimCurve12, nurbsSurf1);
	TopoDS_Edge edge12 = makeEdge12.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve13(1, 2);
	cpArrayTrimCurve13.SetValue(1, gp_Pnt2d(10, -2.44929e-15));
	cpArrayTrimCurve13.SetValue(2, gp_Pnt2d(10, -2.44929e-15));
	TColStd_Array1OfReal knotsArrayTrimCurve13(1, 2);
	knotsArrayTrimCurve13.SetValue(1, 0);
	knotsArrayTrimCurve13.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve13(1, 2);
	multsArrayTrimCurve13.SetValue(1, 2);
	multsArrayTrimCurve13.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve13;
	trimCurve13 = new Geom2d_BSplineCurve(cpArrayTrimCurve13, knotsArrayTrimCurve13, multsArrayTrimCurve13, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge13(trimCurve13, nurbsSurf1);
	TopoDS_Edge edge13 = makeEdge13.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve14(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve14(1, 12);
	cpArrayTrimCurve14.SetValue(1, gp_Pnt2d(10, -3.92699));
	cpArrayTrimCurve14.SetValue(2, gp_Pnt2d(10, 3.92699));
	cpArrayTrimCurve14.SetValue(3, gp_Pnt2d(7.9471, 7.54514));
	cpArrayTrimCurve14.SetValue(4, gp_Pnt2d(4.3606, 10));
	cpArrayTrimCurve14.SetValue(5, gp_Pnt2d(-4.3606, 10));
	cpArrayTrimCurve14.SetValue(6, gp_Pnt2d(-7.9471, 7.54514));
	cpArrayTrimCurve14.SetValue(7, gp_Pnt2d(-10, 3.92699));
	cpArrayTrimCurve14.SetValue(8, gp_Pnt2d(-10, -3.92699));
	cpArrayTrimCurve14.SetValue(9, gp_Pnt2d(-7.9471, -7.54514));
	cpArrayTrimCurve14.SetValue(10, gp_Pnt2d(-4.3606, -10));
	cpArrayTrimCurve14.SetValue(11, gp_Pnt2d(4.3606, -10));
	cpArrayTrimCurve14.SetValue(12, gp_Pnt2d(7.9471, -7.54514));
	wghArrayTrimCurve14.SetValue(1, 2);
	wghArrayTrimCurve14.SetValue(2, 2);
	wghArrayTrimCurve14.SetValue(3, 2.00318);
	wghArrayTrimCurve14.SetValue(4, 1.93961);
	wghArrayTrimCurve14.SetValue(5, 1.93961);
	wghArrayTrimCurve14.SetValue(6, 2.00318);
	wghArrayTrimCurve14.SetValue(7, 2);
	wghArrayTrimCurve14.SetValue(8, 2);
	wghArrayTrimCurve14.SetValue(9, 2.00318);
	wghArrayTrimCurve14.SetValue(10, 1.93961);
	wghArrayTrimCurve14.SetValue(11, 1.93961);
	wghArrayTrimCurve14.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve14(1, 5);
	knotsArrayTrimCurve14.SetValue(1, 0);
	knotsArrayTrimCurve14.SetValue(2, 1.5708);
	knotsArrayTrimCurve14.SetValue(3, 3.14159);
	knotsArrayTrimCurve14.SetValue(4, 4.71239);
	knotsArrayTrimCurve14.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve14(1, 5);
	multsArrayTrimCurve14.SetValue(1, 3);
	multsArrayTrimCurve14.SetValue(2, 3);
	multsArrayTrimCurve14.SetValue(3, 3);
	multsArrayTrimCurve14.SetValue(4, 3);
	multsArrayTrimCurve14.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve14;
	trimCurve14 = new Geom2d_BSplineCurve(cpArrayTrimCurve14, wghArrayTrimCurve14, knotsArrayTrimCurve14, multsArrayTrimCurve14, 4, Standard_True);
	BRepBuilderAPI_MakeEdge makeEdge14(trimCurve14, nurbsSurf1);
	TopoDS_Edge edge14 = makeEdge14.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve15(1, 2);
	cpArrayTrimCurve15.SetValue(1, gp_Pnt2d(10, -2.44929e-15));
	cpArrayTrimCurve15.SetValue(2, gp_Pnt2d(10, -2.44929e-15));
	TColStd_Array1OfReal knotsArrayTrimCurve15(1, 2);
	knotsArrayTrimCurve15.SetValue(1, 0);
	knotsArrayTrimCurve15.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve15(1, 2);
	multsArrayTrimCurve15.SetValue(1, 2);
	multsArrayTrimCurve15.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve15;
	trimCurve15 = new Geom2d_BSplineCurve(cpArrayTrimCurve15, knotsArrayTrimCurve15, multsArrayTrimCurve15, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge15(trimCurve15, nurbsSurf1);
	TopoDS_Edge edge15 = makeEdge15.Edge();
	BRepBuilderAPI_MakeWire wireMaker3;
	wireMaker3.Add(edge12);
	wireMaker3.Add(edge13);
	wireMaker3.Add(edge14);
	wireMaker3.Add(edge15);
	TopoDS_Wire wire3 = wireMaker3.Wire();
	TColgp_Array1OfPnt2d cpArrayTrimCurve16(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve16(1, 12);
	cpArrayTrimCurve16.SetValue(1, gp_Pnt2d(10, -3.92699));
	cpArrayTrimCurve16.SetValue(2, gp_Pnt2d(10, 3.92699));
	cpArrayTrimCurve16.SetValue(3, gp_Pnt2d(7.9471, 7.54514));
	cpArrayTrimCurve16.SetValue(4, gp_Pnt2d(4.3606, 10));
	cpArrayTrimCurve16.SetValue(5, gp_Pnt2d(-4.3606, 10));
	cpArrayTrimCurve16.SetValue(6, gp_Pnt2d(-7.9471, 7.54514));
	cpArrayTrimCurve16.SetValue(7, gp_Pnt2d(-10, 3.92699));
	cpArrayTrimCurve16.SetValue(8, gp_Pnt2d(-10, -3.92699));
	cpArrayTrimCurve16.SetValue(9, gp_Pnt2d(-7.9471, -7.54514));
	cpArrayTrimCurve16.SetValue(10, gp_Pnt2d(-4.3606, -10));
	cpArrayTrimCurve16.SetValue(11, gp_Pnt2d(4.3606, -10));
	cpArrayTrimCurve16.SetValue(12, gp_Pnt2d(7.9471, -7.54514));
	wghArrayTrimCurve16.SetValue(1, 2);
	wghArrayTrimCurve16.SetValue(2, 2);
	wghArrayTrimCurve16.SetValue(3, 2.00318);
	wghArrayTrimCurve16.SetValue(4, 1.93961);
	wghArrayTrimCurve16.SetValue(5, 1.93961);
	wghArrayTrimCurve16.SetValue(6, 2.00318);
	wghArrayTrimCurve16.SetValue(7, 2);
	wghArrayTrimCurve16.SetValue(8, 2);
	wghArrayTrimCurve16.SetValue(9, 2.00318);
	wghArrayTrimCurve16.SetValue(10, 1.93961);
	wghArrayTrimCurve16.SetValue(11, 1.93961);
	wghArrayTrimCurve16.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve16(1, 5);
	knotsArrayTrimCurve16.SetValue(1, 0);
	knotsArrayTrimCurve16.SetValue(2, 1.5708);
	knotsArrayTrimCurve16.SetValue(3, 3.14159);
	knotsArrayTrimCurve16.SetValue(4, 4.71239);
	knotsArrayTrimCurve16.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve16(1, 5);
	multsArrayTrimCurve16.SetValue(1, 3);
	multsArrayTrimCurve16.SetValue(2, 3);
	multsArrayTrimCurve16.SetValue(3, 3);
	multsArrayTrimCurve16.SetValue(4, 3);
	multsArrayTrimCurve16.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve16;
	trimCurve16 = new Geom2d_BSplineCurve(cpArrayTrimCurve16, wghArrayTrimCurve16, knotsArrayTrimCurve16, multsArrayTrimCurve16, 4, Standard_True);
	BRepBuilderAPI_MakeEdge makeEdge16(trimCurve16, nurbsSurf1);
	TopoDS_Edge edge16 = makeEdge16.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve17(1, 2);
	cpArrayTrimCurve17.SetValue(1, gp_Pnt2d(10, -2.44929e-15));
	cpArrayTrimCurve17.SetValue(2, gp_Pnt2d(10, -2.44929e-15));
	TColStd_Array1OfReal knotsArrayTrimCurve17(1, 2);
	knotsArrayTrimCurve17.SetValue(1, 0);
	knotsArrayTrimCurve17.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve17(1, 2);
	multsArrayTrimCurve17.SetValue(1, 2);
	multsArrayTrimCurve17.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve17;
	trimCurve17 = new Geom2d_BSplineCurve(cpArrayTrimCurve17, knotsArrayTrimCurve17, multsArrayTrimCurve17, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge17(trimCurve17, nurbsSurf1);
	TopoDS_Edge edge17 = makeEdge17.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve18(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve18(1, 12);
	cpArrayTrimCurve18.SetValue(1, gp_Pnt2d(10, -3.92699));
	cpArrayTrimCurve18.SetValue(2, gp_Pnt2d(10, 3.92699));
	cpArrayTrimCurve18.SetValue(3, gp_Pnt2d(7.9471, 7.54514));
	cpArrayTrimCurve18.SetValue(4, gp_Pnt2d(4.3606, 10));
	cpArrayTrimCurve18.SetValue(5, gp_Pnt2d(-4.3606, 10));
	cpArrayTrimCurve18.SetValue(6, gp_Pnt2d(-7.9471, 7.54514));
	cpArrayTrimCurve18.SetValue(7, gp_Pnt2d(-10, 3.92699));
	cpArrayTrimCurve18.SetValue(8, gp_Pnt2d(-10, -3.92699));
	cpArrayTrimCurve18.SetValue(9, gp_Pnt2d(-7.9471, -7.54514));
	cpArrayTrimCurve18.SetValue(10, gp_Pnt2d(-4.3606, -10));
	cpArrayTrimCurve18.SetValue(11, gp_Pnt2d(4.3606, -10));
	cpArrayTrimCurve18.SetValue(12, gp_Pnt2d(7.9471, -7.54514));
	wghArrayTrimCurve18.SetValue(1, 2);
	wghArrayTrimCurve18.SetValue(2, 2);
	wghArrayTrimCurve18.SetValue(3, 2.00318);
	wghArrayTrimCurve18.SetValue(4, 1.93961);
	wghArrayTrimCurve18.SetValue(5, 1.93961);
	wghArrayTrimCurve18.SetValue(6, 2.00318);
	wghArrayTrimCurve18.SetValue(7, 2);
	wghArrayTrimCurve18.SetValue(8, 2);
	wghArrayTrimCurve18.SetValue(9, 2.00318);
	wghArrayTrimCurve18.SetValue(10, 1.93961);
	wghArrayTrimCurve18.SetValue(11, 1.93961);
	wghArrayTrimCurve18.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve18(1, 5);
	knotsArrayTrimCurve18.SetValue(1, 0);
	knotsArrayTrimCurve18.SetValue(2, 1.5708);
	knotsArrayTrimCurve18.SetValue(3, 3.14159);
	knotsArrayTrimCurve18.SetValue(4, 4.71239);
	knotsArrayTrimCurve18.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve18(1, 5);
	multsArrayTrimCurve18.SetValue(1, 3);
	multsArrayTrimCurve18.SetValue(2, 3);
	multsArrayTrimCurve18.SetValue(3, 3);
	multsArrayTrimCurve18.SetValue(4, 3);
	multsArrayTrimCurve18.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve18;
	trimCurve18 = new Geom2d_BSplineCurve(cpArrayTrimCurve18, wghArrayTrimCurve18, knotsArrayTrimCurve18, multsArrayTrimCurve18, 4, Standard_True);
	BRepBuilderAPI_MakeEdge makeEdge18(trimCurve18, nurbsSurf1);
	TopoDS_Edge edge18 = makeEdge18.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve19(1, 2);
	cpArrayTrimCurve19.SetValue(1, gp_Pnt2d(10, -2.44929e-15));
	cpArrayTrimCurve19.SetValue(2, gp_Pnt2d(10, -2.44929e-15));
	TColStd_Array1OfReal knotsArrayTrimCurve19(1, 2);
	knotsArrayTrimCurve19.SetValue(1, 0);
	knotsArrayTrimCurve19.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve19(1, 2);
	multsArrayTrimCurve19.SetValue(1, 2);
	multsArrayTrimCurve19.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve19;
	trimCurve19 = new Geom2d_BSplineCurve(cpArrayTrimCurve19, knotsArrayTrimCurve19, multsArrayTrimCurve19, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge19(trimCurve19, nurbsSurf1);
	TopoDS_Edge edge19 = makeEdge19.Edge();
	BRepBuilderAPI_MakeWire wireMaker4;
	wireMaker4.Add(edge16);
	wireMaker4.Add(edge17);
	wireMaker4.Add(edge18);
	wireMaker4.Add(edge19);
	TopoDS_Wire wire4 = wireMaker4.Wire();
	TColgp_Array1OfPnt2d cpArrayTrimCurve20(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve20(1, 12);
	cpArrayTrimCurve20.SetValue(1, gp_Pnt2d(10, -3.92699));
	cpArrayTrimCurve20.SetValue(2, gp_Pnt2d(10, 3.92699));
	cpArrayTrimCurve20.SetValue(3, gp_Pnt2d(7.9471, 7.54514));
	cpArrayTrimCurve20.SetValue(4, gp_Pnt2d(4.3606, 10));
	cpArrayTrimCurve20.SetValue(5, gp_Pnt2d(-4.3606, 10));
	cpArrayTrimCurve20.SetValue(6, gp_Pnt2d(-7.9471, 7.54514));
	cpArrayTrimCurve20.SetValue(7, gp_Pnt2d(-10, 3.92699));
	cpArrayTrimCurve20.SetValue(8, gp_Pnt2d(-10, -3.92699));
	cpArrayTrimCurve20.SetValue(9, gp_Pnt2d(-7.9471, -7.54514));
	cpArrayTrimCurve20.SetValue(10, gp_Pnt2d(-4.3606, -10));
	cpArrayTrimCurve20.SetValue(11, gp_Pnt2d(4.3606, -10));
	cpArrayTrimCurve20.SetValue(12, gp_Pnt2d(7.9471, -7.54514));
	wghArrayTrimCurve20.SetValue(1, 2);
	wghArrayTrimCurve20.SetValue(2, 2);
	wghArrayTrimCurve20.SetValue(3, 2.00318);
	wghArrayTrimCurve20.SetValue(4, 1.93961);
	wghArrayTrimCurve20.SetValue(5, 1.93961);
	wghArrayTrimCurve20.SetValue(6, 2.00318);
	wghArrayTrimCurve20.SetValue(7, 2);
	wghArrayTrimCurve20.SetValue(8, 2);
	wghArrayTrimCurve20.SetValue(9, 2.00318);
	wghArrayTrimCurve20.SetValue(10, 1.93961);
	wghArrayTrimCurve20.SetValue(11, 1.93961);
	wghArrayTrimCurve20.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve20(1, 5);
	knotsArrayTrimCurve20.SetValue(1, 0);
	knotsArrayTrimCurve20.SetValue(2, 1.5708);
	knotsArrayTrimCurve20.SetValue(3, 3.14159);
	knotsArrayTrimCurve20.SetValue(4, 4.71239);
	knotsArrayTrimCurve20.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve20(1, 5);
	multsArrayTrimCurve20.SetValue(1, 3);
	multsArrayTrimCurve20.SetValue(2, 3);
	multsArrayTrimCurve20.SetValue(3, 3);
	multsArrayTrimCurve20.SetValue(4, 3);
	multsArrayTrimCurve20.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve20;
	trimCurve20 = new Geom2d_BSplineCurve(cpArrayTrimCurve20, wghArrayTrimCurve20, knotsArrayTrimCurve20, multsArrayTrimCurve20, 4, Standard_True);
	BRepBuilderAPI_MakeEdge makeEdge20(trimCurve20, nurbsSurf1);
	TopoDS_Edge edge20 = makeEdge20.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve21(1, 2);
	cpArrayTrimCurve21.SetValue(1, gp_Pnt2d(10, -2.44929e-15));
	cpArrayTrimCurve21.SetValue(2, gp_Pnt2d(10, -2.44929e-15));
	TColStd_Array1OfReal knotsArrayTrimCurve21(1, 2);
	knotsArrayTrimCurve21.SetValue(1, 0);
	knotsArrayTrimCurve21.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve21(1, 2);
	multsArrayTrimCurve21.SetValue(1, 2);
	multsArrayTrimCurve21.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve21;
	trimCurve21 = new Geom2d_BSplineCurve(cpArrayTrimCurve21, knotsArrayTrimCurve21, multsArrayTrimCurve21, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge21(trimCurve21, nurbsSurf1);
	TopoDS_Edge edge21 = makeEdge21.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve22(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve22(1, 12);
	cpArrayTrimCurve22.SetValue(1, gp_Pnt2d(10, -3.92699));
	cpArrayTrimCurve22.SetValue(2, gp_Pnt2d(10, 3.92699));
	cpArrayTrimCurve22.SetValue(3, gp_Pnt2d(7.9471, 7.54514));
	cpArrayTrimCurve22.SetValue(4, gp_Pnt2d(4.3606, 10));
	cpArrayTrimCurve22.SetValue(5, gp_Pnt2d(-4.3606, 10));
	cpArrayTrimCurve22.SetValue(6, gp_Pnt2d(-7.9471, 7.54514));
	cpArrayTrimCurve22.SetValue(7, gp_Pnt2d(-10, 3.92699));
	cpArrayTrimCurve22.SetValue(8, gp_Pnt2d(-10, -3.92699));
	cpArrayTrimCurve22.SetValue(9, gp_Pnt2d(-7.9471, -7.54514));
	cpArrayTrimCurve22.SetValue(10, gp_Pnt2d(-4.3606, -10));
	cpArrayTrimCurve22.SetValue(11, gp_Pnt2d(4.3606, -10));
	cpArrayTrimCurve22.SetValue(12, gp_Pnt2d(7.9471, -7.54514));
	wghArrayTrimCurve22.SetValue(1, 2);
	wghArrayTrimCurve22.SetValue(2, 2);
	wghArrayTrimCurve22.SetValue(3, 2.00318);
	wghArrayTrimCurve22.SetValue(4, 1.93961);
	wghArrayTrimCurve22.SetValue(5, 1.93961);
	wghArrayTrimCurve22.SetValue(6, 2.00318);
	wghArrayTrimCurve22.SetValue(7, 2);
	wghArrayTrimCurve22.SetValue(8, 2);
	wghArrayTrimCurve22.SetValue(9, 2.00318);
	wghArrayTrimCurve22.SetValue(10, 1.93961);
	wghArrayTrimCurve22.SetValue(11, 1.93961);
	wghArrayTrimCurve22.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve22(1, 5);
	knotsArrayTrimCurve22.SetValue(1, 0);
	knotsArrayTrimCurve22.SetValue(2, 1.5708);
	knotsArrayTrimCurve22.SetValue(3, 3.14159);
	knotsArrayTrimCurve22.SetValue(4, 4.71239);
	knotsArrayTrimCurve22.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve22(1, 5);
	multsArrayTrimCurve22.SetValue(1, 3);
	multsArrayTrimCurve22.SetValue(2, 3);
	multsArrayTrimCurve22.SetValue(3, 3);
	multsArrayTrimCurve22.SetValue(4, 3);
	multsArrayTrimCurve22.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve22;
	trimCurve22 = new Geom2d_BSplineCurve(cpArrayTrimCurve22, wghArrayTrimCurve22, knotsArrayTrimCurve22, multsArrayTrimCurve22, 4, Standard_True);
	BRepBuilderAPI_MakeEdge makeEdge22(trimCurve22, nurbsSurf1);
	TopoDS_Edge edge22 = makeEdge22.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve23(1, 2);
	cpArrayTrimCurve23.SetValue(1, gp_Pnt2d(10, -2.44929e-15));
	cpArrayTrimCurve23.SetValue(2, gp_Pnt2d(10, -2.44929e-15));
	TColStd_Array1OfReal knotsArrayTrimCurve23(1, 2);
	knotsArrayTrimCurve23.SetValue(1, 0);
	knotsArrayTrimCurve23.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve23(1, 2);
	multsArrayTrimCurve23.SetValue(1, 2);
	multsArrayTrimCurve23.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve23;
	trimCurve23 = new Geom2d_BSplineCurve(cpArrayTrimCurve23, knotsArrayTrimCurve23, multsArrayTrimCurve23, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge23(trimCurve23, nurbsSurf1);
	TopoDS_Edge edge23 = makeEdge23.Edge();
	BRepBuilderAPI_MakeWire wireMaker5;
	wireMaker5.Add(edge20);
	wireMaker5.Add(edge21);
	wireMaker5.Add(edge22);
	wireMaker5.Add(edge23);
	TopoDS_Wire wire5 = wireMaker5.Wire();
	BRepBuilderAPI_MakeFace faceMaker1(nurbsSurf1, wire3, false);
	faceMaker1.Add(wire4);
	faceMaker1.Add(wire5);


	TColgp_Array2OfPnt cpArray2(1, 2, 1, 2);
	cpArray2.SetValue(1, 1, gp_Pnt(-10, -10, 0));
	cpArray2.SetValue(1, 2, gp_Pnt(-10, 10, 0));
	cpArray2.SetValue(2, 1, gp_Pnt(10, -10, 0));
	cpArray2.SetValue(2, 2, gp_Pnt(10, 10, 0));
	TColStd_Array1OfReal uKnotsArray2(1, 2);
	uKnotsArray2.SetValue(1, -10);
	uKnotsArray2.SetValue(2, 10);
	TColStd_Array1OfInteger uMultsArray2(1, 2);
	uMultsArray2.SetValue(1, 2);
	uMultsArray2.SetValue(2, 2);
	TColStd_Array1OfReal vKnotsArray2(1, 2);
	vKnotsArray2.SetValue(1, -10);
	vKnotsArray2.SetValue(2, 10);
	TColStd_Array1OfInteger vMultsArray2(1, 2);
	vMultsArray2.SetValue(1, 2);
	vMultsArray2.SetValue(2, 2);
	Handle(Geom_BSplineSurface) nurbsSurf2;
	nurbsSurf2 = new Geom_BSplineSurface(cpArray2, uKnotsArray2, vKnotsArray2, uMultsArray2, vMultsArray2, 1, 1, Standard_False, Standard_False);



	TColgp_Array1OfPnt2d cpArrayTrimCurve24(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve24(1, 12);
	cpArrayTrimCurve24.SetValue(1, gp_Pnt2d(10, -3.92699));
	cpArrayTrimCurve24.SetValue(2, gp_Pnt2d(10, 3.92699));
	cpArrayTrimCurve24.SetValue(3, gp_Pnt2d(7.9471, 7.54514));
	cpArrayTrimCurve24.SetValue(4, gp_Pnt2d(4.3606, 10));
	cpArrayTrimCurve24.SetValue(5, gp_Pnt2d(-4.3606, 10));
	cpArrayTrimCurve24.SetValue(6, gp_Pnt2d(-7.9471, 7.54514));
	cpArrayTrimCurve24.SetValue(7, gp_Pnt2d(-10, 3.92699));
	cpArrayTrimCurve24.SetValue(8, gp_Pnt2d(-10, -3.92699));
	cpArrayTrimCurve24.SetValue(9, gp_Pnt2d(-7.9471, -7.54514));
	cpArrayTrimCurve24.SetValue(10, gp_Pnt2d(-4.3606, -10));
	cpArrayTrimCurve24.SetValue(11, gp_Pnt2d(4.3606, -10));
	cpArrayTrimCurve24.SetValue(12, gp_Pnt2d(7.9471, -7.54514));
	wghArrayTrimCurve24.SetValue(1, 2);
	wghArrayTrimCurve24.SetValue(2, 2);
	wghArrayTrimCurve24.SetValue(3, 2.00318);
	wghArrayTrimCurve24.SetValue(4, 1.93961);
	wghArrayTrimCurve24.SetValue(5, 1.93961);
	wghArrayTrimCurve24.SetValue(6, 2.00318);
	wghArrayTrimCurve24.SetValue(7, 2);
	wghArrayTrimCurve24.SetValue(8, 2);
	wghArrayTrimCurve24.SetValue(9, 2.00318);
	wghArrayTrimCurve24.SetValue(10, 1.93961);
	wghArrayTrimCurve24.SetValue(11, 1.93961);
	wghArrayTrimCurve24.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve24(1, 5);
	knotsArrayTrimCurve24.SetValue(1, 0);
	knotsArrayTrimCurve24.SetValue(2, 1.5708);
	knotsArrayTrimCurve24.SetValue(3, 3.14159);
	knotsArrayTrimCurve24.SetValue(4, 4.71239);
	knotsArrayTrimCurve24.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve24(1, 5);
	multsArrayTrimCurve24.SetValue(1, 3);
	multsArrayTrimCurve24.SetValue(2, 3);
	multsArrayTrimCurve24.SetValue(3, 3);
	multsArrayTrimCurve24.SetValue(4, 3);
	multsArrayTrimCurve24.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve24;
	trimCurve24 = new Geom2d_BSplineCurve(cpArrayTrimCurve24, wghArrayTrimCurve24, knotsArrayTrimCurve24, multsArrayTrimCurve24, 4, Standard_True);
	BRepBuilderAPI_MakeEdge makeEdge24(trimCurve24, nurbsSurf2);
	TopoDS_Edge edge24 = makeEdge24.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve25(1, 2);
	cpArrayTrimCurve25.SetValue(1, gp_Pnt2d(10, -2.44929e-15));
	cpArrayTrimCurve25.SetValue(2, gp_Pnt2d(10, -2.44929e-15));
	TColStd_Array1OfReal knotsArrayTrimCurve25(1, 2);
	knotsArrayTrimCurve25.SetValue(1, 0);
	knotsArrayTrimCurve25.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve25(1, 2);
	multsArrayTrimCurve25.SetValue(1, 2);
	multsArrayTrimCurve25.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve25;
	trimCurve25 = new Geom2d_BSplineCurve(cpArrayTrimCurve25, knotsArrayTrimCurve25, multsArrayTrimCurve25, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge25(trimCurve25, nurbsSurf2);
	TopoDS_Edge edge25 = makeEdge25.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve26(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve26(1, 12);
	cpArrayTrimCurve26.SetValue(1, gp_Pnt2d(10, -3.92699));
	cpArrayTrimCurve26.SetValue(2, gp_Pnt2d(10, 3.92699));
	cpArrayTrimCurve26.SetValue(3, gp_Pnt2d(7.9471, 7.54514));
	cpArrayTrimCurve26.SetValue(4, gp_Pnt2d(4.3606, 10));
	cpArrayTrimCurve26.SetValue(5, gp_Pnt2d(-4.3606, 10));
	cpArrayTrimCurve26.SetValue(6, gp_Pnt2d(-7.9471, 7.54514));
	cpArrayTrimCurve26.SetValue(7, gp_Pnt2d(-10, 3.92699));
	cpArrayTrimCurve26.SetValue(8, gp_Pnt2d(-10, -3.92699));
	cpArrayTrimCurve26.SetValue(9, gp_Pnt2d(-7.9471, -7.54514));
	cpArrayTrimCurve26.SetValue(10, gp_Pnt2d(-4.3606, -10));
	cpArrayTrimCurve26.SetValue(11, gp_Pnt2d(4.3606, -10));
	cpArrayTrimCurve26.SetValue(12, gp_Pnt2d(7.9471, -7.54514));
	wghArrayTrimCurve26.SetValue(1, 2);
	wghArrayTrimCurve26.SetValue(2, 2);
	wghArrayTrimCurve26.SetValue(3, 2.00318);
	wghArrayTrimCurve26.SetValue(4, 1.93961);
	wghArrayTrimCurve26.SetValue(5, 1.93961);
	wghArrayTrimCurve26.SetValue(6, 2.00318);
	wghArrayTrimCurve26.SetValue(7, 2);
	wghArrayTrimCurve26.SetValue(8, 2);
	wghArrayTrimCurve26.SetValue(9, 2.00318);
	wghArrayTrimCurve26.SetValue(10, 1.93961);
	wghArrayTrimCurve26.SetValue(11, 1.93961);
	wghArrayTrimCurve26.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve26(1, 5);
	knotsArrayTrimCurve26.SetValue(1, 0);
	knotsArrayTrimCurve26.SetValue(2, 1.5708);
	knotsArrayTrimCurve26.SetValue(3, 3.14159);
	knotsArrayTrimCurve26.SetValue(4, 4.71239);
	knotsArrayTrimCurve26.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve26(1, 5);
	multsArrayTrimCurve26.SetValue(1, 3);
	multsArrayTrimCurve26.SetValue(2, 3);
	multsArrayTrimCurve26.SetValue(3, 3);
	multsArrayTrimCurve26.SetValue(4, 3);
	multsArrayTrimCurve26.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve26;
	trimCurve26 = new Geom2d_BSplineCurve(cpArrayTrimCurve26, wghArrayTrimCurve26, knotsArrayTrimCurve26, multsArrayTrimCurve26, 4, Standard_True);
	BRepBuilderAPI_MakeEdge makeEdge26(trimCurve26, nurbsSurf2);
	TopoDS_Edge edge26 = makeEdge26.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve27(1, 2);
	cpArrayTrimCurve27.SetValue(1, gp_Pnt2d(10, -2.44929e-15));
	cpArrayTrimCurve27.SetValue(2, gp_Pnt2d(10, -2.44929e-15));
	TColStd_Array1OfReal knotsArrayTrimCurve27(1, 2);
	knotsArrayTrimCurve27.SetValue(1, 0);
	knotsArrayTrimCurve27.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve27(1, 2);
	multsArrayTrimCurve27.SetValue(1, 2);
	multsArrayTrimCurve27.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve27;
	trimCurve27 = new Geom2d_BSplineCurve(cpArrayTrimCurve27, knotsArrayTrimCurve27, multsArrayTrimCurve27, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge27(trimCurve27, nurbsSurf2);
	TopoDS_Edge edge27 = makeEdge27.Edge();
	BRepBuilderAPI_MakeWire wireMaker6;
	wireMaker6.Add(edge24);
	wireMaker6.Add(edge25);
	wireMaker6.Add(edge26);
	wireMaker6.Add(edge27);
	TopoDS_Wire wire6 = wireMaker6.Wire();
	TColgp_Array1OfPnt2d cpArrayTrimCurve28(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve28(1, 12);
	cpArrayTrimCurve28.SetValue(1, gp_Pnt2d(10, -3.92699));
	cpArrayTrimCurve28.SetValue(2, gp_Pnt2d(10, 3.92699));
	cpArrayTrimCurve28.SetValue(3, gp_Pnt2d(7.9471, 7.54514));
	cpArrayTrimCurve28.SetValue(4, gp_Pnt2d(4.3606, 10));
	cpArrayTrimCurve28.SetValue(5, gp_Pnt2d(-4.3606, 10));
	cpArrayTrimCurve28.SetValue(6, gp_Pnt2d(-7.9471, 7.54514));
	cpArrayTrimCurve28.SetValue(7, gp_Pnt2d(-10, 3.92699));
	cpArrayTrimCurve28.SetValue(8, gp_Pnt2d(-10, -3.92699));
	cpArrayTrimCurve28.SetValue(9, gp_Pnt2d(-7.9471, -7.54514));
	cpArrayTrimCurve28.SetValue(10, gp_Pnt2d(-4.3606, -10));
	cpArrayTrimCurve28.SetValue(11, gp_Pnt2d(4.3606, -10));
	cpArrayTrimCurve28.SetValue(12, gp_Pnt2d(7.9471, -7.54514));
	wghArrayTrimCurve28.SetValue(1, 2);
	wghArrayTrimCurve28.SetValue(2, 2);
	wghArrayTrimCurve28.SetValue(3, 2.00318);
	wghArrayTrimCurve28.SetValue(4, 1.93961);
	wghArrayTrimCurve28.SetValue(5, 1.93961);
	wghArrayTrimCurve28.SetValue(6, 2.00318);
	wghArrayTrimCurve28.SetValue(7, 2);
	wghArrayTrimCurve28.SetValue(8, 2);
	wghArrayTrimCurve28.SetValue(9, 2.00318);
	wghArrayTrimCurve28.SetValue(10, 1.93961);
	wghArrayTrimCurve28.SetValue(11, 1.93961);
	wghArrayTrimCurve28.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve28(1, 5);
	knotsArrayTrimCurve28.SetValue(1, 0);
	knotsArrayTrimCurve28.SetValue(2, 1.5708);
	knotsArrayTrimCurve28.SetValue(3, 3.14159);
	knotsArrayTrimCurve28.SetValue(4, 4.71239);
	knotsArrayTrimCurve28.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve28(1, 5);
	multsArrayTrimCurve28.SetValue(1, 3);
	multsArrayTrimCurve28.SetValue(2, 3);
	multsArrayTrimCurve28.SetValue(3, 3);
	multsArrayTrimCurve28.SetValue(4, 3);
	multsArrayTrimCurve28.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve28;
	trimCurve28 = new Geom2d_BSplineCurve(cpArrayTrimCurve28, wghArrayTrimCurve28, knotsArrayTrimCurve28, multsArrayTrimCurve28, 4, Standard_True);
	BRepBuilderAPI_MakeEdge makeEdge28(trimCurve28, nurbsSurf2);
	TopoDS_Edge edge28 = makeEdge28.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve29(1, 2);
	cpArrayTrimCurve29.SetValue(1, gp_Pnt2d(10, -2.44929e-15));
	cpArrayTrimCurve29.SetValue(2, gp_Pnt2d(10, -2.44929e-15));
	TColStd_Array1OfReal knotsArrayTrimCurve29(1, 2);
	knotsArrayTrimCurve29.SetValue(1, 0);
	knotsArrayTrimCurve29.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve29(1, 2);
	multsArrayTrimCurve29.SetValue(1, 2);
	multsArrayTrimCurve29.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve29;
	trimCurve29 = new Geom2d_BSplineCurve(cpArrayTrimCurve29, knotsArrayTrimCurve29, multsArrayTrimCurve29, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge29(trimCurve29, nurbsSurf2);
	TopoDS_Edge edge29 = makeEdge29.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve30(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve30(1, 12);
	cpArrayTrimCurve30.SetValue(1, gp_Pnt2d(10, -3.92699));
	cpArrayTrimCurve30.SetValue(2, gp_Pnt2d(10, 3.92699));
	cpArrayTrimCurve30.SetValue(3, gp_Pnt2d(7.9471, 7.54514));
	cpArrayTrimCurve30.SetValue(4, gp_Pnt2d(4.3606, 10));
	cpArrayTrimCurve30.SetValue(5, gp_Pnt2d(-4.3606, 10));
	cpArrayTrimCurve30.SetValue(6, gp_Pnt2d(-7.9471, 7.54514));
	cpArrayTrimCurve30.SetValue(7, gp_Pnt2d(-10, 3.92699));
	cpArrayTrimCurve30.SetValue(8, gp_Pnt2d(-10, -3.92699));
	cpArrayTrimCurve30.SetValue(9, gp_Pnt2d(-7.9471, -7.54514));
	cpArrayTrimCurve30.SetValue(10, gp_Pnt2d(-4.3606, -10));
	cpArrayTrimCurve30.SetValue(11, gp_Pnt2d(4.3606, -10));
	cpArrayTrimCurve30.SetValue(12, gp_Pnt2d(7.9471, -7.54514));
	wghArrayTrimCurve30.SetValue(1, 2);
	wghArrayTrimCurve30.SetValue(2, 2);
	wghArrayTrimCurve30.SetValue(3, 2.00318);
	wghArrayTrimCurve30.SetValue(4, 1.93961);
	wghArrayTrimCurve30.SetValue(5, 1.93961);
	wghArrayTrimCurve30.SetValue(6, 2.00318);
	wghArrayTrimCurve30.SetValue(7, 2);
	wghArrayTrimCurve30.SetValue(8, 2);
	wghArrayTrimCurve30.SetValue(9, 2.00318);
	wghArrayTrimCurve30.SetValue(10, 1.93961);
	wghArrayTrimCurve30.SetValue(11, 1.93961);
	wghArrayTrimCurve30.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve30(1, 5);
	knotsArrayTrimCurve30.SetValue(1, 0);
	knotsArrayTrimCurve30.SetValue(2, 1.5708);
	knotsArrayTrimCurve30.SetValue(3, 3.14159);
	knotsArrayTrimCurve30.SetValue(4, 4.71239);
	knotsArrayTrimCurve30.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve30(1, 5);
	multsArrayTrimCurve30.SetValue(1, 3);
	multsArrayTrimCurve30.SetValue(2, 3);
	multsArrayTrimCurve30.SetValue(3, 3);
	multsArrayTrimCurve30.SetValue(4, 3);
	multsArrayTrimCurve30.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve30;
	trimCurve30 = new Geom2d_BSplineCurve(cpArrayTrimCurve30, wghArrayTrimCurve30, knotsArrayTrimCurve30, multsArrayTrimCurve30, 4, Standard_True);
	BRepBuilderAPI_MakeEdge makeEdge30(trimCurve30, nurbsSurf2);
	TopoDS_Edge edge30 = makeEdge30.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve31(1, 2);
	cpArrayTrimCurve31.SetValue(1, gp_Pnt2d(10, -2.44929e-15));
	cpArrayTrimCurve31.SetValue(2, gp_Pnt2d(10, -2.44929e-15));
	TColStd_Array1OfReal knotsArrayTrimCurve31(1, 2);
	knotsArrayTrimCurve31.SetValue(1, 0);
	knotsArrayTrimCurve31.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve31(1, 2);
	multsArrayTrimCurve31.SetValue(1, 2);
	multsArrayTrimCurve31.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve31;
	trimCurve31 = new Geom2d_BSplineCurve(cpArrayTrimCurve31, knotsArrayTrimCurve31, multsArrayTrimCurve31, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge31(trimCurve31, nurbsSurf2);
	TopoDS_Edge edge31 = makeEdge31.Edge();
	BRepBuilderAPI_MakeWire wireMaker7;
	wireMaker7.Add(edge28);
	wireMaker7.Add(edge29);
	wireMaker7.Add(edge30);
	wireMaker7.Add(edge31);
	TopoDS_Wire wire7 = wireMaker7.Wire();
	TColgp_Array1OfPnt2d cpArrayTrimCurve32(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve32(1, 12);
	cpArrayTrimCurve32.SetValue(1, gp_Pnt2d(10, -3.92699));
	cpArrayTrimCurve32.SetValue(2, gp_Pnt2d(10, 3.92699));
	cpArrayTrimCurve32.SetValue(3, gp_Pnt2d(7.9471, 7.54514));
	cpArrayTrimCurve32.SetValue(4, gp_Pnt2d(4.3606, 10));
	cpArrayTrimCurve32.SetValue(5, gp_Pnt2d(-4.3606, 10));
	cpArrayTrimCurve32.SetValue(6, gp_Pnt2d(-7.9471, 7.54514));
	cpArrayTrimCurve32.SetValue(7, gp_Pnt2d(-10, 3.92699));
	cpArrayTrimCurve32.SetValue(8, gp_Pnt2d(-10, -3.92699));
	cpArrayTrimCurve32.SetValue(9, gp_Pnt2d(-7.9471, -7.54514));
	cpArrayTrimCurve32.SetValue(10, gp_Pnt2d(-4.3606, -10));
	cpArrayTrimCurve32.SetValue(11, gp_Pnt2d(4.3606, -10));
	cpArrayTrimCurve32.SetValue(12, gp_Pnt2d(7.9471, -7.54514));
	wghArrayTrimCurve32.SetValue(1, 2);
	wghArrayTrimCurve32.SetValue(2, 2);
	wghArrayTrimCurve32.SetValue(3, 2.00318);
	wghArrayTrimCurve32.SetValue(4, 1.93961);
	wghArrayTrimCurve32.SetValue(5, 1.93961);
	wghArrayTrimCurve32.SetValue(6, 2.00318);
	wghArrayTrimCurve32.SetValue(7, 2);
	wghArrayTrimCurve32.SetValue(8, 2);
	wghArrayTrimCurve32.SetValue(9, 2.00318);
	wghArrayTrimCurve32.SetValue(10, 1.93961);
	wghArrayTrimCurve32.SetValue(11, 1.93961);
	wghArrayTrimCurve32.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve32(1, 5);
	knotsArrayTrimCurve32.SetValue(1, 0);
	knotsArrayTrimCurve32.SetValue(2, 1.5708);
	knotsArrayTrimCurve32.SetValue(3, 3.14159);
	knotsArrayTrimCurve32.SetValue(4, 4.71239);
	knotsArrayTrimCurve32.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve32(1, 5);
	multsArrayTrimCurve32.SetValue(1, 3);
	multsArrayTrimCurve32.SetValue(2, 3);
	multsArrayTrimCurve32.SetValue(3, 3);
	multsArrayTrimCurve32.SetValue(4, 3);
	multsArrayTrimCurve32.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve32;
	trimCurve32 = new Geom2d_BSplineCurve(cpArrayTrimCurve32, wghArrayTrimCurve32, knotsArrayTrimCurve32, multsArrayTrimCurve32, 4, Standard_True);
	BRepBuilderAPI_MakeEdge makeEdge32(trimCurve32, nurbsSurf2);
	TopoDS_Edge edge32 = makeEdge32.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve33(1, 2);
	cpArrayTrimCurve33.SetValue(1, gp_Pnt2d(10, -2.44929e-15));
	cpArrayTrimCurve33.SetValue(2, gp_Pnt2d(10, -2.44929e-15));
	TColStd_Array1OfReal knotsArrayTrimCurve33(1, 2);
	knotsArrayTrimCurve33.SetValue(1, 0);
	knotsArrayTrimCurve33.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve33(1, 2);
	multsArrayTrimCurve33.SetValue(1, 2);
	multsArrayTrimCurve33.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve33;
	trimCurve33 = new Geom2d_BSplineCurve(cpArrayTrimCurve33, knotsArrayTrimCurve33, multsArrayTrimCurve33, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge33(trimCurve33, nurbsSurf2);
	TopoDS_Edge edge33 = makeEdge33.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve34(1, 12);
	TColStd_Array1OfReal wghArrayTrimCurve34(1, 12);
	cpArrayTrimCurve34.SetValue(1, gp_Pnt2d(10, -3.92699));
	cpArrayTrimCurve34.SetValue(2, gp_Pnt2d(10, 3.92699));
	cpArrayTrimCurve34.SetValue(3, gp_Pnt2d(7.9471, 7.54514));
	cpArrayTrimCurve34.SetValue(4, gp_Pnt2d(4.3606, 10));
	cpArrayTrimCurve34.SetValue(5, gp_Pnt2d(-4.3606, 10));
	cpArrayTrimCurve34.SetValue(6, gp_Pnt2d(-7.9471, 7.54514));
	cpArrayTrimCurve34.SetValue(7, gp_Pnt2d(-10, 3.92699));
	cpArrayTrimCurve34.SetValue(8, gp_Pnt2d(-10, -3.92699));
	cpArrayTrimCurve34.SetValue(9, gp_Pnt2d(-7.9471, -7.54514));
	cpArrayTrimCurve34.SetValue(10, gp_Pnt2d(-4.3606, -10));
	cpArrayTrimCurve34.SetValue(11, gp_Pnt2d(4.3606, -10));
	cpArrayTrimCurve34.SetValue(12, gp_Pnt2d(7.9471, -7.54514));
	wghArrayTrimCurve34.SetValue(1, 2);
	wghArrayTrimCurve34.SetValue(2, 2);
	wghArrayTrimCurve34.SetValue(3, 2.00318);
	wghArrayTrimCurve34.SetValue(4, 1.93961);
	wghArrayTrimCurve34.SetValue(5, 1.93961);
	wghArrayTrimCurve34.SetValue(6, 2.00318);
	wghArrayTrimCurve34.SetValue(7, 2);
	wghArrayTrimCurve34.SetValue(8, 2);
	wghArrayTrimCurve34.SetValue(9, 2.00318);
	wghArrayTrimCurve34.SetValue(10, 1.93961);
	wghArrayTrimCurve34.SetValue(11, 1.93961);
	wghArrayTrimCurve34.SetValue(12, 2.00318);
	TColStd_Array1OfReal knotsArrayTrimCurve34(1, 5);
	knotsArrayTrimCurve34.SetValue(1, 0);
	knotsArrayTrimCurve34.SetValue(2, 1.5708);
	knotsArrayTrimCurve34.SetValue(3, 3.14159);
	knotsArrayTrimCurve34.SetValue(4, 4.71239);
	knotsArrayTrimCurve34.SetValue(5, 6.28319);
	TColStd_Array1OfInteger multsArrayTrimCurve34(1, 5);
	multsArrayTrimCurve34.SetValue(1, 3);
	multsArrayTrimCurve34.SetValue(2, 3);
	multsArrayTrimCurve34.SetValue(3, 3);
	multsArrayTrimCurve34.SetValue(4, 3);
	multsArrayTrimCurve34.SetValue(5, 3);
	Handle(Geom2d_BSplineCurve) trimCurve34;
	trimCurve34 = new Geom2d_BSplineCurve(cpArrayTrimCurve34, wghArrayTrimCurve34, knotsArrayTrimCurve34, multsArrayTrimCurve34, 4, Standard_True);
	BRepBuilderAPI_MakeEdge makeEdge34(trimCurve34, nurbsSurf2);
	TopoDS_Edge edge34 = makeEdge34.Edge();
	TColgp_Array1OfPnt2d cpArrayTrimCurve35(1, 2);
	cpArrayTrimCurve35.SetValue(1, gp_Pnt2d(10, -2.44929e-15));
	cpArrayTrimCurve35.SetValue(2, gp_Pnt2d(10, -2.44929e-15));
	TColStd_Array1OfReal knotsArrayTrimCurve35(1, 2);
	knotsArrayTrimCurve35.SetValue(1, 0);
	knotsArrayTrimCurve35.SetValue(2, 25);
	TColStd_Array1OfInteger multsArrayTrimCurve35(1, 2);
	multsArrayTrimCurve35.SetValue(1, 2);
	multsArrayTrimCurve35.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve35;
	trimCurve35 = new Geom2d_BSplineCurve(cpArrayTrimCurve35, knotsArrayTrimCurve35, multsArrayTrimCurve35, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge35(trimCurve35, nurbsSurf2);
	TopoDS_Edge edge35 = makeEdge35.Edge();
	BRepBuilderAPI_MakeWire wireMaker8;
	wireMaker8.Add(edge32);
	wireMaker8.Add(edge33);
	wireMaker8.Add(edge34);
	wireMaker8.Add(edge35);
	TopoDS_Wire wire8 = wireMaker8.Wire();
	BRepBuilderAPI_MakeFace faceMaker2(nurbsSurf2, wire6, false);
	faceMaker2.Add(wire7);
	faceMaker2.Add(wire8);



	// Fix to recover 3D curves
	ShapeFix_Face fix0(faceMaker0.Face());
	fix0.Perform();
	ShapeFix_Face fix1(faceMaker1.Face());
	fix1.Perform();
	ShapeFix_Face fix2(faceMaker2.Face());
	fix2.Perform();

	// Get the generated face
	//TopoDS_Face Nurbsface = faceMaker.Face();
	TopoDS_Face Nurbsface0 = fix0.Face();
	TopoDS_Face Nurbsface1 = fix1.Face();
	TopoDS_Face Nurbsface2 = fix2.Face();

	//ShapeFix_Face faceFix(Nurbsface);
	//faceFix.FixIntersectingWires();
	//faceFix.FixMissingSeam();
	//faceFix.FixOrientation();
	//faceFix.Perform();
	//TopoDS_Face builtFace = faceFix.Face();

	trimmedNurbsShape0 = new AIS_Shape(Nurbsface0);
	trimmedNurbsShape1 = new AIS_Shape(Nurbsface1);
	trimmedNurbsShape2 = new AIS_Shape(Nurbsface2);
	if (trimmedNurbsShape0.IsNull() == false) {

		// Add the trimmed nurbs to the graphic context
		myAISContext->SetDisplayMode(trimmedNurbsShape0, AIS_Shaded);
		myAISContext->Display(trimmedNurbsShape0);  // Draw the face on the Screen  
		myAISContext->DisplayAll();

		// Update the viewer
		myViewer->InitActiveViews();
		Handle(V3d_View) activeView = myViewer->ActiveView();
		if (activeView.IsNull() == false) {
			activeView->FitAll();           // Focus to the View to the Drawn face.  
		}
	}
	if (trimmedNurbsShape1.IsNull() == false) {

		// Add the trimmed nurbs to the graphic context
		myAISContext->SetDisplayMode(trimmedNurbsShape1, AIS_Shaded);
		myAISContext->Display(trimmedNurbsShape1);  // Draw the face on the Screen  
		myAISContext->DisplayAll();

		// Update the viewer
		myViewer->InitActiveViews();
		Handle(V3d_View) activeView = myViewer->ActiveView();
		if (activeView.IsNull() == false) {
			activeView->FitAll();           // Focus to the View to the Drawn face.  
		}
	}
	if (trimmedNurbsShape2.IsNull() == false) {

		// Add the trimmed nurbs to the graphic context
		myAISContext->SetDisplayMode(trimmedNurbsShape2, AIS_Shaded);
		myAISContext->Display(trimmedNurbsShape2);  // Draw the face on the Screen  
		myAISContext->DisplayAll();

		// Update the viewer
		myViewer->InitActiveViews();
		Handle(V3d_View) activeView = myViewer->ActiveView();
		if (activeView.IsNull() == false) {
			activeView->FitAll();           // Focus to the View to the Drawn face.  
		}
	}

	// Dump in brep format
	std::filebuf aFileBuf;
	std::ostream aStream(&aFileBuf);
	if (aFileBuf.open("F:\\Sviluppo\\OCCSampleApp\\trimmedNurbsShape.brep", ios::out)) {
		BRepTools::Write(Nurbsface0, aStream);
	}
	aFileBuf.close();

	// Dump in stl format
	Handle(StlMesh_Mesh) aShapeMesh;
	generateMesh(Nurbsface0, 0.01, 0.5, aShapeMesh);
	saveMesh(aShapeMesh, L"C:\\Sviluppo\\OCCSampleApp\\nurbsTrimMesh.stl");
}


void COCCSampleAppDoc::OnTestAddCone()
{
	// TODO: Add your command handler code here
	AddCone();
}
