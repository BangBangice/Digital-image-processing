// lmr096Doc.h : interface of the CLmr096Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LMR096DOC_H__62505F83_FB0D_4D36_8071_15E4770EE143__INCLUDED_)
#define AFX_LMR096DOC_H__62505F83_FB0D_4D36_8071_15E4770EE143__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CLmr096Doc : public CDocument
{
protected: // create from serialization only
	CLmr096Doc();
	DECLARE_DYNCREATE(CLmr096Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLmr096Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLmr096Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLmr096Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LMR096DOC_H__62505F83_FB0D_4D36_8071_15E4770EE143__INCLUDED_)
