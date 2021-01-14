#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Stack");
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
    bool result;
    output << std::boolalpha;
    while(std::getline(input_stream, buffer)){
        std::ostringstream err;
        result = check_correct_brackets(buffer, err);
        if (result){
            output << "Correct";
            if (!input_stream.eof()) output << '\n';
            ui->textEdit_2->append("Correct");
        } else {
            output << err.str();
            if (!input_stream.eof()) output << '\n';
            ui->textEdit_2->append(QString::fromStdString(err.str()));
        }
        buffer.clear();
    }
    output.close();
}


char MainWindow::opposite_bracket(char bracket){
    if (bracket == '(') return ')';
    else if (bracket == ')') return '(';
    else if (bracket == '{') return '}';
    else if (bracket == '}') return '{';
    else if (bracket == '[') return ']';
    else return '[';
}

bool MainWindow::check_correct_brackets(std::string text_to_check, std::ostream& err){
    Stack<char> stack;
    for(auto& c : text_to_check){
        if(isalpha(c) || isdigit(c) || isspace(c)) continue;
        if(c == '{' || c == '(' || c == '['){
            stack.push(c);
        } else {
            try{
                if (stack.top() != opposite_bracket(c)){
                    err << std::string("Missing ") + opposite_bracket(stack.top());
                    return false;
                }
                stack.pop();
            }catch(std::exception& e){
                err << std::string("Missing ") + opposite_bracket(c);
                return false;
            }
        }
    }
    if (!stack.is_empty()){
        err << std::string("Missing closing bracket for ") + stack.top();
        return false;
    }
    return true;
}
