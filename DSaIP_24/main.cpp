#include <QCoreApplication>
#include <vector>
#include <fstream>
#include <string>
#include <math.h>
#include <tuple>
#include <iostream>
#include <iomanip>
#include <future>

using layer = std::vector<double>;
using matrix = std::vector<layer>;
using perseptrons = std::tuple<matrix,layer>;

layer proc_layer_matrix(layer &lay, perseptrons &values)
{
    layer result(std::get<0>(values).size());
    for(int i = 0; i <std::get<0>(values).size(); i++)
    {
        for(int j = 0; j < lay.size(); j++)
        {
            result[i] += lay[j] * std::get<0>(values)[i][j];
        }
    }
    for(int j = 0; j < result.size(); j++)
    {
        result[j] +=  std::get<1>(values)[j];
    }
    return result;
}

layer get_obj(const char *name_file, int size_obj)
{
    std::fstream file(name_file, std::ios_base::in);
    if(!file.is_open())
    {
        return {};
    }
    std::vector<double> result(size_obj);
    std::string line;
    int index = 0;
    while(std::getline(file,line))
    {
        for(int i = 0; i < line.size(); i++)
        {
            result[index] = line[i] - '0';
            index++;
        }
    }
    return result;
}

layer get_error_network(layer &out, layer &etalon)
{
    layer result(out.size());
    for(int i = 0; i < out.size(); i++)
    {
        result[i] = etalon[i] - out[i];
    }
    return result;
}

bool if_error(layer &out, layer &etalon, double D)
{
    auto errors = get_error_network(out,etalon);
    return *std::max_element(errors.begin(),errors.end()) >= D;
}

void activation(layer &lay)
{
    for(int j = 0; j < lay.size(); j++)
    {
        lay[j] = 1.0/(1.0 + exp(-lay[j]));
    }
}

layer get_error_hide_layer(layer &out, layer &errors, matrix &out_width)
{
    layer result(out_width[0].size());
    for (int i = 0; i < out_width[0].size(); i++)
    {
        for(int j = 0; j < out_width.size(); j++)
        {
            result[i] += errors[j]*out[j]*(1 - out[j])*out_width[j][i];
        }
    }
    return result;
}

void change_out_width(matrix &width, layer &hide, layer &out, layer &errors, double speed)
{
    for (int i = 0; i < width.size(); i++)//персептороны
    {
        for(int j = 0; j <width[i].size(); j++)//связи
        {
            width[i][j] += speed*out[i]*(1 - out[i])*errors[i]*hide[j];
        }
    }
}

void change_out_coef(layer &coef, layer &out, layer &errors, double speed)
{
    for(int i = 0; i < coef.size(); i++)
    {
        coef[i] += speed*out[i]*(1 - out[i])*errors[i];
    }
}

void change_hide_width(matrix &width, layer &in, layer &out_hide, layer &errors, double speed)
{
    for (int i = 0; i < width.size(); i++)//персептроны
    {
        for(int j = 0; j <width[i].size(); j++)//связи
        {
            width[i][j] += speed*out_hide[i]*(1 - out_hide[i])*errors[i]*in[j];
        }
    }
}

void change_hide_coef(layer &coef, layer &out, layer &errors, double speed)
{
    for(int i = 0; i < coef.size(); i++)
    {
        coef[i] += speed*out[i]*(1 - out[i])*errors[i];
    }
}

int lerning(layer &data, perseptrons &hide, perseptrons &out, layer &etalon, double D, double a_speed, double b_speed)
{
    int count = 0;
    layer exit_net(std::get<0>(out).size());
    while(true)
    {
        layer first_exit = proc_layer_matrix(data,hide);
        activation(first_exit);
        exit_net = proc_layer_matrix(first_exit,out);
        activation(exit_net);

        if(!if_error(exit_net,etalon,D))
        {
            break;
        }
        auto errors = get_error_network(exit_net,etalon);
        auto errors_hide = get_error_hide_layer(first_exit,errors,std::get<0>(out));
        change_hide_width(std::get<0>(hide),data,first_exit,errors_hide,b_speed);
        change_hide_coef(std::get<1>(out),first_exit,errors,b_speed);
        change_out_width(std::get<0>(out),first_exit,exit_net,errors,a_speed);
        change_out_coef(std::get<1>(out),exit_net,errors,a_speed);
        count++;
    }
    return count;
}

