#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QImage>

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
};

#endif // MAINWINDOW_H
