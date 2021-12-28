// lmr096View.cpp : implementation of the CLmr096View class
//

#include "stdafx.h"
#include "lmr096.h"

#include "lmr096Doc.h"
#include "lmr096View.h"
#include "DlgHistogram.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void Linear();
/////////////////////////////////////////////////////////////////////////////
// CLmr096View

IMPLEMENT_DYNCREATE(CLmr096View, CScrollView)

BEGIN_MESSAGE_MAP(CLmr096View, CScrollView)
	//{{AFX_MSG_MAP(CLmr096View)
	ON_COMMAND(ID_GRAY, OnGray)
	ON_UPDATE_COMMAND_UI(ID_GRAY, OnUpdateGray)
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_RESET, OnReset)
	ON_COMMAND(ID_HISTOGRAM, OnHistogram)
	ON_UPDATE_COMMAND_UI(ID_HISTOGRAM, OnUpdateHistogram)
	ON_COMMAND(ID_LINEAR, OnLinear)
	ON_COMMAND(ID_FOURIER, OnFourier)
	ON_UPDATE_COMMAND_UI(ID_FOURIER, OnUpdateFourier)
	ON_COMMAND(ID_IFOURIER, OnIfourier)
	ON_UPDATE_COMMAND_UI(ID_IFOURIER, OnUpdateIfourier)
	ON_COMMAND(ID_AVG, OnAvg)
	ON_UPDATE_COMMAND_UI(ID_AVG, OnUpdateAvg)
	ON_COMMAND(ID_RAPLAS, OnRaplas)
	ON_UPDATE_COMMAND_UI(ID_RAPLAS, OnUpdateRaplas)
	ON_COMMAND(ID_MID, OnMid)
	ON_UPDATE_COMMAND_UI(ID_MID, OnUpdateMid)
	ON_COMMAND(ID_GRAD, OnGrad)
	ON_UPDATE_COMMAND_UI(ID_GRAD, OnUpdateGrad)
	ON_COMMAND(ID_FFT, OnFft)
	ON_UPDATE_COMMAND_UI(ID_FFT, OnUpdateFft)
	ON_COMMAND(ID_IFFT, OnIfft)
	ON_UPDATE_COMMAND_UI(ID_IFFT, OnUpdateIfft)
	ON_COMMAND(ID_FFT_FILTER, OnFftFilter)
	ON_UPDATE_COMMAND_UI(ID_FFT_FILTER, OnUpdateFftFilter)
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
extern BITMAPINFO* lpDIB_FT; 
extern BITMAPINFO* lpDIB_IFT; 
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
	

	if(lpDIB_FT)   //����Ҷ�任ͼ������
	{
		LPVOID lpBits = 
		(LPVOID)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];//�ܼ򵥵ػ�����溯��������һ��ָ��

	//������ʾ����
	StretchDIBits( 
		pDC->GetSafeHdc(),
		600,0,lpDIB_FT->bmiHeader.biWidth,lpDIB_FT->bmiHeader.biHeight,
		0,0,lpDIB_FT->bmiHeader.biWidth,lpDIB_FT->bmiHeader.biHeight,
		lpBits, // bitmap bits 
		lpDIB_FT, // bitmap data 
		DIB_RGB_COLORS,//ɫ��ģ��
		SRCCOPY); //��ʾģʽ��������ʾ
	}

	else if(lpDIB_IFT)   //����Ҷ�任ͼ������
	{
		LPVOID lpBits = 
		(LPVOID)&lpDIB_IFT->bmiColors[lpDIB_IFT->bmiHeader.biClrUsed];//�ܼ򵥵ػ�����溯��������һ��ָ��

	//������ʾ����
	StretchDIBits( 
		pDC->GetSafeHdc(),
		600,0,lpDIB_IFT->bmiHeader.biWidth,lpDIB_IFT->bmiHeader.biHeight,
		0,0,lpDIB_IFT->bmiHeader.biWidth,lpDIB_IFT->bmiHeader.biHeight,
		lpBits, // bitmap bits 
		lpDIB_IFT, // bitmap data 
		DIB_RGB_COLORS,//ɫ��ģ��
		SRCCOPY); //��ʾģʽ��������ʾ
	}

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

