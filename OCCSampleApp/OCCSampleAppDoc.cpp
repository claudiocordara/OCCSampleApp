
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

			outputFile << "TColgp_Array2OfPnt cpArray(" << cpArray.LowerRow() << "," << cpArray.UpperRow() << "," << cpArray.LowerCol() << "," << cpArray.UpperCol() << ");" << std::endl;
			if (wghArray != NULL) {
				outputFile << "TColStd_Array2OfReal wghArray(" << wghArray->LowerRow() << "," << wghArray->UpperRow() << "," << wghArray->LowerCol() << "," << wghArray->UpperCol() << ");" << std::endl;
			}
			for (int i = cpArray.LowerRow(); i <= cpArray.UpperRow(); i++) {
				for (int j = cpArray.LowerCol(); j <= cpArray.UpperCol(); j++) {
					outputFile << "cpArray.SetValue(" << i << "," << j << ",gp_Pnt(" << cpArray.Value(i, j).X() << "," << cpArray.Value(i, j).Y() << "," << cpArray.Value(i, j).Z() << "));" << std::endl;
				}
			}
			if (wghArray != NULL) {
				for (int i = wghArray->LowerRow(); i <= wghArray->UpperRow(); i++) {
					for (int j = wghArray->LowerCol(); j <= wghArray->UpperCol(); j++) {
						outputFile << "wghArray.SetValue(" << i << "," << j << "," << wghArray->Value(i, j) << ");" << std::endl;
					}
				}
			}

			outputFile << "TColStd_Array1OfReal uKnotsArray(" << uKnotsArray.Lower() << "," << uKnotsArray.Upper() << ");" << std::endl;
			for (int i = uKnotsArray.Lower(); i <= uKnotsArray.Upper(); i++) {
				outputFile << "uKnotsArray.SetValue(" << i << "," << uKnotsArray.Value(i) << ");" << std::endl;
			}
			outputFile << "TColStd_Array1OfInteger uMultsArray(" << uMultsArray.Lower() << "," << uMultsArray.Upper() << ");" << std::endl;
			for (int i = uMultsArray.Lower(); i <= uMultsArray.Upper(); i++) {
				outputFile << "uMultsArray.SetValue(" << i << "," << uMultsArray.Value(i) << ");" << std::endl;
			}


			outputFile << "TColStd_Array1OfReal vKnotsArray(" << vKnotsArray.Lower() << "," << vKnotsArray.Upper() << ");" << std::endl;
			for (int i = vKnotsArray.Lower(); i <= vKnotsArray.Upper(); i++) {
				outputFile << "vKnotsArray.SetValue(" << i << "," << vKnotsArray.Value(i) << ");" << std::endl;
			}
			outputFile << "TColStd_Array1OfInteger vMultsArray(" << vMultsArray.Lower() << "," << vMultsArray.Upper() << ");" << std::endl;
			for (int i = vMultsArray.Lower(); i <= vMultsArray.Upper(); i++) {
				outputFile << "vMultsArray.SetValue(" << i << "," << vMultsArray.Value(i) << ");" << std::endl;
			}

			outputFile << "Handle(Geom_BSplineSurface) nurbsSurf;" << std::endl;
			outputFile << "nurbsSurf = new Geom_BSplineSurface(cpArray, " << (wghArray != NULL ? "wghArray, " : "") << "uKnotsArray, vKnotsArray, uMultsArray, vMultsArray, " << uDegree << "," << vDegree << "," << (isUPeriodic == Standard_True ? "Standard_True" : "Standard_False") << "," << (isVPeriodic == Standard_True ? "Standard_True" : "Standard_False") << ");" << std::endl;

			outputFile << std::endl << std::endl << std::endl;

			int edgeCounter = 0;
			TopExp_Explorer edgeExpl(_shape, TopAbs_EDGE);
			while (edgeExpl.More() == Standard_True) {
				TopoDS_Shape aShapeEdge = edgeExpl.Current();
				TopoDS_Edge aEdge = TopoDS::Edge(aShapeEdge);


				Standard_Real aFirst, aLast, aPFirst, aPLast;
				Handle(Geom_Curve) aCurve3d = BRep_Tool::Curve(aEdge, aFirst, aLast);
				Handle(Geom2d_Curve) aPCurve = BRep_Tool::CurveOnSurface(aEdge, aFace, aPFirst, aPLast);

				if (aPCurve->IsKind(Geom2d_Line::get_type_descriptor()) == Standard_False) {
					Handle(Geom2d_BSplineCurve) nurbsCurve = Geom2dConvert::CurveToBSplineCurve(aPCurve, Convert_RationalC1);

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
				}

				edgeExpl.Next();
				edgeCounter++;
			}


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
	//Surface

	TColgp_Array2OfPnt cpArray(1, 9, 1, 2);
	TColStd_Array2OfReal wghArray(1, 9, 1, 2);
	cpArray.SetValue(1, 1, gp_Pnt(0, 0, 10));
	wghArray.SetValue(1, 1, 1);
	cpArray.SetValue(1, 2, gp_Pnt(10, 0, 0));
	wghArray.SetValue(1, 2, 1);
	cpArray.SetValue(2, 1, gp_Pnt(0, 0, 7.07107));
	wghArray.SetValue(2, 1, 0.707107);
	cpArray.SetValue(2, 2, gp_Pnt(7.07107, -7.07107, 0));
	wghArray.SetValue(2, 2, 0.707107);
	cpArray.SetValue(3, 1, gp_Pnt(0, 0, 10));
	wghArray.SetValue(3, 1, 1);
	cpArray.SetValue(3, 2, gp_Pnt(6.12323e-16, -10, 0));
	wghArray.SetValue(3, 2, 1);
	cpArray.SetValue(4, 1, gp_Pnt(0, 0, 7.07107));
	wghArray.SetValue(4, 1, 0.707107);
	cpArray.SetValue(4, 2, gp_Pnt(-7.07107, -7.07107, 0));
	wghArray.SetValue(4, 2, 0.707107);
	cpArray.SetValue(5, 1, gp_Pnt(0, 0, 10));
	wghArray.SetValue(5, 1, 1);
	cpArray.SetValue(5, 2, gp_Pnt(-10, -1.22465e-15, 0));
	wghArray.SetValue(5, 2, 1);
	cpArray.SetValue(6, 1, gp_Pnt(0, 0, 7.07107));
	wghArray.SetValue(6, 1, 0.707107);
	cpArray.SetValue(6, 2, gp_Pnt(-7.07107, 7.07107, 0));
	wghArray.SetValue(6, 2, 0.707107);
	cpArray.SetValue(7, 1, gp_Pnt(0, 0, 10));
	wghArray.SetValue(7, 1, 1);
	cpArray.SetValue(7, 2, gp_Pnt(-1.83697e-15, 10, 0));
	wghArray.SetValue(7, 2, 1);
	cpArray.SetValue(8, 1, gp_Pnt(0, 0, 7.07107));
	wghArray.SetValue(8, 1, 0.707107);
	cpArray.SetValue(8, 2, gp_Pnt(7.07107, 7.07107, 0));
	wghArray.SetValue(8, 2, 0.707107);
	cpArray.SetValue(9, 1, gp_Pnt(0, 0, 10));
	wghArray.SetValue(9, 1, 1);
	cpArray.SetValue(9, 2, gp_Pnt(10, 0, 0));
	wghArray.SetValue(9, 2, 1);
	TColStd_Array1OfReal uKnotsArray(1, 5);
	TColStd_Array1OfInteger uMultsArray(1, 5);
	uKnotsArray.SetValue(1, 0);
	uMultsArray.SetValue(1, 3);
	uKnotsArray.SetValue(2, 15.708);
	uMultsArray.SetValue(2, 2);
	uKnotsArray.SetValue(3, 31.4159);
	uMultsArray.SetValue(3, 2);
	uKnotsArray.SetValue(4, 47.1239);
	uMultsArray.SetValue(4, 2);
	uKnotsArray.SetValue(5, 62.8319);
	uMultsArray.SetValue(5, 3);
	TColStd_Array1OfReal vKnotsArray(1, 2);
	TColStd_Array1OfInteger vMultsArray(1, 2);
	vKnotsArray.SetValue(1, 0);
	vMultsArray.SetValue(1, 2);
	vKnotsArray.SetValue(2, 10);
	vMultsArray.SetValue(2, 2);
	Handle(Geom_BSplineSurface) nurbsSurf;
	nurbsSurf = new Geom_BSplineSurface(cpArray, wghArray, uKnotsArray, vKnotsArray, uMultsArray, vMultsArray, 2, 1, Standard_False, Standard_False);

	//Trim Curve

	TColgp_Array1OfPnt2d cpArrayTrimCurve0(1, 2);
	TColStd_Array1OfReal wghArrayTrimCurve0(1, 2);
	cpArrayTrimCurve0.SetValue(1, gp_Pnt2d(0, 0));
	wghArrayTrimCurve0.SetValue(1, 1);
	cpArrayTrimCurve0.SetValue(2, gp_Pnt2d(62.8319, 0));
	wghArrayTrimCurve0.SetValue(2, 1);
	TColStd_Array1OfReal knotsArrayTrimCurve0(1, 2);
	TColStd_Array1OfInteger multsArrayTrimCurve0(1, 2);
	knotsArrayTrimCurve0.SetValue(1, 0);
	multsArrayTrimCurve0.SetValue(1, 2);
	knotsArrayTrimCurve0.SetValue(2, 1);
	multsArrayTrimCurve0.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve0;
	trimCurve0 = new Geom2d_BSplineCurve(cpArrayTrimCurve0, wghArrayTrimCurve0, knotsArrayTrimCurve0, multsArrayTrimCurve0, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge0(trimCurve0, nurbsSurf);
	TopoDS_Edge edge0 = makeEdge0.Edge();

	//Trim Curve

	TColgp_Array1OfPnt2d cpArrayTrimCurve1(1, 2);
	TColStd_Array1OfReal wghArrayTrimCurve1(1, 2);
	cpArrayTrimCurve1.SetValue(1, gp_Pnt2d(62.8319, 0));
	wghArrayTrimCurve1.SetValue(1, 1);
	cpArrayTrimCurve1.SetValue(2, gp_Pnt2d(62.8319, 10));
	wghArrayTrimCurve1.SetValue(2, 1);
	TColStd_Array1OfReal knotsArrayTrimCurve1(1, 2);
	TColStd_Array1OfInteger multsArrayTrimCurve1(1, 2);
	knotsArrayTrimCurve1.SetValue(1, 0);
	multsArrayTrimCurve1.SetValue(1, 2);
	knotsArrayTrimCurve1.SetValue(2, 1);
	multsArrayTrimCurve1.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve1;
	trimCurve1 = new Geom2d_BSplineCurve(cpArrayTrimCurve1, wghArrayTrimCurve1, knotsArrayTrimCurve1, multsArrayTrimCurve1, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge1(trimCurve1, nurbsSurf);
	TopoDS_Edge edge1 = makeEdge1.Edge();

	//Trim Curve

	TColgp_Array1OfPnt2d cpArrayTrimCurve2(1, 2);
	TColStd_Array1OfReal wghArrayTrimCurve2(1, 2);
	cpArrayTrimCurve2.SetValue(1, gp_Pnt2d(62.8319, 10));
	wghArrayTrimCurve2.SetValue(1, 1);
	cpArrayTrimCurve2.SetValue(2, gp_Pnt2d(0, 10));
	wghArrayTrimCurve2.SetValue(2, 1);
	TColStd_Array1OfReal knotsArrayTrimCurve2(1, 2);
	TColStd_Array1OfInteger multsArrayTrimCurve2(1, 2);
	knotsArrayTrimCurve2.SetValue(1, 0);
	multsArrayTrimCurve2.SetValue(1, 2);
	knotsArrayTrimCurve2.SetValue(2, 1);
	multsArrayTrimCurve2.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve2;
	trimCurve2 = new Geom2d_BSplineCurve(cpArrayTrimCurve2, wghArrayTrimCurve2, knotsArrayTrimCurve2, multsArrayTrimCurve2, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge2(trimCurve2, nurbsSurf);
	TopoDS_Edge edge2 = makeEdge2.Edge();

	//Trim Curve

	TColgp_Array1OfPnt2d cpArrayTrimCurve3(1, 2);
	TColStd_Array1OfReal wghArrayTrimCurve3(1, 2);
	cpArrayTrimCurve3.SetValue(1, gp_Pnt2d(0, 10));
	wghArrayTrimCurve3.SetValue(1, 1);
	cpArrayTrimCurve3.SetValue(2, gp_Pnt2d(0, 0));
	wghArrayTrimCurve3.SetValue(2, 1);
	TColStd_Array1OfReal knotsArrayTrimCurve3(1, 2);
	TColStd_Array1OfInteger multsArrayTrimCurve3(1, 2);
	knotsArrayTrimCurve3.SetValue(1, 0);
	multsArrayTrimCurve3.SetValue(1, 2);
	knotsArrayTrimCurve3.SetValue(2, 1);
	multsArrayTrimCurve3.SetValue(2, 2);
	Handle(Geom2d_BSplineCurve) trimCurve3;
	trimCurve3 = new Geom2d_BSplineCurve(cpArrayTrimCurve3, wghArrayTrimCurve3, knotsArrayTrimCurve3, multsArrayTrimCurve3, 1, Standard_False);
	BRepBuilderAPI_MakeEdge makeEdge3(trimCurve3, nurbsSurf);
	TopoDS_Edge edge3 = makeEdge3.Edge();
	BRepBuilderAPI_MakeWire wireMaker0;
	wireMaker0.Add(edge0);
	wireMaker0.Add(edge1);
	wireMaker0.Add(edge2);
	wireMaker0.Add(edge3);
	TopoDS_Wire wire0 = wireMaker0.Wire();
	BRepBuilderAPI_MakeFace faceMaker(nurbsSurf, wire0, false);

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
	saveMesh(aShapeMesh, L"C:\\Sviluppo\\OCCSampleApp\\nurbsTrimMesh.stl");
}


void COCCSampleAppDoc::OnTestAddCone()
{
	// TODO: Add your command handler code here
	AddCone();
}
