#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPixmap>
#include <qcustomplot.h>
#include <QSharedPointer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_enter_clicked();

private:
    Ui::MainWindow *ui;

    QImage first_image;
    QImage second_image;
    QImage third_image;
    QImage fourth_image;
    QImage fiveth_image;
    QImage sixth_image;
    QImage seventh_image;

    QSharedPointer<QCustomPlot> histogram1;
    QSharedPointer<QCustomPlot> histogram2;
    QSharedPointer<QCustomPlot> histogram3;
    QSharedPointer<QCustomPlot> histogram4;
    QSharedPointer<QCustomPlot> histogram5;

    QSharedPointer<QCustomPlot> histogram6;
    QSharedPointer<QCustomPlot> histogram7;
    QSharedPointer<QCustomPlot> histogram8;
    QSharedPointer<QCustomPlot> histogram9;
    QSharedPointer<QCustomPlot> histogram10;
    QSharedPointer<QCustomPlot> histogram11;
};

#endif // MAINWINDOW_H
