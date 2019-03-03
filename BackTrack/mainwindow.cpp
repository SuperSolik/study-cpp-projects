#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    graphicsView = new GraphWidget(this);
    setCentralWidget(graphicsView);
    resize(1024, 768);
    problem = new NQueen;
    board_rdy = false;
    queen_placed = false;
    connect(problem, SIGNAL(widget_placeQueen(int, int)), graphicsView, SLOT(placeQueen(int, int)));
    connect(problem, SIGNAL(widget_removeQueen(int, int)), graphicsView, SLOT(removeQueen(int, int)));
    connect(problem, SIGNAL(setdelay()), this, SLOT(delay()));
    for(int i = 4, delay = 500; i < 9; i++, delay -= 100){
        delays[i] = delay;
    }
    delays[9] = 50;
    delays[10] = 25;
    setWindowTitle("NQueen");
    filename = "solutions.txt";
}

MainWindow::~MainWindow()
{
    delete problem;
    delete ui;
}

void MainWindow::on_actionSolveAll_triggered()
{
    if(!board_rdy){
        QMessageBox::information(nullptr, "Error", "Board isn't ready");
        return;
    }
    graphicsView->drawBoard(board->size());
    problem->solveAll(*board);
    std::ofstream file;
    file.open(filename.toStdString(), std::ios::out);
    if(!file.good()){
        QMessageBox::information(nullptr, "Error", "File can't be opened");
        return;
    }
    problem->print_solutions(file);
    file.close();
}

void MainWindow::on_actionSolveTask_triggered()
{
    if(!board_rdy){
        QMessageBox::information(nullptr, "Error", "Board isn't ready");
        return;
    }
    if(!queen_placed){
        QMessageBox::information(nullptr, "Error", "No start queen");
        return;
    }
    if(!problem->solveTask(*board)){
        QMessageBox::information(nullptr, "Error", "There is no solution for this config");
        return;
    }
    QMessageBox::information(nullptr, "Success", "Solution has been found!");
    std::ofstream file;
    file.open(filename.toStdString(), std::ios::out);
    if(!file.good()){
        QMessageBox::information(nullptr, "Error", "File can't be opened");
        return;
    }
    problem->print_solutions(file);
    file.close();
}

void MainWindow::on_actionplaceQueen_triggered()
{
    if(!board_rdy) return;
    board->clear();
    graphicsView->drawBoard(board->size());
    SizeDialog* dialog = new SizeDialog(this, false, board->size());
    connect(dialog, SIGNAL(get_coords(int, int)), this, SLOT(placeQueen(int, int)));
    dialog->show();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::placeQueen(int row, int col){
    problem->placeQueen(*board, row, col);
    queen_placed = true;
}

void MainWindow::on_actionsetBoardSize_triggered()
{
    SizeDialog* dialog = new SizeDialog(this, true);
    connect(dialog, SIGNAL(get_size(int)), this, SLOT(setBoardSize(int)));
    dialog->show();
    dialog->setAttribute(Qt::WA_DeleteOnClose);
}

void MainWindow::setBoardSize(int size){
    if(board_rdy) delete board;
    board = new Board(size);
    graphicsView->drawBoard(board->size());
    board_rdy = true;
    queen_placed = false;
}

void MainWindow::delay(){
    QEventLoop loop;
    QTimer timer;

    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(delays[board->size()]);
    loop.exec();
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

void MainWindow::on_actionChoose_out_file_triggered()
{
    filename = QFileDialog::getSaveFileName(this, "Choose output file", "","*.txt;;*.dat");
    if(filename.isNull()) filename = "solutions.txt";
}
