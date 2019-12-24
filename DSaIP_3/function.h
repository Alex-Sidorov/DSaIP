#ifndef FUNCTION_H
#define FUNCTION_H

#include <math.h>
#include <QVector>
#include <qobject.h>
#include <complex>
#include <QDebug>

using std::complex;
using std::exp;

constexpr auto PRECISION = 0.0000000000001;
constexpr auto COUNT = 16;

qreal get_function(qreal x);
void create_vector(int count, QVector<qreal> &x,QVector<complex<qreal>> &y);
void DWT(bool is_signed,int count, QVector<complex<qreal>> &data,QVector<complex<qreal>> &new_data);
void FWT(bool is_signed,int count, QVector<complex<qreal>> &data,QVector<complex<qreal>> &new_data);
void change_result(complex<qreal> &value);
QVector<qreal> create_real_vector(QVector<complex<qreal>> &data);
QVector<QVector<int>> create_matrix(int count);

#endif // FUNCTION_H
