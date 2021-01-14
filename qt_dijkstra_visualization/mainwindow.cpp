#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1024, 768);
    setCentralWidget(ui->splitter);
    setWindowTitle("Lab 2");
    ui->checkBox->setChecked(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_triggered()
{
    QList<std::pair<int, QList<std::pair<int, int>>>> g_list;
    QString filename = QFileDialog::getOpenFileName(this, "Open");
    std::ifstream file;
    file.open(filename.toStdString(), std::ios::in);
    if(!file.good())
        return;
    std::string buf;
    int start_vertex, vertex, w;
    QList<std::pair<int, int>> temp;
    while(std::getline(file, buf)){
        std::stringstream ss(buf);
        ss >> start_vertex;
        while(ss >> vertex >> w){
            temp.push_back(std::make_pair(vertex-1, w));
        }
        if(!temp.empty())
            g_list.push_back(std::make_pair(start_vertex-1, temp));
        else{
            temp.push_back(std::make_pair(-1, -1));
            g_list.push_back(std::make_pair(start_vertex-1, temp));
        }
        temp.clear();
    }
    file.close();
    ui->graphview->setGraph(g_list);
}



void MainWindow::on_actionPrint_GraphList_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Choose output file", "","*.txt;;*.dat");
    QFile fileout(filename);
    if(!fileout.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    auto g_list = ui->graphview->getGraphList();
    QTextStream writeStream(&fileout);
    for(auto& el : g_list){
        writeStream << el.first << " : ";
        if(el.second[0].first != -1){
            for(auto& v : el.second){
                writeStream << "(" << el.first << "-" << v.first << ", weight: " << v.second << ") ";
            }
        }
        writeStream << "\n";
    }
    fileout.close();
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

void MainWindow::on_checkBox_stateChanged(int arg1){
    if(ui->checkBox->isChecked()){
        ui->graphview->changeGravity(true);
    } else {
        ui->graphview->changeGravity(false);
    }
}


void MainWindow::on_pushButton_clicked()
{
    int start = ui->startEdit->text().toInt() - 1;
    int goal = ui->goalEdit->text().toInt() - 1;
    int size = ui->graphview->getGraphList().size();
    if(start < 0 || start >= size || goal < 0 || goal >= size){
        QMessageBox::information(nullptr, "Error", "Range error");
        return;
    }
    auto path = ui->graphview->Djikstra(start, goal);
    auto str_path = ui->graphview->make_path(start, goal, path);
    ui->pathEdit->setText(QString::fromStdString(str_path));
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->graphview->refresh();
}
