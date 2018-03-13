#ifndef BMPSTRUCT_H
#define BMPSTRUCT_H

#pragma pack(push)
#pragma pack(1)

struct BMPheader{
    unsigned char b1;
    unsigned char b2;

    unsigned long bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned long bfOffBits;
};

struct BITMAPinfo{
    unsigned long  biSize;
    unsigned long  biWidth;
    unsigned long  biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned long  biCompression;
    unsigned long  biSizeImage;
    unsigned long  biXPelsPerMeter;
    unsigned long  biYPelsPerMeter;
    unsigned long  biClrUsed;
    unsigned long  biClrImportant;
};

struct RGBTriple{
   unsigned char rgbBlue;
   unsigned char rgbGreen;
   unsigned char rgbRed;
};
#pragma pack(pop)

#endif // BMPSTRUCT_H
