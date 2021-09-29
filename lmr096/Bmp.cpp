#include "stdafx.h"
BITMAPINFO* lpBitsInfo=NULL;  //该指针很重要，直接定义成全局   (BITMAPINFO*==LPBITMAPINFO) NULL避免成为野指针


BOOL LoadBmpFile(char* BmpFileName)//参数：路径名和文件名  函数功能：读入整个文件到内存
{
	FILE* fp;
	if(NULL==(fp=fopen(BmpFileName,"rb")))//只读打开，处理文件打开失败的情况
		return FALSE;
	BITMAPFILEHEADER bf;//文件头14byte
	BITMAPINFOHEADER bi;//信息头 40byte

	fread(&bf,14,1,fp);
	fread(&bi,40,1,fp);  //文件指针读完后往下走，指向调色板

	DWORD NumColors; //调色板的长度
	if(bi.biClrUsed!=0) //调色板的长度就是它
		NumColors=bi.biClrUsed;
	else{  //根据位图色数，计算调色板的位数（长度）
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
			NumColors=0;//24 位真彩色没有调色板，长度为0
			break;
		}
	}
	DWORD PalSize=NumColors*4; //调色板的内存占用大小
	//实际的位图数据
	DWORD ImgSize=(bi.biWidth*bi.biBitCount+31)/32*4*bi.biHeight; //微软规定必须是四的整数倍
	
	DWORD size=40+PalSize+ImgSize;//总的大小

	if(NULL==(lpBitsInfo=(BITMAPINFO*)malloc(size)))//分配内存(最核心):信息头、调色板、实际的位图数据
		return FALSE;
	
	fseek(fp,14,SEEK_SET);//将文件指针往前移动
	fread((char*)lpBitsInfo,size,1,fp);//把文件中的数据读入内存

	lpBitsInfo->bmiHeader.biClrUsed=NumColors;//将内存中的数据换成自个算的（更放心）

	return true;
}

void gray()
{
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//每行字节数
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //指向位图数据的指针
	//四句是套路，每次都要用
	//每个像素点，内存中顺序是BGR
	int  i,j;
	BYTE *R,*G,*B,avg; 
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{ //(i,j)通道各个分量的指针
			B=lpBits+i*LineBytes+j*3;
			G=B+1;
			R=G+1;
			avg=(*R+*G+*B)/3;
			*R=*G=*B=avg;
		}
	}
}