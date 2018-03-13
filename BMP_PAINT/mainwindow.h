#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QTimer>
#include <QResizeEvent>
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>

#include "paintscene.h"
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
    QImage img;
    QString filename;
    paintScene *scene;
    BMP_ui bmp;


private:
    void resizeEvent(QResizeEvent *event);

private slots:
    void slotTimer();
    void on_actionOpen_triggered();
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionClear_triggered();
    void on_actionColor_triggered();
    void on_actionSize_triggered();
};

#endif // MAINWINDOW_H
