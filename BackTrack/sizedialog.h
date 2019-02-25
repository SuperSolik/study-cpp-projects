#ifndef SIZEDIALOG_H
#define SIZEDIALOG_H

#include <QDialog>

namespace Ui {
class SizeDialog;
}

class SizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SizeDialog(QWidget *parent = 0, bool flag=false, int size=0);
    ~SizeDialog();

private:
    Ui::SizeDialog *ui;
    bool flag;

signals:
    void get_coords(int row, int col);
    void get_size(int size);

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
};

#endif // SIZEDIALOG_H
