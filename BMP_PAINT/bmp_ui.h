#ifndef BMP_UI_H
#define BMP_UI_H

#include <QImage>
#include <QString>
#include <QPainter>
#include <QPixmap>
#include <QMessageBox>
#include <cstdio>

#include <bmpstruct.h>

class BMP_ui:public QObject
{
    Q_OBJECT
public:
    BMP_ui();

    QPixmap Load(QString filename);
    void Save(QString filename, QImage image);

};

#endif // BMP_UI_H