void init_width(perseptrons &hide, perseptrons &out, int count_in, int count_hide, int count_out)
{
    matrix *width_hide = &std::get<0>(hide);
    layer *coef_hide = &std::get<1>(hide);
    matrix *width_out = &std::get<0>(out);
    layer *coef_out = &std::get<1>(out);

    coef_out->resize(count_out,0);
    auto f1 = std::async(std::launch::async,[&]()
    {
        for (int i = 0; i < coef_out->size(); i++)
        {
            (*coef_out)[i] = static_cast<double>(rand())/(RAND_MAX/2)-1;
        }
    });

    coef_hide->resize(count_hide,0);
    auto f2 = std::async(std::launch::async,[&]()
    {
        for (int i = 0; i < coef_hide->size(); i++)
        {
            (*coef_hide)[i] = static_cast<double>(rand())/(RAND_MAX/2)-1;
        }
    });

    width_out->resize(count_out);
    auto f3 = std::async(std::launch::async,[&]()
    {
        for (int i = 0; i < width_out->size(); i++)
        {
            (*width_out)[i].resize(count_hide,0);
            for (int j = 0; j < (*width_out)[i].size(); j++)
            {
                (*width_out)[i][j] = static_cast<double>(rand())/(RAND_MAX/2)-1;
            }
        }
    });

    width_hide->resize(count_hide);
    auto f4 = std::async(std::launch::async,[&]()
    {
        for (int i = 0; i < width_hide->size(); i++)
        {
            (*width_hide)[i].resize(count_in,0);
            for (int j = 0; j < (*width_hide)[i].size(); j++)
            {
                (*width_hide)[i][j] = static_cast<double>(rand())/(RAND_MAX/2)-1;
            }
        }
    });
    f1.wait();
    f2.wait();
    f3.wait();
    f4.wait();
}

void print_obj(layer &obj)
{
    auto temp = obj;
    int width = sqrt(obj.size());
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < width; j++)
        {
            std::cout << temp[i*width + j];
        }
        std::cout<<std::endl;
    }
}

void add_noize(int level_noize, layer &obj)
{
    for (int i = 0; i < level_noize; i++)
    {
        int x = rand()%static_cast<int>(sqrt(obj.size()));
        int y = rand()%static_cast<int>(sqrt(obj.size()));
        double value = obj[x*sqrt(obj.size())+y];
        obj[x*sqrt(obj.size())+y] = value ? 0 : 1;
    }
}


