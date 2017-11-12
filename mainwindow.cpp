#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "graphwidget.h"
#include "QListView"
#include "QTime"
#include <iostream>

GraphWidget *widget;
QListWidget *list;
QListWidget *list2;
QListWidget *list3;


void add_edge(int id1,int id2,int weig);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    list = ui->listWidget;
    list2=ui->listWidget_2;
    list3=ui->listWidget_3;
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    widget = new GraphWidget(ui->graphicsView);
    std::ifstream file("Z:\\input.txt");
    int n=0;
    file>>n;
    if(n>=49)
    {n=49;}
    for(int j=0;j<n;j++)
    {
        for(int i=0;i<n;i++)

        {
            file>>widget->path[j][i];
            if((widget->path[j][i]>0)&&(i!=j))
            {
                add_edge(j+1,i+1,widget->path[j][i]);//при некорректных данных связи не выводит
            }
        }
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void add_edge(int id1,int id2,int weig){//добавить новую путь в список новых вершин

    widget->add_edge(id1,id2,weig);
    list->addItem(QString::number(id1)+" - "+QString::number(id2)+" вес - "+QString::number(weig));
}

void MainWindow::on_pushButton_clicked()//добавить новую стрелку
{
    int id1 = ui->spinBox_3->value();
    int id2 = ui->spinBox_2->value();
    if (id1==id2) return;
    int weig = ui->spinBox->value();
    ui->listWidget->addItem(QString::number(id1)+" - "+QString::number(id2)+" вес - "+QString::number(weig));
    widget->add_edge(id1,id2,weig);
}



void MainWindow::on_pushButton_2_clicked()//добавить элемент

{
    if(widget->numb()>=49)
    {return;}
    else
    {widget->add_tree_elem(0);}
}

void MainWindow::on_pushButton_3_clicked()//очистить все
{
    widget->clear_scene();//очищаем рисунок
    widget->set_tree_id();//ставим нумерацию вершин в начало
    list->clear();//очищаем все списки
    list3->clear();
    list2->clear();
    ui->pushButton_4->setEnabled(true);//кнопка начать алгоритм-доступна
    ui->pushButton_6->setEnabled(false);//ок-недоступна
    ui->pushButton_5->setEnabled(false);//дальше-недоступна
    ui->pushButton_7->setEnabled(false);//резудьтат недоступна
}
void MainWindow::on_pushButton_4_clicked()//нажата кнопка начать алгоритм
{
    list2->clear();
    ui->pushButton_4->setEnabled(false);
    widget->build_matrix();
    ui->pushButton_5->setEnabled(true);
    algorithm();


}
void MainWindow ::on_pushButton_5_clicked()//нажата кнопка дальше
{
    myLoop_->quit ();
}
void MainWindow::on_pushButton_6_clicked()//нажата кнопка ок для вывода вершин
{
    list3->clear();
    show_to_small=true;
    int id1 = ui->spinBox_4->value();
    int id2 = ui->spinBox_5->value();
    if ((id1==id2)||(id1>widget->numb())||(id2>widget->numb())||id1==0||id2==0) return;
    else
    {
        show_path(id1-1,id2-1);
        widget->next_step(id1-1,id2-1);
    }
    show_to_small=false;

}


void MainWindow::on_pushButton_7_clicked()//нажата кнопка для вывода результата

{
    widget->all_clear();
    widget->scene()->update();
    list2->clear();
    ui->pushButton_4->setEnabled(true);
    show_result();
    ui->pushButton_6->setEnabled(true);

}


void MainWindow::algorithm()//алгоритм работы программы
{

    ui->listWidget_4->addItem("Начальная матрица");
    int N=widget->numb();//число элементов
    for(int k=0;k<N;k++)
    {
        QString list_mass;
        for(int i=0;i<N;i++)
        {
            if(widget->Array[k][i]==inf)
            {
                list_mass.push_back("x");
            }
            else
            { list_mass.push_back(QString::number(widget->Array[k][i]));}

            list_mass.push_back(" ");
        }
        ui->listWidget_4->addItem(list_mass);
    }



    for (int k = 0; k < N; ++k)
    {

        for (int i = 0; i < N; ++i)
        {


            for (int j = 0; j < N; ++j)
            {
                if ((widget->Array[i][k] < inf) && (widget->Array[k][j] < inf )&&
                        (widget->Array[i][k] + widget->Array[k][j]<widget->Array[i][j])) {
                    widget-> Array[i][j] = widget->Array[i][k] + widget->Array[k][j];
                    widget-> path[i][j] = widget->path[i][k];

                    show_path(i,j);
                    widget->next_step(i,j);
                    myLoop_->exec();
                }

            }
        }


    }
    ui->pushButton_5->setEnabled(false);//по окончании нельзя нажать дальше
    ui->pushButton_7->setEnabled(true);//по окончании можно нажать результат

}

void MainWindow::show_result()//функция вывода результата
{
    int N=widget->numb();
    int sum;//суммарные веса

    ui->listWidget_4->addItem("Результат");
    for(int j=0;j<N;j++)
    {
        QString  list_mass;
        for(int i=0;i<N;i++)
          {

            if(widget->Array[j][i]==inf)
            {
                list_mass.push_back("x");
            }
            else
            { list_mass.push_back(QString::number(widget->Array[j][i]));}
            list_mass.push_back(" ");

            list2->addItem( "Путь из " + QString::number(j+1) + " в " +QString::number( i+1) + " : ");
            if (widget->path[j][i] == -1)
            {
                list2->addItem( "НЕТ" );
            }
            else
            {
                int prev = j;
                int next;
                sum=0;
                while (widget->path[prev][i] != -1)
                {
                    list2->addItem( QString::number(prev + 1) + " ");
                    next = widget->path[prev][i];
                    sum+=widget->Array[prev][next];
                    prev=next;
                }

                list2->addItem( QString::number(prev + 1) + " ");
                list2->addItem("Стоимость: "+ QString::number(sum) + " \n");
            }

        }
       ui->listWidget_4->addItem(list_mass);



    }


}


void MainWindow:: show_path(int k, int b)
{
    int j = k;
    int i = b;
    int sum;
    if(show_to_small){list3->addItem( "Путь из " + QString::number(j+1) + " в " +QString::number( i+1) + " : ");}
    else{list2->addItem( "Путь из " + QString::number(j+1) + " в " +QString::number( i+1) + " : ");}
    if (widget->path[j][i] == -1)
    {
        if(show_to_small)
        {list3->addItem( "НЕТ" );}
        else{list2->addItem( "НЕТ" );}
    }
    else
    {
        int prev = j;
        int next;
        sum=0;
        while (widget->path[prev][i] != -1)
        {
            if(show_to_small)
            { list3->addItem( QString::number(prev + 1) + " ");}
            else{list2->addItem( QString::number(prev + 1) + " ");}
            next = widget->path[prev][i];
            sum+=widget->Array[prev][next];
            prev=next;

        }
        if(show_to_small)
        { list3->addItem( QString::number(prev + 1) + " ");
            list3->addItem("Стоимость: "+ QString::number(sum) + " \n");
        }
        else{list2->addItem( QString::number(prev + 1) + " ");
            list2->addItem("Стоимость: "+ QString::number(sum) + " \n");}
    }

}

