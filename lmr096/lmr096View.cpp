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
	
	if(NULL==lpBitsInfo)//看看图片是否加载进来
		return;
	LPVOID lpBits = 
		(LPVOID)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];//很简单地获得下面函数参数的一个指针

	//调用显示函数
	StretchDIBits( 
		pDC->GetSafeHdc(),
		0,0,lpBitsInfo->bmiHeader.biWidth,lpBitsInfo->bmiHeader.biHeight,
		0,0,lpBitsInfo->bmiHeader.biWidth,lpBitsInfo->bmiHeader.biHeight,
		lpBits, // bitmap bits 
		lpBitsInfo, // bitmap data 
		DIB_RGB_COLORS,//色彩模型
		SRCCOPY); //显示模式：覆盖显示
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
void gray(); //声明一下bmp中写的函数
void CLmr096View::OnGray() 
{
	// TODO: Add your command handler code here
	gray();  //把24位真彩变成灰度图像
	Invalidate();//刷新屏幕，重新实现Ondraw函数
}
bool if_gray();
void CLmr096View::OnUpdateGray(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL&&(!if_gray()));//图像加载了&图像不是256灰度

	
}
void pixel(int i,int j,char* rgb);
void CLmr096View::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//记录坐标值
	char xy[100];
	memset(xy,0,100);
	sprintf(xy,"x:%d y:%d  ",point.x,point.y);

	//记录颜色值
	char rgb[100];
	memset(rgb,0,100);
	pixel(point.y,point.x,rgb);

	//坐标与颜色合并
	strcat(xy,rgb);

	//在状态栏显示
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
