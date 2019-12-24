#include "function.h"

QVector<complex<qreal>> mul_vector(QVector<complex<qreal>> &first,QVector<complex<qreal>> &second)
{
    QVector<complex<qreal>> result(first.size());
    std::transform(first.begin(),first.end(),second.begin(),result.begin(),
                   [](complex<qreal> &one, complex<qreal> &two){return one*two;});
    return result;
}

QVector<complex<qreal>> mul_vector(QVector<complex<qreal>> &first,QVector<complex<qreal>> &&second)
{
    QVector<complex<qreal>> result(first.size());
    std::transform(first.begin(),first.end(),second.begin(),result.begin(),
                   [](complex<qreal> &one, complex<qreal> &two){return one*two;});
    return result;
}

QVector<complex<qreal>> get_complex_conjugate_vector(QVector<complex<qreal>> &vector)
{
    QVector<complex<qreal>> result(vector.size());
    std::transform(vector.begin(),vector.end(),result.begin(),
                   [](complex<qreal> &value){return complex<qreal>(value.real(),-1*value.imag());});
    return result;
}

QVector<complex<qreal>> get_convolution_vector(QVector<complex<qreal>> &first, QVector<complex<qreal>> &second)
{
    QVector<complex<qreal>> result(first.size());
    auto count = first.size();
    for (auto i = 0; i <count; i++)
    {
        for (auto j = 0; j < count; j++)
        {
            result[i] += first[j] * second[i<j ? count-(j-i) : i-j];
        }
        result[i]/=count;
    }
    return result;
}

QVector<complex<qreal>> get_correlation_vector(QVector<complex<qreal>> &first, QVector<complex<qreal>> &second)
{
    QVector<complex<qreal>> result(first.size());
    auto count = first.size();
    for (auto i = 0; i <count; i++)
    {
        for (auto j = 0; j < count; j++)
        {
            result[i] += first[j] * second[i+j<count ? i+j : i+j-count];
        }
        result[i] /= count;
    }
    return result;
}

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
        temp.push_back(arg(data[i]));
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

qreal get_function_Z(qreal x)
{
    return cos(x);
}

qreal get_function_Y(qreal x)
{
    return sin(3*x);
}


void create_vector(int count,QVector<qreal> &x,QVector<complex<qreal>> &y,std::function<qreal(qreal)>func)
{
    double temp = 0;
    for(int i=0;i<count;i++)
    {
        temp=i*2*M_PI/count;
        x.push_back(temp);
        y.push_back(func(temp));
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
        if(is_signed)
        {
            new_data[i] /= count;
        }
    }
}

void FFT(bool is_signed,int count, QVector<complex<qreal>> &data,QVector<complex<qreal>> &new_data)
{
    if(count==1)
    {
        new_data=data;
        return;
    }

    complex<qreal>value_signed = is_signed ? -1:1;
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
        new_data[2*i] = first[i];
        new_data[2*i+1] = second[i];
        if(is_signed)
        {
            new_data[2*i]/=2;
            new_data[2*i+1]/=2;
        }
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
