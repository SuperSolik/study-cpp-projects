#include "bmp_ui.h"
BMP_ui::BMP_ui(unsigned int x, unsigned int y)
{
     memset(&b_header, 0,  sizeof(b_header));
     memset(&b_info, 0,  sizeof(b_info));
     b_info.biHeight = y;
     b_info.biWidth = x;
     pixels = (RGBTriple**)malloc(y*sizeof(RGBTriple*));
     for(unsigned int i = 0; i < y; i++)
        pixels[i] = (RGBTriple*)malloc(x*sizeof(RGBTriple));
}

BMP_ui::~BMP_ui(){
    for(unsigned int i = 0; i < b_info.biHeight; i++){
        free(pixels[i]);
    }
    free(pixels);
}

void BMP_ui::resizeRaster(unsigned int new_x, unsigned int new_y, int old_x, int old_y){
    b_info.biHeight = new_y;
    b_info.biWidth = new_x;
    pixels = (RGBTriple**)realloc(pixels, b_info.biHeight*sizeof(RGBTriple*));
    for(unsigned int i = old_y; i < b_info.biHeight; i++)
        pixels[i] = (RGBTriple*)malloc(b_info.biWidth*sizeof(RGBTriple));
    for(unsigned int i = 0; i < b_info.biHeight; i++)
        pixels[i] = (RGBTriple*)realloc(pixels[i], b_info.biWidth*sizeof(RGBTriple));
}

void BMP_ui::New(){
    for(unsigned int y = 0; y < b_info.biHeight; y++){
        for(unsigned int x = 0; x < b_info.biWidth; x++){
            pixels[y][x].rgbBlue = 255;
            pixels[y][x].rgbGreen = 255;
            pixels[y][x].rgbRed = 255;
        }
    }
}

void BMP_ui::Load(QString filename){
    unsigned int old_y = b_info.biHeight;
    //unsigned int old_w = b_info.biWidth;
    QByteArray temp = filename.toLatin1();
    char* fname = temp.data();
    FILE* file;
    if(!(file = fopen(fname, "r+b"))){
        QMessageBox::information(0, "Error", "File can't be opened, try another!");
        return;
    }

    fread(&b_header, sizeof(BMPheader), 1, file);
    fread(&b_info, sizeof(BITMAPinfo), 1, file);

    if(b_info.biBitCount != 24){
        QMessageBox::information(0, "Error", "Invalid bitcount");
        return;
    }

    int padding = 0;
    if((b_info.biWidth * 3) % 4 != 0){
        padding = 4 - (b_info.biWidth * 3) % 4;
    }

    b_info.biXPelsPerMeter = 3780;
    b_info.biYPelsPerMeter = 3780;

    //resizeRaster(b_info.biWidth, b_info.biHeight, old_w, old_h);
    pixels = (RGBTriple**)realloc(pixels, b_info.biHeight*sizeof(RGBTriple*));
    for(unsigned int i = old_y; i < b_info.biHeight; i++){
        pixels[i] = (RGBTriple*)malloc(b_info.biWidth*sizeof(RGBTriple));
    }
    for(unsigned int i = 0; i < b_info.biHeight; i++){
        pixels[i] = (RGBTriple*)realloc(pixels[i], b_info.biWidth*sizeof(RGBTriple));
    }

    for(int y = b_info.biHeight - 1; y >= 0; y--){
        for(unsigned int x = 0; x < b_info.biWidth; x++){
            fread(&pixels[y][x], sizeof(RGBTriple), 1, file);
        }
        if(padding != 0){
            char temp;
            fread(&temp, 1, padding, file);
        }
    }
    fclose(file);
}

void BMP_ui::Save(QString filename){

    QByteArray temp = filename.toLatin1();
    char* fname = temp.data();
    FILE* file = fopen(fname, "w+b");

    int padding = 0;
    if((b_info.biWidth * 3) % 4 != 0){
        padding = (b_info.biWidth * 3) % 4;
    }

    BMPheader save_header;
    memcpy(&save_header, &b_header, sizeof(b_header));
    BITMAPinfo save_info;
    memcpy(&save_info, &b_info, sizeof(b_info));

    save_info.biXPelsPerMeter = 3780;
    save_info.biYPelsPerMeter = 3780;

    fwrite(&save_header, sizeof(save_header), 1, file);
    fwrite(&save_info, sizeof(save_info), 1, file);

    for(int y = b_info.biHeight - 1; y >= 0; y--){
        for(unsigned int x = 0; x < b_info.biWidth; x++){
            fwrite(&pixels[y][x], sizeof(RGBTriple), 1, file);
            }
        if(padding != 0){
            char temp;
            fwrite(&temp, 1, padding, file);
        }
    }
    fclose(file);
}

