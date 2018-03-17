#include "bmp_ui.h"

BMP_ui::BMP_ui()
{

}

QPixmap BMP_ui::Load(QString filename){
    QByteArray temp = filename.toLatin1();
    char* fname = temp.data();

    BMPheader b_header;
    BITMAPinfo b_info;

    FILE* file;
    if(!(file = fopen(fname, "rb"))){
        QMessageBox::information(0, "Error", "File can't be opened, try another!");
        QPixmap temp;
        temp.fill("white");
        return temp;
    }

    fread(&b_header, sizeof(b_header), 1, file);
    fseek(file, sizeof(b_header), SEEK_SET);
    fread(&b_info, sizeof(b_info), 1, file);
    int padding = 0;
    if((b_info.biWidth * 3) % 4 != 0){
        padding = 4 - (b_info.biWidth * 3) % 4;
    }

    RGBTriple pixel_info[b_info.biWidth*b_info.biWidth];

    int i = 0;
    QPixmap image(b_info.biWidth, b_info.biHeight);
    QPainter painter(&image);

    fseek(file, b_header.bfOffBits, SEEK_SET);
    for(int y = b_info.biHeight - 1; y >= 0; y--){
        for(int x = 0; x < (int)b_info.biWidth; x++){
            fread(&pixel_info[i], sizeof(RGBTriple), 1, file);
            painter.setPen(QColor(pixel_info[i].rgbRed, pixel_info[i].rgbGreen, pixel_info[i].rgbBlue));
            painter.drawPoint(x, y);
            i++;
        }
        if(padding != 0){
            RGBTriple temp = {0, 0, 0};
            fwrite(&temp, padding, 1, file);
        }
    }
    fclose(file);
    return image;
}

void BMP_ui::Save(QString filename, QImage image){
    QByteArray temp = filename.toLatin1();
    char* fname = temp.data();
    FILE* file = fopen(fname, "w");
    BMPheader b_header;
    BITMAPinfo b_info;


    int w = image.width();
    int h = image.height();

    memset(&b_header, 0, sizeof(b_header));
    b_header.b1  = 'B';
    b_header.b2  = 'M';
    b_header.bfOffBits = sizeof(b_header) + sizeof(b_info);
    b_header.bfSize = b_header.bfOffBits + sizeof(RGBTriple) * w * h + h*w%4;
    memset(&b_info, 0, sizeof(b_info));

    b_info.biSize = sizeof(b_info);
    b_info.biBitCount = 24;
    b_info.biCompression = 0;
    b_info.biHeight = h;
    b_info.biWidth = w;
    b_info.biPlanes = 1;

    fwrite(&b_header, sizeof(b_header), 1, file);
    fwrite(&b_info, sizeof(b_info), 1, file);
	
	int padding = 0;
    if((b_info.biWidth * 3) % 4 != 0){
        padding = 4 - (b_info.biWidth * 3) % 4;
    }

    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            QRgb pixel = image.pixel(x, y);
            unsigned char red = qRed(pixel);
            unsigned char green = qGreen(pixel);
            unsigned char blue = qBlue(pixel);
            RGBTriple temp = {blue, green, red};
            fwrite(&temp, sizeof(temp), 1, file);
        }
        if(padding != 0){
            RGBTriple temp = {0, 0, 0};
            fwrite(&temp, padding, 1, file);
        }
    }
    fclose(file);
}
