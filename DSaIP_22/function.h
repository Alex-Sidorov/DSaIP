#ifndef FUNCTION_H
#define FUNCTION_H

#include <QImage>
#include <QtMath>
#include <QtConcurrent>
#include <QFuture>
#include <QMap>

enum class RGBG{RED = 0, GREEN, BLUE, GREY};

void to_grayscale(QImage &);
void to_black_white(QImage &image, int firts_range, int second_range);
void scan_image(QImage &image);
void change_all_pixel(QImage &image, int b, int c, int row);
void delete_trash(QImage &image);
int count_values_in_histogram(QImage &image, int value, RGBG pipe);
QVector<qreal> create_data_for_histogram(QImage &image, RGBG pipe);
QMap<int,qreal> get_area_things(QImage &image);
int get_perimetr(QImage &image, int number);
QMap<int,qreal> get_perimetr_things(QImage &image);
QMap<int,qreal> get_compactness(QImage &image);
qreal get_dcm(QImage &image, int i, int j, int index);
qreal get_elongation(QImage &image, int index);
QMap<int,qreal> get_elongation(QImage &image);
qreal get_centre_mass(QImage &image, bool is_x, int index, QMap<int,qreal> &areas);
std::tuple<qreal,qreal> get_centre_mass(QImage &image, int index);
QVector<QMap<int,QVector<qreal>>> create_clasters(QVector<QVector<qreal>> &centres,QVector<QList<qreal>> &parametrs,QList<int> &indexes);
void normalize_parametrs(QVector<QList<qreal>> &parametrs);
QVector<QVector<qreal>> create_centre(int count_centre, int count_parametrs);
QVector<QMap<int,QVector<qreal>>> clastarization(int count_clasters, QList<int> &indexes, QVector<QList<qreal>> &parametrs);

#endif // FUNCTION_H
