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

    QVector<qreal> x_first;
    QVector<complex<qreal>> y_first;
    create_vector(COUNT,x_first,y_first,get_function_Y);
    auto real_y_first = QtConcurrent::run(create_real_vector,y_first);

    QVector<qreal> x_second;
    QVector<complex<qreal>> y_second;
    create_vector(COUNT,x_second,y_second,get_function_Z);
    auto real_y_second = QtConcurrent::run(create_real_vector,y_second);

    decltype (y_first) result_FFT_first(COUNT);
    FFT(true,COUNT,y_first,result_FFT_first);

    decltype (y_second) result_FFT_second(COUNT);
    FFT(true,COUNT,y_second,result_FFT_second);

    auto conjugate_vector = get_complex_conjugate_vector(result_FFT_first);
    auto result_mul_vectors_correlation = mul_vector(result_FFT_second,conjugate_vector);
    decltype (y_first) result_back_FFT_correlation(COUNT);
    FFT(false,COUNT,result_mul_vectors_correlation,result_back_FFT_correlation);
    auto real_back_FFT_correlation = QtConcurrent::run(create_real_vector,result_back_FFT_correlation);

    auto result_correlation = get_correlation_vector(y_first,y_second);
    auto real_result_correlation = QtConcurrent::run(create_real_vector,result_correlation);

    auto result_mul_vectors_convolution = mul_vector(result_FFT_first,result_FFT_second);
    decltype (y_first) result_back_FFT_convolution(COUNT);
    FFT(false,COUNT,result_mul_vectors_convolution,result_back_FFT_convolution);
    auto real_back_FFT_convolution = QtConcurrent::run(create_real_vector,result_back_FFT_convolution);

    auto result_convolution = get_convolution_vector(y_first,y_second);
    auto real_result_convolution = QtConcurrent::run(create_real_vector,result_convolution);

    ui->gridLayout->addWidget(_scene_one.get(),0,0);
    _scene_one->addGraph(_scene_one->xAxis,_scene_one->yAxis);
    _scene_one->xAxis->setRange(-2,7);
    _scene_one->yAxis->setRange(-5,7);
    _scene_one->rescaleAxes();
    _scene_one->graph(0)->setData(x_first,real_y_first.result());
    _scene_one->replot();

    ui->gridLayout->addWidget(_scene_two.get(),1,0);
    _scene_two->addGraph(_scene_two->xAxis,_scene_two->yAxis);
    _scene_two->xAxis->setRange(-2,7);
    _scene_two->yAxis->setRange(-5,7);
    _scene_two->rescaleAxes();
    _scene_two->graph(0)->setData(x_second,real_y_second.result());
    _scene_two->replot();

    ui->gridLayout->addWidget(_scene_three.get(),0,1);
    _scene_three->addGraph(_scene_three->xAxis,_scene_three->yAxis);
    _scene_three->xAxis->setRange(-2,7);
    _scene_three->yAxis->setRange(-1/pow(10,15),1/pow(10,15));
    _scene_three->rescaleAxes();
    _scene_three->graph(0)->setData(x_first,real_back_FFT_correlation.result());
    _scene_three->replot();

    ui->gridLayout->addWidget(_scene_four.get(),1,1);
    _scene_four->addGraph(_scene_four->xAxis,_scene_four->yAxis);
    _scene_four->xAxis->setRange(-2,7);
    _scene_four->yAxis->setRange(-1/pow(10,15),1/pow(10,15));
    _scene_four->rescaleAxes();
    _scene_four->graph(0)->setData(x_first,real_result_correlation.result());
    _scene_four->replot();

    ui->gridLayout->addWidget(_scene_five.get(),0,2);
    _scene_five->addGraph(_scene_five->xAxis,_scene_five->yAxis);
    _scene_five->xAxis->setRange(-2,7);
    _scene_five->yAxis->setRange(-1/pow(10,15),1/pow(10,15));
    _scene_five->rescaleAxes();
    _scene_five->graph(0)->setData(x_first,real_back_FFT_convolution.result());
    _scene_five->replot();

    ui->gridLayout->addWidget(_scene_six.get(),1,2);
    _scene_six->addGraph(_scene_six->xAxis,_scene_six->yAxis);
    _scene_six->xAxis->setRange(-2,7);
    _scene_six->yAxis->setRange(-1/pow(10,15),1/pow(10,15));
    _scene_six->rescaleAxes();
    _scene_six->graph(0)->setData(x_first,real_result_convolution.result());
    _scene_six->replot();
}

MainWindow::~MainWindow()
{
    delete ui;
}
