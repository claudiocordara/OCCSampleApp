
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
#include <ShapeFix_Wire.hxx>
#include <ShapeBuild_Edge.hxx>

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


bool COCCSampleAppDoc::generateMesh(std::vector<TopoDS_Shape> _shapeArray, Standard_Real _linearDeflection, Standard_Real _angularDeflection, std::vector<Handle(StlMesh_Mesh)> _meshArray) {
	for (int i = 0; i < (int)_shapeArray.size(); i++) {
		Handle(StlMesh_Mesh) aShapeMesh;
		generateMesh(_shapeArray[i], _linearDeflection, _angularDeflection, aShapeMesh);
		_meshArray.push_back(aShapeMesh);
	}
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


bool COCCSampleAppDoc::orderWireEdges(TopoDS_Wire _wire, TopoDS_Face _face, TopoDS_Wire _outWire) {
	std::vector<TopoDS_Edge> edgeSequence;
	TopExp_Explorer edgeExpl(_wire, TopAbs_EDGE);
	while (edgeExpl.More() == Standard_True) {
		TopoDS_Shape aShapeEdge = edgeExpl.Current();
		edgeSequence.push_back(TopoDS::Edge(aShapeEdge));
		edgeExpl.Next();
	}

	ShapeBuild_Edge buildEdge;
	BRep_Builder brepBuider;
	
	brepBuider.Add(_outWire, buildEdge.Copy(edgeSequence[0]));


	for (int i = 0; i < (int)edgeSequence.size() - 1; i++) {
		Standard_Real iPFirst, iPLast;
		Handle(Geom2d_Curve) iPCurve = BRep_Tool::CurveOnSurface(edgeSequence[i], _face, iPFirst, iPLast);
		gp_Pnt2d endPt;
		gp_Pnt2d startPt;
		iPCurve->D0(iPLast, endPt);
		iPCurve->D0(iPFirst, startPt);
		int bestEdgeIndex = -1;
		bool reverseEdge = false;
		Standard_Real bestDist = DBL_MAX;
		for (int k = i + 1; k < (int)edgeSequence.size(); k++) {
			Standard_Real kPFirst, kPLast;
			Handle(Geom2d_Curve) kPCurve = BRep_Tool::CurveOnSurface(edgeSequence[k], _face, kPFirst, kPLast);
			gp_Pnt2d k_startPt;
			kPCurve->D0(kPFirst, k_startPt);
			gp_Pnt2d k_endPt;
			kPCurve->D0(kPLast, k_endPt);
			Standard_Real dist_start = endPt.Distance(k_startPt);
			Standard_Real dist_end = endPt.Distance(k_endPt);
			if (dist_start < bestDist) {
				bestEdgeIndex = k;
				bestDist = dist_start;
				reverseEdge = false;
			}
			if (dist_end < bestDist) {
				bestEdgeIndex = k;
				bestDist = dist_end;
				reverseEdge = true;
			}
		}

		TopoDS_Edge newEdge = buildEdge.Copy(edgeSequence[bestEdgeIndex]);
		if (reverseEdge == true) {
			newEdge.Reverse();
		}
		brepBuider.Add(_outWire, newEdge);
	}

	return true;
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


			//std::filebuf aFileBuf;
			//std::ostream aStream(&aFileBuf);
			//if (aFileBuf.open("F:\\Sviluppo\\OCCSampleApp\\trimmedNurbsShape.brep", ios::out)) {
			//	BRepTools::Write(aFace, aStream);
			//}
			//aFileBuf.close();



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
				outputFile << "nurbsSurf" << srfCounter << " = new Geom_BSplineSurface(cpArray" << srfCounter << ", uKnotsArray" << srfCounter << ", vKnotsArray" << srfCounter << ", uMultsArray" << srfCounter << ", vMultsArray" << srfCounter << ", " << uDegree << "," << vDegree << "," << (isUPeriodic == Standard_True ? "Standard_True" : "Standard_False") << "," << (isVPeriodic == Standard_True ? "Standard_True" : "Standard_False") << ");" << std::endl;
			}

			outputFile << std::endl << std::endl << std::endl;

			TopExp_Explorer wireExpl(aShapeFace, TopAbs_WIRE);

			while (wireExpl.More() == Standard_True) {
				TopoDS_Shape aShapeWire = wireExpl.Current();
				TopoDS_Wire aWire = TopoDS::Wire(aShapeWire);

				TopExp_Explorer edgeExpl(aWire, TopAbs_EDGE);

				// Gli edge non sono in generale ordinati correttamente nel Wire, bisogna ordinarli
				//TopoDS_Wire orderedWire;
				//orderWireEdges(aWire, aFace, orderedWire);

				while (edgeExpl.More() == Standard_True) {
					TopoDS_Shape aShapeEdge = edgeExpl.Current();
					TopoDS_Edge aEdge = TopoDS::Edge(aShapeEdge);

					TopAbs_Orientation edgeVersus = aEdge.Orientation();

					Standard_Real aPFirst, aPLast;
					//Handle(Geom_Curve) aCurve3d = BRep_Tool::Curve(aEdge, aFirst, aLast);
					Handle(Geom2d_Curve) aPCurve;
					
					if (edgeVersus == TopAbs_REVERSED) {
						aPCurve = BRep_Tool::CurveOnSurface(aEdge, aFace, aPLast, aPFirst);
					}
					else {
						aPCurve = BRep_Tool::CurveOnSurface(aEdge, aFace, aPFirst, aPLast);
					} 

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


					outputFile << "BRepBuilderAPI_MakeEdge makeEdge" << edgeCounter << "(trimCurve" << edgeCounter << ", nurbsSurf" << srfCounter << ");" << std::endl;
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
				outputFile << "faceMaker" << srfCounter << ".Add(TopoDS::Wire(wire" << ndx << ".Reversed())" << ");" << std::endl;
			}

			outputFile << std::endl << std::endl;

			lastWireIndex = wireCounter;

			srfCounter++;
			shapeExpl.Next();
		}

		outputFile << std::endl << std::endl;
		for (int shapeCounter = 0; shapeCounter < srfCounter; shapeCounter++) {
			outputFile << "ShapeFix_Face fix" << shapeCounter << "(faceMaker" << shapeCounter << ".Face());" << std::endl;
			outputFile << "fix" << shapeCounter << ".Perform();" << std::endl;
			outputFile << "TopoDS_Face Nurbsface" << shapeCounter << " = fix" << shapeCounter << ".Face();" << std::endl;
			outputFile << "trimmedNurbsShapeArray.push_back(new AIS_Shape(Nurbsface" << shapeCounter << "));" << std::endl;

		}
	}

	outputFile.close();

	return true;
}


