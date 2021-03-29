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
    void relearn(const std::vector <bool> vectorC);
    std::vector <bool> getTVector(){return m_detectionValsT;}
    std::vector <double> getBVector(){return m_detectionValsB;}
    
private:
    std::vector <double> m_detectionValsB;
    std::vector <bool> m_detectionValsT;
    double m_outputS;
};

void detectionNeuronART::setNewVals (std::vector <bool> inputSample)
{
    m_detectionValsT = inputSample;
    unsigned sum = vectorSum(inputSample);
    for(int i; i < inputSample.size(); ++i)
        m_detectionValsB[i] = (2*inputSample[i])/(1 + sum);
}

//функция переобучения нейрона
void detectionNeuronART::relearn(const std::vector <bool> vectorC)
{
    m_detectionValsT = vectorC;
    double sum = vectorSum(m_detectionValsT);
    for(int i = 0; i < vectorC.size(); ++i){
        m_detectionValsB[i] = (2*m_detectionValsT[i])/(1+sum);
    }
}
//функция вычисляющая ро - значение сравнения вектора С и Х
double detectionNeuronART::getRoh(const std::vector <bool> vectorC, const std::vector <bool> inputSample)
{
    unsigned l;
    for(int i = 0; i < vectorC.size(); ++i)
        l = ((vectorC[i] && inputSample[i]) == 1)?: ++l;
    //ро - (количство совпадений)/(всего значений)
    double roh = (l/vectorC.size());
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
    void input(const std::vector<bool> sampleX);
    void addNewNeuron(const std::vector<bool> sampleX);
    detectionNeuronART getNeuron (unsigned index){return m_detectionLayer[index];}
private:
    double m_threshold;
    unsigned m_sizeOfVectors;
    std::vector <detectionNeuronART> m_detectionLayer;
};


void ArtNet::addNewNeuron(const std::vector<bool> sampleX)
{
    assert(m_sizeOfVectors == sampleX.size());
    m_detectionLayer.push_back(detectionNeuronART();
    m_detectionLayer.back().setNewVals(sampleX);
}

void ArtNet::input(const std::vector<bool> sampleX)
{
    assert(m_sizeOfVectors == sampleX.size());
    //запихиваем нейрон с индексом равным размеру(если сеть уже с 1 нейроном будет индекс 1 и тд, считаем с 0)
    if (m_detectionLayer.size() == 1)
        addNewNeuron(sampleX);

    else{
        //обход по нейронам, вычисляем максимальное S, в зависимости от победителя делаем вещи
        //в приоретите использовать алгоритмические функции типа for_each, max_element и тд, но здесь идеально подходит цикл
        double max = 0;
        unsigned indexMax;

        for(unsigned i = 0; i < m_sizeOfVectors; ++i){
            m_detectionLayer[i].calculateOutputS(sampleX);
            m_detectionLayer[i].getOutputs() > max?:
                                            max = m_detectionLayer[i].getOutputs();
            indexMax = i;
        }

        if (indexMax == 0)
           addNewNeuron(sampleX);
        
        else
        {
            std::vector <bool> vectorC = m_detectionLayer[indexMax].getVectorC(sampleX);
            m_detectionLayer[indexMax].getRoh(vectorC, sampleX) > m_threshold?
                                                m_detectionLayer[indexMax].relearn(vectorC):
                                                addNewNeuron(sampleX);
        }
    }
}

void ArtNet::initialize()
{
    m_detectionLayer.push_back(detectionNeuronART());
    assert(m_detectionLayer.size() == 1);
    m_detectionLayer.back().initNeuron(m_sizeOfVectors);
}

ArtNet::ArtNet (const unsigned vectorSize, const double threshold){
    m_sizeOfVectors = vectorSize;
    m_threshold = threshold;
}

int main()
{
    ArtNet Net(12, 0.8);
    Net.initialize();
}