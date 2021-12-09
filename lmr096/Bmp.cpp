#include "stdafx.h"
#include <complex> 
#include<math.h>
const double PI = 3.1415926535897932;
BITMAPINFO* lpBitsInfo=NULL;  //��ָ�����Ҫ��ֱ�Ӷ����ȫ��   (BITMAPINFO*==LPBITMAPINFO) NULL�����ΪҰָ��
							//��ָ��ָ���ȡ���ڴ��е����е�����λͼ��Ϣ���ļ�ͷ֮������ݣ�
BITMAPINFO* lpBitsInfo_save=NULL;  //Ϊreset������

BITMAPINFO* lpDIB_FT=NULL;  //Ƶ�׷���ͼ��
BITMAPINFO* lpDIB_IFT=NULL;
DWORD size_save=0;
using namespace std;

BOOL LoadBmpFile(char* BmpFileName)//������·�������ļ���  �������ܣ����������ļ����ڴ�
{
	FILE* fp;
	if(NULL==(fp=fopen(BmpFileName,"rb")))//ֻ���򿪣������ļ���ʧ�ܵ����
		return FALSE;
	BITMAPFILEHEADER bf;//�ļ�ͷ14byte
	BITMAPINFOHEADER bi;//��Ϣͷ 40byte

	fread(&bf,14,1,fp);
	fread(&bi,40,1,fp);  //�ļ�ָ�����������ߣ�ָ���ɫ��

	DWORD NumColors; //��ɫ��ĳ���
	if(bi.biClrUsed!=0) //��ɫ��ĳ��Ⱦ�����
		NumColors=bi.biClrUsed;
	else{  //����λͼɫ���������ɫ���λ�������ȣ�
		switch(bi.biBitCount){
		case 1:
			NumColors=2;
			break;
		case 4:
			NumColors=16;
			break;
		case 8:
			NumColors=256;
			break;
		case 24:
			NumColors=0;//24 λ���ɫû�е�ɫ�壬����Ϊ0
			break;
		}
	}
	DWORD PalSize=NumColors*4; //��ɫ����ڴ�ռ�ô�С
	//ʵ�ʵ�λͼ����
	DWORD ImgSize=(bi.biWidth*bi.biBitCount+31)/32*4*bi.biHeight; //΢��涨�������ĵ�������
	
	DWORD size=40+PalSize+ImgSize;//�ܵĴ�С

	if(NULL==(lpBitsInfo=(BITMAPINFO*)malloc(size)))//�����ڴ�(�����):��Ϣͷ����ɫ�塢ʵ�ʵ�λͼ����
		return FALSE;
	if(NULL==(lpBitsInfo_save=(BITMAPINFO*)malloc(size)))//�����ڴ�(�����):��Ϣͷ����ɫ�塢ʵ�ʵ�λͼ����
		return FALSE;
	
	fseek(fp,14,SEEK_SET);//���ļ�ָ����ǰ�ƶ�
	fread((char*)lpBitsInfo,size,1,fp);//���ļ��е����ݶ����ڴ�  �ļ�ͷ�Ժ��������Ϣ

	lpBitsInfo->bmiHeader.biClrUsed=NumColors;//���ڴ��е����ݻ����Ը�����ģ������ģ�

	memcpy(lpBitsInfo_save,lpBitsInfo,size);
	size_save=size;
	


	return true;
}
bool if_gray();
void gray()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//ÿ���ֽ���
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //ָ��λͼ���ݵ�ָ��
	//�ľ�����·��ÿ�ζ�Ҫ��
	//ÿ�����ص㣬�ڴ���˳����BGR

	BITMAPINFO* lpBitsInfo_2=NULL;  //����һ��λͼ��Ϣָ�룬�����ݴ��µ�λͼ��Ϣ

	DWORD ImgSize=(w*8+31)/32*4*h; //bitcount�ı��ˣ�Ϊ8
	DWORD size=40+256*4+ImgSize;  //�Ҷ�ͼ��ĵ�ɫ���С�̶�Ϊ256����ɫ��һ����λ4byte
								//��Ϣͷ+��ɫ��+λͼ����

	lpBitsInfo_2=(BITMAPINFO*)malloc(size); //Ϊ�µĻҶ�ͼ����Ϣָ�������ڴ�

	memcpy(lpBitsInfo_2,lpBitsInfo,40);//��Ϣͷ���䣬ֱ�ӿ�������

	lpBitsInfo_2->bmiHeader.biClrUsed=256;
	lpBitsInfo_2->bmiHeader.biBitCount=8; //�޸���Ϣͷ�Ļ�����Ϣ:8λ 256ɫ

	BYTE* lpBits_2=(BYTE*)&lpBitsInfo_2->bmiColors[lpBitsInfo_2->bmiHeader.biClrUsed];

	int LineBytes_2 = (w * lpBitsInfo_2->bmiHeader.biBitCount + 31)/32 * 4;//ÿ���ֽ���

	int  i,j;
	for(i=0;i<256;i++)
	{
		lpBitsInfo_2->bmiColors[i].rgbBlue=i;
		lpBitsInfo_2->bmiColors[i].rgbGreen=i;
		lpBitsInfo_2->bmiColors[i].rgbRed=i;
	}
	//����һ��256�Ҷ�ͼ��ĵ�ɫ��
	
	BYTE *R,*G,*B,avg; 
	BYTE *B_2=NULL;  //�Ҷ�ͼ���bitcount��ԭ���Ĳ�ͬ
	BYTE *pixel,bv;
	switch(lpBitsInfo->bmiHeader.biBitCount)  //�ж���Ҫ�ı��ͼ�������
	{
		case 24:   //24λ���ɫ
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{ //(i,j)ͨ������������ָ��
					B=lpBits+i*LineBytes+j*3;
					G=B+1;
					R=G+1;
					avg=(*R+*G+*B)/3;
					B_2=lpBits_2+i*LineBytes_2+j;
					*B_2=avg;			
				}
			}
			break;
		case 8: //256��ɫ
			if(!if_gray())  //���ǻҶ�ͼ����ת��
			{
				for(i=0;i<h;i++)
				{
					for(j=0;j<w;j++)
					{ //(i,j)ͨ������������ָ��
						pixel=lpBits+i*LineBytes+j;
						avg=(lpBitsInfo->bmiColors[*pixel].rgbRed+lpBitsInfo->bmiColors[*pixel].rgbGreen+lpBitsInfo->bmiColors[*pixel].rgbBlue)/3;
						B_2=lpBits_2+i*LineBytes_2+j;
						*B_2=avg;			
					}
				}
			}
			else
				return;
			break;
		case 4:   //16λɫ
			for(i=0;i<h;i++)
				{
					for(j=0;j<w;j++)
					{	//(i,j)ͨ������������ָ��
						pixel=lpBits+LineBytes*i+j/2;
						B_2=lpBits_2+i*LineBytes_2+j/2;
						if(!(j%2)) //����λ(ǰ)��>>4
						{
							bv=*pixel>>4;
							avg=(lpBitsInfo->bmiColors[bv].rgbRed+lpBitsInfo->bmiColors[bv].rgbGreen+lpBitsInfo->bmiColors[bv].rgbBlue)/3;							
						}
						else //����λ(��)��&00001111
						{
							bv=*pixel%16;
							avg=(lpBitsInfo->bmiColors[bv].rgbRed+lpBitsInfo->bmiColors[bv].rgbGreen+lpBitsInfo->bmiColors[bv].rgbBlue)/3;
						}
						B_2=lpBits_2+i*LineBytes_2+j;
						*B_2=avg;
					}
				}
			break;
		case 1:
			for(i=0;i<h;i++)
			{
				for(j=0;j<w;j++)
				{ 
					pixel=lpBits+LineBytes*i+j/8;  //�ҵ����������ڵ��ֽ�
					bv=(*pixel)&(1<<(7-j%8));  //��������λ:7-j%8
					bv=bv>>(7-j%8);
					if(bv>0) //ǰ����
						avg=(lpBitsInfo->bmiColors[bv].rgbRed+lpBitsInfo->bmiColors[bv].rgbGreen+lpBitsInfo->bmiColors[bv].rgbBlue)/3;						
					else  //������
						avg=(lpBitsInfo->bmiColors[bv].rgbRed+lpBitsInfo->bmiColors[bv].rgbGreen+lpBitsInfo->bmiColors[bv].rgbBlue)/3;					
					B_2=lpBits_2+i*LineBytes_2+j;
					*B_2=avg;
				}
			}
			break;
	}
	//���ɻҶ�ͼ�������


	free(lpBitsInfo);  //�ͷžɵ�ָ��ָ����ڴ�
	lpBitsInfo=lpBitsInfo_2;
}
bool if_gray()
{
	bool flag=1;//Ĭ���ǻҶ�
	//if(lpBitsInfo->bmiHeader.biBitCount==8)  //�ȵ���8λͼ
		for(int i=0;i<256;i++)    //���ݵ�ɫ���жϲ���256��ɫ
			if(lpBitsInfo->bmiColors[i].rgbBlue!=i){
				flag=0;
				break;
			}
	//else flag=0;
	return flag;
}  // 1 gray; 0 non gray
void pixel(int i,int j,char* rgb)
{
	if(NULL==lpBitsInfo)
		return;  //û��ͼ��
	int w=lpBitsInfo->bmiHeader.biWidth;
	int h=lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//ÿ���ֽ���
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //ָ��λͼ���ݵ�ָ��
	
	if(i>=h||j>=w)
		return;  //��곬��ͼ��Χ

	BYTE *pixel,bv;
	int r,g,b;

	switch(lpBitsInfo->bmiHeader.biBitCount)
	{
		case 24:
			pixel=lpBits+LineBytes*(h-i-1)+j*3;
			b=*pixel;
			g=*(pixel+1);
			r=*(pixel+2);
			sprintf(rgb,"R:%d G:%d B:%d",r,g,b);
			break;
		case 8: //�ֲ�ɫ��ڰף���Ҫ��ɫ��ת����ɫ
			pixel=lpBits+LineBytes*(h-i-1)+j;
			if(if_gray())
			{
				r=lpBitsInfo->bmiColors[*pixel].rgbBlue;
				sprintf(rgb,"�Ҷ�ֵ:%d",r);
			}
			else
			{
				r=lpBitsInfo->bmiColors[*pixel].rgbRed;
				g=lpBitsInfo->bmiColors[*pixel].rgbGreen;
				b=lpBitsInfo->bmiColors[*pixel].rgbBlue;
				sprintf(rgb,"R:%d G:%d B:%d",r,g,b);
			}
			break;
		case 4:
			pixel=lpBits+LineBytes*(h-i-1)+j/2;  //�ҵ����������ڵ��ֽ�
			if(j%2) //����λ(ǰ)��>>4
			{
				bv=*pixel>>4;
				r=lpBitsInfo->bmiColors[bv].rgbRed;
				g=lpBitsInfo->bmiColors[bv].rgbGreen;
				b=lpBitsInfo->bmiColors[bv].rgbBlue;
			}
			else //����λ(��)��&00001111
			{
				bv=*pixel%16;
				r=lpBitsInfo->bmiColors[bv].rgbRed;
				g=lpBitsInfo->bmiColors[bv].rgbGreen;
				b=lpBitsInfo->bmiColors[bv].rgbBlue;
			}
			sprintf(rgb,"R:%d G:%d B:%d",r,g,b);
			break;
		case 1:
			pixel=lpBits+LineBytes*(h-i-1)+j/8;  //�ҵ����������ڵ��ֽ�
			bv=(*pixel)&(1<<(7-j%8));  //��������λ:7-j%8
			bv=bv>>(7-j%8);
			if(bv>0) //ǰ����
				sprintf(rgb,"�Ҷ�ֵ:%d",lpBitsInfo->bmiColors[bv].rgbRed);
			else  //������
				sprintf(rgb,"�Ҷ�ֵ:%d",lpBitsInfo->bmiColors[bv].rgbRed);
			break;
	}
}
void reset()
{	
	free(lpBitsInfo);
	lpBitsInfo=(BITMAPINFO*)malloc(size_save);//�����ڴ�(�����):��Ϣͷ����ɫ�塢ʵ�ʵ�λͼ����
	memcpy(lpBitsInfo,lpBitsInfo_save,size_save);
	
}
DWORD H[256];
DWORD H_R[256];
DWORD H_G[256];
DWORD H_B[256];
void Histogram()
{
//	if(!if_gray())
//		return ;
	int w= lpBitsInfo->bmiHeader.biWidth;
	int h= lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//ÿ���ֽ���
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //ָ��λͼ���ݵ�ָ��
	 
	int i,j;
	BYTE* pixel;
	int avg,r,g,b,color_index;
	
	for(i=0;i<256;i++)
	{
		H[i]=0;  //��ֱ��ͼ������г�ʼ��;
		H_R[i]=0;
		H_G[i]=0;
		H_B[i]=0;
	}

	switch(lpBitsInfo->bmiHeader.biBitCount)
	{
		case 24:  //��24���ɫ���ֱ�������ͨ����ǿ�ȣ�����ƽ��ͨ����ǿ��
			for(i=0;i<h;i++)
				{
					for(j=0;j<w;j++)
					{
						pixel=lpBits+LineBytes*(h-i-1)+j*3;
						H_B[*pixel]++;
						H_G[*(pixel+1)]++;
						H_R[*(pixel+2)]++;
						avg=(*pixel+*(pixel+1)+*(pixel+2))/3;
						H[avg]++;
					}
				}
			break;
		case 8: //�ֲ�ɫ��ڰף���Ҫ��ɫ��ת����ɫ
			if(if_gray())   //256�Ҷ�
			{
				for(i=0;i<h;i++)
				{
					for(j=0;j<w;j++)
					{
						pixel=lpBits+LineBytes*(h-1-i)+j;
						H[*pixel]++;
					}
				}
			}
			else   //256��ɫ
			{
				for(i=0;i<h;i++)
				{
					for(j=0;j<w;j++)
					{
						pixel=lpBits+LineBytes*(h-1-i)+j;
						r=lpBitsInfo->bmiColors[*pixel].rgbRed;
						g=lpBitsInfo->bmiColors[*pixel].rgbGreen;
						b=lpBitsInfo->bmiColors[*pixel].rgbBlue;
						H_B[b]++;
						H_G[g]++;
						H_R[r]++;
						avg=(r+g+b)/3;
						H[avg]++;
					}
				}
			}
			break;
		case 4:
			for(i=0;i<h;i++)
				{
					for(j=0;j<w;j++)
					{
						pixel=lpBits+LineBytes*(h-i-1)+j/2;  //�ҵ����������ڵ��ֽ�
						if(j%2) //����λ(ǰ)��>>4
						{
							color_index=*pixel>>4;
							r=lpBitsInfo->bmiColors[color_index].rgbRed;
							g=lpBitsInfo->bmiColors[color_index].rgbGreen;
							b=lpBitsInfo->bmiColors[color_index].rgbBlue;
							H_B[b]++;
							H_G[g]++;
							H_R[r]++;
							avg=(r+g+b)/3;
							H[avg]++;
						}
						else //����λ(��)��&00001111
						{
							color_index=*pixel%16;
							r=lpBitsInfo->bmiColors[color_index].rgbRed;
							g=lpBitsInfo->bmiColors[color_index].rgbGreen;
							b=lpBitsInfo->bmiColors[color_index].rgbBlue;
							H_B[b]++;
							H_G[g]++;
							H_R[r]++;
							avg=(r+g+b)/3;
							H[avg]++;
						}
					}
				}
			break;
	}
}
float range(float intensity)//�����������ĻҶ�ֵ��Χ����ֹԽ��
{
	if( intensity>255)
		return 255;
	else
		if(intensity<0)
			return 0;
		else
			return intensity;
}
void Linear()
{
	float la=(float)0.6,lb=(float)20;
	int w= lpBitsInfo->bmiHeader.biWidth;
	int h= lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//ÿ���ֽ���
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //ָ��λͼ���ݵ�ָ��
	 
	int i,j;
	BYTE* pixel;
	int  r,g,b;

	
	switch(lpBitsInfo->bmiHeader.biBitCount)
	{
		case 24:  //��24���ɫ���ֱ�������ͨ����ǿ�ȣ�����ƽ��ͨ����ǿ��
			for(i=0;i<h;i++)
				{
					for(j=0;j<w;j++)
					{
						pixel=lpBits+LineBytes*(h-i-1)+j*3;
						*pixel=range(*pixel*la+lb);
						*(pixel+1)=range(*(pixel+1)*la+lb);
						*(pixel+2)=range(*(pixel+2)*la+lb);
					}
				}
			break;
		case 8: //�ֲ�ɫ��ڰף���Ҫ��ɫ��ת����ɫ
			if(if_gray())   //256�Ҷ�
			{
				for(i=0;i<h;i++)
				{
					for(j=0;j<w;j++)
						{
						pixel=lpBits+LineBytes*(h-1-i)+j;
						*pixel=range(*pixel*la+lb);
						}
				}
			}
			else   //256��ɫ
			{
				for(i=0;i<256;i++)
				{
					
						r=lpBitsInfo->bmiColors[i].rgbRed;
						g=lpBitsInfo->bmiColors[i].rgbGreen;
						b=lpBitsInfo->bmiColors[i].rgbBlue;
						r=range(r*la+lb);
						g=range(g*la+lb);
						b=range(b*la+lb);
						lpBitsInfo->bmiColors[i].rgbRed=r;
						lpBitsInfo->bmiColors[i].rgbGreen=g;
						lpBitsInfo->bmiColors[i].rgbBlue=b;
					
				}
			}
			break;
		case 4:
			for(i=0;i<16;i++)
				{
					r=lpBitsInfo->bmiColors[i].rgbRed;
					g=lpBitsInfo->bmiColors[i].rgbGreen;
					b=lpBitsInfo->bmiColors[i].rgbBlue;
					r=range(r*la+lb);
					g=range(g*la+lb);
					b=range(b*la+lb);
					lpBitsInfo->bmiColors[i].rgbRed=r;
					lpBitsInfo->bmiColors[i].rgbGreen=g;
					lpBitsInfo->bmiColors[i].rgbBlue=b;	
				}
			break;
	}



}