void BMP_ui::DrawLine(int x1, int y1, int x2, int y2, QColor color, int w){
    int deltaY,  deltaX, sign;
    deltaY = y2 - y1;
    deltaX = x1 - x2;
    if (abs(deltaY) > abs(deltaX)) sign = 1;
    else sign = -1;
    int signy, signx;
    if (deltaY < 0) signy = -1;
    else signy = 1;
    if (deltaX < 0) signx = -1;
    else signx = 1;
    int f = 0;
    int x = x1, y = y1;
    if(sign == -1){
        while(x != x2 || y != y2){
            if(w < 1){
                pixels[y][x].rgbBlue = static_cast<char>(color.blue());
                pixels[y][x].rgbGreen = static_cast<char>(color.green());
                pixels[y][x].rgbRed = static_cast<char>(color.red());
            }else FillCircle(x, y, w, color);
            f += deltaY*signy;
            if (f > 0){
                f -= deltaX*signx;
                y += signy;
            }
            x -= signx;
        }
    }
    else{
        while(x != x2 || y != y2){
            if(w < 1){
                pixels[y][x].rgbBlue = static_cast<char>(color.blue());
                pixels[y][x].rgbGreen = static_cast<char>(color.green());
                pixels[y][x].rgbRed = static_cast<char>(color.red());
            }else FillCircle(x, y, w, color);
            f += deltaX*signx;
            if (f > 0) {
                f -= deltaY*signy;
                x -= signx;
            }
            y += signy;
        }
    }
}

void BMP_ui::FillCircle(int x1, int y1, int r, QColor color){
    int valuex1 = x1-r;
    int valuey1 = y1-r;
    if(valuex1 < 0) valuex1 = 0;
    if(valuey1 < 0) valuey1 = 0;
    int valuex2 = x1+r;
    int valuey2 = y1+r;
    if(valuex2 > static_cast<int>(b_info.biWidth)) valuex2 = b_info.biWidth;
    if(valuey2 > static_cast<int>(b_info.biHeight)) valuey2 = b_info.biHeight;

    for(int i  = valuey1; i < valuey2; i++){
        for(int j  = valuex1; j < valuex2; j++){
            int f = pow(x1-j,2)+pow(y1-i,2);
               if ( f<= pow(r,2)){
                pixels[i][j].rgbBlue = static_cast<char>(color.blue());
                pixels[i][j].rgbGreen = static_cast<char>(color.green());
                pixels[i][j].rgbRed = static_cast<char>(color.red());
            }
        }
    }
}

void BMP_ui::Circle(int x1, int y1, int r, QColor color){
    int valuey;
    int valuex;
    int x = 0;
    int y = r;
    int delta = 1 - 2 * r;
    int error = 0;
    while (y >= 0){
        valuex = x1-x;
        valuey = y1-y;
        if(valuex < 0) valuex = 0;
        if(valuey < 0) valuey = 0;
        pixels[valuey][valuex].rgbBlue = static_cast<char>(color.blue());
        pixels[valuey][valuex].rgbGreen = static_cast<char>(color.green());
        pixels[valuey][valuex].rgbRed = static_cast<char>(color.red());
        valuey = y1+y;
        if(valuey > static_cast<int>(b_info.biHeight - 1)) valuey = b_info.biHeight - 1;
        pixels[valuey][valuex].rgbBlue = static_cast<char>(color.blue());
        pixels[valuey][valuex].rgbGreen = static_cast<char>(color.green());
        pixels[valuey][valuex].rgbRed = static_cast<char>(color.red());
        valuex = x1+x;
        if(valuex > static_cast<int>(b_info.biWidth - 1)) valuex = b_info.biWidth - 1;
        pixels[valuey][valuex].rgbBlue = static_cast<char>(color.blue());
        pixels[valuey][valuex].rgbGreen = static_cast<char>(color.green());
        pixels[valuey][valuex].rgbRed = static_cast<char>(color.red());
        valuey = y1-y;
        if(valuey < 0) valuey = 0;
        pixels[valuey][valuex].rgbBlue = static_cast<char>(color.blue());
        pixels[valuey][valuex].rgbGreen = static_cast<char>(color.green());
        pixels[valuey][valuex].rgbRed = static_cast<char>(color.red());

        error = 2 * (delta + y) - 1;
        if ((delta < 0) && (error <= 0)){
            delta += 2 * ++x + 1;
            continue;
        }
        error = 2 * (delta - x) - 1;
        if ((delta > 0) && (error > 0)){
          delta += 1 - 2 * --y;
          continue;
        }
        x++;
        delta += 2 * (x - y);
        y--;
    }
}

void BMP_ui::Invert(int x1, int y1, int r){
    int valuex1 = x1-r;
    int valuey1 = y1-r;
    if(valuex1 < 0) valuex1 = 0;
    if(valuey1 < 0) valuey1 = 0;
    int valuex2 = x1+r;
    int valuey2 = y1+r;
    if(valuex2 > static_cast<int>(b_info.biWidth)) valuex2 = b_info.biWidth;
    if(valuey2 > static_cast<int>(b_info.biHeight)) valuey2 = b_info.biHeight;

    for(int i  = valuey1; i < valuey2; i++){
        for(int j  = valuex1; j < valuex2; j++){
            int f = pow(x1-j,2)+pow(y1-i,2);
               if ( f<= pow(r,2)){
                pixels[i][j].rgbBlue = 255 - pixels[i][j].rgbBlue;
                pixels[i][j].rgbGreen = 255 - pixels[i][j].rgbGreen;
                pixels[i][j].rgbRed = 255 - pixels[i][j].rgbRed;
            }
        }
    }
}

void BMP_ui::Crop(int x1, int y1, int x2, int y2){
    int t;
    int t2;
    if(x1 > x2 && y1 < y2){
        t = x1;
        x1 = x2;
        x2 = t;
    }
    if(x1 < x2 && y1 > y2){
        t = y1;
        y1 = y2;
        y2 = t;
    }
    if(x1 > x2 && y1 > y2){
        t2 = y1;
        y1 = y2;
        y2 = t2;
        t = x1;
        x1 = x2;
        x2 = t;
    }
}


