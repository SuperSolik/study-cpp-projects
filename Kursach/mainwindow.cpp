#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->splitter_5);
    setWindowTitle("Characters encoding");
    resize(1200, 600);
    ui->textEdit->setReadOnly(true);
    ui->textEdit_2->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionHelp_triggered()
{
    Help* help = new Help(":/doc/", "source.html");
    help->setWindowTitle("Help");
    help->resize(450, 350);
    help->show();
    help->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_actionAbout_triggered()
{
    About* about = new About(":/doc/", "author.html");
    about->setWindowTitle("About");
    about->resize(450, 350);
    about->show();
    about->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_InputButton_clicked()
{
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    infile_name = QFileDialog::getOpenFileName(this,"Choose input file" , "","*.txt *.dat *.cpp *.h");
    std::ifstream file;
    file.open(infile_name.toStdString(), std::ios::in);
    if (!file.is_open()){
        return;
    }
    std::stringstream ss;
    ss << file.rdbuf();
    ui->textEdit->setText(QString::fromStdString(ss.str()));
    file.close();
}

void MainWindow::on_OutputButton_clicked()
{
    outfile_name = QFileDialog::getSaveFileName(this,"Choose output file" , "","*.txt;;*.dat");
}

void MainWindow::on_EncodeButton_clicked()
{
    ui->textEdit_2->clear();
    if(ui->radioButton->isChecked()){
        coder.huffman_encode(infile_name.toStdString());
    } else if(ui->radioButton_2->isChecked()){
        coder.shannon_encode(infile_name.toStdString());
    } else {
        QMessageBox::information(nullptr, "Error", "You haven't chosen encode method");
        return;
    }
    auto map = coder.get_map();
    for(auto& el : map){
        if(el.first == '\t'){
            ui->textEdit_2->append(QString::fromStdString("tab : " + el.second));
        } else if(el.first == '\r'){
            ui->textEdit_2->append(QString::fromStdString("enter : " + el.second));
        } else {
            ui->textEdit_2->append(QString::fromStdString(std::string(1, el.first) + " : " + el.second));
        }
    }
    coder.encode_file(outfile_name.toStdString());
}

void MainWindow::on_DecodeButton_clicked()
{
    std::string content = coder.decode_file(infile_name.toStdString());
    if(!content.empty()){
        ui->textEdit_2->setText(QString::fromStdString(content));
    } else
        QMessageBox::information(nullptr, "Error", "No data to encode");
}
