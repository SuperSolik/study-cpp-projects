#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <vector>
#include <fstream>
#include <sstream>
#include "help.h"
#include "h_list.h"

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
    void on_OutputButton_clicked();

    void on_InputButton_clicked();

    void on_ActionButton_clicked();

    void on_actionHelp_triggered();

private:
    Ui::MainWindow *ui;
    QString infile_name = "";
    QString outfile_name = "output.txt";
    std::vector<h_list::lisp> h_lists;
};

#endif // MAINWINDOW_H
