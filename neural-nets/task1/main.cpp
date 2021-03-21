#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>



//класс отвечабщий за обучение через чтение файла с тренировочными данными
class TrainingData
{
public:
    TrainingData (const std::string fileName);
    void getTopology(std::vector<unsigned> &topology);
    bool isEoF(void) {return m_trainingDataFile.eof();}
    unsigned getNextInputs(std::vector<double> &inputVals);
    unsigned getTargetOutputs(std::vector <double> &targetOutputVals);
private:
   std::ifstream m_trainingDataFile;
};

TrainingData::TrainingData(const std::string fileName)
{
    m_trainingDataFile.open(fileName.c_str());
}

void TrainingData::getTopology(std::vector<unsigned> &topology)
{
    std::string line;
    std::string label;

    //считывается строка
    getline(m_trainingDataFile, line);
    std::stringstream ss(line);
    //первое слово записывается в label
    ss >> label;
    //проверка на конец файла и тому что это строка топологии
    if (this -> isEoF() || label.compare("topology:") != 0){
        abort();
    }
    //есл все в порядке записываем топологию в вектор
    while (!ss.eof()) {
        unsigned n;
        ss >> n;
        topology.push_back(n);
    }

    return;
}
//ищем строки с in: - в них данные для входов
unsigned TrainingData::getNextInputs(std::vector<double> &inputVals)
{
    inputVals.clear();

    std::string line;
    getline(m_trainingDataFile, line);
    std::stringstream ss(line);

    std::string label;
    ss >> label;
    if (label.compare("in:") == 0) {
        double oneValue;
        while (ss >> oneValue){
            inputVals.push_back(oneValue);
        }
    }

    return inputVals.size();
}
//ищес строки с out: в них данные выходов
unsigned TrainingData::getTargetOutputs(std::vector<double> &targetOutputVals)
{
    targetOutputVals.clear();

    std::string line;
    getline(m_trainingDataFile, line);
    std::stringstream ss(line);

    std::string label;
    ss >> label;
    if (label.compare("out:") == 0) {
        double oneValue;
        while (ss >> oneValue){
            targetOutputVals.push_back(oneValue);
        }
    }

    return targetOutputVals.size();   
}

//структура "связь"
struct Connection
{
    double weight;
    double deltaWeight;
};


//объявление класса Neuron что бы typedef Layer работал
class Neuron;

typedef std::vector<Neuron> Layer;

// ******************* class Neuron ********************

class Neuron
{
public:
    //единственное что должен знать нейрон о следующем слое - кол-во нейронов в нем, для соединений
    Neuron(unsigned numOutputs, unsigned myIndex);
    void setOutputVal(double val){m_outputVal = val;}
    double getOutputVal(void) const {return m_outputVal;}
    //реализация распространения в конкретном нейроне(хэндлер пред.слоя)
    void feedForward(const Layer &prevLayer);
    //вычисление градиентов входного и скрытых слоев
    //(P.S градиент - то как нужно изменить вектор-слой что бы изменение было наиболее значиным, много -> этот нейрон сильно повлияет на итог!)
    void calcOutputGradients(double targetVal);
    void calcHiddenGradients(const Layer &nextLayer);
    void updateInputWeights(Layer &prevLayer);
    
private:
    static double eta; 
    static double alpha;
    //передаточная функция нейрона и ее производная
    static double transferFunction(double x);
    static double transferFunctionDerivative(double x);
    //задание случайного веса связи в пределах 0-1
    static double randomWeight(void){return rand() / double(RAND_MAX); }
    //сумма ошибок нейронов, которые соеденены с этим нейроном(фид) * градиент каждого этого нейрона
    double sumDOW(const Layer &nextLayer) const;
    //нейрон должен иметь выходное значение
    double m_outputVal;
    //вектор со значениями всех весов и дельт связей нейронов след. слоя
    std::vector<Connection> m_outputWeights;
    //индекс нейрона в слое
    unsigned m_myIndex;
    //значение градиента в нейроне
    double m_gradient;
};

