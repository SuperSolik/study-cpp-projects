#ifndef HELP_H
#define HELP_H

#include <QtWidgets>
#include <QTextDocument>
#include <QDebug>

class Help:public QWidget{
    Q_OBJECT
public:
    Help(const QString& path, const QString& file_name, QWidget *parent = 0)
        :QWidget(parent){
        setWindowTitle("Help");
        pcmdBack    = new QPushButton("<<");
        pcmdHome    = new QPushButton("Home");
        pcmdForward = new QPushButton(">>");
        ptxtBrowser = new QTextBrowser;

        connect(pcmdBack, SIGNAL(clicked()),
                ptxtBrowser, SLOT(backward())
               );
        connect(pcmdHome, SIGNAL(clicked()),
                ptxtBrowser, SLOT(home())
               );
        connect(pcmdForward, SIGNAL(clicked()),
                ptxtBrowser, SLOT(forward())
               );
        connect(ptxtBrowser, SIGNAL(backwardAvailable(bool)),
                pcmdBack, SLOT(setEnabled(bool))
               );
        connect(ptxtBrowser, SIGNAL(forwardAvailable(bool)),
                pcmdForward, SLOT(setEnabled(bool))
               );

        ptxtBrowser->setSearchPaths(QStringList() << path);
        ptxtBrowser->setSource(QString(file_name));

        //Layout setup
        pvbxLayout = new QVBoxLayout;
        phbxLayout = new QHBoxLayout;
        phbxLayout->addWidget(pcmdBack);
        phbxLayout->addWidget(pcmdHome);
        phbxLayout->addWidget(pcmdForward);
        pvbxLayout->addLayout(phbxLayout);
        pvbxLayout->addWidget(ptxtBrowser);
        setLayout(pvbxLayout);
    }

    ~Help(){
        delete ptxtBrowser;
        delete pcmdBack;
        delete pcmdHome;
        delete pcmdForward;
        delete phbxLayout;
        delete pvbxLayout;
    }

private:
    QVBoxLayout* pvbxLayout;
    QHBoxLayout* phbxLayout;
    QPushButton*  pcmdBack;
    QPushButton*  pcmdHome;
    QPushButton*  pcmdForward;
    QTextBrowser* ptxtBrowser;

};

#endif // HELP_H