void FT(complex<double>*TD,complex<double>*FD,int m)
{
	int x,u;
	double angle;
	for(u=0;u<m;u++)
	{
		FD[u]=0;
		for(x=0;x<m;x++)
		{
			angle=-2*PI*u*x/m;
			FD[u]+=TD[x]*complex<double>(cos(angle),sin(angle));
		}
		FD[u]/=m;
	}
}

void IFT(complex<double>*FD,complex<double>*TD,int m)
{
	int x,u;
	double angle;
	for(x=0;x<m;x++)
	{
		FD[x]=0;
		for(u=0;u<m;u++)
		{
			angle=2*PI*u*x/m;
			TD[x]+=FD[u]*complex<double>(cos(angle),sin(angle));
		}
	}
}

complex<double>* gFD=NULL;
BOOL is_gFD_OK()
{
	return (gFD!=NULL);
}
void Fourier()
{
	int w= lpBitsInfo->bmiHeader.biWidth;
	int h= lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//ÿ���ֽ���
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //ָ��λͼ���ݵ�ָ��

	complex<double>* TD=new complex<double>[w*h];  //�������� 
	complex<double>* FD=new complex<double>[w*h];  //Ƶ��

	int i,j;
	BYTE *pixel;

	for(i=0;i<h;i++)   //����ת��Ϊ����
		{
			for(j=0;j<w;j++)
				{
				pixel=lpBits+LineBytes*(h-1-i)+j;
				TD[i*w+j]=complex<double>(*pixel,0); //�������鲿Ϊ0
				//TD[i*w+j]=complex<double>(*pixel*pow(-1,i+j),0)
				}
		}

	for(i=0;i<h;i++)
		 FT(&TD[i*w],&FD[i*w],w);  //ˮƽ���򣬶�ÿһ�н���һά��ɢ����Ҷ�任

	for(i=0;i<h;i++)
		for(j=0;j<w;j++) //�ھ����У��в����㵥�����ʣ������ȶ�FDת�ã��Ž�TD
			TD[j*h+i]=FD[i*w+j];

	for(i=0;i<w;i++)
		 FT(&TD[i*h],&FD[i*h],h);  //��ֱ���򣬶�ÿһ�н���һά��ɢ����Ҷ�任

	gFD=FD;
	delete(TD);

	//��FD���ӻ�����������ֵ��ӳ�䵽0-255��
	LONG size=40+1024+LineBytes*h;
	lpDIB_FT=(BITMAPINFO* )malloc(size);
	memcpy(lpDIB_FT,lpBitsInfo,size);
	lpBits = (BYTE*)&lpDIB_FT->bmiColors[lpDIB_FT->bmiHeader.biClrUsed];

	//�����ֵ
	double temp;
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			pixel=lpBits+LineBytes*(h-1-i)+j;
			temp=sqrt(FD[j*h+i].real()*FD[j*h+i].real()+FD[j*h+i].imag()*FD[j*h+i].imag())*2000;
			
			//ת�õķ�ʽ����
			if (temp > 255 )
				temp = 255;
			*pixel=(BYTE)(temp);
		}
	}
}
void IFourier()
{
	int w= lpBitsInfo->bmiHeader.biWidth;
	int h= lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//ÿ���ֽ���
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //ָ��λͼ���ݵ�ָ��

	complex<double>* TD=new complex<double>[w*h];  //�������� 
	complex<double>* FD=new complex<double>[w*h];  //Ƶ��

	int i,j;
	BYTE *pixel;
	for(i=0;i<h;i++)
		for(j=0;j<w;j++)
			FD[i*w+j]=gFD[h*j+i];
			

	//�Ա任������TD���ӻ�����������ֵ��ӳ�䵽0-255��
	LONG size=40+1024+LineBytes*h;
	lpDIB_IFT=(BITMAPINFO* )malloc(size);
	memcpy(lpDIB_IFT,lpBitsInfo,size);
	lpBits = (BYTE*)&lpDIB_IFT->bmiColors[lpDIB_IFT->bmiHeader.biClrUsed];

	//�����ֵ
	double temp=0;
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{
			pixel=lpBits+LineBytes*(h-1-i)+j;
			*pixel=(BYTE)(TD[h*j+i].real()/pow(-1,i+j));  //ע��/pow(-1,i+j)   û�еĻ��ǰ�ɫ

			//range(temp);
			if (temp > 255 )
				temp = 255;
			*pixel=(BYTE)(temp);
		}
	}
	delete TD;
	delete FD;
	delete gFD;
}

