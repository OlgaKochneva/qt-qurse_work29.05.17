#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>

class Node;

class Edge : public QGraphicsItem
{
public:
    Edge(Node *sourceNode, Node *destNode, int wt);
    bool red;//окраска
    int get_weight();//возвращает вес вершины
    Node *sourceNode() const;//вершина начала
    Node *destNode() const;//вершина конца

    void adjust();//чтобы начиналась в круге и кончалась там же

    enum { Type = UserType + 2 };
    int type() const override { return Type; }

protected:
    QRectF boundingRect() const override;//занимаемое место треугольниками
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    Node *source, *dest;
    int weight;//вес
    QPointF sourcePoint;//координаты начала и конца
    QPointF destPoint;
    qreal arrowSize;//размеры стрелок
};

#endif // EDGE_H
