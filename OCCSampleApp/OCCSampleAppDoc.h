
// OCCSampleAppDoc.h : interface of the COCCSampleAppDoc class
//


#pragma once


#include <StlMesh_Mesh.hxx>


class COCCSampleAppDoc : public CDocument
{
protected: // create from serialization only
	COCCSampleAppDoc();
	DECLARE_DYNCREATE(COCCSampleAppDoc)

// Attributes
public:

// Operations
public:

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
	virtual ~COCCSampleAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	Handle(V3d_Viewer) GetViewer() { return myViewer; }
	void AddSphere(double Radius);
	void AddCone();

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS


private:
	Handle(AIS_InteractiveContext) myAISContext;		///< Graphic context
	Handle(V3d_Viewer) myViewer;						///< 3d viewer instance
	Handle(AIS_Shape) sphereShape;						///< Sample object (a sphere)
	Handle(AIS_Shape) nurbsShape;
	Handle(AIS_Shape) boxWithHoleShape;
	Handle(AIS_Shape) circleWithHoleShape;

	std::vector<Handle(AIS_Shape)> trimmedNurbsShapeArray;
	//Handle(AIS_Shape) trimmedNurbsShape;
	//Handle(AIS_Shape) trimmedNurbsShape0;
	//Handle(AIS_Shape) trimmedNurbsShape1;
	//Handle(AIS_Shape) trimmedNurbsShape2;

	Handle(AIS_Shape) coneShape;


public:
	afx_msg void OnTestAddsphere();
	afx_msg void OnTestMeshing();

private:
	bool generateMesh(const TopoDS_Shape &_shape, Standard_Real _linearDeflection, Standard_Real _angularDeflection, Handle(StlMesh_Mesh) &_mesh);
	bool generateMesh(std::vector<TopoDS_Shape> _shapeArray, Standard_Real _linearDeflection, Standard_Real _angularDeflection, std::vector<Handle(StlMesh_Mesh)> _meshArray);
	bool saveMesh(const Handle(StlMesh_Mesh) &_mesh, std::wstring _fileName);
	bool dumpNurbs(TopoDS_Shape _shape);
	bool orderWireEdges(TopoDS_Wire _wire, TopoDS_Face _face, TopoDS_Wire _outWire);
public:
	afx_msg void OnTestAddNurbsSurface();
	afx_msg void OnTestAddRationalNurbsSurface();
	afx_msg void OnTestNurbsDump();
	afx_msg void OnTestAddBoxWithHole();
	afx_msg void OnTestAddCircleWithHole();
	afx_msg void OnTestAddTrimmedNurbsSurface();
	afx_msg void OnTestAddDumpedNurbsSurface();
	afx_msg void OnTestAddCone();
};
