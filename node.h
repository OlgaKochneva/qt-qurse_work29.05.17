
#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QList>

class Edge;
class GraphWidget;
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
QT_END_NAMESPACE

//! [0]
class Node : public QGraphicsItem
{
public:

    Node(GraphWidget *graphWidget,int id);
    void addEdge(Edge *edge);//добавить путь
    QList<Edge *> edges() const;

    enum { Type = UserType + 1 };
    int type() const override { return Type; }
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    //рисуем
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    int id;
    QList<Edge *> edgeList;
    QPointF newPos;
    GraphWidget *graph;
};
//! [0]

#endif // NODE_H
