// lmr096Doc.cpp : implementation of the CLmr096Doc class
//

#include "stdafx.h"
#include "lmr096.h"

#include "lmr096Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLmr096Doc

IMPLEMENT_DYNCREATE(CLmr096Doc, CDocument)

BEGIN_MESSAGE_MAP(CLmr096Doc, CDocument)
	//{{AFX_MSG_MAP(CLmr096Doc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLmr096Doc construction/destruction

CLmr096Doc::CLmr096Doc()
{
	// TODO: add one-time construction code here

}

CLmr096Doc::~CLmr096Doc()
{
}

BOOL CLmr096Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CLmr096Doc serialization

void CLmr096Doc::Serialize(CArchive& ar)
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

/////////////////////////////////////////////////////////////////////////////
// CLmr096Doc diagnostics

#ifdef _DEBUG
void CLmr096Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLmr096Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLmr096Doc commands
BOOL LoadBmpFile(char* BmpFileName);//先声明函数
BOOL CLmr096Doc::OnOpenDocument(LPCTSTR lpszPathName)  //知道打开对话框的响应函数
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	LoadBmpFile((char*)lpszPathName);  //通过响应函数（响应了打开的文件），将打开文件的信息传入打开图片的函数
	return TRUE;
}
