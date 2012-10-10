#ifndef IMG_H_INCLUDED
#define IMG_H_INCLUDED

#pragma pack(2)
typedef struct
{
	unsigned short	bfType;
	unsigned int	bfSize;
	unsigned short	bfReserved1;
	unsigned short	bfReserved2;
	unsigned int	bfOffBits;
}BitMapFileHeader;
#pragma pack()

typedef struct
{
	unsigned int	biSize;
	unsigned int	biWidth;
	unsigned int	biHeight;
	unsigned short	biPlanes;
	unsigned short	biBitCount;
	unsigned int	biCompression;
	unsigned int	biSizeImage;
	unsigned int	biXPelsPerMeter;
	unsigned int	biYPelsPerMeter;
	unsigned int	biClrUsed;
	unsigned int	biClrImportant;
}BitMapInfoHeader;

typedef struct
{
	unsigned char	R1;
	unsigned char	G1;
	unsigned char	B1;
	unsigned char	NO1;
	unsigned char	R2;
	unsigned char	G2;
	unsigned char	B2;
	unsigned char	NO2;
}RGBQUAD;

typedef struct
{
	unsigned char	R;
	unsigned char	G;
	unsigned char	B;
}RGBPixel;

typedef struct
{
	unsigned char	ESC;
	unsigned char	G;
	unsigned char	Size;
}RLEHeader;

typedef struct
{
	unsigned char	ESC1;
	unsigned char	ESC2;
	unsigned char	G;
	unsigned char	N;
}RLEFooter;

#endif // IMG_H_INCLUDED
