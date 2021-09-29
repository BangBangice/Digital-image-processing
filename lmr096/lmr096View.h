// lmr096View.h : interface of the CLmr096View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LMR096VIEW_H__32683841_446C_4CDE_AED7_8DDBCEB23922__INCLUDED_)
#define AFX_LMR096VIEW_H__32683841_446C_4CDE_AED7_8DDBCEB23922__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLmr096View : public CScrollView
{
protected: // create from serialization only
	CLmr096View();
	DECLARE_DYNCREATE(CLmr096View)

// Attributes
public:
	CLmr096Doc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLmr096View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLmr096View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLmr096View)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in lmr096View.cpp
inline CLmr096Doc* CLmr096View::GetDocument()
   { return (CLmr096Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMR096VIEW_H__32683841_446C_4CDE_AED7_8DDBCEB23922__INCLUDED_)
