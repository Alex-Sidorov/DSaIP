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
constexpr auto COUNT = 8;
constexpr complex<qreal> ONE_IMAGE(0,1);

qreal get_function(qreal x);
void create_vector(int count, QVector<qreal> &x,QVector<complex<qreal>> &y);
void DFT(bool is_signed,int count, QVector<complex<qreal>> &data,QVector<complex<qreal>> &new_data);
void FFT(bool is_signed,int count, QVector<complex<qreal>> &data,QVector<complex<qreal>> &new_data);
void change_result(complex<qreal> &value);
QVector<qreal> create_abs_vector(QVector<complex<qreal>> &data);
QVector<qreal> create_arg_vector(QVector<complex<qreal>> &data);
QVector<qreal> create_real_vector(QVector<complex<qreal>> &data);

#endif // FUNCTION_H
