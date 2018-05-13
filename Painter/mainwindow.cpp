#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
    ui->setupUi(this);
    setWindowTitle("BMP_DRAWING");
    scene = new paintScene();
    ui->graphicsView->setScene(scene);
    connect(scene, SIGNAL(mouse_pressed(QPointF, QPointF)), this, SLOT(Action(QPointF, QPointF)));
    this->setCentralWidget(ui->splitter);
    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);
    data = new Info();
    start_dialog = new SizeDialog();
    connect(start_dialog, SIGNAL(get_size(int, int)), this, SLOT(Set_Size(int, int)));
    start_dialog->setWindowFlags(Qt::WindowStaysOnTopHint);
    start_dialog->show();
    filename = "new.bmp";
}

MainWindow::~MainWindow(){
    delete ui;
    delete scene;
    delete data;
    delete start_dialog;
    delete timer;
    if(created_bmp) delete bmp;
}

void MainWindow::slotTimer(){
    timer->stop();
    if(bmp->b_info.biWidth >= ui->graphicsView->width() &&
       bmp->b_info.biHeight >= ui->graphicsView->height()){
        scene->setSceneRect(0, 0, bmp->b_info.biWidth - 10,  bmp->b_info.biHeight - 10);
    }
    if(bmp->b_info.biWidth >= ui->graphicsView->width() &&
       bmp->b_info.biHeight < ui->graphicsView->height()){
        scene->setSceneRect(0, 0, bmp->b_info.biWidth - 10,  ui->graphicsView->height() - 10);
    }
    if(bmp->b_info.biWidth < ui->graphicsView->width() &&
       bmp->b_info.biHeight >= ui->graphicsView->height()){
        scene->setSceneRect(0, 0, ui->graphicsView->width() - 10,  bmp->b_info.biHeight - 10);
    }
    if(bmp->b_info.biWidth < ui->graphicsView->width() &&
       bmp->b_info.biHeight < ui->graphicsView->height()){
        scene->setSceneRect(0, 0, ui->graphicsView->width() - 10,  ui->graphicsView->height() - 10);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event){
    timer->start(100);
    QWidget::resizeEvent(event);
}

void MainWindow::Set_Size(int s_width, int s_height){
    if(created_bmp){
        delete bmp;
        created_new = true;
    }
    scene->clear();
    bmp = new BMP_ui(s_width, s_height);
    MainWindow::resize(s_width + ui->groupBox->width() + 20, s_height + 60);
    created_bmp = true;
    bmp->Clear();
    drawRaster();
    data->setData(filename,
                  bmp->b_info.biBitCount,
                  bmp->b_info.biWidth,
                  bmp->b_info.biHeight,
                  bmp->b_header.bfSize,
                  bmp->b_info.biXPelsPerMeter,
                  bmp->b_info.biYPelsPerMeter);
}

void MainWindow::Action(QPointF start, QPointF end){
    if(draw_flag){
        bmp->DrawLine(start.x(),
                      start.y(),
                      end.x(),
                      end.y(),
                      color,
                      size_l);
        drawRaster();
    }
    if(invert_flag){
        bmp->Invert(start.x(),
                    start.y(),
                    sqrt(pow(end.x() - start.x(),2) +pow(end.y() - start.y(), 2)));
        drawRaster();
    }
    if(crop_flag){
        bmp->Crop(start.x(), start.y(), end.x(), end.y());
        scene->clear();
        drawRaster();
    }
}

void MainWindow::drawRaster(){
    scene->addPixmap(QPixmap::fromImage(bmp->DrawImage()));
}

void MainWindow::on_actionNew_triggered(){
    start_dialog->show();
    if(created_new) filename = "new.bmp";
    created_new = false;
}

void MainWindow::on_actionOpen_triggered(){
    if(!created_bmp){
        QMessageBox::information(0, "Error", "Image hasn't been created yet");
        return;
    }
    scene->clear();
    filename = QFileDialog::getOpenFileName(this, "Open");
    bmp->Load(filename);
    drawRaster();
    data->setData(filename,
                  bmp->b_info.biBitCount,
                  bmp->b_info.biWidth,
                  bmp->b_info.biHeight,
                  bmp->b_header.bfSize,
                  bmp->b_info.biXPelsPerMeter,
                  bmp->b_info.biYPelsPerMeter);
}

void MainWindow::on_actionSave_triggered(){
    if(!created_bmp){
        QMessageBox::information(0, "Error", "Image hasn't been created yet");
        return;
    }	
    bmp->Save(filename);
}

void MainWindow::on_actionSave_As_triggered(){
    if(!created_bmp){
        QMessageBox::information(0, "Error", "Image hasn't been created yet");
        return;
    }
    filename = QFileDialog::getSaveFileName(this,"Save As" , "","*.bmp");
    bmp->Save(filename);
}

void MainWindow::on_actionClear_triggered(){
    scene->clear();
    bmp->Clear();
    drawRaster();
}

void MainWindow::on_actionColor_triggered(){
    color = QColorDialog::getColor(Qt::white, this, "Pick color");
}

void MainWindow::on_actionSize_triggered(){
    size_l = QInputDialog::getInt(this, "ChangeSize", "Enter size: ", 0, 0, 10, 1);
}

void MainWindow::on_drawButton_clicked(){
    draw_flag = true;
    invert_flag = false;
    crop_flag = false;
}

void MainWindow::on_invButton_clicked(){
    draw_flag = false;
    invert_flag = true;
    crop_flag = false;
}

void MainWindow::on_cropButton_clicked(){
    draw_flag = false;
    invert_flag = false;
    crop_flag = true;
}

void MainWindow::on_actionShowData_triggered(){
    if(!created_bmp){
        QMessageBox::information(0, "Error", "Image hasn't been created/opened yet!");
        return;
    }
    data->setData(filename,
                  bmp->b_info.biBitCount,
                  bmp->b_info.biWidth,
                  bmp->b_info.biHeight,
                  bmp->b_header.bfSize,
                  bmp->b_info.biXPelsPerMeter,
                  bmp->b_info.biYPelsPerMeter);
    data->show(); 
}
