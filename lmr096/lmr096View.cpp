// lmr096View.cpp : implementation of the CLmr096View class
//

#include "stdafx.h"
#include "lmr096.h"

#include "lmr096Doc.h"
#include "lmr096View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLmr096View

IMPLEMENT_DYNCREATE(CLmr096View, CScrollView)

BEGIN_MESSAGE_MAP(CLmr096View, CScrollView)
	//{{AFX_MSG_MAP(CLmr096View)
	ON_COMMAND(ID_GRAY, OnGray)
	ON_UPDATE_COMMAND_UI(ID_GRAY, OnUpdateGray)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_HISTOGRA, OnHistogra)
	ON_UPDATE_COMMAND_UI(ID_HISTOGRA, OnUpdateHistogra)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLmr096View construction/destruction

CLmr096View::CLmr096View()
{
	// TODO: add construction code here

}

CLmr096View::~CLmr096View()
{
}

BOOL CLmr096View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLmr096View drawing
extern BITMAPINFO* lpBitsInfo;
void CLmr096View::OnDraw(CDC* pDC)
{
	CLmr096Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	
	if(NULL==lpBitsInfo)//����ͼƬ�Ƿ���ؽ���
		return;
	LPVOID lpBits = 
		(LPVOID)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//�ܼ򵥵ػ�����溯��������һ��ָ��

	//������ʾ����
	StretchDIBits( 
		pDC->GetSafeHdc(),
		0,0,lpBitsInfo->bmiHeader.biWidth,lpBitsInfo->bmiHeader.biHeight,
		0,0,lpBitsInfo->bmiHeader.biWidth,lpBitsInfo->bmiHeader.biHeight,
		lpBits, // bitmap bits 
		lpBitsInfo, // bitmap data 
		DIB_RGB_COLORS,//ɫ��ģ��
		SRCCOPY); //��ʾģʽ��������ʾ
}

void CLmr096View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CLmr096View printing

BOOL CLmr096View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CLmr096View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CLmr096View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CLmr096View diagnostics

#ifdef _DEBUG
void CLmr096View::AssertValid() const
{
	CScrollView::AssertValid();
}

void CLmr096View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CLmr096Doc* CLmr096View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLmr096Doc)));
	return (CLmr096Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLmr096View message handlers
void gray(); //����һ��bmp��д�ĺ���
void CLmr096View::OnGray() 
{
	// TODO: Add your command handler code here
	gray();  //��24λ��ʱ�ɻҶ�ͼ��
	Invalidate();//ˢ����Ļ������ʵ��Ondraw����
}
bool if_gray();
void CLmr096View::OnUpdateGray(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL&&(!if_gray()));//ͼ�������&ͼ����256�Ҷ�

	
}
void pixel(int i,int j,char* rgb);
void CLmr096View::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//��¼����ֵ
	char xy[100];
	memset(xy,0,100);
	sprintf(xy,"x:%d y:%d  ",point.x,point.y);

	//��¼��ɫֵ
	char rgb[100];
	memset(rgb,0,100);
	pixel(point.y,point.x,rgb);

	//��������ɫ�ϲ�
	strcat(xy,rgb);

	//��״̬����ʾ
	((CFrameWnd*)GetParent())->SetMessageText(xy);

	CScrollView::OnMouseMove(nFlags, point);
}

void CLmr096View::OnHistogra() 
{
	// TODO: Add your command handler code here
	
}

void CLmr096View::OnUpdateHistogra(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}
