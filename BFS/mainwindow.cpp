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
    QList<QStringList> g_list;
    QString filename = QFileDialog::getOpenFileName(this, "Open");
    QFile filein(filename);
    if(!filein.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    while(!filein.atEnd()){
        QString line = filein.readLine();
        QStringList l = line.replace("\n", "").split(" ");
        g_list << l;
    }
    filein.close();
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
    for(int i = 0; i < g_list.length(); i++){
        for(int j = 0; j < g_list[i].length(); j++){
            writeStream << g_list[i][j];
            if(j != g_list[i].length() - 1) writeStream << "->";
        }
        if(i != g_list.length() - 1) writeStream << "\n";
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
    ui->graphview->BFS();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->graphview->refresh();
}
