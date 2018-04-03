#include "bmp_ui.h"
#include "QDebug"
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

void BMP_ui::resizeRaster(int new_x, int new_y){

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
    unsigned int old_h = b_info.biHeight;
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

    pixels = (RGBTriple**)realloc(pixels, b_info.biHeight*sizeof(RGBTriple*));
    for(unsigned int i = old_h; i < b_info.biHeight; i++)
        pixels[i] = (RGBTriple*)malloc(b_info.biWidth*sizeof(RGBTriple));
    for(unsigned int i = 0; i < b_info.biHeight; i++)
        pixels[i] = (RGBTriple*)realloc(pixels[i], b_info.biWidth*sizeof(RGBTriple));

    for(int y = b_info.biHeight - 1; y >= 0; y--){
        for(unsigned int x = 0; x < b_info.biWidth; x++){
            fread(&pixels[y][x], sizeof(RGBTriple), 1, file);
        }
        if(padding != 0){
            char temp = 0;
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
            }else Circle(x, y, w, color);
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
            }else Circle(x, y, w, color);
            f += deltaX*signx;
            if (f > 0) {
                f -= deltaY*signy;
                x -= signx;
            }
            y += signy;
        }
    }
}

void BMP_ui::Circle(int x1, int y1, int r, QColor color){
    int x = 0;
    int y = r;
    int delta = 1 - 2 * r;
    int error = 0;
    while (y >= 0){
        //--------------------1 quarter
        pixels[y1+y][x1+x].rgbBlue = static_cast<char>(color.blue());
        pixels[y1+y][x1+x].rgbGreen = static_cast<char>(color.green());
        pixels[y1+y][x1+x].rgbRed = static_cast<char>(color.red());
        //------------------------------ 2
        pixels[y1+y][x1-x].rgbBlue = static_cast<char>(color.blue());
        pixels[y1+y][x1-x].rgbGreen = static_cast<char>(color.green());
        pixels[y1+y][x1-x].rgbRed = static_cast<char>(color.red());
        for(int j = x1 - x; j < x1 + x; j++){
            pixels[y1+y][j].rgbBlue = static_cast<char>(color.blue());
            pixels[y1+y][j].rgbGreen = static_cast<char>(color.green());
            pixels[y1+y][j].rgbRed = static_cast<char>(color.red());
        }
        //------------------------------ 3
        pixels[y1-y][x1-x].rgbBlue = static_cast<char>(color.blue());
        pixels[y1-y][x1-x].rgbGreen = static_cast<char>(color.green());
        pixels[y1-y][x1-x].rgbRed = static_cast<char>(color.red());
        //------------------------------ 4
        pixels[y1-y][x1+x].rgbBlue = static_cast<char>(color.blue());
        pixels[y1-y][x1+x].rgbGreen = static_cast<char>(color.green());
        pixels[y1-y][x1+x].rgbRed = static_cast<char>(color.red());
        for(int j = x1 - x; j < x1 + x; j++){
            pixels[y1-y][j].rgbBlue = static_cast<char>(color.blue());
            pixels[y1-y][j].rgbGreen = static_cast<char>(color.green());
            pixels[y1-y][j].rgbRed = static_cast<char>(color.red());
        }
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
    //fix that shit
    for(int i = y1-r; i < y1+r; i++){
        for(int j = x1-r; j < x1+r; j++){
            if((j > x1 - r && j < x1+r) && (y1 - sqrt(-x1*x1 + 2*x1*j - j*j + r*r) < i && i < y1 + sqrt(-x1*x1 + 2*x1*j - j*j + r*r))){
                pixels[i][j].rgbBlue = 255 - pixels[i][j].rgbBlue;
                pixels[i][j].rgbGreen = 255 - pixels[i][j].rgbGreen;
                pixels[i][j].rgbRed = 255 - pixels[i][j].rgbRed;
            }
        }
    }
    //add check edges
    /*for(unsigned int y = 0; y < b_info.biHeight; y++){
        for(unsigned int x = 0; x < b_info.biWidth; x++){
            pixels[y][x].rgbBlue = 255 - pixels[y][x].rgbBlue;
            pixels[y][x].rgbGreen = 255 - pixels[y][x].rgbGreen;
            pixels[y][x].rgbRed = 255 - pixels[y][x].rgbRed;
        }
    }
    int x = 0;
    int y = r;
    int delta = 1 - 2 * r;
    int error = 0;
    while (y >= 0){
        for(int j = x1 - x; j < x1 + x; j++){
            pixels[y1+y][j].rgbBlue = 255 - pixels[y1+y][j].rgbBlue;
            pixels[y1+y][j].rgbGreen = 255 - pixels[y1+y][j].rgbGreen;
            pixels[y1+y][j].rgbRed = 255 - pixels[y1+y][j].rgbRed;
        }
        for(int j = x1 - x; j < x1 + x; j++){
            pixels[y1-y][j].rgbBlue = 255 - pixels[y1-y][j].rgbBlue;
            pixels[y1-y][j].rgbGreen = 255 - pixels[y1-y][j].rgbGreen;
            pixels[y1-y][j].rgbRed = 255 - pixels[y1-y][j].rgbRed;
        }
        for(unsigned int i = 0; i < b_info.biHeight; i++){
        for(unsigned int j = 0; j < b_info.biWidth; j++){
            if((i > y1 - y && i < y1 + y) && (j > x1 - x && j < x1 + x)){
                pixels[i][j].rgbBlue = 255 - pixels[i][j].rgbBlue;
                pixels[i][j].rgbGreen = 255 - pixels[i][j].rgbGreen;
                pixels[i][j].rgbRed = 255 - pixels[i][j].rgbRed;
            }
        }
        }
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
    }*/
}


