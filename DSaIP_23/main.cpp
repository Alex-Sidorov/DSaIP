#include <QCoreApplication>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <math.h>

using matrix = std::vector<std::vector<double>>;

constexpr int SIZE_OBJ = 100;

/*matrix first_obj = {
    {0,0,0,0,0,0,0,0,0,0},
    {0,0,0,1,1,1,1,1,0,0},
    {0,0,0,1,1,1,1,1,0,0},
    {0,0,0,1,1,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0},
    {0,0,0,1,1,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0}
};*/

void change_values(std::vector<double> &obj, bool flag)
{
    if(flag)
    {
        for(int i = 0; i <obj.size(); i++)
        {
            obj[i] = obj[i] == -1 ? 0: obj[i];
        }
    }
    else
    {
        for(int i = 0; i <obj.size(); i++)
        {
            obj[i] = obj[i] == 0 ? -1: obj[i];
        }
    }
}

matrix create_matrix(std::vector<double> &first, std::vector<double> &second)
{
    if(first.size() != second.size())
    {
        return {};
    }
    auto size = first.size();
    matrix result(size);
    for (auto i = 0; i < size; i++)
    {
        result[i].resize(size,0);
        for (int j = 0; j <size; j++)
        {
            int temp = i != j;
            result[i][j] = temp*first[i]*second[j];
        }
    }
    return result;
}

std::vector<double> get_obj(std::string &name_file)
{
    std::fstream file(name_file, std::ios_base::in);
    if(!file.is_open())
    {
        return {};
    }
    std::vector<double> result(SIZE_OBJ);
    std::string line;
    int index = 0;
    while(std::getline(file,line))
    {
        for(int i = 0; i < line.size(); i++)
        {
            result[index] = line[i] - '0';
            index++;
        }
        line.clear();
    }
    change_values(result,false);
    return result;
}

template <class Type>
Type sum_matrix(Type &last)
{
    return last;
}

template <class Type, class... Other>
Type sum_matrix(Type &first, Other& ... other)
{
    auto result = sum_matrix(other...);
    auto temp = first;
    for (auto i = 0; i < SIZE_OBJ; i++)
    {
        for (int j = 0; j <SIZE_OBJ; j++)
        {
            temp[i][j] += result[i][j];
        }
    }
    return temp;
}

std::vector<double> mul_vector_matrix(std::vector<double> &vec, matrix mat)
{
    if(vec.size() != mat.size())
    {
        return {};
    }
    std::vector<double> result(SIZE_OBJ);
    for (int j = 0; j < SIZE_OBJ; j++)
    {
        for (int i = 0; i < SIZE_OBJ; i++)
        {
            result[j] += vec[i]*mat[i][j];
        }
    }
    return result;
}

void add_noize(int level_noize, std::vector<double> &obj)
{
    for (int i = 0; i < level_noize; i++)
    {
        int x = rand()%static_cast<int>(sqrt(SIZE_OBJ));
        int y = rand()%static_cast<int>(sqrt(SIZE_OBJ));
        obj[x*sqrt(SIZE_OBJ)+y] *= -1;
    }
}

void normalize(std::vector<double> &vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        vec[i] = vec[i] < 0 ? -1 : 1;
    }
}

std::vector<double> work_network(const matrix &width,const std::vector<double> &obj)
{
    std::vector<double> result;
    auto temp = obj;
    do
    {
        result = temp;
        temp = mul_vector_matrix(result,width);
        normalize(temp);
    }while(!std::equal(result.begin(),result.end(),temp.begin()));
    return result;
}

void print_obj(std::vector<double> &obj)
{
    auto temp = obj;
    change_values(temp,true);
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



int main(int argc, char *argv[])
{
    srand(time(0));
    QCoreApplication a(argc, argv);

    std::string name_file = "o1.txt";
    auto first = get_obj(name_file);
    name_file = "o2.txt";
    auto second = get_obj(name_file);
    name_file = "o3.txt";
    auto third = get_obj(name_file);
    auto first_matrix = create_matrix(first,first);
    auto second_matrix = create_matrix(second,second);
    auto third_matrix = create_matrix(third,third);

    auto widthes = sum_matrix(first_matrix,second_matrix,third_matrix);

    while(true)
    {
        std::cout << std::endl;
        std::cout << "LEVEL NOIZE:";
        int level_noize = 0;
        std::cin >> level_noize;
        std::cout << std::endl;
        if(level_noize == -1)
        {
            break;
        }

        std::cout<<std::endl << std::setfill('-') << std::setw(100) << " " << std::endl;

        auto first_with_noize = first;
        add_noize(level_noize,first_with_noize);
        auto result = work_network(widthes,first_with_noize);

        print_obj(first);
        std::cout<<std::endl;
        print_obj(first_with_noize);
        std::cout<<std::endl;
        print_obj(result);

        std::cout<<std::endl << std::setfill('-') << std::setw(100) << " " << std::endl;

        auto second_with_noize = second;
        add_noize(level_noize,second_with_noize);
        result = work_network(widthes,second_with_noize);
        print_obj(second);
        std::cout<<std::endl;
        print_obj(second_with_noize);
        std::cout<<std::endl;
        print_obj(result);

        std::cout<<std::endl << std::setfill('-') << std::setw(100) << " " << std::endl;

        auto third_with_noize = third;
        add_noize(level_noize,third_with_noize);
        result = work_network(widthes,third_with_noize);
        print_obj(third);
        std::cout<<std::endl;
        print_obj(third_with_noize);
        std::cout<<std::endl;
        print_obj(result);
    }

    return a.exec();
}
