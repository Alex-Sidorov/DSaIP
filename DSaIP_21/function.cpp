#include <function.h>

constexpr int FIRST_CORE[3][3] = {{-1,0,1},
                                  {-2,0,2},
                                  {-1,0,1}};

constexpr int SECOND_CORE[3][3] = {{-1,-2,-1},
                                   {0,0,0},
                                   {1,2,1}};
/*constexpr int FIRST_CORE[3][3] = {{10,0,-10},
                                  {3,0,-3},
                                  {10,0,-10}};

constexpr int SECOND_CORE[3][3] = {{10,3,10},
                                   {0,0,0},
                                   {-10,-3,-10}};*/

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

void first_filter(QImage &image, int range)
{
    int width = image.width();
    int height = image.height();

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(qGray(image.pixel(j,i)) < range)
            {
                image.setPixel(j,i,qRgb(0,0,0));
            }
            else
            {
                image.setPixel(j,i,qRgb(255,255,255));
            }
        }
    }
}

void second_filter(QImage &image, int left, int right)
{
    int width = image.width();
    int height = image.height();

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if(qGray(image.pixel(j,i)) < left || qGray(image.pixel(j,i)) > right)
            {
                image.setPixel(j,i,qRgb(0,0,0));
            }
            else
            {
                image.setPixel(j,i,qRgb(255,255,255));
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


QVector<QVector<int>> image_to_core(QImage &image, const int CORE[3][3])
{
    int width = image.width();
    int height = image.height();

    QVector<QVector<int>> data;
    QVector<int> row;
    int result = 0;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {

            for (int k = - 1; k < 2; k++)
            {
                for (int l = - 1; l < 2; l++)
                {
                    int x = i + l;
                    int y = j + k;
                    if(x < 0 || y< 0 || x >= width || y >= height)
                    {
                        continue;
                    }
                    result += CORE[k+1][l+1] * qGray(image.pixel(x,y));
                }
            }
            row.push_back(result);
            result = 0;
        }
        data.push_back(row);
        row.clear();
    }
    return data;
}

void third_filter(QImage &image)
{
    auto first_result =  image_to_core(image,FIRST_CORE);
    auto second_result =  image_to_core(image,SECOND_CORE);

    int width = image.width();
    int height = image.height();

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            int value = sqrt(qPow(first_result[i][j],2) + qPow(second_result[i][j],2));
            image.setPixel(i,j,qRgb(value,value,value));
        }
    }



}

void third_filter1(QImage &image)
{
    auto first_result =  image_to_core(image,FIRST_CORE);

    int width = image.width();
    int height = image.height();

    for (int i = 0; i < width - 3; i++)
    {
        for (int j = 0; j < height - 3; j++)
        {
            int value = sqrt(qPow(first_result[i][j],2));
            image.setPixel(i+1,j+1,qRgb(value,value,value));
        }
    }
    /*double pixel_x = 0;
    double pixel_y = 0;
    QImage temp = img.copy();
    for(int x = 1 ; x <img.width()-2;x++)
    {
        for(int y = 1 ; y <img.height()-2;y++)
        {
            pixel_x = (FIRST_CORE[0][0] * qGray(img.pixel(x-1,y-1))) + (FIRST_CORE[0][1] * qGray(img.pixel(x,y-1))) + (FIRST_CORE[0][2] * qGray(img.pixel(x+1,y-1))) +
                      (FIRST_CORE[1][0] * qGray(img.pixel(x-1,y)))   + (FIRST_CORE[1][1] * qGray(img.pixel(x,y)))   + (FIRST_CORE[1][2] * qGray(img.pixel(x+1,y))) +
                      (FIRST_CORE[2][0] * qGray(img.pixel(x-1,y+1))) + (FIRST_CORE[2][1] * qGray(img.pixel(x,y+1))) + (FIRST_CORE[2][2] * qGray(img.pixel(x+1,y+1)));

            pixel_y = (SECOND_CORE[0][0] * qGray(img.pixel(x-1,y-1))) + (SECOND_CORE[0][1] * qGray(img.pixel(x,y-1))) + (SECOND_CORE[0][2] * qGray(img.pixel(x+1,y-1))) +
                      (SECOND_CORE[1][0] * qGray(img.pixel(x-1,y)))   + (SECOND_CORE[1][1] * qGray(img.pixel(x,y)))  + (SECOND_CORE[1][2] * qGray(img.pixel(x+1,y))) +
                      (SECOND_CORE[2][0] * qGray(img.pixel(x-1,y+1))) + (SECOND_CORE[2][1] * qGray(img.pixel(x,y+1))) + (SECOND_CORE[2][2] * qGray(img.pixel(x+1,y+1)));
            int v = sqrt((pixel_x*pixel_x)+(pixel_y*pixel_y));
            temp.setPixel(x,y,qRgb(v,v,v));
        }

    }
    img = temp.copy();*/


}

