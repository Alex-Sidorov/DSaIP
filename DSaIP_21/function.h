#ifndef FUNCTION_H
#define FUNCTION_H

#include <QImage>
#include <QtMath>
#include <QtConcurrent>
#include <QFuture>

enum class FILTER{MIN = 0,MAX,MED};
enum class RGBG{RED = 0, GREEN, BLUE, GREY};

void to_grayscale(QImage &);
void first_filter(QImage &image, int range);
void second_filter(QImage &image, int left, int right);
void third_filter(QImage &image);
void third_filter1(QImage &image);
void third_filter2(QImage &image);
void fourth_filter(QImage &image,FILTER, int RADIUS_FILTER);
QVector<QVector<int>> image_to_core(QImage &image, const int CORE[3][3]);
int count_values_in_histogram(QImage &image, int value, RGBG pipe);
QVector<qreal> create_data_for_histogram(QImage &image, RGBG pipe);

#endif // FUNCTION_H
