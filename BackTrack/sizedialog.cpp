#include "sizedialog.h"
#include "ui_sizedialog.h"

SizeDialog::SizeDialog(QWidget *parent, bool flag, int size) :
    QDialog(parent),
    ui(new Ui::SizeDialog)
{
    this->setFixedSize(QSize(325, 75));
    ui->setupUi(this);
    this->flag = flag;
    if(flag){
        this->setWindowTitle("Board size");
        ui->wBox->setMinimum(0);
        ui->wBox->setMaximum(12);
        ui->wBox->setValue(8);
        ui->label->setText("Enter board size");
        ui->label_2->hide();
        ui->hBox->hide();
    } else {
        this->setWindowTitle("Start queen");
        ui->wBox->setMinimum(0);
        ui->wBox->setMaximum(size-1);
        ui->wBox->setValue((0));
        ui->hBox->setMinimum(0);
        ui->hBox->setMaximum(size-1);
        ui->hBox->setValue((0));
        ui->label->setText("Enter queen row");
        ui->label_2->setText("Enter queen col");
    }
}

SizeDialog::~SizeDialog(){
    delete ui;
}

void SizeDialog::on_okButton_clicked(){
    if(flag){
        emit get_size(ui->wBox->value());
    } else {
        emit get_coords(ui->wBox->value(), ui->hBox->value());
    }
    SizeDialog::closeEvent(0);
}

void SizeDialog::on_cancelButton_clicked(){
    SizeDialog::closeEvent(0);
}
