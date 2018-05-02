#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

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
    data = new Info();
    filename = "new.bmp";
    connect(scene, SIGNAL(mouse_pressed()), this, SLOT(Action()));
    start_dialog = new SizeDialog();
    connect(start_dialog, SIGNAL(start_size(int, int)), this, SLOT(set_start(int, int)));
    start_dialog->setWindowFlags(Qt::WindowStaysOnTopHint);
    start_dialog->show();
}

MainWindow::~MainWindow(){
    delete scene;
    delete data;
    delete start_dialog;
    delete timer;
    if(created) delete bmp;
}

void MainWindow::slotTimer(){
    timer->stop();
    scene->setSceneRect(0, 0, ui->graphicsView->width() - 10,  ui->graphicsView->height() - 10);
}

void MainWindow::resizeEvent(QResizeEvent *event){
    timer->start(100);
    QWidget::resizeEvent(event);
}

void MainWindow::set_start(int s_width, int s_height){
    if(s_width == 0 || s_height == 0){
        start_dialog->close();
        QMessageBox::information(0, "Error", "Wrong size (can't be 0), try again");
        qApp->exit(0);
    }
    int w = s_width;
    int h = s_height;
    bmp = new BMP_ui(w, h);
    MainWindow::resize(w + ui->groupBox->width() + 20, h + 60);
    created = true;
    bmp->New();
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
    drawRaster();
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

void MainWindow::on_actionNew_triggered(){
    scene->clear();
    filename = "new.bmp";
    bmp->New();
    drawRaster();
}

void MainWindow::on_actionOpen_triggered(){
    scene->clear();
    filename = QFileDialog::getOpenFileName(this, "Open");
    bmp->Load(filename);
    drawRaster();
    if(bmp->b_info.biHeight > 230){
        MainWindow::resize(bmp->b_info.biWidth + ui->groupBox->width() + 20, bmp->b_info.biHeight + 60);
    }
    else{
        MainWindow::resize(bmp->b_info.biWidth + ui->groupBox->width() + 20, 250);
    }
}

void MainWindow::on_actionSave_triggered(){
    bmp->Save(filename);
}

void MainWindow::on_actionSave_As_triggered(){
    filename = QFileDialog::getSaveFileName(this,"Save As" , "","*.bmp");
    on_actionSave_triggered();
}

void MainWindow::on_actionClear_triggered(){
    scene->clear();
    bmp->New();
    drawRaster();
}

void MainWindow::on_actionColor_triggered(){
   scene->color = QColorDialog::getColor(Qt::white, this, "Pick color");
}


void MainWindow::on_actionSize_triggered(){
    scene->size_l = QInputDialog::getInt(this, "ChangeSize", "Enter size: ", 0, 0, 10, 1);
}

void MainWindow::on_drawButton_clicked(){
    draw_flag = true;
    invert_flag = false;
    crop_flag = false;
    circle_flag = false;
}

void MainWindow::on_invButton_clicked(){
    draw_flag = false;
    invert_flag = true;
    crop_flag = false;
    circle_flag = false;
}

void MainWindow::on_cropButton_clicked(){
    draw_flag = false;
    invert_flag = false;
    crop_flag = true;
    circle_flag = false;
}

void MainWindow::on_circleButton_clicked(){
    draw_flag = false;
    invert_flag = false;
    crop_flag = false;
    circle_flag = true;
}

void MainWindow::on_actionShowData_triggered(){
    data->setData(bmp->b_info.biBitCount,
                      bmp->b_info.biWidth,
                      bmp->b_info.biHeight,
                      bmp->b_header.bfSize,
                      bmp->b_info.biXPelsPerMeter,
                      bmp->b_info.biYPelsPerMeter);
    data->show(); 
}
