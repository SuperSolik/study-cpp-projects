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
#include "edge.h"
#include "node.h"

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
    scene->setSceneRect(-300, -300, 600, 600);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal(1.2), qreal(1.2));
    setMinimumSize(400, 400);
}

GraphWidget::~GraphWidget(){
    scene->clear();
    graphList.clear();
    if(nodes.size() > 0){
        foreach (QGraphicsItem *item, scene->items()) {
            delete item;
        }
    }
    nodes.clear();
}

void GraphWidget::setGraph(QList<std::pair<int, QList<std::pair<int, int>>>> _graphList){
    scene->clear();
    graphList.clear();
    if(nodes.size() > 0){
        foreach (QGraphicsItem *item, scene->items()) {
            delete item;
        }
        nodes.clear();
    }
    graphList = _graphList;
    int nodes_len = graphList.size();
    int d = 150;
    int angle = 0;
    for(int i = 0; i < nodes_len; i++){
        nodes.push_back(new Node(i+1, this));
        scene->addItem(nodes[nodes.size() - 1]);
        nodes[nodes.size() - 1]->setPos(d*cos((angle+i*360/nodes_len) * M_PI/180), d*sin(angle+i*360/nodes_len * M_PI/180));
    }
    for(int i = 0; i < nodes_len; i++){
        for(int j = 0; j < graphList[i].second.length(); j++){
            if(graphList[i].second[0].first != -1)
                scene->addItem(new Edge(nodes[graphList[i].first], nodes[graphList[i].second[j].first],
                                        graphList[i].second[j].second, true));
        }
    }
}

void GraphWidget::refresh(){
    for(auto& el : nodes){
        el->setColor(Qt::lightGray);
    }
}

QList<std::pair<int, QList<std::pair<int, int>>>> GraphWidget::getGraphList() const{
    return graphList;
}

std::vector<int> GraphWidget::Djikstra(int start, int source){
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, Compare> q;
    q.push(std::make_pair(start, 0));
    int nodes_len = graphList.size();
    std::vector<int> came_from(nodes_len, -1);
    came_from[start] = 0;
    std::vector<int> cost(nodes_len, INT_MAX);
    cost[start] = 0;
    int new_cost;
    while(!q.empty()){
        auto cur_pair = q.top();
        q.pop();
        for(auto& vertex : graphList[cur_pair.first].second){
            new_cost = cost[cur_pair.first] + vertex.second;
            if(new_cost < cost[vertex.first]){
                cost[vertex.first] = new_cost;
                q.push(std::make_pair(vertex.first, new_cost));
                came_from[vertex.first] = cur_pair.first;
            }
        }
    }
    return came_from;
}

std::string GraphWidget::make_path(int start, int goal, std::vector<int>& came_from){
    int current = goal;
    std::string str_path;
    std::vector<int> path;
    path.push_back(current);
    while(current != start){
        current = came_from[current];
        path.push_back(current);
    }
    std::reverse(path.begin(), path.end());
    for(auto& el : path){
        str_path += (char)(el+49);
        str_path += " ";
        nodes[el]->setColor(Qt::blue);
        delay();
    }
    return str_path;
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

    QList<Node *> nodes;
    foreach (QGraphicsItem *item, scene->items()) {
        if (Node *node = qgraphicsitem_cast<Node *>(item))
            nodes << node;
    }

    foreach (Node *node, nodes)
        node->calculateForces();

    bool itemsMoved = false;
    foreach (Node *node, nodes) {
        if (node->advance())
            itemsMoved = true;
    }

    if (!itemsMoved) {
        killTimer(timerId);
        timerId = 0;
    }
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

void GraphWidget::changeGravity(bool flag){
    for(auto& node : nodes){
        node->setGravity(flag);
    }
}

void GraphWidget::delay(int msec)
{
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    timer.start(msec);
    loop.exec();
}
