#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    on_actionNew_triggered();
    connect(scene, SIGNAL(mouse_pressed()), this, SLOT(Action()));
}

MainWindow::~MainWindow()
{
    delete bmp;
}

void MainWindow::slotTimer(){
    timer->stop();
    scene->setSceneRect(0, 0, ui->graphicsView->width() - 10,  ui->graphicsView->height() - 10);
}

void MainWindow::resizeEvent(QResizeEvent *event){
    timer->start(100);
    QWidget::resizeEvent(event);
}

void MainWindow::resize(int x, int y){
    scene->setSceneRect(0, 0, x, y);
    ui->graphicsView->resize(scene->width() + 10, scene->height() + 10);
    ui->splitter->resize(ui->graphicsView->width() + ui->groupBox->width(), ui->graphicsView->height());
    MainWindow::setGeometry(MainWindow::x(), MainWindow::y(), ui->splitter->width(), ui->splitter->height()+42);
}

void MainWindow::drawRaster(){
    QPixmap image(bmp->b_info.biWidth, bmp->b_info.biHeight);
    QPainter painter(&image);
    for(unsigned int y = 0; y < bmp->b_info.biHeight; y++){
        for(unsigned int x = 0; x < bmp->b_info.biWidth; x++){
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
    filename = QFileDialog::getSaveFileName( this,"Save As" , "","*.bmp");
    on_actionSave_triggered();
}

void MainWindow::on_actionClear_triggered()
{
    scene->clear();
    bmp->New();
}

void MainWindow::on_actionColor_triggered()
{
   scene->color = QColorDialog::getColor(Qt::white, this, "Pick color");
}


void MainWindow::on_actionSize_triggered()
{
    scene->size_l = QInputDialog::getInt(this, "ChangeSize", "Enter size: ", 0, 0, 10, 1);
}

void MainWindow::on_pushButton_clicked()
{
    /*bmp->DrawLine(scene->beginPoint.x(), scene->beginPoint.y(), scene->endPoint.x(), scene->endPoint.y(), scene->color, scene->size_l);
    drawRaster();*/
    draw_flag = true;
    invert_flag = false;
    crop_flag = false;
}

void MainWindow::on_pushButton_2_clicked()
{
    /*bmp->Invert((int)scene->beginPoint.x(), (int)scene->beginPoint.y(), (int)sqrt(pow(scene->endPoint.x() - scene->beginPoint.x(),2)
                                                                   +pow(scene->endPoint.y() - scene->beginPoint.y(), 2)));
    drawRaster();*/
    draw_flag = false;
    invert_flag = true;
    crop_flag = false;
}

void MainWindow::on_pushButton_3_clicked()
{
    draw_flag = false;
    invert_flag = false;
    crop_flag = true;
}
