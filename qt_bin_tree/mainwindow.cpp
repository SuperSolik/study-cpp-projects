#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Tree");
    setCentralWidget(ui->splitter_4);
    resize(500, 400);
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
    about->setWindowTitle("Help");
    about->resize(450, 350);
    about->show();
    about->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::on_InputButton_clicked()
{
    ui->textEdit->clear();
    infile_name = QFileDialog::getOpenFileName(this,"Choose input file" , "","*.txt *.dat");
    std::ifstream file;
    file.open(infile_name.toStdString(), std::ios::in);
    if (!file.is_open()){
        return;
    }
    std::string file_content;
    while(std::getline(file, file_content)){
        ui->textEdit->append(QString::fromStdString(file_content));
    }
    file.close();
}

void MainWindow::on_OutputButton_clicked()
{
    outfile_name = QFileDialog::getSaveFileName(this,"Choose output file" , "","*.txt;;*.dat");
}

void MainWindow::on_ActionButton_clicked()
{
    ui->textEdit_2->clear();
    std::ofstream output;
    output.open(outfile_name.toStdString(), std::ios::out);
    std::string text(ui->textEdit->toPlainText().toStdString());
    std::istringstream input_stream(text);
    std::string buffer;
    int result;
    while(std::getline(input_stream, buffer)){
        std::istringstream ss(buffer);
        result = max_tree_height(ss, output);
        ui->textEdit_2->append(QString::number(result));
        buffer.clear();
    }
    output.close();
}

int MainWindow::max_tree_height(std::istream& input, std::ostream& output){
    BinTree<int> tree;
    int val;
    while(input >> val)
        tree.insert(val);
    tree.print(output);
    return tree.get_max_height();
}
