#include <stdio.h>
#include <stdlib.h>
#include "img.h"


int main(int argc, char *argv[])
{
	BitMapFileHeader FileHeader;
	BitMapInfoHeader InfoHeader;
	RGBQUAD PicQuad;
	int i=0,j=0,k=0;
	FILE * Pic;
	unsigned char MonoBit;
	Pic = fopen(argv[1],"r");
	printf("%d,%d\n",Pic,sizeof(InfoHeader));
	fread(&FileHeader,14,1,Pic);
	printf("MagicNumber:%d\nFileSize:%d\n%d\n%d\nOffSet:%d\n",FileHeader.bfType,FileHeader.bfSize,FileHeader.bfReserved1,FileHeader.bfReserved2,FileHeader.bfOffBits);
	fread(&InfoHeader,40,1,Pic);
	printf("Width:%d\nHeight:%d\nHeaderSize:%d\nCompression:%d\n",InfoHeader.biWidth,InfoHeader.biHeight,InfoHeader.biSize,InfoHeader.biCompression);
	fread(&PicQuad,sizeof(RGBQUAD),1,Pic);

	unsigned char PicBuffer[InfoHeader.biHeight][InfoHeader.biWidth];

	for(j = 0; j < InfoHeader.biHeight;j++)
	{
		for(i=0;i < InfoHeader.biWidth / 8;i++)
		{
			fread(&MonoBit,1,1,Pic);
			unsigned char BitMask = 128;
			unsigned char IsWhite = 0;
			for(k=0;k<8;k++)
			{
				IsWhite = MonoBit & BitMask;
				if(IsWhite>0)
				{
					printf(" ");
					PicBuffer[InfoHeader.biHeight - j-1][i*8+k] = 0x00;
				}
				else
				{
					printf("-");
					PicBuffer[InfoHeader.biHeight - j-1][i*8+k] = 0xFF;
				}
//				printf("i,j,k,%d,%d,%d,%d\n",i,j,k,InfoHeader.biWidth);
				BitMask = BitMask >> 1;
			}
		}
		printf("\n");
	}
	printf("Begin\n");
	for(j = 0; j < InfoHeader.biHeight;j++)
	{
		for(i=0;i < InfoHeader.biWidth;i++)
		{
			if(PicBuffer[j][i]==0x00)
			{
				printf(" ");
			}
			else if(PicBuffer[j][i]==0xFF)
			{
				printf("-");
			}
		}
		printf("\n");
	}
	return 0;
}
