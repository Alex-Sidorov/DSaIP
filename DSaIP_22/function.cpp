#include <function.h>

void to_grayscale(QImage &image)
{
    int width = image.width();
    int height = image.height();

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image.setPixel(j,i,qRgb(qGray(image.pixel(j,i)),qGray(image.pixel(j,i)),qGray(image.pixel(j,i))));
        }
    }
}

bool is_alone(QImage &image, int i, int j)
{
    if(
       (!qGray(image.pixel(j-1,i)) &&
       !qGray(image.pixel(j,i-1)) &&
       !qGray(image.pixel(j+1,i)) &&
       qGray(image.pixel(j,i+1))))
    {
        return true;
    }
    return false;
}

void to_black_white(QImage &image, int firts_range, int second_range)
{
    int width = image.width();
    int height = image.height();

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width - 1; j++)
        {
            if(qGray(image.pixel(j,i)) > firts_range && qGray(image.pixel(j + 1,i)) > second_range)
            {
                image.setPixel(j,i,qRgb(255,255,255));

            }
            else
            {
               image.setPixel(j,i,qRgb(0,0,0));
            }
        }
    }

    ///////////////////
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(j == 0 || i == 0 || i + 1 == height || j + 1 == width)
            {
                continue;
            }

            if(qGray(image.pixel(j,i)) == 0  &&
                   qGray(image.pixel(j-1,i)) &&
                   qGray(image.pixel(j,i-1)) &&
                   qGray(image.pixel(j+1,i)) &&
                   qGray(image.pixel(j,i+1)))
            {
                image.setPixel(j,i,qRgb(1,1,1));
            }
            else if(qGray(image.pixel(j,i)) && is_alone(image,i,j))
            {
                image.setPixel(j,i,qRgb(0,0,0));
            }
        }
    }
}

int count_values_in_histogram(QImage &image, int value, RGBG pipe)
{
    int width = image.width();
    int height = image.height();
    int result = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            switch (pipe)
            {
            case RGBG::RED:
            {
                if(qRed(image.pixel(j,i)) == value)
                {
                    result++;
                }
                break;
            }
            case RGBG::GREEN:
            {
                if(qGreen(image.pixel(j,i)) == value)
                {
                    result++;
                }
                break;
            }
            case RGBG::BLUE:
            {
                if(qBlue(image.pixel(j,i)) == value)
                {
                    result++;
                }
                break;
            }
            case RGBG::GREY:
            {
                if(qGray(image.pixel(j,i)) == value)
                {
                    result++;
                }
                break;
            }
            }
        }
    }
    return result;
}

QVector<qreal> create_data_for_histogram(QImage &image, RGBG pipe)
{
    QVector<qreal> data(256);
    for(int i = 0; i < 256; i++)
    {
        data[i] = count_values_in_histogram(image,i, pipe);
    }
    return data;
}

void change_all_pixel(QImage &image, int b, int c, int row)
{
    int width = image.width();

    for (int i = row - 1; i <= row; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(qGray(image.pixel(j,i)) == c)
            {
                image.setPixel(j,i,qRgb(b,b,b));
            }
        }
    }
}

void change_all_pixel(QImage &image, QMap<uint,uint> &map)
{
    int width = image.width();
    int height = image.height();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image.setPixel(j,i,map.value(qGray(image.pixel(j,i)),qGray(image.pixel(j,i))));
        }
    }
}
void scan_image(QImage &image)
{
    //QMap<uint,uint> map;
    int width = image.width();
    int height = image.height();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(qGray(image.pixel(j,i)) == 255)
            {
                image.setPixel(j,i,qRgb(1,1,1));
            }
        }
    }

    int count = 1;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            if(j - 1 < 0 || i - 1 < 0 || qGray(image.pixel(j,i)) == 0)
            {
                continue;
            }


            if(qGray(image.pixel(j - 1,i)) == 0 && qGray(image.pixel(j,i - 1)) == 0)
            {

                image.setPixel(j,i,qRgb(count,count,count));
                count++;
            }
            else if(qGray(image.pixel(j - 1,i)) != 0 && qGray(image.pixel(j,i - 1)) == 0)
            {
                image.setPixel(j,i,image.pixel(j - 1,i));
            }
            else if(qGray(image.pixel(j - 1,i)) == 0 && qGray(image.pixel(j,i - 1)) != 0)
            {
                image.setPixel(j,i,image.pixel(j,i - 1));
            }
            else
            {
                image.setPixel(j,i,image.pixel(j,i - 1));
                //map.insert(qGray(image.pixel(j - 1, i)),qGray(image.pixel(j, i - 1)));
                change_all_pixel(image,qGray(image.pixel(j, i - 1)),qGray(image.pixel(j - 1, i)),i);
            }

        }
    }
    //qDebug()<<map;
    //change_all_pixel(image,map);
}


