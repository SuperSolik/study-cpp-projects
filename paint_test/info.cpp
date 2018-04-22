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

void Info::setData(char* bitcount, char* width, char* height, char* size){
    QString test;
    test.append("Bitcount: ");
    test.append(bitcount);
    test.append(" bytes per pixel");
    test.append("\n");
    test.append("Width: ");
    test.append(width);
    test.append(" pixels");
    test.append("\n");
    test.append("Height: ");
    test.append(height);
    test.append(" pixels");
    test.append("\n");
    test.append("Size: ");
    test.append(size);
    test.append(" bytes");
    test.append("\n");
    ui->textBrowser->setText(test);
}
