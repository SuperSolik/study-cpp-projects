#ifndef BMP_UI_H
#define BMP_UI_H

#include <QString>
#include <QMessageBox>
#include <algorithm> // for min and max
#include <fstream> //open and write

#define PADDING(x) ((4 - ((x*3)%4))%4)

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
    long  biWidth;
    long  biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned long  biCompression;
    unsigned long  biSizeImage;
    long  biXPelsPerMeter;
    long  biYPelsPerMeter;
    unsigned long  biClrUsed;
    unsigned long  biClrImportant;
};

struct RGBTriple{
   unsigned char rgbBlue;
   unsigned char rgbGreen;
   unsigned char rgbRed;
};
#pragma pack(pop)

class BMP_ui:public QObject{
    Q_OBJECT

public:
    BMPheader b_header;
    BITMAPinfo b_info;
    RGBTriple** pixels;

public:
    BMP_ui(int x, int y);
    ~BMP_ui();


public:
    QImage DrawImage();
    void resizeRaster(int old_y, int new_x, int new_y);
    void Load(QString filename);
    void Save(QString filename);
    void DrawLine(int x1, int y1, int x2, int y2, QColor color, int w);
    void FillCircle(int x1, int y1, int r, QColor color);
    void Invert(int x1, int y1, int r);
    void Crop(int x1, int y1, int x2, int y2);
    void Clear();
};

#endif // BMP_UI_H
