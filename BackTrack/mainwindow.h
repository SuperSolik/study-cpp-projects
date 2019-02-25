#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPen>
#include <fstream>
#include "graphwidget.h"
#include "sizedialog.h"
#include "nqueen.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSolveAll_triggered();

    void on_actionSolveTask_triggered();

    void on_actionplaceQueen_triggered();

    void on_actionsetBoardSize_triggered();

    void setBoardSize(int size);

    void placeQueen(int row, int col);

private:
    Ui::MainWindow *ui;
    GraphWidget* graphicsView;
    NQueen* problem;
    Board* board;
    bool board_rdy;
    bool queen_placed;
};

#endif // MAINWINDOW_H