//ģ���������
void Template(int* Array,float coef)
{
	int w= lpBitsInfo->bmiHeader.biWidth;
	int h= lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//ÿ���ֽ���
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //ָ��λͼ���ݵ�ָ��

	LONG size=40+1024+LineBytes*h;
	BITMAPINFO* new_lpBitsInfo=(BITMAPINFO* )malloc(size);
	memcpy(new_lpBitsInfo,lpBitsInfo,size);
	BYTE* new_lpBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed]; //ָ��λͼ���ݵ�ָ��


	int i,j,k,l;
	BYTE *pixel;
	BYTE *new_pixel;
	float temp;

	for(i=1;i<h-1;i++)  //��Ե������
	{
		for(j=1;j<w-1;j++)
		{
			new_pixel=new_lpBits+LineBytes*(h-1-i)+j;  //ָ��Ҫ����������ص�λ��
			temp=0;
			for(k=0;k<3;k++)  //��ÿ�����ؽ��У�3*3ģ������
			{
				for(l=0;l<3;l++)
				{
					pixel = lpBits+LineBytes*(h-k-i)+j-1+l;   //����˳�򣺴��µ��ϣ�������
					temp+=(*pixel)*Array[k*3+l];   //�Ҷ�ֵ*�������,���ۼ�
				}
			}
			temp*=coef;   //���*ϵ��
			if(temp<0)   //��һ�� 
				*new_pixel=0;
			else if(temp>255)
				*new_pixel=255;
			else
				*new_pixel=(BYTE)(temp+0.5);  //�������룬��׼ȷ
			
		}
	}
	free(lpBitsInfo);
	lpBitsInfo=new_lpBitsInfo;   //����ͼ���滻 ԭͼ��
	
}
void AverageFilter()
{
	int Array[9];
	Array[0]=1; Array[1]=1; Array[2]=1;
	Array[3]=1; Array[4]=1; Array[5]=1;
	Array[6]=1; Array[7]=1; Array[8]=1;
	Template(Array,(float)1/9);	

}
void RaplasSharp()
{
	int Array[9];
	Array[0]=-1; Array[1]=-1; Array[2]=-1;
	Array[3]=-1; Array[4]=9; Array[5]=-1;
	Array[6]=-1; Array[7]=-1; Array[8]=-1;
	Template(Array,(float)1);
}
BYTE GetMidNum(BYTE *Array)
{
	int i,j,t;
	/*
	for(i=0;i<9;i++)
	{
		for(j=i;j<8;j++)
		{
			if(Array[j]>Array[j+1])
			{
				t=Array[j];
				Array[j]=Array[j+1];
				Array[j+1]=t;
			}
		}

	} �����������*/ 
	for(j=0;j<8;j++)
	{
		for(i=0;i<8-j;i++)
		{
			if(Array[i]>Array[i+1])
			{
				t=Array[i];
				Array[i]=Array[i+1];
				Array[i+1]=t;
			}
		}
	}
	return (Array[4]);
}
void MedianFilter()
{
	int w= lpBitsInfo->bmiHeader.biWidth;
	int h= lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//ÿ���ֽ���
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //ָ��λͼ���ݵ�ָ��

	LONG size=40+1024+LineBytes*h;
	BITMAPINFO* new_lpBitsInfo=(BITMAPINFO* )malloc(size);
	memcpy(new_lpBitsInfo,lpBitsInfo,size);
	BYTE* new_lpBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed]; //ָ��λͼ���ݵ�ָ��


	int i,j,k,l;
	BYTE *pixel;
	BYTE *new_pixel;
	BYTE Value[9];

	for(i=1;i<h-1;i++)  //��Ե������
	{
		for(j=1;j<w-1;j++)
		{
			new_pixel=new_lpBits+LineBytes*(h-1-i)+j;  //ָ��Ҫ����������ص�λ��
			for(k=0;k<3;k++)  //��ÿ�����ؽ��У�3*3ģ������
			{
				for(l=0;l<3;l++)
				{
					pixel = lpBits+LineBytes*(h-k-i)+j-1+l;  
					Value[k*3+l]=*pixel;
				}
			}
			*new_pixel=(BYTE)GetMidNum(Value);
		}
	}
	free(lpBitsInfo);
	lpBitsInfo=new_lpBitsInfo;   //����ͼ���滻 ԭͼ��
}

