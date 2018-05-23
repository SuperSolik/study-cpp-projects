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
    explicit SizeDialog(QWidget *parent = 0);
    ~SizeDialog();

private:
    Ui::SizeDialog *ui;

signals:
    get_size(int width, int height);

private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
};

#endif // SIZEDIALOG_H
