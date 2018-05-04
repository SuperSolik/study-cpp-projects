#ifndef INFO_H
#define INFO_H

#include <QWidget>
#include <QString>
#include <QTimer>

namespace Ui{
class Info;
}

class Info:public QWidget{
    Q_OBJECT
public:
    explicit Info(QWidget *parent = 0);
    ~Info();
public:
    void setData(QString filename, int bitcount, int width, int height, int size, int x_pix_per_m, int y_pix_per_m);

private:
    void resizeEvent(QResizeEvent *event);


private:
    Ui::Info *ui;
    QTimer* timer;
private slots:
    void slotTimer();
};


#endif // INFO_H
