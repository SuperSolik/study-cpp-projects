#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QWidget>
#include <QTimer>
#include <QResizeEvent>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QDesktopWidget>
#include <cmath>

#include "paintscene.h"
#include "info.h"
#include "bmp_ui.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QString filename;
    paintScene *scene;
    BMP_ui* bmp;
    Info* data = new Info();

    bool draw_flag = false;
    bool invert_flag = false;
    bool crop_flag = false;
    bool circle_flag = false;


private:
    void resizeEvent(QResizeEvent *event);
    void drawRaster();
    void resize(int x, int y);

private slots:
    void Action();
    void slotTimer();
    void on_actionOpen_triggered();
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionClear_triggered();
    void on_actionColor_triggered();
    void on_actionSize_triggered();
    void on_drawButton_clicked();
    void on_invButton_clicked();
    void on_cropButton_clicked();
    void on_circleButton_clicked();
    void on_actionShowData_triggered();
};

#endif // MAINWINDOW_H
