#ifndef INFO_H
#define INFO_H

#include <QWidget>
#include <QMainWindow>
#include <QApplication>
#include <QString>
#include <QDebug>
#include <QVBoxLayout>

namespace Ui{
class Info;
}

class Info:public QWidget
{
    Q_OBJECT
public:
    explicit Info(QWidget *parent = 0);
    ~Info();
public:
    void setData(int bitcount, int width, int height, int size, int x_pix_per_m, int y_pix_per_m);

private:
    Ui::Info *ui;
};


#endif // INFO_H
