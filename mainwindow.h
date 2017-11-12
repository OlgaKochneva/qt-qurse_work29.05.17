#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMessageBox>
#include<QEventLoop>



namespace Ui {
class MainWindow;
class algorithm;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    const int inf = 1E9;//бесконечность
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void algorithm();//функция алгоритма
    void show_path(int k, int b);//вывод пути от k до b
    void show_result();//вывод результата в список

private slots:
    void on_pushButton_clicked();//кнопка для добавления новой стрелки
    void on_pushButton_2_clicked();//кнопка для добавления нового узла
    void on_pushButton_3_clicked();//кнопка для очищения всего
    void on_pushButton_4_clicked();//начать алгоритм
    void on_pushButton_5_clicked();//дальше
    void on_pushButton_6_clicked();//вывод пути для 2 вершин
    void on_pushButton_7_clicked();//вывод результата

private:

    QEventLoop* myLoop_ = new QEventLoop (this);
    bool show_to_small=false;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
