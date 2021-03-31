#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>

//функция суммирования элементов вектора для улучшение читаемости(нахуя?? она юзатся 1 раз лол)
unsigned vectorSum(const std::vector<bool> vec)
{
    unsigned sum = 0;
    for(std::vector<bool>::const_iterator i = vec.begin(); i != vec.end(); ++i){
        sum+=*i;
    }
    return sum;
}


/*************** class ART - detection Neuron**************************************/

class detectionNeuronART
{
public:
    void initNeuron(const double size);
    void setNewVals (std::vector <bool> inputSample);

    void calculateOutputS(const std::vector<bool> inputSample);
    const double getOutputs(){return m_outputS;}

    std::vector <bool> getVectorC (const std::vector <bool> inputSample);
    double getRoh (const std::vector <bool> vectorC,const std::vector <bool> inputSample);
    std::vector <bool> getTVector(){return m_detectionValsT;}
    std::vector <double> getBVector(){return m_detectionValsB;}
    
private:
    std::vector <double> m_detectionValsB;
    std::vector <bool> m_detectionValsT;
    double m_outputS;
};

void detectionNeuronART::setNewVals (std::vector <bool> inputSample)
{
    unsigned sum = vectorSum(inputSample);
    m_detectionValsB.clear();
    m_detectionValsT.clear();
    for(int i = 0; i < inputSample.size(); ++i){
        m_detectionValsT.push_back(inputSample[i]);
        m_detectionValsB.push_back((double)2*m_detectionValsT.back()/(1 + sum));
    }
}

//функция вычисляющая ро - значение сравнения вектора С и Х
double detectionNeuronART::getRoh(const std::vector <bool> vectorC, const std::vector <bool> inputSample)
{
    unsigned l=0;
    for(int i = 0; i < vectorC.size(); ++i){
     //   std:: cout << " X = " << inputSample[i]  << " T = " << m_detectionValsT[i]<< " C = " << vectorC[i]<<"\n";
        if (vectorC[i] == inputSample[i])
            ++l;
    }
    //ро - (количство совпадений)/(всего значений)
    double roh = (double)l / (double)vectorC.size();
    std::cout << "Roh = " << roh <<"\n \n";
    return roh;
}

//функция, вычисляющая вектор С - логическое И векторов Т и Х
std::vector <bool> detectionNeuronART::getVectorC (const std::vector <bool> inputSample)
{
    std::vector <bool> vectorC;
    for(int i = 0; i < inputSample.size(); ++i)
        vectorC.push_back(m_detectionValsT[i] && inputSample[i]);
    return vectorC;
}

//конструктор нейрона, если индекс 0 - это инициализация нераспределенного нейрона
void detectionNeuronART::initNeuron (const double size)
{
        for(int i = 0;i < size; ++i){
            m_detectionValsT.push_back(1);
            m_detectionValsB.push_back(2/(1+size) + 0.01);
        }
}

//функция вычисление выхода нейрона
void detectionNeuronART::calculateOutputS(const std::vector<bool> inputSample)
{
    m_outputS = 0;
    for(int i = 0; i < inputSample.size(); ++i){
        m_outputS += inputSample[i]*m_detectionValsB[i];
    }
}

/****************** class ART - neural net *********************************/
class ArtNet
{
public:
    ArtNet(const unsigned vectorSize, const double threshold);
    //инициализация - создание нераспределенного нейрона(его индекс - 0)
    void initialize();
    void inputX(const std::vector<bool> sampleX);
    std::vector <detectionNeuronART> detectionLayer;
private:
    void addNewNeuron(const std::vector<bool> sampleX);
    double m_threshold;
    unsigned m_sizeOfVectors;
};


void ArtNet::addNewNeuron(const std::vector<bool> sampleX)
{
    detectionLayer.push_back(detectionNeuronART());
    detectionLayer.back().setNewVals(sampleX);
    std::cout << "ADDED NEURON \n";
}

