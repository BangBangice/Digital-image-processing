#include "stdafx.h"
BITMAPINFO* lpBitsInfo=NULL;  //��ָ�����Ҫ��ֱ�Ӷ����ȫ��   (BITMAPINFO*==LPBITMAPINFO) NULL�����ΪҰָ��
							//��ָ��ָ���ȡ���ڴ��е����е�����λͼ��Ϣ���ļ�ͷ֮������ݣ�

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
	
	fseek(fp,14,SEEK_SET);//���ļ�ָ����ǰ�ƶ�
	fread((char*)lpBitsInfo,size,1,fp);//���ļ��е����ݶ����ڴ�  �ļ�ͷ�Ժ��������Ϣ

	lpBitsInfo->bmiHeader.biClrUsed=NumColors;//���ڴ��е����ݻ����Ը�����ģ������ģ�

	return true;
}

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
	BYTE *B_2;  //�Ҷ�ͼ���bitcount��ԭ���Ĳ�ͬ
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
	//���ɻҶ�ͼ�������

	free(lpBitsInfo);  //�ͷžɵ�ָ��ָ����ڴ�
	lpBitsInfo=lpBitsInfo_2;
}
bool if_gray()
{
	bool flag=1;//Ĭ���ǻҶ�
	for(int i=0;i<256;i++)
		if(lpBitsInfo->bmiColors[i].rgbBlue!=i){
			flag=0;
			break;
		}
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
				*pixel=*pixel>>4;
				r=lpBitsInfo->bmiColors[*pixel].rgbRed;
				g=lpBitsInfo->bmiColors[*pixel].rgbGreen;
				b=lpBitsInfo->bmiColors[*pixel].rgbBlue;
			}
			else //����λ(��)��&00001111
			{
				*pixel=*pixel%16;
				r=lpBitsInfo->bmiColors[*pixel].rgbRed;
				g=lpBitsInfo->bmiColors[*pixel].rgbGreen;
				b=lpBitsInfo->bmiColors[*pixel].rgbBlue;
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

/*
	

*/