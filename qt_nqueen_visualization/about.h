#ifndef ABOUT_H
#define ABOUT_H

#include <QtWidgets>
#include <QTextDocument>
#include <QDebug>

class About:public QWidget{
    Q_OBJECT
public:
    About(const QString& path, const QString& file_name, QWidget *parent = 0)
        :QWidget(parent){
        pvbxLayout = new QVBoxLayout;
        ptxtBrowser = new QTextBrowser;
        ptxtBrowser->setSearchPaths(QStringList() << path);
        ptxtBrowser->setSource(QString(file_name));
        pvbxLayout->addWidget(ptxtBrowser);
        setLayout(pvbxLayout);
    }

    ~About(){
        delete ptxtBrowser;
        delete pvbxLayout;
    }

private:
    QVBoxLayout* pvbxLayout;
    QTextBrowser* ptxtBrowser;

};

#endif // ABOUT_H
