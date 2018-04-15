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
    void setData(char* bitcount, char* width, char* height, char* size);

private:
    Ui::Info *ui;
};


#endif // INFO_H
