#include "paintscene.h"
#include "mainwindow.h"

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent){
    size_l = 0;
}

paintScene::~paintScene(){

}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    beginPoint = event->scenePos();
}

void paintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    endPoint = event->scenePos();
    emit mouse_pressed();
}
