#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QColor>

class paintScene : public QGraphicsScene{

    Q_OBJECT

public:
    explicit paintScene(QObject *parent = 0);
    ~paintScene();

public:
   QPointF beginPoint;
   QPointF endPoint;

signals:
   void mouse_pressed(QPointF start, QPointF end);

private:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};

#endif // PAINTSCENE_H
