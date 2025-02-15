#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QResizeEvent>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QDesktopWidget>

#include "paintscene.h"
#include "info.h"
#include "bmp_ui.h"
#include "sizedialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow{
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
    Info* data;
    SizeDialog* start_dialog;    
    QColor color = "black";
    int size_l = 1;
    bool draw_flag = false;
    bool invert_flag = false;
    bool crop_flag = false;
    bool created_bmp = false;
    bool created_new = false;
    bool error = false;

private:
    void resizeEvent(QResizeEvent *event);
    void drawRaster();

private slots:
    void Set_Size(int s_width, int s_height);
    void Action(QPointF start, QPointF end);
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
    void on_actionShowData_triggered();
};

#endif // MAINWINDOW_H
