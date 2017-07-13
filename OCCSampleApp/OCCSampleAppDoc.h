
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


public:
	afx_msg void OnTestAddsphere();
	afx_msg void OnTestMeshing();

private:
	bool generateMesh(const TopoDS_Shape &_shape, Standard_Real _linearDeflection, Standard_Real _angularDeflection, Handle(StlMesh_Mesh) &_mesh);
	bool saveMesh(const Handle(StlMesh_Mesh) &_mesh, std::wstring _fileName);
public:
	afx_msg void OnTestAddNurbsSurface();
};
