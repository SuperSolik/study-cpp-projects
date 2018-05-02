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

public:
   QColor color;
   int size_l;

signals:
   void mouse_pressed();

private:
    void mousePressEvent(QGraphicsSceneMouseEvent * event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

};

#endif // PAINTSCENE_H