//мера темпа обучения [0.0..1.0]
double Neuron::eta = 0.15;
//момент обучения [0.0..1.0]
double Neuron::alpha = 0.5;

void Neuron::updateInputWeights(Layer &prevLayer)
{
    //веса обновятся в векторе связей нейронов предыдущего слоя

    for(unsigned n = 0; n < prevLayer.size(); ++n){
        Neuron &neuron = prevLayer[n];
        double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;

        double newDeltaWeight = 
                //изменение вход. весов, используя градиент, темп и импульс(?) обучения
                eta
                * neuron.getOutputVal()
                * m_gradient
                * alpha
                * oldDeltaWeight;
        neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
        neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
    }
}

double Neuron::sumDOW(const Layer &nextLayer) const
{
    double sum = 0.0;
    //суммируем все ошибки*градиент нейронов, которых мы фидим

    for (unsigned n = 0; n < nextLayer.size() - 1; ++n){
        sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
    }

    return sum;
}

void Neuron::calcHiddenGradients(const Layer &nextLayer)
{
    double dow = sumDOW(nextLayer);
    m_gradient = dow * Neuron::transferFunctionDerivative(m_outputVal);
}

void Neuron::calcOutputGradients(double targetVal)
{
    double delta = targetVal - m_outputVal;
    //один из способов, так или иначе все стремиться к уменьшению ошибки 
    m_gradient = delta * Neuron::transferFunctionDerivative(m_outputVal);
}

double Neuron::transferFunction(double x)
{
    //выберем тестовую передаточную функцию tanh(гиперболический тангенс). Возвращает в пределах (-1;1)
    return tanh(x);
}

double Neuron::transferFunctionDerivative(double x)
{
    //аппроксимация производной ПФ (tanh(x)^2 ~= x^2)
    return 1.0 - x * x;
}

void Neuron::feedForward(const Layer &prevLayer)
{
    double sum = 0.0;

    //суммировать выходные значения предыдущего слоя(которые входные в этом слое)*вес связи
    //+включить базис предыдущего слоя
    for (unsigned n = 0; n< prevLayer.size(); ++n){
        sum += prevLayer[n].getOutputVal() * prevLayer[n].m_outputWeights[m_myIndex].weight;
    }

    m_outputVal = Neuron::transferFunction(sum);
}

//конструктор нейрона(выходы, индекс в слое)
Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
    //проходим через все соединения
    for (unsigned c = 0; c< numOutputs; ++c){
        m_outputWeights.push_back(Connection());
        m_outputWeights.back().weight = randomWeight();
    }
    //получаем индекс этого нейрона в слое
    m_myIndex = myIndex;
}

// ******************* class Net ********************

class Net 
{
public:
    Net(const std::vector<unsigned> &topology);
    //функ. распротранения значений, начиная со входов
    void feedForward(const std::vector<double> &inputVals){};
    //обратное распространение, обучение сети
    void backProp(const std::vector<double> &targetVals){};
    //результируюшие выходы, передаются в контейнер, которые передали функции
    void getResults(std::vector<double> &resultVals) const;

private:
    //вектор слоев
    std::vector<Layer> m_layers;
    //общая ошибка сети и тд
    double m_error;
    double m_recentAvarageError;
    double m_recentAvarageSmoothingFactor;
};

void Net::getResults(std::vector<double> &resultVals) const
{
    resultVals.clear();

    for (unsigned n = 0; n < m_layers.back().size() - 1; ++n){
        resultVals.push_back(m_layers.back()[n].getOutputVal());
    }
}