QMap<int,qreal> get_area_things(QImage &image)
{
    QMap<int,qreal> temp;
    int width = image.width();
    int height = image.height();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int value = qGray(image.pixel(j,i));
            if(value)
            {
                if(temp.find(value)!=temp.end())
                {
                    temp.insert(value,temp.find(value).value()+1);
                }
                else
                {
                    temp.insert(value,1);
                }
            }
        }
    }
    return temp;
}

QMap<int,qreal> get_perimetr_things(QImage &image)
{
    QMap<int,qreal> temp;
    int width = image.width();
    int height = image.height();
    int max = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(max < qGray(image.pixel(j,i)))
            {
                max = qGray(image.pixel(j,i));
            }
        }
    }
    for(int i = 1; i <= max; i++)
    {
        int perimetr = get_perimetr(image,i);
        if(perimetr)
        {
            temp.insert(i,perimetr);
        }
    }
    return temp;
}

void delete_trash(QImage &image)
{
    auto areas = get_area_things(image);
    auto perimetrs = get_perimetr_things(image);

    auto area_it = areas.begin();
    auto perimetr_it = perimetrs.begin();
    while(area_it != areas.end())
    {
        if(area_it.value() <= perimetr_it.value())
        {
            int count = 0;
            int x = 0;
            int y = 0;
            while(count < perimetr_it.value())
            {
                if(qGray(image.pixel(x,y)) == area_it.key())
                {
                    image.setPixel(x,y,qRgb(0,0,0));
                    count++;
                }
                x++;
                if(x == image.width())
                {
                    y++;
                    x = 0;
                }
            }
        }
        area_it++;
        perimetr_it++;
    }
}
int get_perimetr(QImage &image, int number)
{
    int result = 0;
    int width = image.width();
    int height = image.height();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(qGray(image.pixel(j,i)) != number)
            {
                continue;
            }
            bool flag = false;
            for (int k = -1; k < 2 && !flag; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    int y = i + k;
                    int x = j + l;
                    if(x < 0 || y < 0 || x >= width || y >= height)
                    {
                        continue;
                    }
                    if(qGray(image.pixel(x,y))!=number)
                    {
                        result++;
                        flag = !flag;
                        break;
                    }
                }
            }
        }
    }
    return result;
}

QMap<int,qreal> get_compactness(QImage &image)
{
    QMap<int,qreal> temp;
    auto perimetrs = get_perimetr_things(image);
    auto areas = get_area_things(image);
    auto it_perimetrs = perimetrs.begin();
    auto it_areas = areas.begin();
    while(it_perimetrs != perimetrs.end() && it_areas != areas.end())
    {
        temp.insert(it_areas.key(),static_cast<qreal>(qPow(it_perimetrs.value(),2))/it_areas.value());
        it_perimetrs++;
        it_areas++;
    }
    return temp;
}

qreal get_centre_mass(QImage &image, bool is_x, int index, QMap<int,qreal> &areas)
{
    qreal result = 0;
    int width = image.width();
    int height = image.height();
    bool is_end = false;
    bool is_start = false;
    if(is_x)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if(qGray(image.pixel(j,i)) == index)
                {
                    is_end = false;
                    is_start = true;
                    result += j;
                }
            }
            if(is_end && is_start)
            {
                break;
            }
        }
    }
    else
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if(qGray(image.pixel(j,i)) == index)
                {
                    is_end = false;
                    is_start = true;
                    result += i;
                }
            }
            if(is_end && is_start)
            {
                break;
            }
        }
    }
    return result/areas.value(index);
}

std::tuple<qreal,qreal> get_centre_mass(QImage &image, int index)
{
    auto map = get_area_things(image);
    return std::make_tuple(get_centre_mass(image,true,index,map),get_centre_mass(image,false,index,map));
}

qreal get_dcm(QImage &image, int i, int j, int index)
{
    qreal result = 0;
    int width = image.width();
    int height = image.height();
    auto centre_mass = get_centre_mass(image,index);
    for (int k = 0; k < height; k++)
    {
        for (int l = 0; l < width; l++)
        {
            if(qGray(image.pixel(l,k)) == index)
            {
                result += qPow((l - std::get<0>(centre_mass)),i) *
                        qPow((k - std::get<1>(centre_mass)),j);
            }
        }
    }
    return result;
}

qreal get_elongation(QImage &image, int index)
{
    auto m20 = get_dcm(image,2,0,index);
    auto m02 = get_dcm(image,0,2,index);
    auto m11 = get_dcm(image,1,1,index);
    return (m20 + m02 + sqrt(qPow((m20 - m02),2) + 4*qPow(m11,2))) /
           (m20 + m02 - sqrt(qPow((m20 - m02),2) + 4*qPow(m11,2)));
}

