#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

#include <function.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void fill_graph_histogram(QSharedPointer<QCustomPlot> &graph, QVector<qreal> &data)
{

    graph->setMinimumSize(400,500);
    auto max_value = QtConcurrent::run([&]
    {
        qreal result = 0;
        foreach(auto item,data)
        {
            if(result<item)
            {
                result = item;
            }
        }
        return result;
    });

    auto scene = new QCPBars(graph->xAxis, graph->yAxis);

    QPen pen;
    pen.setWidthF(5);
    pen.setColor(QColor(50, 50, 100));
    graph->graph(0)->setPen(pen);
    graph->graph(0)->setBrush(QColor(50, 50, 250, 70));

    QVector<qreal> ticks(256);
    for(int i = 0; i <256; i++)
    {
        ticks[i] = i;
    }

    scene->addData(ticks,data);
    scene->setWidth(1);

    QSharedPointer<QCPAxisTickerFixed> fixedTicker(new QCPAxisTickerFixed);
    fixedTicker->setTickStep(1);

    graph->xAxis->setTicker(fixedTicker);
    graph->xAxis->setTickLength(0, 10);
    graph->xAxis->grid()->setVisible(true);
    graph->xAxis->setRange(0,256);

    graph->yAxis->setRange(0,max_value.result()*1.5);
    graph->yAxis->setPadding(2);
    graph->yAxis->grid()->setSubGridVisible(true);

    QPen gridPen;
    gridPen.setStyle(Qt::SolidLine);
    gridPen.setColor(QColor(0, 0, 0, 25));
    graph->yAxis->grid()->setPen(gridPen);
    gridPen.setStyle(Qt::DotLine);
    graph->yAxis->grid()->setSubGridPen(gridPen);
}

void MainWindow::on_enter_clicked()
{
    if(!first_image.load("D:\\projectQt\\DSaIP_21\\image3.jpg"))
    //if(!first_image.load("D:\\projectQt\\DSaIP_21\\image2.png"))
    {
        qDebug() << "error load";
        return;
    }
    ui->image_one->setPixmap(QPixmap::fromImage(first_image));
    second_image = first_image.copy();

    to_grayscale(second_image);
    ui->image_two->setPixmap(QPixmap::fromImage(second_image));

    histogram1.reset(new QCustomPlot);
    ui->histograms->addWidget(histogram1.get());
    histogram1->addGraph();
    auto data_for_histogram = create_data_for_histogram(second_image,RGBG::GREY);
    fill_graph_histogram(histogram1,data_for_histogram);
    histogram1->replot();

    third_image = second_image.copy();
    first_filter(third_image, ui->first_range->value());
    ui->image_three->setPixmap(QPixmap::fromImage(third_image));

    histogram2.reset(new QCustomPlot);
    ui->histograms->addWidget(histogram2.get());
    histogram2->addGraph();
    data_for_histogram = create_data_for_histogram(third_image,RGBG::GREY);
    fill_graph_histogram(histogram2,data_for_histogram);
    histogram2->replot();

    fourth_image = second_image.copy();
    second_filter(fourth_image, ui->second_range_1->value(), ui->second_range_2->value());
    ui->image_four->setPixmap(QPixmap::fromImage(fourth_image));

    histogram3.reset(new QCustomPlot);
    ui->histograms->addWidget(histogram3.get());
    histogram3->addGraph();
    data_for_histogram = create_data_for_histogram(fourth_image, RGBG::GREY);
    fill_graph_histogram(histogram3,data_for_histogram);
    histogram3->replot();

    fiveth_image = second_image.copy();
    third_filter(fiveth_image);
    /*third_filter(fiveth_image);
    third_filter(fiveth_image);*/
    ui->image_five->setPixmap(QPixmap::fromImage(fiveth_image));

    histogram4.reset(new QCustomPlot);
    ui->histograms->addWidget(histogram4.get());
    histogram4->addGraph();
    data_for_histogram = create_data_for_histogram(fiveth_image,RGBG::GREY);
    fill_graph_histogram(histogram4,data_for_histogram);
    histogram4->replot();

    sixth_image = first_image.copy();
    fourth_filter(sixth_image,FILTER::MED,ui->radius->value());////////////////
    ui->image_six->setPixmap(QPixmap::fromImage(sixth_image));

    histogram5.reset(new QCustomPlot);
    ui->histograms->addWidget(histogram5.get());
    histogram5->addGraph();
    data_for_histogram = create_data_for_histogram(sixth_image,RGBG::GREY);
    fill_graph_histogram(histogram5,data_for_histogram);
    histogram5->replot();
    /////////
    histogram6.reset(new QCustomPlot);
    ui->rgb_histograms1->addWidget(histogram6.get());
    histogram6->addGraph();
    data_for_histogram = create_data_for_histogram(first_image,RGBG::RED);
    fill_graph_histogram(histogram6,data_for_histogram);
    histogram6->replot();

    histogram7.reset(new QCustomPlot);
    ui->rgb_histograms1->addWidget(histogram7.get());
    histogram7->addGraph();
    data_for_histogram = create_data_for_histogram(first_image,RGBG::GREEN);
    fill_graph_histogram(histogram7,data_for_histogram);
    histogram7->replot();

    histogram8.reset(new QCustomPlot);
    ui->rgb_histograms1->addWidget(histogram8.get());
    histogram8->addGraph();
    data_for_histogram = create_data_for_histogram(first_image,RGBG::BLUE);
    fill_graph_histogram(histogram8,data_for_histogram);
    histogram8->replot();

    histogram9.reset(new QCustomPlot);
    ui->rgb_histograms2->addWidget(histogram9.get());
    histogram9->addGraph();
    data_for_histogram = create_data_for_histogram(sixth_image,RGBG::RED);
    fill_graph_histogram(histogram9,data_for_histogram);
    histogram9->replot();

    histogram10.reset(new QCustomPlot);
    ui->rgb_histograms2->addWidget(histogram10.get());
    histogram10->addGraph();
    data_for_histogram = create_data_for_histogram(sixth_image,RGBG::GREEN);
    fill_graph_histogram(histogram10,data_for_histogram);
    histogram10->replot();

    histogram11.reset(new QCustomPlot);
    ui->rgb_histograms2->addWidget(histogram11.get());
    histogram11->addGraph();
    data_for_histogram = create_data_for_histogram(sixth_image,RGBG::BLUE);
    fill_graph_histogram(histogram11,data_for_histogram);
    histogram11->replot();

    sixth_image = second_image.copy();
    third_filter1(sixth_image);
    ui->image_six->setPixmap(QPixmap::fromImage(sixth_image));

    seventh_image = second_image.copy();
    third_filter2(seventh_image);
    ui->image_seven->setPixmap(QPixmap::fromImage(seventh_image));

}