void ArtNet::inputX(const std::vector<bool> sampleX)
{
    assert(m_sizeOfVectors == sampleX.size());
    //запихиваем нейрон с индексом равным размеру(если сеть уже с 1 нейроном будет индекс 1 и тд, считаем с 0)
    if (detectionLayer.size() == 1)
        addNewNeuron(sampleX);

    else{
        //обход по нейронам, вычисляем максимальное S, в зависимости от победителя делаем вещи
        //в приоретите использовать алгоритмические функции типа for_each, max_element и тд, но здесь идеально подходит цикл
        double max = 0;
        unsigned indexMax;

        for(unsigned i = 0; i < detectionLayer.size(); ++i){
            detectionLayer[i].calculateOutputS(sampleX);
            if(detectionLayer[i].getOutputs() > max){
                max = detectionLayer[i].getOutputs();
                indexMax = i;
            }
        }
        std::cout << "\nNeuron #"<< indexMax << " WON! \n";
        if (indexMax == 0)
           addNewNeuron(sampleX);
        
        else
        {
            std::vector <bool> vectorC = detectionLayer[indexMax].getVectorC(sampleX);
            double roh = detectionLayer[indexMax].getRoh(vectorC, sampleX);
            if(roh > m_threshold){
                detectionLayer[indexMax].setNewVals(vectorC);
            }
            else{
                addNewNeuron(sampleX);
            }
        }
    }
}

void ArtNet::initialize()
{
    assert(detectionLayer.size() == 0);
    detectionLayer.push_back(detectionNeuronART());
    detectionLayer.back().initNeuron(m_sizeOfVectors);
}

ArtNet::ArtNet (const unsigned vectorSize, const double threshold){
    m_sizeOfVectors = vectorSize;
    m_threshold = threshold;
}

std::vector< std::vector<bool> > sampels = {{1, 1, 1, 1,
                                             1, 1 ,1 ,1,
                                             1, 0, 0, 1,
                                             1, 0 ,0, 1},

                                            {1, 0, 0, 1,
                                             1, 0 ,0 ,1,
                                             0, 1, 1, 0,
                                             0, 1 ,1, 0},
                                                        
                                            {0, 1, 1, 0,
                                             0, 1 ,1 ,0,
                                             1, 0, 0, 1,
                                             1, 0 ,0, 1},

                                            {1, 0, 0, 1,
                                             1, 0 ,0 ,1,
                                             0, 1, 0, 0,
                                             0, 1 ,1, 0},

                                            {0, 1, 1, 0,
                                             0, 1 ,1 ,0,
                                             1, 0, 0, 1,
                                             1, 0 ,1, 0}};


void printTVector (std::vector <bool> vector)
{
    std::cout << "Vector T:";
    for(unsigned i = 0; i < vector.size(); ++i){
        if(i % 4 == 0)
            std::cout << "\n";
        std::cout << " " << vector[i];
    }
    std::cout << ";\n";
}

void printBVector (std::vector <double> vector)
{
    std::cout << "Vector B:";
    for(unsigned i = 0; i < vector.size(); ++i){
        if(i % 4 == 0)
            std::cout << "\n";
        std::cout << " " << vector[i];
    }
    std::cout << "; \n";
}

int main()
{
    std::vector<bool> Tvector;
    std::vector<double> Bvector;
    ArtNet Net(16, 0.8);
    Net.initialize();
    for(int i = 0; i <= 4; ++i){
        std::cout << "          EPOCH #" << i+1 << "\n";
        Net.inputX(sampels[i]);
        for(int j = 0; j < Net.detectionLayer.size(); ++j){
                std::cout << "Neuron #" << j <<": \n";
                printTVector(Net.detectionLayer[j].getTVector());
                printBVector(Net.detectionLayer[j].getBVector());
                Net.detectionLayer[j].calculateOutputS(sampels[i]);
                std::cout << "S output = " << Net.detectionLayer[j].getOutputs() << ".\n";
        }
        std::cout << "\n \n";
    }
    
    return 0;
}