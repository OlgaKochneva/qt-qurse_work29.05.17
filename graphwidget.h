
#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>
#include<fstream>
#include<QTextStream>

#include "edge.h"
#include "QVector"

class Node;

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget *parent = 0);
    void itemMoved();
    //////////
    void set_tree_id(){tree_id=1;}//начинает отсчет заново
    void next_step(int k, int b);//рисует шаги
    int Array[50][50];//массив данных
    int path[50][50];//массив путей
    void build_matrix();//строим матрицу по данным
    int numb(){return(vect_tree.size());}//возвращает количество узлов
    void all_clear();//стирает  с экрана цвет
    const int inf = 1E9;//бесконечность
    void add_tree_elem(int choice);
    void add_edge(int id1,int id2,int weigth);
    void clear_scene();//стереть все вообще
    ///////////
public slots:
    void shuffle();
    void zoomIn();
    void zoomOut();
protected:
    void keyPressEvent(QKeyEvent *event) override;

#ifndef QT_NO_WHEELEVENT
    void wheelEvent(QWheelEvent *event) override;
#endif
    void drawBackground(QPainter *painter, const QRectF &rect) override;

    void scaleView(qreal scaleFactor);
private:

    struct corner;//хранит пути
    struct tree;//хранит узлы
    tree* get_tree(int id);
    int tree_id;

    struct corner{
        int weigth;
        tree *tr1,*tr2;
        Edge *elem;
        corner(tree *tr1,tree *tr2,Edge *elem,int weig){
            this->tr1=tr1;
            this->tr2=tr2;
            this->elem=elem;
            weigth = weig;
        }
    };

    struct tree{
        int id;
        Node *elem;
        tree(Node *node,int id){
            elem = node;
            this->id=id;
        }
    };
    //списки путей и узлов
    QVector<corner *> vect_corn;
    QVector<tree*> vect_tree;
    int timerId;

};

#endif // GRAPHWIDGET_H
