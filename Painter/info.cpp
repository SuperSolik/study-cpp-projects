#include "info.h"
#include "ui_info.h"

Info::Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Info)
{
    ui->setupUi(this);
    setWindowTitle("Info");
    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &Info::slotTimer);
    timer->start(100);
}

Info::~Info(){
    delete ui;
    delete timer;
}

void Info::slotTimer(){
    timer->stop();
    ui->tabWidget->resize(Info::width(), Info::height());
    ui->textBrowser_1->resize(Info::width() - 25, Info::height() - 45);
    ui->textBrowser_2->resize(Info::width() - 25, Info::height() - 45);
    ui->textBrowser_3->resize(Info::width() - 25, Info::height() - 45);
}

void Info::resizeEvent(QResizeEvent *event){
    timer->start(100);
    QWidget::resizeEvent(event);
}

void Info::setData(QString filename, int bitcount, int width, int height, int size, int x_pix_per_m, int y_pix_per_m){
    QString file_info;
    file_info.append("------FILE INFO------\nFile: " + filename + "\nBitcount: " + QString::number(bitcount) + "\n");
    file_info.append("Width: " + QString::number(width) + "\n");
    file_info.append("Height: " + QString::number(height) + "\n");
    file_info.append("Size: " + QString::number(size) + "\n");
    file_info.append("X pixels per meter: " + QString::number(x_pix_per_m) + "\n");
    file_info.append("Y pixels per meter: " + QString::number(y_pix_per_m) + "\n");
    ui->textBrowser_1->setText(file_info);
    QString help;
    help.append("------HOW TO USE------\n"
                "   To create a new file, click on the <New> button on the toolbar or select 'File'->'New' or press Ctrl+N\n"
                "   To open file, select 'File'->'Open' or press Ctrl+O\n"
                "   To save file with current filename, select 'File'->'Save' or press Ctrl+S.\n"
                "   To save file with needed filename, select 'File'->'Save As' or press Ctrl+F.\n"
                "   To clear image with needed filename, click on the <Clear> button.\n"
                "   To pick a needed line's color click on the <Color> button.\n"
                "   To pick a needed line's width click on the <Size> button.\n"
                "   To get some file info and help select 'Info'->'ShowData' or press Ctrl+I.\n"
                "   To draw a line, click on the <Draw> button,"
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
