#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <function.h>
#include <QFile>
#include <QTextStream>
#include <QVector>

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

void MainWindow::on_enter_clicked()
{
    while(ui->table->rowCount())
    {
        ui->table->removeRow(0);
    }
    if(!first_image.load("D:\\projectQt\\DSaIP_22\\image.jpg"))
    {
        qDebug() << "error load";
        return;
    }

    ui->first->setPixmap(QPixmap::fromImage(first_image));
    second_image = first_image.copy();

    to_black_white(second_image,ui->first_range_box->value(),ui->second_range_box->value());
    ui->second->setPixmap(QPixmap::fromImage(second_image));


    third_image = second_image.copy();
    scan_image(third_image);
    ui->third->setPixmap(QPixmap::fromImage(third_image));

    delete_trash(third_image);
    ///////////////////
    /*QFile file("D:\\projectQt\\DSaIP_22\\file.txt");
    QTextStream in(&file);
    file.open(QIODevice::WriteOnly);
    if(!file.isOpen())
    {
        qDebug() << "error open";
        return;
    }
    int width = third_image.width();
    int height = third_image.height();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            in << qGray(third_image.pixel(j,i));
        }
        in<<"\n";
    }*/
    /////////////////

    QVector<QList<qreal>> parametrs;
    auto areas = get_area_things(third_image);
    for(auto i = areas.begin(); i != areas.end(); i++)
    {
        int row_count = ui->table->rowCount();
        ui->table->insertRow(row_count);
        ui->table->setItem(row_count,0,new QTableWidgetItem(QString::number(i.key())));
        ui->table->setItem(row_count,1,new QTableWidgetItem(QString::number(i.value())));
    }
    parametrs.push_back(areas.values());
    QList<int> indexes = areas.keys();

    auto perimters = get_perimetr_things(third_image);
    int index = 0;
    foreach(auto item, perimters)
    {
        ui->table->setItem(index,2,new QTableWidgetItem(QString::number(item)));
        index++;
    }
    parametrs.push_back(perimters.values());

    auto compactness = get_compactness(third_image);
    index = 0;
    foreach(auto item, compactness)
    {
        ui->table->setItem(index,3,new QTableWidgetItem(QString::number(item)));
        index++;
    }
    parametrs.push_back(compactness.values());

    auto elongation = get_elongation(third_image);
    index = 0;
    foreach(auto item, elongation)
    {
        ui->table->setItem(index,4,new QTableWidgetItem(QString::number(item)));
        index++;
    }
    parametrs.push_back(elongation.values());
    normalize_parametrs(parametrs);

    auto result = clastarization(ui->count_clasters->value(),indexes,parametrs);
    QMap<int,int> number_cluster;
    for(int j = 0; j < result.count();j++)
    {
        auto temp = result[j].keys();
        foreach(auto item, temp)
        {
            number_cluster.insert(item,j);
        }
    }
    index = 0;
    foreach(auto item, number_cluster)
    {
        ui->table->setItem(index,5,new QTableWidgetItem(QString::number(item)));
        index++;
    }
}



