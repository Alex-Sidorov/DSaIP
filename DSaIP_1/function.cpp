#include "function.h"

QVector<qreal> create_abs_vector(QVector<complex<qreal>> &data)
{
    QVector<qreal> temp;
    for (int i = 0; i < data.count(); i++)
    {
        temp.push_back(abs(data[i]));
    }
    return temp;
}

QVector<qreal> create_arg_vector(QVector<complex<qreal>> &data)
{
    QVector<qreal> temp;
    for (int i = 0; i < data.count(); i++)
    {
        //temp.push_back(arg(data[i]));
        temp.push_back(atan2(data[i].imag(),data[i].real()));
    }
    return temp;
}

QVector<qreal> create_real_vector(QVector<complex<qreal>> &data)
{
    QVector<qreal> temp;
    for (int i = 0; i < COUNT; i++)
    {
        temp.push_back(data[i].real());
    }
    return temp;
}

qreal get_function(qreal x)
{
    return sin(x)+cos(x);
}

void create_vector(int count,QVector<qreal> &x,QVector<complex<qreal>> &y)
{
        double temp = 0;
        for(int i=0;i<count;i++)
        {
            temp=i*2*M_PI/count;
            x.push_back(temp);
            y.push_back(get_function(temp));
        }
}


void DFT(bool is_signed,int count, QVector<complex<qreal>> &data,QVector<complex<qreal>> &new_data)
{
    complex<qreal>value_signed = is_signed ? 1:-1;
    for(auto i = 0; i <count; i++)
    {
        for(auto j = 0; j <count; j++)
        {
            new_data[i]+=exp(value_signed*ONE_IMAGE*
                                  static_cast<decltype(ONE_IMAGE)>(i)*
                                  static_cast<decltype(ONE_IMAGE)>(j)*
                                  static_cast<decltype(ONE_IMAGE)>(M_PI)*2.0/
                                  static_cast<decltype(ONE_IMAGE)>(count))*data[j];
        }
        new_data[i]/=sqrt(count);
        change_result(new_data[i]);
        /*if(is_signed)
        {
            new_data[i]/=count;
        }*/
    }
}

void FFT(bool is_signed,int count, QVector<complex<qreal>> &data,QVector<complex<qreal>> &new_data)
{
    if(count==1)
    {
        new_data=data;
        return;
    }

    complex<qreal>value_signed = is_signed ? 1:-1;
    auto W=complex<qreal>(1,0);
    auto Wn=exp(value_signed*ONE_IMAGE*
                     static_cast<decltype(ONE_IMAGE)>(M_PI)*2.0/
                     static_cast<decltype(ONE_IMAGE)>(count));

    QVector<complex<qreal>> odd(count/2);
    QVector<complex<qreal>> even(count/2);
    for(int i=0;i<count/2;i++)
    {
           odd[i] = data[i] + data[i + count / 2];
           even[i] = (data[i] - data[i + count / 2]) * W;
           W *= Wn;
    }
    decltype(odd) first(count/2);
    decltype(odd) second(count/2);
    FFT(is_signed,count/2,odd,first);
    FFT(is_signed,count/2,even,second);
    for (int i = 0; i < count/2; i++)
    {
        new_data[2*i] = first[i]/sqrt(2);
        new_data[2*i+1] = second[i]/sqrt(2);
        change_result(new_data[2*i]);
        change_result(new_data[2*i+1]);
    }
}

void change_result(complex<qreal> &value)
{
    if(value.real()<PRECISION && value.real()>-PRECISION)
    {
        value.real(0);
    }
    if(value.imag()<PRECISION && value.imag()>-PRECISION)
    {
        value.imag(0);
    }
}