int main(int argc, char *argv[])
{
    srand(time(0)+5);

    QCoreApplication a(argc, argv);
    auto first = get_obj("o1.txt",36);
    layer first_etalon = {1,0,0,0,0};
    auto second = get_obj("o2.txt",36);
    layer second_etalon = {0,1,0,0,0};
    auto third = get_obj("o3.txt",36);
    layer third_etalon = {0,0,1,0,0};
    auto fourth = get_obj("o4.txt",36);
    layer fourth_etalon = {0,0,0,1,0};
    auto fiveth = get_obj("o5.txt",36);
    layer fiveth_etalon = {0,0,0,0,1};

    while(true)
    {
        perseptrons hide;
        perseptrons out;
        double a_speed = 0.5;
        double b_speed = 0.5;

        int size_hide_layer = 0;
        std::cout << std::endl << "SIZE HIDE LAYER:";
        std::cin >> size_hide_layer;
        if(size_hide_layer == -1)
        {
            break;
        }
        init_width(hide,out,36,size_hide_layer,5);
        int count = 10;
        for (int i = 0; i < 2000; i++)
        {
            count = lerning(first,hide,out,first_etalon,0.05,a_speed,b_speed);
            count = lerning(second,hide,out,second_etalon,0.05,a_speed,b_speed);
            count = lerning(third,hide,out,third_etalon,0.05,a_speed,b_speed);
            count = lerning(fourth,hide,out,fourth_etalon,0.05,a_speed,b_speed);
            count = lerning(fiveth,hide,out,fiveth_etalon,0.05,a_speed,b_speed);
        }

        while(true)
        {

            auto first_with_noize = first;
            auto second_with_noize = second;
            auto third_with_noize = third;
            auto fourth_with_noize = fourth;
            auto fiveth_with_noize = fiveth;

            int level_noize = 20;
            std::cout << "LEVEL NOIZE:";
            std::cin >> level_noize;
            if(level_noize == -1)
            {
                break;
            }


            std::cout<<std::endl << std::setfill('-') << std::setw(100) << " " << std::endl;

            add_noize(level_noize,first_with_noize);
            add_noize(level_noize,second_with_noize);
            add_noize(level_noize,third_with_noize);
            add_noize(level_noize,fourth_with_noize);
            add_noize(level_noize,fiveth_with_noize);

            auto res1 = proc_layer_matrix(first_with_noize,hide);
            activation(res1);
            auto res2 = proc_layer_matrix(res1,out);
            activation(res2);

            print_obj(first);
            std::cout<<std::endl;
            print_obj(first_with_noize);
            for (int i = 0; i < res2.size();i++)
            {
                std::cout << std::endl << i << '.' << res2[i] << std::endl;
            }
            std::cout<<std::endl << std::setfill('-') << std::setw(100) << " " << std::endl;

            res1 = proc_layer_matrix(second_with_noize,hide);
            activation(res1);
            res2 = proc_layer_matrix(res1,out);
            activation(res2);

            print_obj(second);
            std::cout<<std::endl;
            print_obj(second_with_noize);
            for (int i = 0; i < res2.size();i++)
            {
                std::cout << std::endl << i << '.' << res2[i] << std::endl;
            }
            std::cout<<std::endl << std::setfill('-') << std::setw(100) << " " << std::endl;

            res1 = proc_layer_matrix(third_with_noize,hide);
            activation(res1);
            res2 = proc_layer_matrix(res1,out);
            activation(res2);

            print_obj(third);
            std::cout<<std::endl;
            print_obj(third_with_noize);
            for (int i = 0; i < res2.size();i++)
            {
                std::cout << std::endl << i << '.' << res2[i] << std::endl;
            }
            std::cout<<std::endl << std::setfill('-') << std::setw(100) << " " << std::endl;


            res1 = proc_layer_matrix(fourth_with_noize,hide);
            activation(res1);
            res2 = proc_layer_matrix(res1,out);
            activation(res2);

            print_obj(fourth);
            std::cout<<std::endl;
            print_obj(fourth_with_noize);
            for (int i = 0; i < res2.size();i++)
            {
                std::cout << std::endl << i << '.' << res2[i] << std::endl;
            }
            std::cout<<std::endl << std::setfill('-') << std::setw(100) << " " << std::endl;

            res1 = proc_layer_matrix(fiveth_with_noize,hide);
            activation(res1);
            res2 = proc_layer_matrix(res1,out);
            activation(res2);

            print_obj(fiveth);
            std::cout<<std::endl;
            print_obj(fiveth_with_noize);
            for (int i = 0; i < res2.size();i++)
            {
                std::cout << std::endl << i << '.' << res2[i] << std::endl;
            }
            std::cout<<std::endl << std::setfill('-') << std::setw(100) << " " << std::endl;
        }
    }
    return a.exec();
}
