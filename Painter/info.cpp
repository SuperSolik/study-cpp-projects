#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);
    setWindowTitle("Info");
}

Info::~Info(){

}

void Info::setData(int bitcount, int width, int height, int size, int x_pix_per_m, int y_pix_per_m){
    QString file_info;
    file_info.append("------FILE INFO------\nBitcount: " + QString::number(bitcount) + "\n");
    file_info.append("Width: " + QString::number(width) + "\n");
    file_info.append("Height: " + QString::number(height) + "\n");
    file_info.append("Size: " + QString::number(size) + "\n");
    file_info.append("X pixels per meter: " + QString::number(x_pix_per_m) + "\n");
    file_info.append("Y pixels per meter: " + QString::number(y_pix_per_m) + "\n");
    ui->textBrowser_1->setText(file_info);
    QString help;
    help.append("------HOW TO USE------\n   To draw a line, click on the <Line> button,"
                " then draw a line like in Paint, except the line itself won't come out until you release the mouse key.\n"
                "   To invert the circle area, click on the <Invert> button, then put your mouse in some point of image, then"
                " drag mouse to make a circle radius, release key to stop, and programm will invert pixels in needed area.\n"
                "   To draw a circle, click on the <Circle> button, then put your mouse in some point of image, then"
                " drag mouse to make a circle radius, release key to stop, and programm will draw a nice circle with this radius.\n"
                "   To crop the image, click on the <Crop> button, then put your mouse in some point of image, then"
                " drag mouse to choose a rectangular area(first point will be 1 corner, and second will be the opposite corner),"
                " release key to stop, and programm will crop an image to this area.\n");
    ui->textBrowser_2->setText(help);
    QString a_info;
    a_info.append("------AUTHOR INFO------\n"
                  "Sergey Petrov, LETI university, 2018\n"
                  "serpetr99@gmail.com");
    ui->textBrowser_3->setText(a_info);
}
