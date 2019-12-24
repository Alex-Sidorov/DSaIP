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

    decltype (y) result_DFT(COUNT);
    DFT(true,COUNT,y,result_DFT);
    auto create_abs_y = QtConcurrent::run(create_abs_vector,result_DFT);
    auto create_arg_y= QtConcurrent::run(create_arg_vector,result_DFT);

    //qDebug()<<result_DFT;

    decltype (y) result_back_DFT(COUNT);
    DFT(false,COUNT,result_DFT,result_back_DFT);
    auto real_back_DFT = QtConcurrent::run(create_real_vector,result_back_DFT);

    decltype (y) result_FFT(COUNT);
    FFT(true,COUNT,y,result_FFT);
    auto create_abs_y_FFT = QtConcurrent::run(create_abs_vector,result_FFT);
    auto create_arg_y_FFT= QtConcurrent::run(create_arg_vector,result_FFT);

    ui->gridLayout->addWidget(_scene_one.get(),0,0);
    _scene_one->addGraph(_scene_one->xAxis,_scene_one->yAxis);
    _scene_one->xAxis->setRange(-2,7);
    _scene_one->yAxis->setRange(-5,7);
    _scene_one->rescaleAxes();

    //real_y.waitForFinished();
    _scene_one->graph(0)->setData(x,real_y.result());
    _scene_one->replot();

    ui->gridLayout->addWidget(_scene_two.get(),0,1);
    _scene_two->addGraph(_scene_two->xAxis,_scene_two->yAxis);
    _scene_two->xAxis->setRange(-2,7);
    _scene_two->yAxis->setRange(-5,7);
    _scene_two->rescaleAxes();

    //create_abs_y.waitForFinished();
    _scene_two->graph(0)->setData(x,create_abs_y.result());
    _scene_two->replot();

    ui->gridLayout->addWidget(_scene_three.get(),0,2);
    _scene_three->addGraph(_scene_three->xAxis,_scene_three->yAxis);
    _scene_three->xAxis->setRange(-2,7);
    _scene_three->yAxis->setRange(-5,7);
    _scene_three->rescaleAxes();

    //create_arg_y.waitForFinished();
    _scene_three->graph(0)->setData(x,create_arg_y.result());
    _scene_three->replot();

    ui->gridLayout->addWidget(_scene_four.get(),1,0);
    _scene_four->addGraph(_scene_four->xAxis,_scene_four->yAxis);
    _scene_four->xAxis->setRange(-2,7);
    _scene_four->yAxis->setRange(-5,7);
    _scene_four->rescaleAxes();

    //real_back_DFT.waitForFinished();
    _scene_four->graph(0)->setData(x,real_back_DFT.result());
    _scene_four->replot();

    ui->gridLayout->addWidget(_scene_five.get(),1,1);
    _scene_five->addGraph(_scene_five->xAxis,_scene_five->yAxis);
    _scene_five->xAxis->setRange(-2,7);
    _scene_five->yAxis->setRange(-5,7);
    _scene_five->rescaleAxes();

    //create_abs_y_FFT.waitForFinished();
    _scene_five->graph(0)->setData(x,create_abs_y_FFT.result());
    _scene_five->replot();

    ui->gridLayout->addWidget(_scene_six.get(),1,2);
    _scene_six->addGraph(_scene_six->xAxis,_scene_six->yAxis);
    _scene_six->xAxis->setRange(-2,7);
    _scene_six->yAxis->setRange(-5,7);
    _scene_six->rescaleAxes();

    //create_arg_y_FFT.waitForFinished();
    _scene_six->graph(0)->setData(x,create_arg_y_FFT.result());
    _scene_six->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
