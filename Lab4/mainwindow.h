#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <sstream>
#include "help.h"
#include "about.h"
#include "cpp_tree.h"

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

    void on_ActionButton_clicked();

private:
    int max_tree_height(std::istream& input, std::ostream& output);

    Ui::MainWindow *ui;
    QString infile_name = "";
    QString outfile_name = "output.txt";
};

#endif // MAINWINDOW_H
