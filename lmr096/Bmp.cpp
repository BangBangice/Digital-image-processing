#include "stdafx.h"
#include <complex> 
#include<math.h>
const double PI = 3.1415926535897932;
BITMAPINFO* lpBitsInfo=NULL;  //该指针很重要，直接定义成全局   (BITMAPINFO*==LPBITMAPINFO) NULL避免成为野指针
							//该指针指向读取到内存中的所有的有用位图信息（文件头之后的内容）
BITMAPINFO* lpBitsInfo_save=NULL;  //为reset做保留

BITMAPINFO* lpDIB_FT=NULL;  //频谱幅度图像
BITMAPINFO* lpDIB_IFT=NULL;

//BITMAPINFO*  lpDIB_IFFT=NULL;
DWORD size_save=0;
using namespace std;

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
	if(NULL==(lpBitsInfo_save=(BITMAPINFO*)malloc(size)))//分配内存(最核心):信息头、调色板、实际的位图数据
		return FALSE;
	
	fseek(fp,14,SEEK_SET);//将文件指针往前移动
	fread((char*)lpBitsInfo,size,1,fp);//把文件中的数据读入内存  文件头以后的所有信息

	lpBitsInfo->bmiHeader.biClrUsed=NumColors;//将内存中的数据换成自个刚算的（更放心）

	memcpy(lpBitsInfo_save,lpBitsInfo,size);
	size_save=size;
	


	return true;
}
bool if_gray();
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
	BYTE *B_2=NULL;  //灰度图像的bitcount与原来的不同
	BYTE *pixel,bv;
	switch(lpBitsInfo->bmiHeader.biBitCount)  //判断需要改变的图像的种类
	{
		case 24:   //24位真彩色
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
			break;
		case 8: //256彩色
			if(!if_gray())  //不是灰度图像，需转换
			{
				for(i=0;i<h;i++)
				{
					for(j=0;j<w;j++)
					{ //(i,j)通道各个分量的指针
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
		case 4:   //16位色
			for(i=0;i<h;i++)
				{
					for(j=0;j<w;j++)
					{	//(i,j)通道各个分量的指针
						pixel=lpBits+LineBytes*i+j/2;
						B_2=lpBits_2+i*LineBytes_2+j/2;
						if(!(j%2)) //高四位(前)：>>4
						{
							bv=*pixel>>4;
							avg=(lpBitsInfo->bmiColors[bv].rgbRed+lpBitsInfo->bmiColors[bv].rgbGreen+lpBitsInfo->bmiColors[bv].rgbBlue)/3;							
						}
						else //低四位(后)：&00001111
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
					pixel=lpBits+LineBytes*i+j/8;  //找到该像素所在的字节
					bv=(*pixel)&(1<<(7-j%8));  //像素所在位:7-j%8
					bv=bv>>(7-j%8);
					if(bv>0) //前景点
						avg=(lpBitsInfo->bmiColors[bv].rgbRed+lpBitsInfo->bmiColors[bv].rgbGreen+lpBitsInfo->bmiColors[bv].rgbBlue)/3;						
					else  //背景点
						avg=(lpBitsInfo->bmiColors[bv].rgbRed+lpBitsInfo->bmiColors[bv].rgbGreen+lpBitsInfo->bmiColors[bv].rgbBlue)/3;					
					B_2=lpBits_2+i*LineBytes_2+j;
					*B_2=avg;
				}
			}
			break;
	}
	//生成灰度图像的内容


	free(lpBitsInfo);  //释放旧的指针指向的内存
	lpBitsInfo=lpBitsInfo_2;
}
bool if_gray()
{
	bool flag=1;//默认是灰度
	//if(lpBitsInfo->bmiHeader.biBitCount==8)  //先得是8位图
		for(int i=0;i<256;i++)    //根据调色板判断不是256彩色
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
				bv=*pixel>>4;
				r=lpBitsInfo->bmiColors[bv].rgbRed;
				g=lpBitsInfo->bmiColors[bv].rgbGreen;
				b=lpBitsInfo->bmiColors[bv].rgbBlue;
			}
			else //低四位(后)：&00001111
			{
				bv=*pixel%16;
				r=lpBitsInfo->bmiColors[bv].rgbRed;
				g=lpBitsInfo->bmiColors[bv].rgbGreen;
				b=lpBitsInfo->bmiColors[bv].rgbBlue;
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
void reset()
{	
	free(lpBitsInfo);
	lpBitsInfo=(BITMAPINFO*)malloc(size_save);//分配内存(最核心):信息头、调色板、实际的位图数据
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
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//每行字节数
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //指向位图数据的指针
	 
	int i,j;
	BYTE* pixel;
	int avg,r,g,b,color_index;
	
	for(i=0;i<256;i++)
	{
		H[i]=0;  //对直方图数组进行初始化;
		H_R[i]=0;
		H_G[i]=0;
		H_B[i]=0;
	}

	switch(lpBitsInfo->bmiHeader.biBitCount)
	{
		case 24:  //对24真彩色，分别计算各个通道的强度，并求平均通道的强度
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
		case 8: //分彩色与黑白，需要调色板转换颜色
			if(if_gray())   //256灰度
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
			else   //256彩色
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
						pixel=lpBits+LineBytes*(h-i-1)+j/2;  //找到该像素所在的字节
						if(j%2) //高四位(前)：>>4
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
						else //低四位(后)：&00001111
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
float range(float intensity)//纠正点运算后的灰度值范围，防止越界
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
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//每行字节数
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //指向位图数据的指针
	 
	int i,j;
	BYTE* pixel;
	int  r,g,b;

	
	switch(lpBitsInfo->bmiHeader.biBitCount)
	{
		case 24:  //对24真彩色，分别计算各个通道的强度，并求平均通道的强度
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
		case 8: //分彩色与黑白，需要调色板转换颜色
			if(if_gray())   //256灰度
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
			else   //256彩色
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
complex<double>* gFD=NULL;  //频域空间数据
void FT(complex<double>*TD,complex<double>*FD,int m)
{
	int x,u;
	double angle;
	for(u=0;u<m;u++)  //遍历一维数据
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
void Fourier(){
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	DWORD LineBytes = (w*lpBitsInfo->bmiHeader.biBitCount+31)/32*4;  //每行字节数
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];  //指向位图数据的指针
	
	complex<double>*TD = new complex<double>[w*h]; //空域数据 
	complex<double>*FD = new complex<double>[w*h];  //频域
	
	int i,j;
	BYTE*pixel;
	for(i=0;i<h;i++){   //空域转换为复数
		for(j=0;j<w;j++){
			pixel = lpBits + LineBytes * (h-1-i)+j;
			TD[w*i+j] = complex<double>(*pixel*pow(-1,i+j),0);
			//TD[w*i+j] = complex<double>(*pixel,0);
		}
	}
	for(i=0;i<h;i++){  //水平方向，对每一行进行一维离散傅里叶变换
		FT(&TD[i * w],&FD[i * w],w);
	}
	for(i=0;i<h;i++){  //在矩阵中，列不方便单独访问，我们先对FD转置，放进TD
		for(j=0;j<w;j++){
			TD[h*j+i]=FD[w*i+j];
		}
	}
	for(i=0;i<w;i++){  //垂直方向，对每一列进行一维离散傅里叶变换
		FT(&TD[i * h],&FD[i * h],h);
	}
	DWORD size = 40+1024 + LineBytes * h;
	lpDIB_FT =(BITMAPINFO*) malloc(size);
	memcpy(lpDIB_FT,lpBitsInfo,size);
	lpBits = (BYTE*)&lpDIB_FT->bmiColors[256];
	double temp=0;
	//对FD可视化输出（计算幅值，映射到0-255）
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			pixel = lpBits + LineBytes * (h-1-i)+j;
			temp = sqrt(FD[j*h+i].real()*FD[j*h+i].real()+FD[j*h+i].imag()*FD[j*h+i].imag())*1000;
			if(temp>255)
				temp = 255;
			*pixel = (BYTE)temp;
		}
	}
	delete TD;
	gFD = FD;
	lpDIB_IFT=NULL;//删除反变换图片
}
void IFT(complex<double>*FD,complex<double>*TD,int m){
	int u,x;
	double angle;
	for(u=0;u<m;u++){
		TD[u]=0;
		for(x=0;x<m;x++){
			angle = 2*PI*u*x/m;
			TD[u]+=FD[x]*complex<double>(cos(angle),sin(angle));
		}
	}
}

BOOL is_gFD_OK()
{
	return (gFD!=NULL);
}

void IFourier(){
	int w = lpBitsInfo->bmiHeader.biWidth;
	int h = lpBitsInfo->bmiHeader.biHeight;
	DWORD LineBytes = (w*lpBitsInfo->bmiHeader.biBitCount+31)/32*4;  //每行字节数
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	
	complex<double>*TD = new complex<double>[w*h]; //空域数据 
	complex<double>*FD = new complex<double>[w*h];  //频域
	 
	int i,j;
	BYTE*pixel;
	
	for(i=0;i<h;i++)
		for(j=0;j<w;j++)
			FD[w*i+j] = gFD[w*i+j];    //将保存的全局频域数据赋值到局部变量

	for(i=0;i<h;i++){
		IFT(&FD[i * w],&TD[i * w],w);
	}
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			FD[h*j+i]=TD[w*i+j];   //转置FD矩阵  
		}
	}
	for(i=0;i<w;i++){
		IFT(&FD[i * h],&TD[i * h],h);
	}
	DWORD size = 40+1024 + LineBytes * h;
	lpDIB_IFT =(BITMAPINFO*) malloc(size);
	memcpy(lpDIB_IFT,lpBitsInfo,size);
	lpBits = (BYTE*)&lpDIB_IFT->bmiColors[256];
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			pixel = lpBits + LineBytes * (h-1-i)+j;
			//*pixel = (BYTE)TD[w*i+j].real();
			*pixel = (BYTE)(TD[w*i+j].real()/pow(-1,i+j));
		}
	}
	delete TD;
	delete FD;
	gFD=NULL;
	lpDIB_FT=NULL;  //反变换后删除频域图像，避免显示不出来
}
//模板操作函数
void Template(int* Array,float coef)
{
	int w= lpBitsInfo->bmiHeader.biWidth;
	int h= lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//每行字节数
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //指向位图数据的指针

	LONG size=40+1024+LineBytes*h;
	BITMAPINFO* new_lpBitsInfo=(BITMAPINFO* )malloc(size);
	memcpy(new_lpBitsInfo,lpBitsInfo,size);
	BYTE* new_lpBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed]; //指向位图数据的指针


	int i,j,k,l;
	BYTE *pixel;
	BYTE *new_pixel;
	float temp;

	for(i=1;i<h-1;i++)  //边缘不访问
	{
		for(j=1;j<w-1;j++)
		{
			new_pixel=new_lpBits+LineBytes*(h-1-i)+j;  //指向要运算的新像素的位置
			temp=0;
			for(k=0;k<3;k++)  //对每个像素进行，3*3模板运算
			{
				for(l=0;l<3;l++)
				{
					pixel = lpBits+LineBytes*(h-k-i)+j-1+l;   //遍历顺序：从下到上，从左到右
					temp+=(*pixel)*Array[k*3+l];   //灰度值*矩阵参数,并累加
				}
			}
			temp*=coef;   //结果*系数
			if(temp<0)   //归一化 
				*new_pixel=0;
			else if(temp>255)
				*new_pixel=255;
			else
				*new_pixel=(BYTE)(temp+0.5);  //四舍五入，更准确
			
		}
	}
	free(lpBitsInfo);
	lpBitsInfo=new_lpBitsInfo;   //用新图像替换 原图像
	
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

	} 有问题的排序*/ 
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
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//每行字节数
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //指向位图数据的指针

	LONG size=40+1024+LineBytes*h;
	BITMAPINFO* new_lpBitsInfo=(BITMAPINFO* )malloc(size);
	memcpy(new_lpBitsInfo,lpBitsInfo,size);
	BYTE* new_lpBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed]; //指向位图数据的指针


	int i,j,k,l;
	BYTE *pixel;
	BYTE *new_pixel;
	BYTE Value[9];

	for(i=1;i<h-1;i++)  //边缘不访问
	{
		for(j=1;j<w-1;j++)
		{
			new_pixel=new_lpBits+LineBytes*(h-1-i)+j;  //指向要运算的新像素的位置
			for(k=0;k<3;k++)  //对每个像素进行，3*3模板运算
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
	lpBitsInfo=new_lpBitsInfo;   //用新图像替换 原图像
}

void GradSharp()
{
	int w= lpBitsInfo->bmiHeader.biWidth;
	int h= lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//每行字节数
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //指向位图数据的指针

	LONG size=40+1024+LineBytes*h;
	BITMAPINFO* new_lpBitsInfo=(BITMAPINFO* )malloc(size);
	memcpy(new_lpBitsInfo,lpBitsInfo,size);
	BYTE* new_lpBits = (BYTE*)&new_lpBitsInfo->bmiColors[new_lpBitsInfo->bmiHeader.biClrUsed]; //指向位图数据的指针


	int i,j;
	BYTE *pixel,*pixel_1,*pixel_2;
	BYTE *new_pixel;
	BYTE temp;

	for(i=0;i<h-1;i++)  //边缘不访问
	{
		for(j=0;j<w-1;j++)
		{
			new_pixel=new_lpBits+LineBytes*(h-1-i)+j;  //指向要运算的新像素的位置
			pixel=lpBits+LineBytes*(h-1-i)+j;
			pixel_1=lpBits+LineBytes*(h-1-i)+j+1;
			pixel_2=lpBits+LineBytes*(h-2-i)+j;
			temp=abs(*pixel-*pixel_1)+abs(*pixel-*pixel_2);
			*new_pixel=(BYTE)temp;
		}
	}
	free(lpBitsInfo);
	lpBitsInfo=new_lpBitsInfo;   //用新图像替换 原图像
}

void FFT(complex<double> * TD, complex<double> * FD, int r)
{
	// 计算付立叶变换点数
	LONG count = 1 << r;
	// 计算加权系数
	int i;
	double angle;
	complex<double>* W = new complex<double>[count / 2];
	for(i = 0; i < count / 2; i++)
	{
		angle = -i * PI * 2 / count;
		W[i] = complex<double> (cos(angle), sin(angle));
	}
	// 将时域点写入X1
	complex<double>* X1 = new complex<double>[count];
	memcpy(X1, TD, sizeof(complex<double>) * count);
	
	// 采用蝶形算法进行快速付立叶变换，输出为频域值X2
	complex<double>* X2 = new complex<double>[count]; 

	int k,j,p,size;
	complex<double>* temp;
	for (k = 0; k < r; k++)
	{
		for (j = 0; j < 1 << k; j++)
		{
			size = 1 << (r-k);
			for (i = 0; i < size/2; i++)
			{
				p = j * size;
				X2[i + p] = X1[i + p] + X1[i + p + size/2];
				X2[i + p + size/2] = (X1[i + p] - X1[i + p + size/2]) * W[i * (1<<k)];
			}
		}
		temp  = X1;
		X1 = X2;
		X2 = temp;
	}
	
	// 重新排序（码位倒序排列）
	for (j = 0; j < count; j++)
	{
		p = 0;
		for (i = 0; i < r; i++)
		{
			if (j & (1<<i))
			{
				p += 1<<(r-i-1);
			}
		}
		FD[j]=X1[p];
		FD[j] /= count;
	}
	
	// 释放内存
	delete W;
	delete X1;
	delete X2;
}

void FFourier()
{
	//图像的宽度和高度
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	//指向图像数据指针
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[256];

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂数，即迭代次数
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂数，即迭代次数
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// 分配内存
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	BYTE* pixel;
	
	for(i = 0; i < FFT_h; i++)  // 行
	{
		for(j = 0; j < FFT_w; j++)  // 列
		{
			// 指向DIB第i行，第j个象素的指针
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// 给时域赋值
			TD[j + FFT_w * i] = complex<double>(*pixel* pow(-1,i+j), 0);
		}
	}
	
	for(i = 0; i < FFT_h; i++)
	{
		// 对y方向进行快速付立叶变换
		FFT(&TD[FFT_w * i], &FD[FFT_w * i], wp);
	}
	
	// 保存中间变换结果
	for(i = 0; i < FFT_h; i++)
	{
		for(j = 0; j < FFT_w; j++)
		{
			TD[i + FFT_h * j] = FD[j + FFT_w * i];
		}
	}
	
	for(i = 0; i < FFT_w; i++)
	{
		// 对x方向进行快速付立叶变换
		FFT(&TD[i * FFT_h], &FD[i * FFT_h], hp);
	}

	// 删除临时变量
	delete TD;

	//生成频谱图像
	//为频域图像分配内存
	LONG size = 40 + 1024 + LineBytes * height;
	lpDIB_FT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_FT)
		return;
	memcpy(lpDIB_FT, lpBitsInfo, size);

	//指向频域图像数据指针
	lpBits = (BYTE*)&lpDIB_FT->bmiColors[256];

	double temp;
	for(i = 0; i < FFT_h; i++) // 行
	{
		for(j = 0; j < FFT_w; j++) // 列
		{
			// 计算频谱幅度
			temp = sqrt(FD[j * FFT_h + i].real() * FD[j * FFT_h + i].real() + 
				        FD[j * FFT_h + i].imag() * FD[j * FFT_h + i].imag()) *2000;
			
			// 判断是否超过255
			if (temp > 255)
			{
				// 对于超过的，直接设置为255
				temp = 255;
			}
			
			pixel = lpBits + LineBytes * (height - 1 - i) + j;

			// 更新源图像
			*pixel = (BYTE)(temp);
		}
	}

	gFD=FD;
	lpDIB_IFT=NULL;

}
//IFFT反变换
void IFFT(complex<double> * FD, complex<double> * TD, int r)
{
	// 付立叶变换点数
	LONG	count;
	// 计算付立叶变换点数
	count = 1 << r;

	// 分配运算所需存储器
	complex<double> * X = new complex<double>[count];
	// 将频域点写入X
	memcpy(X, FD, sizeof(complex<double>) * count);
	
	// 求共轭
	for(int i = 0; i < count; i++)
		X[i] = complex<double> (X[i].real(), -X[i].imag());
	
	// 调用快速付立叶变换
	FFT(X, TD, r);
	
	// 求时域点的共轭
	for(i = 0; i < count; i++)
		TD[i] = complex<double> (TD[i].real() * count, -TD[i].imag() * count);
	
	// 释放内存
	delete X;
}

