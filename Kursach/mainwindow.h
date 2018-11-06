#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include "help.h"
#include "about.h"
#include "huffman.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionHelp_triggered();

    void on_actionAbout_triggered();

    void on_InputButton_clicked();

    void on_OutputButton_clicked();

    void on_EncodeButton_clicked();

    void on_DecodeButton_clicked();

private:
    Ui::MainWindow *ui;
    QString infile_name = "";
    QString outfile_name = "output.txt";
    Coder coder;
};


#endif // MAINWINDOW_H