void COCCSampleAppDoc::OnTestNurbsDump() {
	//const Standard_Real aRadius = 10.0;
	//const Standard_Real aHeight = 25.0;
	//BRepPrimAPI_MakeCylinder aCylinder(aRadius, aHeight);
	//TopoDS_Shape aShape = aCylinder.Shape();

	BRepPrimAPI_MakeCone mkCone(10, 0, 10);
	TopoDS_Shape aShape = mkCone.Shape();

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
	

	dumpNurbs(Nurbsface);
	//ShapeFix_Face faceFix(Nurbsface);
	//faceFix.FixIntersectingWires();
	//faceFix.FixMissingSeam();
	//faceFix.FixOrientation();
	//faceFix.Perform();
	//TopoDS_Face builtFace = faceFix.Face();

	trimmedNurbsShapeArray.push_back(new AIS_Shape(Nurbsface));
	//trimmedNurbsShape = new AIS_Shape(Nurbsface);

	for (int shapeCounter = 0; shapeCounter < (int)trimmedNurbsShapeArray.size(); shapeCounter++) {
		if (trimmedNurbsShapeArray[shapeCounter].IsNull() == false) {

			// Add the trimmed nurbs to the graphic context
			myAISContext->SetDisplayMode(trimmedNurbsShapeArray[shapeCounter], AIS_Shaded);
			myAISContext->Display(trimmedNurbsShapeArray[shapeCounter]);  // Draw the face on the Screen  
			myAISContext->DisplayAll();

			// Update the viewer
			myViewer->InitActiveViews();
			Handle(V3d_View) activeView = myViewer->ActiveView();
			if (activeView.IsNull() == false) {
				activeView->FitAll();           // Focus to the View to the Drawn face.  
			}
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
	//TColgp_Array2OfPnt cpArray0(1, 2, 1, 2);
	//cpArray0.SetValue(1, 1, gp_Pnt(-50, -50, 0));
	//cpArray0.SetValue(1, 2, gp_Pnt(-50, 50, 0));
	//cpArray0.SetValue(2, 1, gp_Pnt(50, -50, 0));
	//cpArray0.SetValue(2, 2, gp_Pnt(50, 50, 0));
	//TColStd_Array1OfReal uKnotsArray0(1, 2);
	//uKnotsArray0.SetValue(1, -50);
	//uKnotsArray0.SetValue(2, 50);
	//TColStd_Array1OfInteger uMultsArray0(1, 2);
	//uMultsArray0.SetValue(1, 2);
	//uMultsArray0.SetValue(2, 2);
	//TColStd_Array1OfReal vKnotsArray0(1, 2);
	//vKnotsArray0.SetValue(1, -50);
	//vKnotsArray0.SetValue(2, 50);
	//TColStd_Array1OfInteger vMultsArray0(1, 2);
	//vMultsArray0.SetValue(1, 2);
	//vMultsArray0.SetValue(2, 2);
	//Handle(Geom_BSplineSurface) nurbsSurf0;
	//nurbsSurf0 = new Geom_BSplineSurface(cpArray0, uKnotsArray0, vKnotsArray0, uMultsArray0, vMultsArray0, 1, 1, Standard_False, Standard_False);



	//TColgp_Array1OfPnt2d cpArrayTrimCurve0(1, 12);
	//TColStd_Array1OfReal wghArrayTrimCurve0(1, 12);
	//cpArrayTrimCurve0.SetValue(1, gp_Pnt2d(50, -19.635));
	//cpArrayTrimCurve0.SetValue(2, gp_Pnt2d(50, 19.635));
	//cpArrayTrimCurve0.SetValue(3, gp_Pnt2d(39.7355, 37.7257));
	//cpArrayTrimCurve0.SetValue(4, gp_Pnt2d(21.803, 50));
	//cpArrayTrimCurve0.SetValue(5, gp_Pnt2d(-21.803, 50));
	//cpArrayTrimCurve0.SetValue(6, gp_Pnt2d(-39.7355, 37.7257));
	//cpArrayTrimCurve0.SetValue(7, gp_Pnt2d(-50, 19.635));
	//cpArrayTrimCurve0.SetValue(8, gp_Pnt2d(-50, -19.635));
	//cpArrayTrimCurve0.SetValue(9, gp_Pnt2d(-39.7355, -37.7257));
	//cpArrayTrimCurve0.SetValue(10, gp_Pnt2d(-21.803, -50));
	//cpArrayTrimCurve0.SetValue(11, gp_Pnt2d(21.803, -50));
	//cpArrayTrimCurve0.SetValue(12, gp_Pnt2d(39.7355, -37.7257));
	//wghArrayTrimCurve0.SetValue(1, 2);
	//wghArrayTrimCurve0.SetValue(2, 2);
	//wghArrayTrimCurve0.SetValue(3, 2.00318);
	//wghArrayTrimCurve0.SetValue(4, 1.93961);
	//wghArrayTrimCurve0.SetValue(5, 1.93961);
	//wghArrayTrimCurve0.SetValue(6, 2.00318);
	//wghArrayTrimCurve0.SetValue(7, 2);
	//wghArrayTrimCurve0.SetValue(8, 2);
	//wghArrayTrimCurve0.SetValue(9, 2.00318);
	//wghArrayTrimCurve0.SetValue(10, 1.93961);
	//wghArrayTrimCurve0.SetValue(11, 1.93961);
	//wghArrayTrimCurve0.SetValue(12, 2.00318);
	//TColStd_Array1OfReal knotsArrayTrimCurve0(1, 5);
	//knotsArrayTrimCurve0.SetValue(1, 0);
	//knotsArrayTrimCurve0.SetValue(2, 1.5708);
	//knotsArrayTrimCurve0.SetValue(3, 3.14159);
	//knotsArrayTrimCurve0.SetValue(4, 4.71239);
	//knotsArrayTrimCurve0.SetValue(5, 6.28319);
	//TColStd_Array1OfInteger multsArrayTrimCurve0(1, 5);
	//multsArrayTrimCurve0.SetValue(1, 3);
	//multsArrayTrimCurve0.SetValue(2, 3);
	//multsArrayTrimCurve0.SetValue(3, 3);
	//multsArrayTrimCurve0.SetValue(4, 3);
	//multsArrayTrimCurve0.SetValue(5, 3);
	//Handle(Geom2d_BSplineCurve) trimCurve0;
	//trimCurve0 = new Geom2d_BSplineCurve(cpArrayTrimCurve0, wghArrayTrimCurve0, knotsArrayTrimCurve0, multsArrayTrimCurve0, 4, Standard_True);
	//BRepBuilderAPI_MakeEdge makeEdge0(trimCurve0, nurbsSurf0);
	//TopoDS_Edge edge0 = makeEdge0.Edge();
	//BRepBuilderAPI_MakeWire wireMaker0;
	//wireMaker0.Add(edge0);
	//TopoDS_Wire wire0 = wireMaker0.Wire();
	//TColgp_Array1OfPnt2d cpArrayTrimCurve1(1, 12);
	//TColStd_Array1OfReal wghArrayTrimCurve1(1, 12);
	//cpArrayTrimCurve1.SetValue(1, gp_Pnt2d(25, -9.81748));
	//cpArrayTrimCurve1.SetValue(2, gp_Pnt2d(25, 9.81748));
	//cpArrayTrimCurve1.SetValue(3, gp_Pnt2d(19.8677, 18.8629));
	//cpArrayTrimCurve1.SetValue(4, gp_Pnt2d(10.9015, 25));
	//cpArrayTrimCurve1.SetValue(5, gp_Pnt2d(-10.9015, 25));
	//cpArrayTrimCurve1.SetValue(6, gp_Pnt2d(-19.8677, 18.8629));
	//cpArrayTrimCurve1.SetValue(7, gp_Pnt2d(-25, 9.81748));
	//cpArrayTrimCurve1.SetValue(8, gp_Pnt2d(-25, -9.81748));
	//cpArrayTrimCurve1.SetValue(9, gp_Pnt2d(-19.8677, -18.8629));
	//cpArrayTrimCurve1.SetValue(10, gp_Pnt2d(-10.9015, -25));
	//cpArrayTrimCurve1.SetValue(11, gp_Pnt2d(10.9015, -25));
	//cpArrayTrimCurve1.SetValue(12, gp_Pnt2d(19.8677, -18.8629));
	//wghArrayTrimCurve1.SetValue(1, 2);
	//wghArrayTrimCurve1.SetValue(2, 2);
	//wghArrayTrimCurve1.SetValue(3, 2.00318);
	//wghArrayTrimCurve1.SetValue(4, 1.93961);
	//wghArrayTrimCurve1.SetValue(5, 1.93961);
	//wghArrayTrimCurve1.SetValue(6, 2.00318);
	//wghArrayTrimCurve1.SetValue(7, 2);
	//wghArrayTrimCurve1.SetValue(8, 2);
	//wghArrayTrimCurve1.SetValue(9, 2.00318);
	//wghArrayTrimCurve1.SetValue(10, 1.93961);
	//wghArrayTrimCurve1.SetValue(11, 1.93961);
	//wghArrayTrimCurve1.SetValue(12, 2.00318);
	//TColStd_Array1OfReal knotsArrayTrimCurve1(1, 5);
	//knotsArrayTrimCurve1.SetValue(1, 0);
	//knotsArrayTrimCurve1.SetValue(2, 1.5708);
	//knotsArrayTrimCurve1.SetValue(3, 3.14159);
	//knotsArrayTrimCurve1.SetValue(4, 4.71239);
	//knotsArrayTrimCurve1.SetValue(5, 6.28319);
	//TColStd_Array1OfInteger multsArrayTrimCurve1(1, 5);
	//multsArrayTrimCurve1.SetValue(1, 3);
	//multsArrayTrimCurve1.SetValue(2, 3);
	//multsArrayTrimCurve1.SetValue(3, 3);
	//multsArrayTrimCurve1.SetValue(4, 3);
	//multsArrayTrimCurve1.SetValue(5, 3);
	//Handle(Geom2d_BSplineCurve) trimCurve1;
	//trimCurve1 = new Geom2d_BSplineCurve(cpArrayTrimCurve1, wghArrayTrimCurve1, knotsArrayTrimCurve1, multsArrayTrimCurve1, 4, Standard_True);
	//BRepBuilderAPI_MakeEdge makeEdge1(trimCurve1, nurbsSurf0);
	//TopoDS_Edge edge1 = makeEdge1.Edge();
	//BRepBuilderAPI_MakeWire wireMaker1;
	//wireMaker1.Add(edge1);
	//TopoDS_Wire wire1 = wireMaker1.Wire();
	//BRepBuilderAPI_MakeFace faceMaker0(nurbsSurf0, wire0, false);
	//faceMaker0.Add(TopoDS::Wire(wire1.Reversed()));

	//Surface

	TColgp_Array2OfPnt cpArray(1, 2, 1, 9);
	TColStd_Array2OfReal wghArray(1, 2, 1, 9);
	cpArray.SetValue(1, 1, gp_Pnt(5, 16, 9));
	wghArray.SetValue(1, 1, 1);
	cpArray.SetValue(1, 2, gp_Pnt(4.24264, 11.3137, 9.89949));
	wghArray.SetValue(1, 2, 0.707107);
	cpArray.SetValue(1, 3, gp_Pnt(1, 16, 15));
	wghArray.SetValue(1, 3, 1);
	cpArray.SetValue(1, 4, gp_Pnt(-2.82843, 11.3137, 11.3137));
	wghArray.SetValue(1, 4, 0.707107);
	cpArray.SetValue(1, 5, gp_Pnt(-5, 16, 11));
	wghArray.SetValue(1, 5, 1);
	cpArray.SetValue(1, 6, gp_Pnt(-4.24264, 11.3137, 4.24264));
	wghArray.SetValue(1, 6, 0.707107);
	cpArray.SetValue(1, 7, gp_Pnt(-1, 16, 5));
	wghArray.SetValue(1, 7, 1);
	cpArray.SetValue(1, 8, gp_Pnt(2.82843, 11.3137, 2.82843));
	wghArray.SetValue(1, 8, 0.707107);
	cpArray.SetValue(1, 9, gp_Pnt(5, 16, 9));
	wghArray.SetValue(1, 9, 1);
	cpArray.SetValue(2, 1, gp_Pnt(5, -15, 9));
	wghArray.SetValue(2, 1, 1);
	cpArray.SetValue(2, 2, gp_Pnt(4.24264, -10.6066, 9.89949));
	wghArray.SetValue(2, 2, 0.707107);
	cpArray.SetValue(2, 3, gp_Pnt(1, -15, 15));
	wghArray.SetValue(2, 3, 1);
	cpArray.SetValue(2, 4, gp_Pnt(-2.82843, -10.6066, 11.3137));
	wghArray.SetValue(2, 4, 0.707107);
	cpArray.SetValue(2, 5, gp_Pnt(-5, -15, 11));
	wghArray.SetValue(2, 5, 1);
	cpArray.SetValue(2, 6, gp_Pnt(-4.24264, -10.6066, 4.24264));
	wghArray.SetValue(2, 6, 0.707107);
	cpArray.SetValue(2, 7, gp_Pnt(-1, -15, 5));
	wghArray.SetValue(2, 7, 1);
	cpArray.SetValue(2, 8, gp_Pnt(2.82843, -10.6066, 2.82843));
	wghArray.SetValue(2, 8, 0.707107);
	cpArray.SetValue(2, 9, gp_Pnt(5, -15, 9));
	wghArray.SetValue(2, 9, 1);
	TColStd_Array1OfReal uKnotsArray(1, 2);
	TColStd_Array1OfInteger uMultsArray(1, 2);
	uKnotsArray.SetValue(1, 0);
	uMultsArray.SetValue(1, 2);
	uKnotsArray.SetValue(2, 31);
	uMultsArray.SetValue(2, 2);
	TColStd_Array1OfReal vKnotsArray(1, 5);
	TColStd_Array1OfInteger vMultsArray(1, 5);
	vKnotsArray.SetValue(1, 0);
	vMultsArray.SetValue(1, 3);
	vKnotsArray.SetValue(2, 8.00952);
	vMultsArray.SetValue(2, 2);
	vKnotsArray.SetValue(3, 16.019);
	vMultsArray.SetValue(3, 2);
	vKnotsArray.SetValue(4, 24.0286);
	vMultsArray.SetValue(4, 2);
	vKnotsArray.SetValue(5, 32.0381);
	vMultsArray.SetValue(5, 3);
	Handle(Geom_BSplineSurface) nurbsSurf;
	nurbsSurf = new Geom_BSplineSurface(cpArray, wghArray, uKnotsArray, vKnotsArray, uMultsArray, vMultsArray, 1, 2, Standard_False, Standard_False);

	////Trim Curve

	//TColgp_Array1OfPnt2d cpArrayTrimCurve0(1, 2);
	//TColStd_Array1OfReal wghArrayTrimCurve0(1, 2);
	//cpArrayTrimCurve0.SetValue(1, gp_Pnt2d(6, 0));
	//wghArrayTrimCurve0.SetValue(1, 1);
	//cpArrayTrimCurve0.SetValue(2, gp_Pnt2d(26, 0));
	//wghArrayTrimCurve0.SetValue(2, 1);
	//TColStd_Array1OfReal knotsArrayTrimCurve0(1, 2);
	//TColStd_Array1OfInteger multsArrayTrimCurve0(1, 2);
	//knotsArrayTrimCurve0.SetValue(1, 0);
	//multsArrayTrimCurve0.SetValue(1, 2);
	//knotsArrayTrimCurve0.SetValue(2, 1);
	//multsArrayTrimCurve0.SetValue(2, 2);
	//Handle(Geom2d_BSplineCurve) trimCurve0;
	//trimCurve0 = new Geom2d_BSplineCurve(cpArrayTrimCurve0, wghArrayTrimCurve0, knotsArrayTrimCurve0, multsArrayTrimCurve0, 1, Standard_False);
	//BRepBuilderAPI_MakeEdge makeEdge0(trimCurve0, nurbsSurf);
	//TopoDS_Edge edge0 = makeEdge0.Edge();

	////Trim Curve

	//TColgp_Array1OfPnt2d cpArrayTrimCurve1(1, 2);
	//TColStd_Array1OfReal wghArrayTrimCurve1(1, 2);
	//cpArrayTrimCurve1.SetValue(1, gp_Pnt2d(26, 0));
	//wghArrayTrimCurve1.SetValue(1, 1);
	//cpArrayTrimCurve1.SetValue(2, gp_Pnt2d(26, 32.0381));
	//wghArrayTrimCurve1.SetValue(2, 1);
	//TColStd_Array1OfReal knotsArrayTrimCurve1(1, 2);
	//TColStd_Array1OfInteger multsArrayTrimCurve1(1, 2);
	//knotsArrayTrimCurve1.SetValue(1, 0);
	//multsArrayTrimCurve1.SetValue(1, 2);
	//knotsArrayTrimCurve1.SetValue(2, 1);
	//multsArrayTrimCurve1.SetValue(2, 2);
	//Handle(Geom2d_BSplineCurve) trimCurve1;
	//trimCurve1 = new Geom2d_BSplineCurve(cpArrayTrimCurve1, wghArrayTrimCurve1, knotsArrayTrimCurve1, multsArrayTrimCurve1, 1, Standard_False);
	//BRepBuilderAPI_MakeEdge makeEdge1(trimCurve1, nurbsSurf);
	//TopoDS_Edge edge1 = makeEdge1.Edge();

	////Trim Curve

	//TColgp_Array1OfPnt2d cpArrayTrimCurve2(1, 2);
	//TColStd_Array1OfReal wghArrayTrimCurve2(1, 2);
	//cpArrayTrimCurve2.SetValue(1, gp_Pnt2d(26, 32.0381));
	//wghArrayTrimCurve2.SetValue(1, 1);
	//cpArrayTrimCurve2.SetValue(2, gp_Pnt2d(6, 32.0381));
	//wghArrayTrimCurve2.SetValue(2, 1);
	//TColStd_Array1OfReal knotsArrayTrimCurve2(1, 2);
	//TColStd_Array1OfInteger multsArrayTrimCurve2(1, 2);
	//knotsArrayTrimCurve2.SetValue(1, 0);
	//multsArrayTrimCurve2.SetValue(1, 2);
	//knotsArrayTrimCurve2.SetValue(2, 1);
	//multsArrayTrimCurve2.SetValue(2, 2);
	//Handle(Geom2d_BSplineCurve) trimCurve2;
	//trimCurve2 = new Geom2d_BSplineCurve(cpArrayTrimCurve2, wghArrayTrimCurve2, knotsArrayTrimCurve2, multsArrayTrimCurve2, 1, Standard_False);
	//BRepBuilderAPI_MakeEdge makeEdge2(trimCurve2, nurbsSurf);
	//TopoDS_Edge edge2 = makeEdge2.Edge();

	////Trim Curve

	//TColgp_Array1OfPnt2d cpArrayTrimCurve3(1, 2);
	//TColStd_Array1OfReal wghArrayTrimCurve3(1, 2);
	//cpArrayTrimCurve3.SetValue(1, gp_Pnt2d(6, 32.0381));
	//wghArrayTrimCurve3.SetValue(1, 1);
	//cpArrayTrimCurve3.SetValue(2, gp_Pnt2d(6, 0));
	//wghArrayTrimCurve3.SetValue(2, 1);
	//TColStd_Array1OfReal knotsArrayTrimCurve3(1, 2);
	//TColStd_Array1OfInteger multsArrayTrimCurve3(1, 2);
	//knotsArrayTrimCurve3.SetValue(1, 0);
	//multsArrayTrimCurve3.SetValue(1, 2);
	//knotsArrayTrimCurve3.SetValue(2, 1);
	//multsArrayTrimCurve3.SetValue(2, 2);
	//Handle(Geom2d_BSplineCurve) trimCurve3;
	//trimCurve3 = new Geom2d_BSplineCurve(cpArrayTrimCurve3, wghArrayTrimCurve3, knotsArrayTrimCurve3, multsArrayTrimCurve3, 1, Standard_False);
	//BRepBuilderAPI_MakeEdge makeEdge3(trimCurve3, nurbsSurf);
	//TopoDS_Edge edge3 = makeEdge3.Edge();
	//BRepBuilderAPI_MakeWire wireMaker0;
	//wireMaker0.Add(edge0);
	//wireMaker0.Add(edge1);
	//wireMaker0.Add(edge2);
	//wireMaker0.Add(edge3);
	//TopoDS_Wire wire0 = wireMaker0.Wire();


	//BRepBuilderAPI_MakeFace faceMaker0(nurbsSurf, wire0, false);
	BRepBuilderAPI_MakeFace faceMaker0(nurbsSurf, Precision::Confusion());



	ShapeFix_Face fix0(faceMaker0.Face());
	fix0.Perform();
	TopoDS_Face Nurbsface0 = fix0.Face();
	trimmedNurbsShapeArray.push_back(new AIS_Shape(Nurbsface0));


	for (int shapeCounter = 0; shapeCounter < (int)trimmedNurbsShapeArray.size(); shapeCounter++) {
		if (trimmedNurbsShapeArray[shapeCounter].IsNull() == false) {

			// Add the trimmed nurbs to the graphic context
			myAISContext->SetDisplayMode(trimmedNurbsShapeArray[shapeCounter], AIS_Shaded);
			myAISContext->Display(trimmedNurbsShapeArray[shapeCounter]);  // Draw the face on the Screen  
			myAISContext->DisplayAll();

			// Update the viewer
			myViewer->InitActiveViews();
			Handle(V3d_View) activeView = myViewer->ActiveView();
			if (activeView.IsNull() == false) {
				activeView->FitAll();           // Focus to the View to the Drawn face.  
			}
		}
	}

	//// Dump in brep format
	//std::filebuf aFileBuf;
	//std::ostream aStream(&aFileBuf);
	//if (aFileBuf.open("F:\\Sviluppo\\OCCSampleApp\\trimmedNurbsShape.brep", ios::out)) {
	//	BRepTools::Write(Nurbsface0, aStream);
	//}
	//aFileBuf.close();

	//// Dump in stl format
	Handle(StlMesh_Mesh) aShapeMesh;
	generateMesh(Nurbsface0, 0.01, 0.5, aShapeMesh);
	saveMesh(aShapeMesh, L"C:\\temp\\nurbsTrimMesh.stl");


}


void COCCSampleAppDoc::OnTestAddCone()
{
	// TODO: Add your command handler code here
	AddCone();
}