void IFFourier()
{
	//图像的宽度和高度
	int width = lpBitsInfo->bmiHeader.biWidth;
	int height = lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (width * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;

	// FFT宽度（必须为2的整数次方）
	int FFT_w = 1;
	// FFT宽度的幂数，即迭代次数
	int wp = 0;
	while(FFT_w * 2 <= width)
	{
		FFT_w *= 2;
		wp ++;
	}

	// FFT高度（必须为2的整数次方）
	int FFT_h = 1;
	// FFT高度的幂数，即迭代次数
	int hp = 0;
	while(FFT_h * 2 <= height)
	{
		FFT_h *= 2;
		hp ++;
	}

	// 分配内存
	complex<double>* TD = new complex<double>[FFT_w * FFT_h];
	complex<double>* FD = new complex<double>[FFT_w * FFT_h];
	
	int i, j;
	for(i = 0; i < FFT_h; i++)  // 行
		for(j = 0; j < FFT_w; j++)  // 列
			FD[j + FFT_w * i] = gFD[i + FFT_h*j];
	
	// 沿水平方向进行快速付立叶变换
	for(i = 0; i < FFT_h; i++)
		IFFT(&FD[FFT_w * i], &TD[FFT_w * i], wp);
	
	// 保存中间变换结果
	for(i = 0; i < FFT_h; i++)
		for(j = 0; j < FFT_w; j++)
			FD[i + FFT_h * j] = TD[j + FFT_w * i];
	
	// 沿垂直方向进行快速付立叶变换
	for(i = 0; i < FFT_w; i++)
		IFFT(&FD[i * FFT_h], &TD[i * FFT_h], hp);

	//为反变换图像分配内存
	LONG size = 40 + 1024 + LineBytes * height;

	lpDIB_IFT = (LPBITMAPINFO) malloc(size);
	if (NULL == lpDIB_IFT)
		return;
	memcpy(lpDIB_IFT, lpBitsInfo, size);

	//指向反变换图像数据指针
	BYTE* lpBits = (BYTE*)&lpDIB_IFT->bmiColors[256];
	BYTE* pixel;
	double temp;
	for(i = 0; i < FFT_h; i++) // 行
	{
		for(j = 0; j < FFT_w; j++) // 列
		{
			pixel = lpBits + LineBytes * (height - 1 - i) + j;
			temp= (TD[j*FFT_h + i].real() / pow(-1, i+j));
			if (temp < 0)
				temp = 0;
			else if (temp >255)
				temp = 255;
			*pixel = (BYTE)temp;
		}
	}

	// 删除临时变量
	delete FD;
	delete TD;
	delete gFD;
	gFD = NULL;
	lpDIB_FT=NULL;
}
void FFT_Filter(int D)
{
	//1 乘一个滤波器函数
	//2 更新频谱图像数据
	//傅里叶反变换
	int w= lpBitsInfo->bmiHeader.biWidth;
	int h= lpBitsInfo->bmiHeader.biHeight;
	int LineBytes = (w * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;//每行字节数
	BYTE* lpBits = (BYTE*)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed]; //指向位图数据的指针



	complex<double>* origin_FD=new complex<double>[w*h];
	for(int n=0;n<w*h;n++)
		origin_FD[n]=gFD[n];

	int i,j;
	double dis; //到圆心的距离
	for(i=0;i<h;i++)  //边缘不访问
	{
		for(j=0;j<w;j++)
		{
		dis=sqrt((i-h/2)*(i-h/2)+(j-w/2)*(j-w/2));
		if(D>0) //low pass
		{
			if(dis>D)
				gFD[i*w+j]=0;
		}
		else //high pass
		{
			if(dis<-D)
				gFD[i*w+j]=0;
		}
		//巴特沃斯 [B]  [Ga]  
		//最好做三个按钮
		/*if(D>0) //low pass
		{
				gFD[i*w+j]*=1/（1+pow(dis/D,4));
		}
		else //high pass
		{
			if(dis<-D)
				gFD[i*w+j]*=1/(1+pow(-D/dis,4));
		}
			*/
		}
	
	}

	double temp;
	/*for(i = 0; i < h; i++) // 行
	{
		for(j = 0; j < w; j++) // 列
		{
			pixel = lpBits + LineBytes * (h - 1 - i) + j;
			temp= (FD[j*h + i].real() / pow(-1, i+j));
			if (temp < 0)
				temp = 0;
			else if (temp >255)
				temp = 255;
			*pixel = (int)temp;
		}
	}
*/
	IFFourier();

	delete gFD;
	gFD=origin_FD;
}

/*
*/
/*
*/