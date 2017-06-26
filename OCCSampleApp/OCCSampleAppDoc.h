
// OCCSampleAppDoc.h : interface of the COCCSampleAppDoc class
//


#pragma once


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


public:
	afx_msg void OnTestAddsphere();
};
