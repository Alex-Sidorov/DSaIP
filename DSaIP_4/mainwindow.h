#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qcustomplot.h"
#include <QMainWindow>
#include "function.h"
#include <QFuture>
#include <QtConcurrent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSharedPointer<QCustomPlot> _scene_one;
    QSharedPointer<QCustomPlot> _scene_two;
    QSharedPointer<QCustomPlot> _scene_three;
    QSharedPointer<QCustomPlot> _scene_four;
    QSharedPointer<QCustomPlot> _scene_five;
    QSharedPointer<QCustomPlot> _scene_six;
    QSharedPointer<QCustomPlot> _scene_seven;
    QSharedPointer<QCustomPlot> _scene_eight;
    QSharedPointer<QCustomPlot> _scene_nine;
    QSharedPointer<QCustomPlot> _scene_teen;
};

#endif // MAINWINDOW_H