void Net::backProp(const std::vector<double> &targetVals)
{
    //Вычислить общую ошибку сети(корень квадрата арифмитичесого среднего, RMS)

    //хендлер выходного слоя
    Layer &outputLayer = m_layers.back();
    m_error = 0.0;

    for (unsigned n = 0; n < outputLayer.size() - 1; ++n){
        double delta = targetVals[n] - outputLayer[n].getOutputVal();
        m_error += delta*delta;
    }
    //средний квадрат ошибки
    m_error /= outputLayer.size() - 1;
    //RMS
    m_error = sqrt(m_error);

    //Средняя ошибка по сети(проверка успещности!)
    m_recentAvarageError = 
            (m_recentAvarageError * m_recentAvarageSmoothingFactor + m_error)
            / (m_recentAvarageSmoothingFactor + 1.0);

    //Вычислить градиенты выходного слоя

    for (unsigned n = 0; n < outputLayer.size() - 1; ++n) {
        outputLayer[n].calcOutputGradients(targetVals[n]);
    }

    //Вычислить градиенты скрытых слоев

    for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; --layerNum){
        //2 хендлера текущего и следущего слоя, скорее для удобства восприятия
        Layer &hiddenLayer = m_layers[layerNum];
        Layer &nextLayer = m_layers[layerNum + 1];

        for (unsigned n = 0; n < hiddenLayer.size(); ++n){
            hiddenLayer[n].calcHiddenGradients(nextLayer);
        }
    }

    //Для всех слоев от выходного до первого скрытого, обновить веса связей

    for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; --layerNum){
        //снова хендлеры для удобства
        Layer &layer = m_layers[layerNum];
        Layer &prevLayer = m_layers[layerNum - 1];

        for (unsigned n = 0; n< layer.size() - 1; ++n){
            layer[n].updateInputWeights(prevLayer);
        }
    }

}

void Net::feedForward(const std::vector<double> &inputVals)
{
    //банальная проверка, что кол-во входных хначений = размеру входного слоя минус базис(assert!)
    assert(inputVals.size() == m_layers[0].size() - 1);

    //присвоить входные значения входным нейронам
    for (unsigned i = 0; i < inputVals.size(); ++i){
        m_layers[0][i].setOutputVal(inputVals[i]);
    }

    //распространение вперед(не включая входной слой)
    for (unsigned layerNum = 1; layerNum < m_layers.size(); ++layerNum){
        //хэндлер предыдущего слоя, эффективнее создания отдельного контейнера
        Layer &prevLayer = m_layers[layerNum - 1];
        for (unsigned n = 0; n < m_layers[layerNum].size() - 1; ++n){
            //само распространение обеспечивается классом нейрона
            m_layers[layerNum][n].feedForward(prevLayer);
        }
    }
}

/*конструктор НС, принимающий на вход топологию НС - 
кол-во слоев (a,b,...,n - n слоев), 
кол-во нейронов в слоях(а = 10 -> 10 нейронов в первом слое) */
Net::Net(const std::vector<unsigned> &topology)
{
    unsigned numLayers = topology.size();

    for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum){
        m_layers.push_back(Layer()); 
        //если слой - финальный, то кол-во выходов = 0, иначе = размер следующего слоя
        unsigned numOutputs = layerNum == topology.size() - 1 ? : topology[layerNum + 1];

        //теперь новый слой надо заполнить нейронами, а так же добавить 
        //нейрон-базис в каждый слой(поэтому <=)
        for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum){
            //последний элемент в контейнере слоев на данный момент - запихать туда нейрон с его выходами и индексом
            m_layers.back().push_back(Neuron(numOutputs, neuronNum));
            
            //для отладки
            std::cout << "Made a Neuron!" << std::endl;
        }

        //Сделать значение базиса всегда 1.0
        m_layers.back().back().setOutputVal(1.0);
    }
}

int main()
{
    //пример топологии - (3,2,1)
    std::vector<unsigned> topology;
    topology.push_back(3);
    topology.push_back(2);
    topology.push_back(1);
    Net myNet(topology);

    std::vector<double> inputVals;
    myNet.feedForward(inputVals);

    std::vector<double> targetVals;
    myNet.backProp(targetVals);

    std::vector<double> resultVals;
    myNet.getResults(resultVals);
}