#include "function.h"

QVector<qreal> temp_s;
qreal get_function(qreal x)
{
    //return sin(3*x)+cos(x);
    return sin(6*x);
}

qreal get_noize(qreal x)
{
    //return sin(3*x)+cos(x);
    return cos(x);
}

void add_noise_in_signal(QVector<complex<qreal>> &data)
{
    QVector<complex<qreal>> temp;
    create_vector(COUNT,temp_s,temp,get_noize);
    for (int i = 0; i <data.size();i++)
    {
        data[i] = data[i] + temp[i];
    }
}

void create_vector(int count,QVector<qreal> &x,QVector<complex<qreal>> &y,std::function<qreal(qreal)>func)
{
    double temp = 0;
    for(int i=0;i<count;i++)
    {
        temp=i*4*M_PI/count;
        x.push_back(temp);
        y.push_back(func(temp));
    }
    temp_s = x;
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

QVector<qreal> create_arg_vector(QVector<complex<qreal>> &data)
{
    QVector<qreal> temp;
    for (int i = 0; i < COUNT; i++)
    {
        temp.push_back(abs(data[i]));
    }
    return temp;
}

QVector<complex<qreal>> get_convolution_vector(QVector<complex<qreal>> &first, QVector<complex<qreal>> &second)
{
    QVector<complex<qreal>> result(second.size());
    auto count = second.size();
    for (auto i = 0; i <count; i++)
    {
        for (auto j = 0; j < first.size(); j++)
        {
            result[i] += first[j] * second[i<j ? count-(j-i) : i-j];
        }
        //result[i]/=count;
    }
    return result;
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

QVector<complex<qreal>> HIF(QVector<complex<qreal>> &data, qreal fs,qreal fz,qreal discr)
{
    auto x = exp(-2*M_PI*((fs+fz)/(2*discr)));
    auto a0 = (1+x)/2;
    auto a1= -1*a0;
    auto b1 = x;

    auto count = data.size();
    QVector<complex<qreal>> result(count);
    for (auto i = 1; i < count; i++)
    {
        result[i] = a0*data[i] + a1*data[i-1] + b1*result[i-1];
    }
    return result;
}

QVector<complex<qreal>> LIF(QVector<complex<qreal>> &data, qreal fs,qreal fz,qreal discr,int degree)
{
    auto filter = RP_filter(false,fs,fz,discr, degree);
    return get_convolution_vector(filter,data);
}

QVector<complex<qreal>> RP_filter(bool RP, qreal fs,qreal fz,qreal discr,int degree)
{
    auto LF = LH_filter(true,fs,fz,discr,degree);
    auto HF = LH_filter(false,fs,fz,discr,degree);
    QVector<complex<qreal>> result(degree);
    std::transform(LF.begin(),LF.end(),HF.begin(),result.begin(),
                   [RP](complex<qreal> &first,complex<qreal>&second)
    {
        auto sign = RP ? 1 : -1;
        return (first.real() + second.real())*sign;
    });
    return result;
}

QVector<complex<qreal>> LH_filter(bool LH, qreal fs,qreal fz,qreal discr,int degree)
{
    auto ferquence = (fs+fz)/(2*discr);
    QVector<complex<qreal>> F(degree);
    auto sum = 0.0;
    for (auto i = 0; i < degree; i++)
    {
        if(i!=degree/2)
        {
            F[i] = sin(2*M_PI*ferquence*(i-degree/2))/(i-degree/2);
            //F[i] = sin(2*M_PI*ferquence*i)/(M_PI*i);
            F[i] *= 0.54-0.46*cos(2*M_PI*i/degree);
        }
        else
        {
            F[i] = 2*M_PI*ferquence;
        }
        //F[i] *= 0.54-0.46*cos(2*M_PI*i/degree);
        sum +=F[i].real();
    }

    auto sign = LH ? 1 : -1;
    for(auto i = 0; i <F.count(); i++)
    {
        F[i]/=sum;
        if(i%2==0)
            F[i]*=sign;
    }

    return F;
}

