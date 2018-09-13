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
        qDebug() << path;
        QTextBrowser* help_browser = new QTextBrowser;
        help_browser->setSearchPaths(QStringList() << path);
        help_browser->setSource(QString(file_name));
        QVBoxLayout* layout = new QVBoxLayout;
        layout->addWidget(help_browser);
        setLayout(layout);
    }
};

#endif // HELP_H
