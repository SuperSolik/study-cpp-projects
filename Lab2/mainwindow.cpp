#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace h_list;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Lab2");
    setCentralWidget(ui->splitter_4);
    resize(500, 400);
}

MainWindow::~MainWindow()
{
    if (!h_lists.empty()){
        for(uint16_t i = 0; i < h_lists.size(); i++){
            destroy(h_lists[i]);
        }
        h_lists.clear();
    }
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
    if (!h_lists.empty()){
        for(uint16_t i = 0; i < h_lists.size(); i++){
            destroy(h_lists[i]);
        }
        h_lists.clear();
    }
    std::string str;
    std::string input;
    while(std::getline(file, str)){
        std::istringstream ss(str);
        lisp temp;
        read_lisp(temp, ss);
        h_lists.push_back(temp);
        input += str;
        if (!file.eof()) input += '\n';
        ui->textBrowser->setText(QString::fromStdString(input));
    }
    file.close();
}

void MainWindow::on_ActionButton_clicked()
{
    std::ofstream output;
    output.open(outfile_name.toStdString(), std::ios::out);
    std::string result_text;
    for(auto& c : h_lists){
        std::string res;
        std::ostringstream ss(res);
        lisp t = reverse(c);
        write_lisp(t, ss);
        result_text += ss.str();
        output << ss.str();
        if(c != h_lists.at(h_lists.size()-1)){
            output<<'\n';
            result_text += '\n';
        }
        ui->textBrowser_2->setText(QString::fromStdString(result_text));
    }
    output.close();
}

void MainWindow::on_actionHelp_triggered()
{
    Help* help = new Help(":/doc/", "source.html");
    help->setAttribute(Qt::WA_DeleteOnClose);
    help->resize(450, 350);
    help->show();
}
