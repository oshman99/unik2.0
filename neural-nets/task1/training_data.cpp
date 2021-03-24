/*маленький файл генерирующий 2000 семплов для обучения
отправить его вывод в файл с помощью:
gcc training_data.cpp -o training_data
./training_data > ./training_data.txt
*/
#include <iostream>
#include <cstdlib>
#include <vector>
//символ "1"
std::vector<int> symb1 = {0, 0, 1, 0, 0,
                          0, 1, 1, 0, 0,
                          0, 0, 1, 0, 0,
                          0, 0, 1, 0, 0,
                          0, 0, 1, 0, 0,
                          0, 1, 1, 1, 0};


//символ "2"
std::vector<int> symb2 = {0, 1, 1, 0, 0,
                          1, 0, 0, 1, 0,
                          0, 0, 1, 0, 0,
                          0, 1, 0, 0, 0,
                          1, 0, 0, 0, 0,
                          1, 1, 1, 1, 0};

//символ "Я"
std::vector<int> symb3 = {0, 1, 1, 1, 0,
                          0, 1, 0, 1, 0,
                          0, 0, 1, 1, 0,
                          0, 1, 0, 1, 0,
                          0, 1, 0, 1, 0,
                          0, 1, 0, 1, 0};

//символ "Ъ"
std::vector<int> symb4 = {1, 1, 0, 0, 0,
                          0, 1, 0, 0, 0,
                          0, 1, 0, 0, 0,
                          0, 1, 1, 1, 0,
                          0, 1, 0, 1, 0,
                          0, 1, 1, 1, 0};


int main()
{
    unsigned flag;
    std::cout << "topology: 30 6 4" << std::endl;
    for (int i = 500; i>=0; i--){
        flag = rand() % 4;
        switch (flag)
        {
            case 0:
            std::cout << "in: ";
            for (std::vector<int>::const_iterator i = symb1.begin();i != symb1.end(); ++i){
                std::cout << *i << ".0 ";
            }
            std::cout<< std::endl << "out: " << "1.0 0.0 0.0 0.0" <<std::endl;
            break;

            case 1:
            std::cout << "in: ";
            for (std::vector<int>::const_iterator i = symb2.begin();i != symb2.end(); ++i){
                std::cout << *i << ".0 ";
            }
            std::cout<< std::endl << "out: " << "0.0 1.0 0.0 0.0" <<std::endl;
            break;

            case 2:
            std::cout << "in: ";
            for (std::vector<int>::const_iterator i = symb3.begin();i != symb3.end(); ++i){
                std::cout << *i << ".0 ";
            }
            std::cout<< std::endl << "out: " << "0.0 0.0 1.0 0.0" <<std::endl;
            break;

            case 3:
            std::cout << "in: ";
            for (std::vector<int>::const_iterator i = symb4.begin();i != symb4.end(); ++i){
                std::cout << *i << ".0 ";
            }
            std::cout<< std::endl << "out: " << "0.0 0.0 0.0 1.0" <<std::endl;
            break;
        }
        for (std::vector<int>::const_iterator i = symb1.begin();i != symb1.end(); ++i){
                std::cout << *i << ".0 ";
            }
            std::cout<< std::endl << "out: " << "1.0 0.0 0.0 0.0" <<std::endl;
    }
}