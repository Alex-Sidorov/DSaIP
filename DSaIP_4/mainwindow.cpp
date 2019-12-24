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
    _scene_six(new QCustomPlot()),
    _scene_seven(new QCustomPlot()),
    _scene_eight(new QCustomPlot()),
    _scene_nine(new QCustomPlot()),
    _scene_teen(new QCustomPlot())
{
    ui->setupUi(this);

    QVector<qreal> x;
    QVector<complex<qreal>> y;
    create_vector(COUNT,x,y,get_function);
    auto real_y_first = QtConcurrent::run(create_real_vector,y);


    QVector<qreal> x_noize;
    QVector<complex<qreal>> y_noize;
    create_vector(COUNT,x_noize,y_noize,get_noize);
    auto real_y_noize_first = QtConcurrent::run(create_real_vector,y_noize);

    decltype (y) result_FFT_signal_noize(COUNT);
    DFT(true,COUNT,y,result_FFT_signal_noize);
    auto real_FFT_signal_noize = QtConcurrent::run(create_arg_vector,result_FFT_signal_noize);

    decltype (y) result_FFT_signal(COUNT);
    DFT(true,COUNT,y,result_FFT_signal);
    auto real_FFT_signal = QtConcurrent::run(create_arg_vector,result_FFT_signal);

    QVector<complex<qreal>> signal_with_noize = y;
    add_noise_in_signal(signal_with_noize);
    auto real_signal_with_noize = QtConcurrent::run(create_real_vector,signal_with_noize);

    decltype (y) result_FFT_signal_with_noize(COUNT);
    DFT(true,COUNT,signal_with_noize,result_FFT_signal_with_noize);
    auto real_FFT_signal_with_noize = QtConcurrent::run(create_arg_vector,result_FFT_signal_with_noize);

    QVector<complex<qreal>> signal_after_LIF = LIF(signal_with_noize,20,100,1000,COUNT/2);
    //auto real_signal_after_LIF = QtConcurrent::run(create_real_vector,signal_after_LIF);

    /*for (auto i = 0; i < COUNT; i++)
    {
        signal_after_LIF[i] = signal_with_noize[i]+signal_after_LIF[i];
    }*/
    //signal_after_LIF = get_convolution_vector(signal_after_LIF,signal_with_noize,true);
    auto real_signal_after_LIF = QtConcurrent::run(create_real_vector,signal_after_LIF);

    decltype (y) result_FFT_signal_after_LIF(COUNT);
    DFT(true,COUNT,signal_after_LIF,result_FFT_signal_after_LIF);
    auto real_FFT_signal_after_LIF = QtConcurrent::run(create_arg_vector,result_FFT_signal_after_LIF);

    QVector<complex<qreal>> signal_after_HIF = HIF(signal_with_noize,10,70,1000);
    auto real_signal_after_HIF = QtConcurrent::run(create_real_vector,signal_after_HIF);

    decltype (y) result_FFT_signal_after_HIF(COUNT);
    DFT(true,COUNT,signal_after_HIF,result_FFT_signal_after_HIF);
    auto real_FFT_signal_after_HIF = QtConcurrent::run(create_arg_vector,result_FFT_signal_after_HIF);

    ui->gridLayout->addWidget(_scene_one.get(),0,0);
    _scene_one->addGraph(_scene_one->xAxis,_scene_one->yAxis);
    _scene_one->xAxis->setRange(0,20);
    _scene_one->yAxis->setRange(-5,10);
    _scene_one->rescaleAxes();
    _scene_one->graph(0)->setData(x,real_y_first.result());
    _scene_one->replot();

    ui->gridLayout->addWidget(_scene_two.get(),1,0);
    _scene_two->addGraph(_scene_two->xAxis,_scene_two->yAxis);
    _scene_two->xAxis->setRange(0,20);
    _scene_two->yAxis->setRange(-3,3);
    _scene_two->rescaleAxes();
    _scene_two->graph(0)->setData(x,real_FFT_signal.result());
    _scene_two->replot();

    ui->gridLayout->addWidget(_scene_nine.get(),0,4);
    _scene_nine->addGraph(_scene_nine->xAxis,_scene_nine->yAxis);
    _scene_nine->xAxis->setRange(0,20);
    _scene_nine->yAxis->setRange(-5,10);
    _scene_nine->rescaleAxes();
    _scene_nine->graph(0)->setData(x,real_y_noize_first.result());
    _scene_nine->replot();

    ui->gridLayout->addWidget(_scene_teen.get(),1,4);
    _scene_teen->addGraph(_scene_teen->xAxis,_scene_teen->yAxis);
    _scene_teen->xAxis->setRange(0,20);
    _scene_teen->yAxis->setRange(-3,3);
    _scene_teen->rescaleAxes();
    _scene_teen->graph(0)->setData(x,real_FFT_signal_noize.result());
    _scene_teen->replot();

    ui->gridLayout->addWidget(_scene_three.get(),0,1);
    _scene_three->addGraph(_scene_three->xAxis,_scene_three->yAxis);
    _scene_three->xAxis->setRange(0,20);
    _scene_three->yAxis->setRange(-5,10);
    _scene_three->rescaleAxes();
    _scene_three->graph(0)->setData(x,real_signal_with_noize.result());
    _scene_three->replot();

    ui->gridLayout->addWidget(_scene_four.get(),1,1);
    _scene_four->addGraph(_scene_four->xAxis,_scene_four->yAxis);
    _scene_four->xAxis->setRange(0,20);
    _scene_four->yAxis->setRange(-3,3);
    _scene_four->rescaleAxes();
    _scene_four->graph(0)->setData(x,real_FFT_signal_with_noize.result());
    _scene_four->replot();

    ui->gridLayout->addWidget(_scene_five.get(),0,2);
    _scene_five->addGraph(_scene_five->xAxis,_scene_five->yAxis);
    _scene_five->xAxis->setRange(0,20);
    _scene_five->yAxis->setRange(-5,10);
    _scene_five->rescaleAxes();
    _scene_five->graph(0)->setData(x,real_signal_after_LIF.result());
    _scene_five->replot();

    ui->gridLayout->addWidget(_scene_six.get(),1,2);
    _scene_six->addGraph(_scene_six->xAxis,_scene_six->yAxis);
    _scene_six->xAxis->setRange(0,20);
    _scene_six->yAxis->setRange(-3,3);
    _scene_six->rescaleAxes();
    _scene_six->graph(0)->setData(x,real_FFT_signal_after_LIF.result());
    _scene_six->replot();

    ui->gridLayout->addWidget(_scene_seven.get(),0,3);
    _scene_seven->addGraph(_scene_seven->xAxis,_scene_seven->yAxis);
    _scene_seven->xAxis->setRange(0,20);
    _scene_seven->yAxis->setRange(-5,10);
    _scene_seven->rescaleAxes();
    _scene_seven->graph(0)->setData(x,real_signal_after_HIF.result());
    _scene_seven->replot();

    ui->gridLayout->addWidget(_scene_eight.get(),1,3);
    _scene_eight->addGraph(_scene_eight->xAxis,_scene_eight->yAxis);
    _scene_eight->xAxis->setRange(0,20);
    _scene_eight->yAxis->setRange(-3,3);
    _scene_eight->rescaleAxes();
    _scene_eight->graph(0)->setData(x,real_FFT_signal_after_HIF.result());
    _scene_eight->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
