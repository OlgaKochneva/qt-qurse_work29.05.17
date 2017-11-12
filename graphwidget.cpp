#include "graphwidget.h"
#include "edge.h"
#include "node.h"
#include <math.h>
#include <iostream>
#include <QKeyEvent>


using namespace std;
static const double Pi = 3.14159265358979323846264338327950288419717;
//! [0]
GraphWidget::GraphWidget(QWidget *parent)
    : QGraphicsView(parent), timerId(0)
{
    tree_id=1;
    QGraphicsScene *scene = new QGraphicsScene(parent);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    // Ввод с файла или создание 1 вершины
    ifstream file("Z:\\input.txt");
    int choice=0;
    file>>choice;
    if(choice>=49)
    {choice=49;}
    add_tree_elem(choice);

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
    case Qt::Key_Enter:
        shuffle();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
    scene()->update();
}



#ifndef QT_NO_WHEELEVENT

void GraphWidget::wheelEvent(QWheelEvent *event)
{
    scaleView(pow((double)2, -event->delta() / 240.0));
}

#endif

void GraphWidget::drawBackground(QPainter *painter, const QRectF &rect)
{
    Q_UNUSED(rect);

    // Shadow
    QRectF sceneRect = this->sceneRect();
    QRectF rightShadow(sceneRect.right(), sceneRect.top() + 5, 5, sceneRect.height());
    QRectF bottomShadow(sceneRect.left() + 5, sceneRect.bottom(), sceneRect.width(), 5);
    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
        painter->fillRect(rightShadow, Qt::darkGray);
    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
        painter->fillRect(bottomShadow, Qt::darkGray);

    // Fill
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
}

void GraphWidget::scaleView(qreal scaleFactor)
{
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);
}

void GraphWidget::shuffle()
{
    foreach (QGraphicsItem *item, scene()->items()) {
        if (qgraphicsitem_cast<Node *>(item))
            item->setPos(-150 + qrand() % 300, -150 + qrand() % 300);
    }
}

void GraphWidget::zoomIn()
{
    scaleView(qreal(1.2));
}

void GraphWidget::zoomOut()
{
    scaleView(1 / qreal(1.2));
}



void GraphWidget::add_tree_elem(int choice){
    if(choice<=0)
    { Node *node = new Node(this,tree_id);
        node->setPos(qrand()%20-10,qrand()%20-10);
        scene()->addItem(node);
        tree *tr = new tree(node,tree_id++);
        vect_tree.push_back(tr);}
    else
    {
        const double bla=360/choice;
        double coor=360/choice;
        for(int i=0; i<choice;i++)
        {
            Node *node = new Node(this,tree_id);
            node->setPos(100*cos(coor*Pi/180),100*sin(coor*Pi/180));
            scene()->addItem(node);
            tree *tr = new tree(node,tree_id++);
            vect_tree.push_back(tr);
            coor+=bla;

        }
    }
}
void GraphWidget::add_edge(int id1,int id2,int weigth){
    tree *tr1 = get_tree(id1);
    tree *tr2 = get_tree(id2);
    if(tr1==NULL || tr2==NULL) return ;
    Edge *edge = new Edge(tr1->elem,tr2->elem,weigth);
    corner *corn = new corner(tr1,tr2,edge,weigth);
    scene()->addItem(edge);
    vect_corn.push_back(corn);

}

GraphWidget::tree* GraphWidget::get_tree(int id){
    for(int i=0;i<vect_tree.length();i++){
        if (vect_tree[i]->id==id) return vect_tree[i];
    }
    return NULL;
}



void GraphWidget::clear_scene(){//стереть граф

    vect_corn.clear();
    vect_tree.clear();
    scene()->clear();
}

void GraphWidget::all_clear(){//убрать графическую подсветку
    for(int i=0;i<vect_corn.length();i++){
        vect_corn[i]->elem->red=0;
    }
}



void GraphWidget::next_step(int k, int b){//вывод графической подсветки

    all_clear();
    int i = b;
    int prev = k;
    int next;
    while (path[prev][i] != -1)
    {
        next =path[prev][i];
        for(int j=0;j<vect_corn.size();j++)
        {
            if(vect_corn[j]->tr1->id==prev+1&&vect_corn[j]->tr2->id==next+1)
            {
                vect_corn[j]->elem->red=1;
                scene()->update();
            }
        }
        prev=next;
    }
}



void GraphWidget::build_matrix()
{
    int N=vect_tree.size();
    for(int i=0;i<N;i++)//забиваем матрицу смежности бесконечностями
        //а матрицу путей -1
    {
        for(int j=0;j<N;j++)
        {
            if(i==j)
            {
                Array[i][j]=0;
            }
            else
            {Array[i][j]=inf;}
            path[i][j]=-1;
        }

    }
    for(int i=0;i<vect_corn.size();i++)//забиваем матрицу по данным хранящимся в векторах
    {

        Array[vect_corn[i]->tr1->id-1][vect_corn[i]->tr2->id-1]=vect_corn[i]->weigth;
        path[vect_corn[i]->tr1->id-1][vect_corn[i]->tr2->id-1]=vect_corn[i]->tr2->id-1;

    }
}










