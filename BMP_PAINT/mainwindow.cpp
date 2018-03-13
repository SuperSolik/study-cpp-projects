#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new paintScene();
    ui->graphicsView->setScene(scene);
    this->setCentralWidget(ui->graphicsView);
    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, &MainWindow::slotTimer);
    timer->start(100);
    setWindowTitle("TEST");
}

MainWindow::~MainWindow()
{

}

void MainWindow::slotTimer(){
    timer->stop();
    scene->setSceneRect(0, 0, ui->graphicsView->width()-10,  ui->graphicsView->height()-10);
}

void MainWindow::resizeEvent(QResizeEvent *event){
    timer->start(100);
    QWidget::resizeEvent(event);
}

void MainWindow::on_actionNew_triggered()
{
    scene->clear();
    filename = "new.bmp";
    img = QImage(scene->width(), scene->height(),QImage::Format_RGB32);
    QRgb value;
     for (int x=0; x<scene->width(); x++) {
      for (int y=0; y<scene->height(); y++) {
       value = qRgb(255, 255, 255);
       img.setPixel (x, y, value);
      }
     }
    img.save(filename);
    scene->addPixmap(filename);
}

void MainWindow::on_actionOpen_triggered()
{
    scene->clear();
    filename = QFileDialog::getOpenFileName( this,"Open");
    QPixmap image = bmp.Load(filename);
    scene->addPixmap(image);
}

void MainWindow::on_actionSave_triggered()
{
    img = QImage(scene->width(), scene->height(),QImage::Format_RGB32);
    QRgb value;
     for (int x=0; x<scene->width(); x++) {
      for (int y=0; y<scene->height(); y++) {
       value = qRgb(255, 255, 255);
       img.setPixel (x, y, value);
      }
     }
    QPainter painter(&img);
    scene->render(&painter);
    //img.save(filename);
    bmp.Save(filename, img);
}

void MainWindow::on_actionSave_As_triggered()
{
    filename = QFileDialog::getSaveFileName( this,"Save As" , "","*.bmp");
    on_actionSave_triggered();
}

void MainWindow::on_actionClear_triggered()
{
    scene->clear();
}

void MainWindow::on_actionColor_triggered()
{
   scene->color = QColorDialog::getColor(Qt::white, this, "Pick color");
}


void MainWindow::on_actionSize_triggered()
{
    scene->size_l = QInputDialog::getInt(this, "ChangeSize", "Enter size: ", 5, 1, 10);
}
