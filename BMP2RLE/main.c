#include <stdio.h>
#include <stdlib.h>
#include "img.h"


int main(int argc, char *argv[])
{
	BitMapFileHeader FileHeader;
	BitMapInfoHeader InfoHeader;
	RGBQUAD BMPInQuad;
	int i=0,j=0,k=0;
	FILE *BMPIn,*RLEOut;
	unsigned char MonoBit;
	BMPIn = fopen(argv[1],"r");
	printf("%d,%d\n",BMPIn,sizeof(InfoHeader));
	fread(&FileHeader,14,1,BMPIn);
	printf("MagicNumber:%d\nFileSize:%d\n%d\n%d\nOffSet:%d\n",FileHeader.bfType,FileHeader.bfSize,FileHeader.bfReserved1,FileHeader.bfReserved2,FileHeader.bfOffBits);
	fread(&InfoHeader,40,1,BMPIn);
	printf("Width:%d\nHeight:%d\nHeaderSize:%d\nCompression:%d\n",InfoHeader.biWidth,InfoHeader.biHeight,InfoHeader.biSize,InfoHeader.biCompression);
	fread(&BMPInQuad,sizeof(RGBQUAD),1,BMPIn);

	unsigned char BMPInBuffer[InfoHeader.biHeight][InfoHeader.biWidth];

	for(i = 0; i < InfoHeader.biHeight;i++)
	{
		for(j=0;j < InfoHeader.biWidth / 8;j++)
		{
			fread(&MonoBit,1,1,BMPIn);
			unsigned char BitMask = 128;
			unsigned char IsWhite = 0;
			for(k=0;k<8;k++)
			{
				IsWhite = MonoBit & BitMask;
				if(IsWhite>0)
				{
					printf(" ");
					BMPInBuffer[InfoHeader.biHeight - i-1][j*8+k] = 0x00;
				}
				else
				{
					printf("-");
					BMPInBuffer[InfoHeader.biHeight - i-1][j*8+k] = 0xFF;
				}
//				printf("i,i,k,%d,%d,%d,%d\n",i,i,k,InfoHeader.biWidth);
				BitMask = BitMask >> 1;
			}
		}
		printf("\n");
	}
	fclose(BMPIn);
	printf("Begin\n");
	for(i = 0; i < InfoHeader.biHeight;i++)
	{
		for(j=0;j < InfoHeader.biWidth;j++)
		{
			if(BMPInBuffer[i][j]==0x00)
			{
				printf("-");
			}
			else if(BMPInBuffer[i][j]==0xFF)
			{
				printf(" ");
			}
		}
		printf("\n");
	}

	RLEOut = fopen(argv[2],"w");
	RLEHeader OutputHeader;
	RLEFooter OutputFooter;
	unsigned short RLEByte;

	OutputHeader.ESC = 0x1B;
	OutputHeader.G = 0x47;
	OutputHeader.Size = 0x48;

	OutputFooter.ESC1 = 0x1B;
	OutputFooter.ESC2 = 0x1B;
	OutputFooter.G = 0x47;
	OutputFooter.N = 0x4E;

	printf("%d",sizeof(OutputHeader));
	fwrite(&OutputHeader,1,sizeof(OutputHeader),RLEOut);

	unsigned char BlackCount = 0x20;
	unsigned char WhiteCount = 0x20;
	int Mode = 1;

	for(i = 0; i < InfoHeader.biHeight;i++)
	{
		for(j=0;j < InfoHeader.biWidth;j++)
		{
			if(Mode == 1)
			{
				if(BMPInBuffer[i][j]==0x00)
				{
					BlackCount++;
				}
				else
				{
					Mode = 2;
					WhiteCount++;
				}

				if(BlackCount == 0x7E)
				{
					Mode = 2;
				}
			}
			else if(Mode == 2)
			{
				if(BMPInBuffer[i][j]==0xFF)
				{
					WhiteCount++;
				}
				else
				{
					RLEByte = BlackCount<<4 | WhiteCount;
//					printf("%x",RLEByte);
					fwrite(&BlackCount,1,sizeof(BlackCount),RLEOut);
					fwrite(&WhiteCount,1,sizeof(WhiteCount),RLEOut);
//					fwrite(&RLEByte,1,sizeof(RLEByte),RLEOut);
					BlackCount = 0x21;
					WhiteCount = 0x20;
					Mode = 1;
				}
				if(WhiteCount == 0x7E)
				{
					RLEByte = BlackCount<<4 | WhiteCount;
//					printf("%x",RLEByte);
					fwrite(&BlackCount,1,sizeof(BlackCount),RLEOut);
					fwrite(&WhiteCount,1,sizeof(WhiteCount),RLEOut);
//					fwrite(&RLEByte,1,sizeof(RLEByte),RLEOut);
					BlackCount = 0x20;
					WhiteCount = 0x20;
					Mode = 1;
				}
			}

		}
	}
	fwrite(&BlackCount,1,sizeof(BlackCount),RLEOut);
	fwrite(&WhiteCount,1,sizeof(WhiteCount),RLEOut);

	fwrite(&OutputFooter,1,sizeof(OutputFooter),RLEOut);
	fclose(RLEOut);
	return 0;
}
