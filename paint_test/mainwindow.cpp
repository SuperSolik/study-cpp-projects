#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QFile styleF;

    styleF.setFileName(":/files/style.css");
    styleF.open(QFile::ReadOnly);
    QString qssStr = styleF.readAll();
    styleF.close();
    qApp->setStyleSheet(qssStr);

    scene = new paintScene();
    ui->graphicsView->setScene(scene);
    this->setCentralWidget(ui->splitter);
    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);
    setWindowTitle("BMP_DRAWING");
    bmp = new BMP_ui(640, 480);
    resize(640, 480);
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
    bmp->New();
    filename = "new.bmp";
    connect(scene, SIGNAL(mouse_pressed()), this, SLOT(Action()));
    drawRaster();
}

MainWindow::~MainWindow()
{
    delete data;
    delete bmp;
}

void MainWindow::slotTimer(){
    timer->stop();
    scene->setSceneRect(0, 0, ui->graphicsView->width() - 10,  ui->graphicsView->height() - 10);
    /*bmp->resizeRaster(ui->graphicsView->width(),
                      ui->graphicsView->height(),
                      bmp->b_info.biWidth,
                      bmp->b_info.biHeight);*/
    //drawRaster();
}

void MainWindow::resizeEvent(QResizeEvent *event){
    timer->start(100);
    QWidget::resizeEvent(event);
}

void MainWindow::resize(int x, int y){
    ui->graphicsView->resize(x + 10, y + 10);
    scene->setSceneRect(0, 0, x, y);
    if(y > 230){
        ui->splitter->resize(ui->graphicsView->width() + ui->groupBox->width(), ui->graphicsView->height());
    }
    else{
        ui->splitter->resize(ui->graphicsView->width() + ui->groupBox->width(), 230);
    }
    MainWindow::setGeometry(MainWindow::x(), MainWindow::y(), ui->splitter->width() + 5, ui->splitter->height()+45);
}

void MainWindow::drawRaster(){
    QPixmap image(bmp->b_info.biWidth, bmp->b_info.biHeight);
    QPainter painter(&image);
    for(int y = 0; y < bmp->b_info.biHeight; y++){
        for(int x = 0; x < bmp->b_info.biWidth; x++){
            painter.setPen(QColor(bmp->pixels[y][x].rgbRed, bmp->pixels[y][x].rgbGreen, bmp->pixels[y][x].rgbBlue));
            painter.drawPoint(x, y);
        }
    }
    scene->addPixmap(image);
}
void MainWindow::Action(){
    if(draw_flag){
        bmp->DrawLine(scene->beginPoint.x(),
                      scene->beginPoint.y(),
                      scene->endPoint.x(),
                      scene->endPoint.y(),
                      scene->color,
                      scene->size_l);
        drawRaster();
    }
    if(invert_flag){
        bmp->Invert((int)scene->beginPoint.x(), (int)scene->beginPoint.y(), (int)sqrt(pow(scene->endPoint.x() - scene->beginPoint.x(),2)
                                                                       +pow(scene->endPoint.y() - scene->beginPoint.y(), 2)));
        drawRaster();
    }
    if(crop_flag){
        bmp->Crop((int)scene->beginPoint.x(), (int)scene->beginPoint.y(), (int)scene->endPoint.x(), (int)scene->endPoint.y());
        scene->clear();
        drawRaster();
    }

    if(circle_flag){
        bmp->Circle((int)scene->beginPoint.x(), (int)scene->beginPoint.y(), (int)sqrt(pow(scene->endPoint.x() - scene->beginPoint.x(),2)
                                                                       +pow(scene->endPoint.y() - scene->beginPoint.y(), 2)), scene->color);
        drawRaster();
    }
}
void MainWindow::on_actionNew_triggered()
{
    scene->clear();
    filename = "new.bmp";
    bmp->New();
    drawRaster();
}

void MainWindow::on_actionOpen_triggered()
{
    scene->clear();
    filename = QFileDialog::getOpenFileName(this, "Open");
    bmp->Load(filename);
    drawRaster();
    MainWindow::resize(bmp->b_info.biWidth, bmp->b_info.biHeight);
}

void MainWindow::on_actionSave_triggered()
{
    bmp->Save(filename);
}

void MainWindow::on_actionSave_As_triggered()
{
    filename = QFileDialog::getSaveFileName(this,"Save As" , "","*.bmp");
    on_actionSave_triggered();
}

void MainWindow::on_actionClear_triggered()
{
    scene->clear();
    bmp->New();
    drawRaster();
}

void MainWindow::on_actionColor_triggered()
{
   scene->color = QColorDialog::getColor(Qt::white, this, "Pick color");
}


void MainWindow::on_actionSize_triggered()
{
    scene->size_l = QInputDialog::getInt(this, "ChangeSize", "Enter size: ", 0, 0, 10, 1);
}

void MainWindow::on_drawButton_clicked()
{
    draw_flag = true;
    invert_flag = false;
    crop_flag = false;
    circle_flag = false;
}

void MainWindow::on_invButton_clicked()
{
    draw_flag = false;
    invert_flag = true;
    crop_flag = false;
    circle_flag = false;
}

void MainWindow::on_cropButton_clicked()
{
    draw_flag = false;
    invert_flag = false;
    crop_flag = true;
    circle_flag = false;
}

void MainWindow::on_circleButton_clicked()
{
    draw_flag = false;
    invert_flag = false;
    crop_flag = false;
    circle_flag = true;
}

void MainWindow::on_actionShowData_triggered()
{
    data->setData(bmp->b_info.biBitCount,
                  bmp->b_info.biWidth,
                  bmp->b_info.biHeight,
                  bmp->b_header.bfSize,
                  bmp->b_info.biXPelsPerMeter,
                  bmp->b_info.biYPelsPerMeter);
    data->show();
}
