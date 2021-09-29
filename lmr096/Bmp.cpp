#include "stdafx.h"
BITMAPINFO*   lpBitsInfo=NULL;  //��ָ�����Ҫ��ֱ�Ӷ����ȫ��   (BITMAPINFO*==LPBITMAPINFO) NULL�����ΪҰָ��


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

	if(NULL==(lpBitsInfo=(BITMAPINFO*)malloc(size)))//�����ڴ�:��Ϣͷ����ɫ�塢ʵ�ʵ�λͼ����
		return FALSE;
	
	fseek(fp,14,SEEK_SET);//���ļ�ָ����ǰ�ƶ�
	fread((char*)lpBitsInfo,size,1,fp);//���ļ��е����ݶ����ڴ�

	lpBitsInfo->bmiHeader.biClrUsed=NumColors;//���ڴ��е����ݻ����Ը���ģ������ģ�

	return true;
}