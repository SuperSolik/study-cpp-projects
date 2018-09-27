#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
using namespace h_list;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Reverse Hlist");
    setCentralWidget(ui->splitter_4);
    resize(500, 400);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_OutputButton_clicked()
{
    outfile_name = QFileDialog::getSaveFileName(this,"Choose output file" , "","*.txt;;*.dat");
}

void MainWindow::on_InputButton_clicked()
{
    infile_name = QFileDialog::getOpenFileName(this,"Choose input file" , "","*.txt *.dat");
    std::ifstream file;
    file.open(infile_name.toStdString(), std::ios::in);
    if (!file.is_open()){
        return;
    }
    std::string str;
    while(std::getline(file, str)){
        ui->textEdit->append(QString::fromStdString(str));
    }
    file.close();
}

void MainWindow::on_ActionButton_clicked()
{
    ui->textEdit_2->clear();
    std::ofstream output;
    output.open(outfile_name.toStdString(), std::ios::out);
    QString text_edit_content(ui->textEdit->toPlainText());
    std::string text(text_edit_content.toStdString());
    std::istringstream ss(text);
    std::string temp;
    while(std::getline(ss, temp)){
        std::string res;
        std::istringstream is(temp);
        std::ostringstream os(res);
        lisp t;
        read_lisp(t, is);
        write_lisp(reverse(t), os);
        ui->textEdit_2->append(QString::fromStdString(os.str()));
        temp.clear();
    }
    output.close();
}

void MainWindow::on_actionHelp_triggered()
{
    Help* help = new Help(":/doc/", "source.html");
    help->setWindowTitle("Help");
    help->setAttribute(Qt::WA_DeleteOnClose);
    help->resize(450, 350);
    help->show();
}

void MainWindow::on_actionAbout_triggered()
{
    Help* help = new Help(":/doc/", "author.html");
    help->setWindowTitle("Author Info");
    help->setAttribute(Qt::WA_DeleteOnClose);
    help->resize(450, 350);
    help->show();
}