void reset();
void CLmr096View::OnReset() 
{
	// TODO: Add your command handler code here
	reset();  //��24λ��ʱ�ɻҶ�ͼ��
	Invalidate();//ˢ����Ļ������ʵ��Ondraw����
	
}

void CLmr096View::OnHistogram() 
{
	// TODO: Add your command handler code here
	CDlgHistogram dlg;
	dlg.DoModal();
	
}

void CLmr096View::OnUpdateHistogram(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL&&lpBitsInfo->bmiHeader.biBitCount!=1);//ͼ������ˣ����Ҳ���2ֵͼ��
	
}

void CLmr096View::OnLinear() 
{
	// TODO: Add your command handler code here
	Linear();
	Invalidate();//ˢ����Ļ������ʵ��Ondraw����
}
void Fourier();
void CLmr096View::OnFourier() 
{
	printf("!!");
	// TODO: Add your command handler code here
	Fourier();
	Invalidate();//ˢ����Ļ������ʵ��Ondraw����
	
}

void CLmr096View::OnUpdateFourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL&&(if_gray()));//ͼ�������&ͼ����256�Ҷ�
	
}
void IFourier();
void CLmr096View::OnIfourier() 
{
	// TODO: Add your command handler code here
	IFourier();
	Invalidate();//ˢ����Ļ������ʵ��Ondraw����
	
}
BOOL is_gFD_OK();
void CLmr096View::OnUpdateIfourier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL&&is_gFD_OK());//��Ƶ�����ݲ��������任
	
}
void AverageFilter();
void CLmr096View::OnAvg() 
{
	// TODO: Add your command handler code here
	AverageFilter();
	Invalidate();//ˢ����Ļ������ʵ��Ondraw����
	
}

void CLmr096View::OnUpdateAvg(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL&&(if_gray()));//ͼ�������&ͼ����256�Ҷ�
}
void RaplasSharp();
void CLmr096View::OnRaplas() 
{
	// TODO: Add your command handler code here
	RaplasSharp();
	Invalidate();//ˢ����Ļ������ʵ��Ondraw����

}

void CLmr096View::OnUpdateRaplas(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL&&(if_gray()));//ͼ�������&ͼ����256�Ҷ�
}
void MedianFilter();
void CLmr096View::OnMid() 
{
	// TODO: Add your command handler code here
	MedianFilter();
	Invalidate();//ˢ����Ļ������ʵ��Ondraw����

}

void CLmr096View::OnUpdateMid(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL&&(if_gray()));//ͼ�������&ͼ����256�Ҷ�

	
}
void GradSharp();
void CLmr096View::OnGrad() 
{
	// TODO: Add your command handler code here
	GradSharp();
	Invalidate();//ˢ����Ļ������ʵ��Ondraw����
}

void CLmr096View::OnUpdateGrad(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL&&(if_gray()));//ͼ�������&ͼ����256�Ҷ�

}
void FFourier();
void CLmr096View::OnFft()   //���ٸ���Ҷ�任
{
	if (lpDIB_FT)
	{
		free(lpDIB_FT);
		lpDIB_FT = NULL;
	}

	if (lpDIB_IFT)
	{
		free(lpDIB_IFT);
		lpDIB_IFT = NULL;
	} 

	FFourier();
	Invalidate();
}
void IFFourier();
void CLmr096View::OnUpdateFft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL&&(if_gray()));//ͼ�������&ͼ����256�Ҷ�

}

void CLmr096View::OnIfft() 
{
	// TODO: Add your command handler code here
	if (lpDIB_IFT)
	{
		free(lpDIB_IFT);
	lpDIB_IFT = NULL;
	}

	IFFourier();
	Invalidate();
}

void CLmr096View::OnUpdateIfft(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL&&is_gFD_OK());//��Ƶ�����ݲ��������任

}
void FFT_Filter(int D);
void CLmr096View::OnFftFilter() 
{
	// TODO: Add your command handler code here
	FFT_Filter(5);  //5��ͨ��-5��ͨ
	Invalidate();

}

void CLmr096View::OnUpdateFftFilter(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(lpBitsInfo!=NULL&&is_gFD_OK());//��Ƶ�����ݲ������˲�

	
}
