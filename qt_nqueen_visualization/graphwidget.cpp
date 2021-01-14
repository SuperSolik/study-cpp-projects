/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "graphwidget.h"

#include <iostream>
#include <cmath>
#include <cstdlib>

#include <QKeyEvent>
#include <QDebug>
#include <QThread>

GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-700, -700, 1400, 1400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(0.9), qreal(0.9));
    setMinimumSize(400, 400);
    board_rdy = false;
}

GraphWidget::~GraphWidget(){
    scene->clear();
    if(board_rdy){
        foreach (QGraphicsItem *item, scene->items()) {
            delete item;
        }
    }
}

void GraphWidget::placeQueen(int row, int col){
    int x = start_x + col*100;
    int y = start_y + row*100;
    QImage image(":/doc/queen.png");
    queens.push_back(new QGraphicsPixmapItem(QPixmap::fromImage(image)));
    queens[queens.size()-1]->setX(x-50);
    queens[queens.size()-1]->setY(y-50);
    scene->addItem(queens[queens.size()-1]);
}

void GraphWidget::removeQueen(int row, int col){
    for(int i = 0; i < queens.size(); i++){
        if((queens[i]->y() == row*100+start_y-50) && (queens[i]->x() == col*100+start_x-50)){
            scene->removeItem(queens[i]);
            delete queens[i];
            queens.erase(queens.begin() + i);
            break;
        }
    }
}

void GraphWidget::drawBoard(int b_size){
    if(board_rdy){
        scene->clear();
        foreach (QGraphicsItem *item, scene->items()) {
            delete item;
        }
        queens.clear();
    }
    start_x = -b_size*100/2;
    start_y = -b_size*100/2;
    scene->addLine(start_x-50, start_y-50, -start_x-50, start_y-50, QPen(Qt::black));
    scene->addLine(-start_x-50, start_y-50, -start_x-50, -start_y-50, QPen(Qt::black));
    scene->addLine(-start_x-50, -start_y-50, start_x-50, -start_y-50, QPen(Qt::black));
    scene->addLine(start_x-50, -start_y-50, start_x-50, start_y-50, QPen(Qt::black));
    int x = start_x, y = start_y;
    for(int i = 0; i < b_size; i++){
        for(int j = 0; j < b_size; j++){
            QColor color;
            if(i%2 == 0){
                if(j%2 == 1) color = Qt::darkGray;
                else color = Qt::lightGray;
            } else {
                if(j%2 == 0) color = Qt::darkGray;
                else color = Qt::lightGray;
            }
            scene->addItem(new Node(this, color, QPointF(x, y), i, j));
            x += 100;
        }
        x = start_x;
        y += 100;
    }
    board_rdy = true;
}

void GraphWidget::itemMoved()
{
    if (!timerId)
        timerId = startTimer(1000 / 25);
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Plus:
            zoomIn();
            break;
        case Qt::Key_Minus:
            zoomOut();
            break;
        case Qt::Key_Space:
            break;
        default:
            QGraphicsView::keyPressEvent(event);
            break;
    }
}

void GraphWidget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
}

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