void third_filter2(QImage &image)
{
    auto second_result =  image_to_core(image,SECOND_CORE);

    int width = image.width();
    int height = image.height();

    for (int i = 0; i < width - 3; i++)
    {
        for (int j = 0; j < height - 3; j++)
        {
            int value = sqrt(qPow(second_result[i][j],2));
            image.setPixel(i+1,j+1,qRgb(value,value,value));
        }
    }
    /*double pixel_x = 0;
    double pixel_y = 0;
    QImage temp = img.copy();
    for(int x = 1 ; x <img.width()-2;x++)
    {
        for(int y = 1 ; y <img.height()-2;y++)
        {
            pixel_x = (FIRST_CORE[0][0] * qGray(img.pixel(x-1,y-1))) + (FIRST_CORE[0][1] * qGray(img.pixel(x,y-1))) + (FIRST_CORE[0][2] * qGray(img.pixel(x+1,y-1))) +
                      (FIRST_CORE[1][0] * qGray(img.pixel(x-1,y)))   + (FIRST_CORE[1][1] * qGray(img.pixel(x,y)))   + (FIRST_CORE[1][2] * qGray(img.pixel(x+1,y))) +
                      (FIRST_CORE[2][0] * qGray(img.pixel(x-1,y+1))) + (FIRST_CORE[2][1] * qGray(img.pixel(x,y+1))) + (FIRST_CORE[2][2] * qGray(img.pixel(x+1,y+1)));

            pixel_y = (SECOND_CORE[0][0] * qGray(img.pixel(x-1,y-1))) + (SECOND_CORE[0][1] * qGray(img.pixel(x,y-1))) + (SECOND_CORE[0][2] * qGray(img.pixel(x+1,y-1))) +
                      (SECOND_CORE[1][0] * qGray(img.pixel(x-1,y)))   + (SECOND_CORE[1][1] * qGray(img.pixel(x,y)))  + (SECOND_CORE[1][2] * qGray(img.pixel(x+1,y))) +
                      (SECOND_CORE[2][0] * qGray(img.pixel(x-1,y+1))) + (SECOND_CORE[2][1] * qGray(img.pixel(x,y+1))) + (SECOND_CORE[2][2] * qGray(img.pixel(x+1,y+1)));
            int v = sqrt((pixel_x*pixel_x)+(pixel_y*pixel_y));
            temp.setPixel(x,y,qRgb(v,v,v));
        }

    }
    img = temp.copy();*/

}

uint get_true_value_filter(QVector<uint> &values, FILTER filter)
{
    const int count = values.count();
    QVector<uint> temp(count);

    int r[count];
    int g[count];
    int b[count];

    for(int j = 0; j < count; j++)
    {
        r[j] = qRed(values[j]);
        g[j] = qGreen(values[j]);
        b[j] = qBlue(values[j]);
    }

    switch(filter)
    {
    case FILTER::MAX:
        return qRgb(*std::max_element(r,r+count),
                    *std::max_element(g,g+count),
                    *std::max_element(b,b+count));

    case FILTER::MIN:
        return qRgb(*std::min_element(r,r+count),
                    *std::min_element(g,g+count),
                    *std::min_element(b,b+count));
    case FILTER::MED:
        std::sort(r,r+count);
        std::sort(g,g+count);
        std::sort(b,b+count);
        return qRgb(r[count/2],
                    g[count/2],
                    b[count/2]);
    }

}


void fourth_filter(QImage &image, FILTER filter, int RADIUS_FILTER)
{
    const int width = image.width();
    const int height = image.height();

    //uint pixels[width][height];
    QImage temp = image.copy();

    for(int i = 0; i <width; i++)
    {
        for (int j = 0; j <height; j++)
        {
            QVector<uint> values;
            for (int k = - RADIUS_FILTER/2; k < RADIUS_FILTER/2 + 1; k++)
            {
                for (int l = - RADIUS_FILTER/2; l < RADIUS_FILTER/2 + 1; l++)
                {
                    int x = i + l;
                    int y = j + k;
                    if(x < 0 || y < 0 || x >= width || y >= height)
                    {
                        continue;
                    }
                    values.push_back(image.pixel(x,y));
                }
            }
            //pixels[i][j] = get_true_value_filter(values, filter);
            temp.setPixel(i,j,get_true_value_filter(values, filter));
        }
    }

    image = temp.copy();
    /*for(int i = 0; i <width; i++)
    {
        for (int j = 0; j <height; j++)
        {
            image.setPixel(i,j,pixels[i][j]);
        }
    }*/
}










