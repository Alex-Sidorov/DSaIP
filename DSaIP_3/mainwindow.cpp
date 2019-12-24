#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _scene_one(new QCustomPlot()),
    _scene_two(new QCustomPlot()),
    _scene_three(new QCustomPlot()),
    _scene_four(new QCustomPlot()),
    _scene_five(new QCustomPlot()),
    _scene_six(new QCustomPlot())
{
    ui->setupUi(this);

    QVector<qreal> x;
    QVector<complex<qreal>> y;
    create_vector(COUNT,x,y);
    auto real_y = QtConcurrent::run(create_real_vector,y);

    decltype (y) result_FWT(COUNT);
    FWT(true,COUNT,y,result_FWT);
    auto real_FWT = QtConcurrent::run(create_real_vector,result_FWT);

    decltype (y) result_back_FWT(COUNT);
    FWT(false,COUNT,result_FWT,result_back_FWT);
    auto real_back_FWT = QtConcurrent::run(create_real_vector,result_back_FWT);

    decltype (y) result_DWT(COUNT);
    DWT(true,COUNT,y,result_DWT);
    auto real_DWT = QtConcurrent::run(create_real_vector,result_DWT);

    decltype (y) result_back_DWT(COUNT);
    DWT(false,COUNT,result_DWT,result_back_DWT);
    auto real_back_DWT = QtConcurrent::run(create_real_vector,result_back_DWT);

    foreach(auto item,result_DWT)
    {
        qDebug()<<item.real()<<" "<<item.imag();
    }

    qDebug()<<"********************************";

    foreach(auto item,result_FWT)
    {
        qDebug()<<item.real()<<" "<<item.imag();
    }

    ui->gridLayout->addWidget(_scene_one.get(),0,0);
    _scene_one->addGraph(_scene_one->xAxis,_scene_one->yAxis);
    _scene_one->xAxis->setRange(-2,7);
    _scene_one->yAxis->setRange(-5,7);
    _scene_one->rescaleAxes();
    _scene_one->graph(0)->setData(x,real_y.result());
    _scene_one->replot();

    ui->gridLayout->addWidget(_scene_two.get(),0,1);
    _scene_two->addGraph(_scene_two->xAxis,_scene_two->yAxis);
    _scene_two->xAxis->setRange(-2,7);
    _scene_two->yAxis->setRange(-5,7);
    _scene_two->rescaleAxes();
    _scene_two->graph(0)->setData(x,real_FWT.result());
    _scene_two->replot();

    ui->gridLayout->addWidget(_scene_three.get(),0,2);
    _scene_three->addGraph(_scene_three->xAxis,_scene_three->yAxis);
    _scene_three->xAxis->setRange(-2,7);
    _scene_three->yAxis->setRange(-5,7);
    _scene_three->rescaleAxes();
    _scene_three->graph(0)->setData(x,real_back_FWT.result());
    _scene_three->replot();

    ui->gridLayout->addWidget(_scene_four.get(),1,1);
    _scene_four->addGraph(_scene_four->xAxis,_scene_four->yAxis);
    _scene_four->xAxis->setRange(-2,7);
    _scene_four->yAxis->setRange(-5,7);
    _scene_four->rescaleAxes();
    _scene_four->graph(0)->setData(x,real_DWT.result());
    _scene_four->replot();

    ui->gridLayout->addWidget(_scene_five.get(),1,2);
    _scene_five->addGraph(_scene_five->xAxis,_scene_five->yAxis);
    _scene_five->xAxis->setRange(-2,7);
    _scene_five->yAxis->setRange(-5,7);
    _scene_five->rescaleAxes();
    _scene_five->graph(0)->setData(x,real_back_DWT.result());
    _scene_five->replot();

}

MainWindow::~MainWindow()
{
    delete ui;
}
