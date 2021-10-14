#include "stdafx.h"
BITMAPINFO* lpBitsInfo=NULL;  //该指针很重要，直接定义成全局   (BITMAPINFO*==LPBITMAPINFO) NULL避免成为野指针
							//该指针指向读取到内存中的所有的有用位图信息（文件头之后的内容）

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
	fread((char*)lpBitsInfo,size,1,fp);//把文件中的数据读入内存  文件头以后的所有信息

	lpBitsInfo->bmiHeader.biClrUsed=NumColors;//将内存中的数据换成自个刚算的（更放心）

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

	BITMAPINFO* lpBitsInfo_2=NULL;  //定义一个位图信息指针，方便暂存新的位图信息

	DWORD ImgSize=(w*8+31)/32*4*h; //bitcount改变了，为8
	DWORD size=40+256*4+ImgSize;  //灰度图像的调色板大小固定为256，调色板一个单位4byte
								//信息头+调色板+位图数据

	lpBitsInfo_2=(BITMAPINFO*)malloc(size); //为新的灰度图像信息指针申请内存

	memcpy(lpBitsInfo_2,lpBitsInfo,40);//信息头不变，直接拷贝过来

	lpBitsInfo_2->bmiHeader.biClrUsed=256;
	lpBitsInfo_2->bmiHeader.biBitCount=8; //修改信息头的基础信息:8位 256色

	BYTE* lpBits_2=(BYTE*)&lpBitsInfo_2->bmiColors[lpBitsInfo_2->bmiHeader.biClrUsed];

	int LineBytes_2 = (w * lpBitsInfo_2->bmiHeader.biBitCount + 31)/32 * 4;//每行字节数

	int  i,j;
	for(i=0;i<256;i++)
	{
		lpBitsInfo_2->bmiColors[i].rgbBlue=i;
		lpBitsInfo_2->bmiColors[i].rgbGreen=i;
		lpBitsInfo_2->bmiColors[i].rgbRed=i;
	}
	//生成一个256灰度图像的调色板
	
	BYTE *R,*G,*B,avg; 
	BYTE *B_2;  //灰度图像的bitcount与原来的不同
	for(i=0;i<h;i++)
	{
		for(j=0;j<w;j++)
		{ //(i,j)通道各个分量的指针
			B=lpBits+i*LineBytes+j*3;
			G=B+1;
			R=G+1;
			avg=(*R+*G+*B)/3;
			B_2=lpBits_2+i*LineBytes_2+j;
			*B_2=avg;			
		}
	}
	//生成灰度图像的内容

	free(lpBitsInfo);  //释放旧的指针指向的内存
	lpBitsInfo=lpBitsInfo_2;
}
bool if_gray()
{
	bool flag=1;//默认是灰度
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
		return;  //没有图像
	int w=lpBitsInfo->bmiHeader.biWidth;
	int h=lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//每行字节数
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //指向位图数据的指针
	
	if(i>=h||j>=w)
		return;  //光标超出图像范围

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
		case 8: //分彩色与黑白，需要调色板转换颜色
			pixel=lpBits+LineBytes*(h-i-1)+j;
			if(if_gray())
			{
				r=lpBitsInfo->bmiColors[*pixel].rgbBlue;
				sprintf(rgb,"灰度值:%d",r);
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
			pixel=lpBits+LineBytes*(h-i-1)+j/2;  //找到该像素所在的字节
			if(j%2) //高四位(前)：>>4
			{
				*pixel=*pixel>>4;
				r=lpBitsInfo->bmiColors[*pixel].rgbRed;
				g=lpBitsInfo->bmiColors[*pixel].rgbGreen;
				b=lpBitsInfo->bmiColors[*pixel].rgbBlue;
			}
			else //低四位(后)：&00001111
			{
				*pixel=*pixel%16;
				r=lpBitsInfo->bmiColors[*pixel].rgbRed;
				g=lpBitsInfo->bmiColors[*pixel].rgbGreen;
				b=lpBitsInfo->bmiColors[*pixel].rgbBlue;
			}
			sprintf(rgb,"R:%d G:%d B:%d",r,g,b);
			break;
		case 1:
			pixel=lpBits+LineBytes*(h-i-1)+j/8;  //找到该像素所在的字节
			bv=(*pixel)&(1<<(7-j%8));  //像素所在位:7-j%8
			bv=bv>>(7-j%8);
			if(bv>0) //前景点
				sprintf(rgb,"灰度值:%d",lpBitsInfo->bmiColors[bv].rgbRed);
			else  //背景点
				sprintf(rgb,"灰度值:%d",lpBitsInfo->bmiColors[bv].rgbRed);
			break;
	}
}

/*
	

*/