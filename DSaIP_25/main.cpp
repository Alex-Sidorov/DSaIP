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

layer proc_layer_matrix(layer &lay, matrix &values)
{
    layer result(values.size());
    for(int i = 0; i < values.size(); i++)
    {
        for(int j = 0; j < lay.size(); j++)
        {
            result[i] += lay[j] * values[i][j];
        }
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
    std::transform(errors.begin(),errors.end(),errors.begin(),[](double value)
    {
        return abs(value);
    });
    return *std::max_element(errors.begin(),errors.end()) >= D;
}

layer activation(layer &x, matrix &t, layer &s)
{
    layer res(s.size());
    for(int i = 0; i < s.size(); i++)
    {
        double temp = 0;
        for (int j = 0; j < t[i].size(); j++)
        {
            temp += pow(x[j] - t[i][j],2);
        }
        res[i] = exp(-temp/pow(s[i],2));
    }
    return res;
}

void change_out_width(matrix &width, layer &hide, layer &out, layer &errors, double speed)
{
    for (int i = 0; i < width.size(); i++)//персептороны
    {
        for(int j = 0; j <width[i].size(); j++)//связи
        {
            //width[i][j] += speed*out[i]*(1 - out[i])*errors[i]*hide[j];
            width[i][j] += speed*errors[i]*hide[j];
        }
    }
}

int lerning(layer &data, matrix &out, layer &etalon, matrix &t, layer &s, double D, double a_speed)
{
    int count = 0;
    layer exit_net(out.size());
    while(true)
    {
        auto first_exit = activation(data,t,s);
        exit_net = proc_layer_matrix(first_exit,out);
        if(!if_error(exit_net,etalon,D))
        {
            break;
        }
        auto errors = get_error_network(exit_net,etalon);
        change_out_width(out,first_exit,exit_net,errors,a_speed);
        count++;
    }
    return count;
}

void init_width(matrix &out, int count_out, int count_hide)
{

    out.resize(count_out);
    for (int i = 0; i < out.size(); i++)
    {
        out[i].resize(count_hide,0);
        for (int j = 0; j < out[i].size(); j++)
        {
            out[i][j] = static_cast<double>(rand())/RAND_MAX;
        }
    }
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
    srand(time(0));

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

    int count_template = 5;

    while(true)
    {
        matrix out;
        matrix t;
        layer s;
        double a_speed = 0.1;
        double d = 0.1;

        init_width(out,count_template,count_template);
        t.push_back(first);
        t.push_back(second);
        t.push_back(third);
        t.push_back(fourth);
        t.push_back(fiveth);
        s.resize(count_template,(3.0*sqrt(count_template)/2)-(sqrt(count_template)/2));

        for (int i = 0; i < 5; i++)
        {
            lerning(first,out,first_etalon,t,s,d,a_speed);
            lerning(second,out,second_etalon,t,s,d,a_speed);
            lerning(third,out,third_etalon,t,s,d,a_speed);
            lerning(fourth,out,fourth_etalon,t,s,d,a_speed);
            lerning(fiveth,out,fiveth_etalon,t,s,d,a_speed);
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


            std::cout<<std::endl << std::setfill('#') << std::setw(100) << " " << std::endl;

            add_noize(level_noize,first_with_noize);
            add_noize(level_noize,second_with_noize);
            add_noize(level_noize,third_with_noize);
            add_noize(level_noize,fourth_with_noize);
            add_noize(level_noize,fiveth_with_noize);

            auto res1 = activation(first_with_noize,t,s);
            auto res2 = proc_layer_matrix(res1,out);

            print_obj(first);
            std::cout<<std::endl;
            print_obj(first_with_noize);
            for (int i = 0; i < res2.size();i++)
            {
                std::cout << std::endl << i << '.' << res2[i] << std::endl;
            }
            std::cout<<std::endl << std::setfill('-') << std::setw(100) << " " << std::endl;

            res1 = activation(second_with_noize,t,s);
            res2 = proc_layer_matrix(res1,out);

            print_obj(second);
            std::cout<<std::endl;
            print_obj(second_with_noize);
            for (int i = 0; i < res2.size();i++)
            {
                std::cout << std::endl << i << '.' << res2[i] << std::endl;
            }
            std::cout<<std::endl << std::setfill('-') << std::setw(100) << " " << std::endl;

            res1 = activation(third_with_noize,t,s);
            res2 = proc_layer_matrix(res1,out);

            print_obj(third);
            std::cout<<std::endl;
            print_obj(third_with_noize);
            for (int i = 0; i < res2.size();i++)
            {
                std::cout << std::endl << i << '.' << res2[i] << std::endl;
            }
            std::cout<<std::endl << std::setfill('-') << std::setw(100) << " " << std::endl;


            res1 = activation(fourth_with_noize,t,s);
            res2 = proc_layer_matrix(res1,out);

            print_obj(fourth);
            std::cout<<std::endl;
            print_obj(fourth_with_noize);
            for (int i = 0; i < res2.size();i++)
            {
                std::cout << std::endl << i << '.' << res2[i] << std::endl;
            }
            std::cout<<std::endl << std::setfill('-') << std::setw(100) << " " << std::endl;

            res1 = activation(fiveth_with_noize,t,s);
            res2 = proc_layer_matrix(res1,out);

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