void GradSharp()
{
	int w= lpBitsInfo->bmiHeader.biWidth;
	int h= lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//ÿ���ֽ���
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //ָ��λͼ���ݵ�ָ��

	LONG size=40+1024+LineBytes*h;
	BITMAPINFO* new_lpBitsInfo=(BITMAPINFO* )malloc(size);
	memcpy(new_lpBitsInfo,lpBitsInfo,size);
	BYTE* new_lpBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed]; //ָ��λͼ���ݵ�ָ��


	int i,j;
	BYTE *pixel,*pixel_1,*pixel_2;
	BYTE *new_pixel;
	BYTE temp;

	for(i=0;i<h-1;i++)  //��Ե������
	{
		for(j=0;j<w-1;j++)
		{
			new_pixel=new_lpBits+LineBytes*(h-1-i)+j;  //ָ��Ҫ����������ص�λ��
			pixel=lpBits+LineBytes*(h-1-i)+j;
			pixel_1=lpBits+LineBytes*(h-1-i)+j+1;
			pixel_2=lpBits+LineBytes*(h-2-i)+j;
			temp=abs(*pixel-*pixel_1)+abs(*pixel-*pixel_2);
			*new_pixel=(BYTE)temp;
		}
	}
	free(lpBitsInfo);
	lpBitsInfo=new_lpBitsInfo;   //����ͼ���滻 ԭͼ��
}


/*
*/
/*
*/