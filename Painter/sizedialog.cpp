#include "sizedialog.h"
#include "ui_sizedialog.h"
#include "QDebug"

SizeDialog::SizeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SizeDialog)
{
    ui->setupUi(this);
}

SizeDialog::~SizeDialog(){
    delete ui;
}

void SizeDialog::on_okButton_clicked(){
    emit start_size(ui->wBox->value(), ui->hBox->value());
    SizeDialog::closeEvent(0);
}

void SizeDialog::on_cancelButton_clicked(){
    SizeDialog::closeEvent(0);
    qApp->exit(0);
}
