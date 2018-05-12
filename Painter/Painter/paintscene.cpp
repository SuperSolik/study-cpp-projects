#include "paintscene.h"

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent){}

paintScene::~paintScene(){}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event){
    beginPoint = event->scenePos();
}

void paintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    endPoint = event->scenePos();
    emit mouse_pressed(beginPoint, endPoint);
}