QMap<int,qreal> get_elongation(QImage &image)
{
    QMap<int,qreal> temp;
    int width = image.width();
    int height = image.height();
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int value = qGray(image.pixel(j,i));
            if(value && temp.value(value, -1.0) == -1.0)
            {
                temp.insert(value,get_elongation(image,value));
            }
        }
    }
    return temp;
}

void normalize_parametrs(QVector<QList<qreal>> &parametrs)
{
    int count = parametrs.count();
    for (int i = 0; i <count; i++)
    {
        auto max = *std::max_element(parametrs[i].begin(),parametrs[i].end());
        std::transform(parametrs[i].begin(),parametrs[i].end(),parametrs[i].begin(),
        [&](qreal value){
            return value/max;
        });
    }
}

QVector<QVector<qreal>> create_centre(int count_centre, int count_parametrs)
{
    srand(time(nullptr));
    QVector<QVector<qreal>> result;
    auto fun = [](int count)
    {
        QVector<qreal> values(count);
        for (int i = 0; i <count; i++)
        {
            values[i] = static_cast<qreal>(rand())/RAND_MAX;
        }
        return values;
    };

    for (int i = 0; i <count_centre; i++)
    {
        result.push_back(fun(count_parametrs));
    }
    return result;
}

qreal get_distance(const QVector<qreal> &first, const QVector<qreal> &second)
{
    qreal result = 0;
    for (int i = 0; i < first.count(); i++)
    {
        result += pow(first[i] - second[i],2);
    }
    return sqrt(result);
}

////
/// \brief create_clasters
/// \param centres Вектор центров. У каждого центра вектор параметров(площадь,периметр,компактность,удлиненность)
/// \param parametrs Параметры(площадь,периметр,компактность,удлиненность) каждого объекта.
///
/// \return Вектор кластеров. Один кластер - QMap : ключ - номер объекта,
///                           значение - вектор параметров (площадь,периметр,компактность,удлиненность)
///
QVector<QMap<int,QVector<qreal>>> create_clasters(QVector<QVector<qreal>> &centres,QVector<QList<qreal>> &parametrs,QList<int> &indexes)
{
    QVector<QMap<int,QVector<qreal>>> result(centres.count());
    for (int i = 0; i < indexes.count();i++)
    {
        QVector<qreal> param_obj(parametrs.count());
        for(int j = 0; j < param_obj.count();j++)
        {
            param_obj[j] = parametrs[j][i];
        }

        qreal min = -1.0;
        int index_claster = 0;
        for(int j = 0; j < centres.count();j++)
        {
            qreal temp = get_distance(centres[j],param_obj);
            if(temp < min || min == -1.0)
            {
                min = temp;
                index_claster = j;
            }
        }
        result[index_claster].insert(indexes[i],param_obj);
    }
    return result;

}

QVector<QVector<qreal>> create_new_centre(int count_param, QVector<QMap<int,QVector<qreal>>> &clasters,QVector<QVector<qreal>> &old_centre)
{
    QVector<QVector<qreal>> result(clasters.count());
    for(int i = 0; i < clasters.count(); i++)
    {
        auto temp = clasters[i].values();
        if(!temp.count())
        {
            result[i] = old_centre[i];
            continue;
        }
        for (int j = 0; j < count_param; j++)
        {
            qreal value = 0.0;
            for (int k = 0; k < temp.count(); k++)
            {
                value += temp[k][j];
            }
            result[i].push_back(value/temp.count());
        }

    }
    return result;
}

bool is_change_centres(QVector<QVector<qreal>> &old_centre, QVector<QVector<qreal>> &new_centre)
{
    QVector<qreal> temp(old_centre[0].size());
    for(int i = 0; i < old_centre.count(); i++)
    {
        qreal first = get_distance(temp,new_centre[i]);
        qreal second = get_distance(temp,old_centre[i]);
        if(std::min(first,second)/std::max(first,second) < 0.8)
        {
            return true;
        }
    }
    return false;
}

QVector<QMap<int,QVector<qreal>>> clastarization(int count_clasters, QList<int> &indexes, QVector<QList<qreal>> &parametrs)
{
    QVector<QMap<int,QVector<qreal>>> result;
    auto new_centres = create_centre(count_clasters,parametrs.count());
    decltype (new_centres) old_centres;
    do
    {
        old_centres = new_centres;
        result = create_clasters(new_centres,parametrs,indexes);
        new_centres = create_new_centre(parametrs.count(),result,old_centres);
    }while(is_change_centres(old_centres,new_centres));
    return result;
}









