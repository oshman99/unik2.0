#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <algorithm>
#include <functional>
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
    detectionNeuronART (const double size,const unsigned index);
    void calculateOutputS(const std::vector<bool> inputSample);
    const double getOutputs(){return m_outputS;}
    void setNewVals (std::vector <bool> inputSample);
    void compareAndRelearn (std::vector <bool> inputSample, double threshold);
private:
    std::vector <double> m_detectionValsB;
    std::vector <bool> m_detectionValsT;
    unsigned m_sizeOfVectors;//попытаться избавиться, лишние данные
    double m_outputS;
};


//сравнение Хi и вектора С, переобучение в случае если ро > трешхолда
//лучше сделать отдельную функцию переобучения, а сравивать в нейросети
void detectionNeuronART::compareAndRelearn(std::vector <bool> inputSample, double threshold)
{
    assert(m_sizeOfVectors == inputSample.size());
    //создавать полный вектор смысла тупа нет
    std::vector <bool> vectorC;
    int l;
    for(int i = 0; i < m_sizeOfVectors; ++i)
    {
        vectorC[i] = (m_detectionValsT[i] && inputSample[i]);
        l = ((vectorC[i] && inputSample[i]) == 0)?: ++l;
    }
    double roh = (l/m_sizeOfVectors);
    if (roh > threshold){
        double sum = vectorSum(vectorC);
        m_detectionValsT = vectorC;
        for(int i = 0; i < m_sizeOfVectors; ++i){
            m_detectionValsB[i] = (2*m_detectionValsT[i])/(1+sum);
        }
    }
}

//конструктор нейрона, если индекс 0 - это инициализация нераспределенного нейрона
detectionNeuronART::detectionNeuronART (const double size,const unsigned index)
{
    m_sizeOfVectors = size;
    if (index == 0){
        for(int i = 0;i < size; ++i){
            m_detectionValsT.push_back(1);
            m_detectionValsB.push_back(2/(1+index) + 0.01);
        }
    }
    assert(m_detectionValsT.size() == m_sizeOfVectors && m_detectionValsB.size()== m_sizeOfVectors);
}

//функция вычисление выхода нейрона
void detectionNeuronART::calculateOutputS(const std::vector<bool> inputSample)
{
    assert(m_sizeOfVectors == inputSample.size());
    m_outputS = 0;
    for(int i = 0; i < m_sizeOfVectors; ++i){
        m_outputS += inputSample[i]*m_detectionValsB[i];
    }
};

/****************** class ART - neural net *********************************/
class ArtNet
{
public:
    ArtNet(const unsigned vectorSize);
    //инициализация - создание нераспределенного нейрона(его индекс - 0)
    void initialize();
    void input(const std::vector<bool> sampleX);
    void compare ();
private:
    unsigned m_sizeOfVectors;
    std::vector <detectionNeuronART> m_detectionLayer;
};

void ArtNet::input(const std::vector<bool> sampleX)
{
    assert(m_sizeOfVectors == sampleX.size());
    //запихиваем нейрон с индексом равным размеру(если сеть уже с 1 нейроном будет индекс 1 и тд, считаем с 0)
    m_detectionLayer.push_back(detectionNeuronART(m_sizeOfVectors,
                                                m_detectionLayer.size()));
    if (m_detectionLayer.size() == 2)
        m_detectionLayer.back().setNewVals(sampleX);
    else{
        //обход по нейронам, вычисляем максимальное S, в зависимости от победителя делаем вещи
        //в приоретите использовать алгоритмические функции типа for_each, max_element и тд, но здесь идеально подходит цикл
        double max = 0;
        unsigned indexMax;
        for(unsigned i = 0; i < m_sizeOfVectors; ++i){
            m_detectionLayer[i].calculateOutputS(sampleX);
            m_detectionLayer[i].getOutputs() > max?:max = m_detectionLayer[i].getOutputs();
            indexMax = i;
        }
    }
}

void ArtNet::initialize()
{
    m_detectionLayer.push_back(detectionNeuronART(m_sizeOfVectors,0));
}

ArtNet::ArtNet (const unsigned vectorSize){
    m_sizeOfVectors = vectorSize;
}

int main()
{
    return 0;
}