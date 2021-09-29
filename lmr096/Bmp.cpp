#include "stdafx.h"

BOOL LoadBmpFile(char* BmpFileName)//路径名和文件名
{
	FILE* fp;
	if(NULL==(fp=fopen(BmpFileName,"rb")))//只读打开，处理文件打开失败的情况
		return FALSE;
	BITMAPFILEHEADER bf;//文件头14BYTE
	BITMAPINFOHEADER bi;//信息头 40byte

	fread(&bf,14,1,fp);
	fread(&bi,40,1,fp);

	return true;
}