#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("SyntaxAnalizer");
    this->setCentralWidget(ui->splitter_4);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionHelp_triggered()
{
    Help* help = new Help(":/files", "source.html");
    help->resize(550, 300);
    help->show();
}

void MainWindow::on_pushButton_clicked()
{
    infile_name = QFileDialog::getOpenFileName(this,"Choose input file" , "","*.txt *.dat");
    std::ifstream file;
    file.open(infile_name.toStdString(), std::ios::in);
    if (!file.is_open()){
        return;
    }
    std::string str;
    std::string input;
    while(std::getline(file, str)){
        input += str;
        if (!file.eof()) input += '\n';
        ui->textBrowser->setText(QString::fromStdString(input));
    }
    file.close();
}

void MainWindow::on_pushButton_2_clicked()
{
    outfile_name = QFileDialog::getSaveFileName(this,"Choose output file" , "output", "*.txt;;*.dat");
}

void MainWindow::on_pushButton_3_clicked()
{
    qDebug() << infile_name;
    std::ifstream input;
    std::ofstream output;
    input.open(infile_name.toStdString(), std::ios::in);
    if (!input.is_open()){
        QMessageBox::information(0, "Error", "No file specified for input");
        return;
    }
    output.open(outfile_name.toStdString(), std::ios::out);
    output << std::boolalpha;
    std::string str;
    std::string result;
    bool res;
    while(std::getline(input, str)){
        res = false;
        if (!str.empty()){
            std::istringstream ss(str);
            res = check(ss);
        }
        output << res;
        result += res ? "True":"False";
        if (!input.eof()){
            output << "\n";
            result += '\n';
        }
        ui->textBrowser_2->setText(QString::fromStdString(result));
    }
    input.close();
    output.close();
}

bool MainWindow::is_op(char c){
    if (c == '+' || c == '-' || c == '*') return true;
    else return false;
}

bool MainWindow::simple_expr(std::istringstream& input){
    char c;
    input >> c;
    bool res;
    if (c != '('){
        return isalpha(c);
    } else {
        res = simple_expr(input);
        if (res){
            input >> c;
            res = is_op(c);
            if (res){
                res = simple_expr(input);
                input >> c;
                if (c != ')') res = false;
            }
        }
    }
    return res;
}

bool MainWindow::check(std::istringstream& input){
    char c;
    input >> c;
    input.peek();
    if (isalpha(c)){
        if (input.eof()) return true;
        else return false;
    }
    input.seekg(0, input.beg);
    return simple_expr(input);
}
