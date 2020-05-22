#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <cmath>
#include "./qcustomplot.h"
#include<QPixmap>
#include "teoria.h"
#define PI 3.14159265
#define G 9.8

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->angle->setMaximum(90); //ограничения угла и скорости
    ui->speed->setMaximum(50);

    graphic = ui->widget;
    //составление графика
    graphic->addGraph();

    // добавляем название осей
    graphic->xAxis->setLabel("Дальность, м");
    graphic->yAxis->setLabel("Высота, м");
    // диапозон осей
    graphic->xAxis->setRange(0, 20);
    graphic->yAxis->setRange(0, 20);
    graphic->replot();


    //для картинки
    QPixmap pix(":/img/Ym.PNG");
    int w=ui->image->width();
    int h=ui->image->height();
    ui->image->setPixmap(pix.scaled(w,h, Qt::KeepAspectRatio));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printGraphic(int speed, int angle, double distance, double height, double Fsopr,double mas){
    QVector<double> x(1000), y(1000);

    double Fsopr_x=Fsopr*cos(angle*PI/180); //силы сопротивления для х и у
    double Fsopr_y=Fsopr*sin(angle*PI/180);
    double max_time = 2 * speed * sin(angle * PI / 180) / G;   //max время

    double maxAxis = 0,
           value = 0,
            step=1/1000;
           //step =distance / 1000;

    //x[0]=0;
    double time = step;
    for (int i=0;i < 1000;i++)
    {
      //x[i] = value;
      //y[i] = x[i] * tan(angle * PI / 180) - (G / (2 * pow(speed, 2) * pow(cos(angle * PI / 180), 2))) * pow(x[i], 2);
        x[i]=speed*cos(angle*PI/180)*time+Fsopr_x*time*time/(2*mas);
        y[i]=speed*sin(angle*PI/180)*time-(G-Fsopr_y)*time*time/(2*mas);
       //value+=step;
        ui->coffec->setNum(x[i]); //вывод силы сопротивления
      time += step;
    }

    if (distance > height) maxAxis = distance; else maxAxis = height;

    graphic->graph(0)->setData(x, y);
    graphic->xAxis->setRange(0, maxAxis);
    graphic->yAxis->setRange(0, maxAxis);
    graphic->replot();

    x.clear();
    y.clear();
}

void MainWindow::on_buttonStart_clicked() //кнопка вычислить
{
    int speed = ui->speed->value();   //скорость
    int angle = ui->angle->value();   //угол
    double plotn = ui->plotnosti->value();  //плотность среды
    double radius = ui->doubleSpinBox->value();   //радиус шара
    double mas = ui->massa->value(); //масса нужна для вычисления у(х) и х с учетом сопротивления

    double coff = 0.5*0.15*PI*radius*radius*plotn; //рассчитываем коэффициент сопротивления k=0.5*C*S*ro C=0.15 S=pi*r^2 ro=1.29
    double Fsopr = coff*speed*speed; //вычисление силы сопротивления F=k*V^2
    //ui->coffec->setNum(Fsopr); //вывод силы сопротивления

    double time = 2 * speed * sin(angle * PI / 180) / G;   //время
    double distance = pow(speed, 2) * sin(2 * angle * PI / 180) / G;  //дистанция
    double height = pow(speed * sin(angle * PI / 180), 2) / (2 * G);   //высота

    ui->resultTime->setNum(time);   //вывод результатов
    ui->resultDistance->setNum(distance);
    ui->resultHeight->setNum(height);

    printGraphic(speed, angle, distance, height, Fsopr,mas);
}

void MainWindow::on_action_triggered()
{
    ui->statusbar->showMessage("Новый проект создан!");
}

void MainWindow::on_action_2_triggered()
{
    ui->statusbar->showMessage("Проект открыт!");
}

void MainWindow::on_action_3_triggered()
{
   QMessageBox::StandardButton reply = QMessageBox::question(this,"Закрытие проекта","Закрыть проект?",QMessageBox::No|QMessageBox::Yes);
   if (reply==QMessageBox::Yes)
   {
       QApplication::quit();
   }
}

void MainWindow::on_action_9_triggered()
{
    Teoria win;
    win.setModal(true);
    win.exec();
}

void MainWindow::on_action_8_triggered()
{
    ui->statusbar->showMessage("Сохранено!");
}

void MainWindow::on_action_7_triggered()
{
    ui->statusbar->showMessage("Сохранено!");
}
