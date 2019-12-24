#include "function.h"

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
    return sin(3*x)+cos(x);
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

QVector<QVector<int>> create_matrix(int count)
{
    if(count == 0)
    {
        return {{1}};
    }

    auto prev_matrix = create_matrix(count - 1);
    decltype (prev_matrix) not_prev_matrix(prev_matrix.count());

    std::transform(prev_matrix.begin(),prev_matrix.end(),not_prev_matrix.begin(),
                   [](QVector<int> row)
    {
        std::transform(row.begin(),row.end(),row.begin(),
                       [](int value)
        {
            return -1*value;
        });
        return row;
    });

    decltype (prev_matrix) result(prev_matrix.count()*2);
    for (auto i = 0; i < prev_matrix.count()*2; i++)
    {
        if(i >= prev_matrix.count())
        {
            result[i] = prev_matrix[i % prev_matrix.count()] +
                    not_prev_matrix[i % prev_matrix.count()];
        }
        else
        {
            result[i] = prev_matrix[i] + prev_matrix[i];
        }
    }
    return result;
}


void DWT(bool is_signed,int count, QVector<complex<qreal>> &data,QVector<complex<qreal>> &new_data)
{
    QVector<int> indexs = {0,8,12,4,6,14,10,2,3,11,15,7,5,13,9,1};

    auto matrix = create_matrix(static_cast<int>(log2(COUNT)));
    new_data.reserve(count);
    for (auto i = 0; i < count; i++)
    {
        for (auto j = 0; j < count; j++)
        {
            new_data[i] += data[j] * static_cast<qreal>(matrix[i][j]);
        }
        if(is_signed)
        {
            new_data[i] /= count;
        }
        //change_result(new_data[i]);
    }
}

void FWT(bool is_signed,int count, QVector<complex<qreal>> &data,QVector<complex<qreal>> &new_data)
{
    if(count==1)
    {
        new_data=data;
        return;
    }

    QVector<complex<qreal>> odd(count/2);
    QVector<complex<qreal>> even(count/2);
    for(int i=0;i<count/2;i++)
    {
           odd[i] = data[i] + data[i + count / 2];
           even[i] = data[i] - data[i + count / 2];

    }

    decltype(odd) first(count/2);
    decltype(odd) second(count/2);

    FWT(is_signed,count/2,odd,first);
    FWT(is_signed,count/2,even,second);

    for (int i = 0; i < count/2; i++)
    {
        if(is_signed)
        {
            new_data[i] = first[i]/2.0;
            new_data[i+count/2] = second[i]/2.0;
        }
        else
        {
            new_data[i] = first[i];
            new_data[i+count/2] = second[i];
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
