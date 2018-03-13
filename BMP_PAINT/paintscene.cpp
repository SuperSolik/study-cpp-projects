#include "paintscene.h"
#include "mainwindow.h"

paintScene::paintScene(QObject *parent) : QGraphicsScene(parent)
{
    size_l = 5;
}

paintScene::~paintScene()
{

}

void paintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    addEllipse(event->scenePos().x() - size_l/2,
               event->scenePos().y() - size_l/2,
               size_l,
               size_l,
               QPen(Qt::NoPen),
               QBrush(color));
    previousPoint = event->scenePos();
}

void paintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    addLine(previousPoint.x(),
            previousPoint.y(),
            event->scenePos().x(),
            event->scenePos().y(),
            QPen(color, size_l, Qt::SolidLine, Qt::RoundCap));
    previousPoint = event->scenePos();

}

void paintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
    addEllipse(event->scenePos().x() - size_l/2,
               event->scenePos().y() - size_l/2,
               size_l,
               size_l,
               QPen(Qt::NoPen),
               QBrush(color));
}
