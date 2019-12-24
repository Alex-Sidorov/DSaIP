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
constexpr auto COUNT = 128;
constexpr complex<qreal> ONE_IMAGE(0,1);

qreal get_function(qreal x);
qreal get_noize(qreal x);
void create_vector(int count,QVector<qreal> &x,
                   QVector<complex<qreal>> &y,
                   std::function<qreal(qreal)>func);
QVector<qreal> create_real_vector(QVector<complex<qreal>> &data);
QVector<qreal> create_arg_vector(QVector<complex<qreal>> &data);
void DFT(bool is_signed,int count, QVector<complex<qreal>> &data,QVector<complex<qreal>> &new_data);
void FFT(bool is_signed,int count, QVector<complex<qreal>> &data,QVector<complex<qreal>> &new_data);
void change_result(complex<qreal> &value);
QVector<complex<qreal>> get_convolution_vector(QVector<complex<qreal>> &first,
                                               QVector<complex<qreal>> &second,bool);

void add_noise_in_signal(QVector<complex<qreal>> &data);

QVector<complex<qreal>> LIF(QVector<complex<qreal>> &data, qreal fs,qreal fz,qreal discr,int degree);
QVector<complex<qreal>> HIF(QVector<complex<qreal>> &data, qreal fs,qreal fz,qreal discr);
QVector<complex<qreal>> LH_filter(bool LH, qreal fs,qreal fz,qreal discr,int degree);
QVector<complex<qreal>> RP_filter(bool RP, qreal fs,qreal fz,qreal discr,int degree);

#endif // FUNCTION_H
