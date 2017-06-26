
// OCCSampleAppView.h : interface of the COCCSampleAppView class
//

#pragma once


class COCCSampleAppView : public CView
{
protected: // create from serialization only
	COCCSampleAppView();
	DECLARE_DYNCREATE(COCCSampleAppView)

// Attributes
public:
	COCCSampleAppDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();

protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~COCCSampleAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);

private:
	Handle(V3d_View) myView;		///< Graphic view instance
};

#ifndef _DEBUG  // debug version in OCCSampleAppView.cpp
inline COCCSampleAppDoc* COCCSampleAppView::GetDocument() const
   { return reinterpret_cast<COCCSampleAppDoc*>(m_pDocument); }
#endif

