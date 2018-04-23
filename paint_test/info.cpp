#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);
    setWindowTitle("File info");
}

Info::~Info(){

}

void Info::setData(int bitcount, int width, int height, int size, int x_pix_per_m, int y_pix_per_m){
    QString test;
    test.append("Bitcount: " + QString::number(bitcount) + "\n");
    test.append("Width: " + QString::number(width) + "\n");
    test.append("Width: " + QString::number(height) + "\n");
    test.append("Size: " + QString::number(size) + "\n");
    test.append("X pixels per meter: " + QString::number(x_pix_per_m) + "\n");
    test.append("Y pixels per meter: " + QString::number(y_pix_per_m) + "\n");
    ui->textBrowser->setText(test);
}
