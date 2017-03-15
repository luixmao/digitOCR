#ifndef BMPLIB_H
#define BMPLIB_H

#define SIZE 256
#define RGB 3

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

struct BMPTYPE {
  char signature[2];
} ;

struct BMPFILEHEADER {
  uint32 fileSize;
  uint16 rsvd1;
  uint16 rsvd2;
  uint32 pixelOffset;
} ;

struct BITMAPHEADER  {
  uint32 dibHeaderSize;
  uint32 width;
  uint32 height;
  uint16 planes;
  uint16 bpp;
  uint32 compression;
  uint32 imageSize;
  uint32 xppm;
  uint32 yppm;
  uint32 colorTableSize;
  uint32 importantColor;
};

struct BMPHDR {
  BMPFILEHEADER fh;
  BITMAPHEADER bh;
};

int readGS_BMP(char filename[], unsigned char image[][SIZE], BMPHDR *hdr);

int writeGS_BMP(char *filename, uint8 outputImage[][SIZE], BMPHDR *hdr );

//int readGS_BMP(char filename[], unsigned char image[][SIZE], BMPTYPE *type,  BMPFILEHEADER *fHeader, BITMAPHEADER *bmpHeader);

//int writeGS_BMP(char *filename, uint8 outputImage[][SIZE], BMPTYPE type,  BMPFILEHEADER fHeader, BITMAPHEADER bmpHeader );

#endif
