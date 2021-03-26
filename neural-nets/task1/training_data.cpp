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

//20% зашумленных символов
std::vector<int> fuzz_dat(std::vector<int> symbol)
{

    std::vector<int> noise(symbol.size());
    int fuzz_check;
    for (int i = 0; i < symbol.size(); ++i){
                fuzz_check = rand() % 5;
                if (fuzz_check == 0)
                    noise[i] = symbol[i]^1;
                else
                    noise[i] = symbol[i];
            }
    return noise;
}


//нойз не читается почему то
int main()
{
    unsigned flag;
    std::vector<int> noisee;
    std::cout << "topology: 30 16 4" << std::endl;
    for (int i = 500; i>=0; i--){
        flag = rand() % 4;
        switch (flag)
        {
            case 0:
            std::cout << "in: ";
            for (int i = 0; i < symb1.size(); ++i){
                std::cout << symb1[i] << ".0 ";
            }
            std::cout<< std::endl << "out: " << "1.0 0.0 0.0 0.0" <<std::endl;
            std::cout << "noise in: ";
            noisee = fuzz_dat(symb1);
            for (int i = 0; i < noisee.size(); ++i){
                std::cout << noisee[i] << ".0 ";
            }
            std::cout << std::endl;
            break;

            case 1:
            std::cout << "in: ";
            for (int i = 0; i < symb2.size(); ++i){
                std::cout << symb2[i] << ".0 ";
            }
            std::cout<< std::endl << "out: " << "0.0 1.0 0.0 0.0" <<std::endl;
            std::cout << "noise in: ";
            noisee = fuzz_dat(symb2);
            for (int i = 0; i < noisee.size(); ++i){
                std::cout << noisee[i] << ".0 ";
            }
            std::cout << std::endl;
            break;

            case 2:
            std::cout << "in: ";
            for (int i = 0; i < symb3.size(); ++i){
                std::cout << symb3[i] << ".0 ";
            }
            std::cout<< std::endl << "out: " << "0.0 0.0 1.0 0.0" <<std::endl;
            std::cout << "noise in: ";
            noisee = fuzz_dat(symb3);
            for (int i = 0; i < noisee.size(); ++i){
                std::cout << noisee[i] << ".0 ";
            }
            std::cout << std::endl;
            break;

            case 3:
            std::cout << "in: ";
            for (int i = 0; i < symb4.size(); ++i){
                std::cout << symb4[i] << ".0 ";
            }
            std::cout<< std::endl << "out: " << "0.0 0.0 0.0 1.0" <<std::endl;
            std::cout << "noise in: ";
            noisee = fuzz_dat(symb4);
            for (int i = 0; i < noisee.size(); ++i){
                std::cout << noisee[i] << ".0 ";
            }
            std::cout << std::endl;
            break;
        }
    }
}