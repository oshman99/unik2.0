import numpy as np
import math as math
import plotly.graph_objects as go
from contextlib import redirect_stdout
import random as rnd
#5 варик
#получаем прирощение приспособленности


class Genetic(object):

    def __init__(self, f, pop_size = 50, n_variables = 2):
        self.f = f
        self.min_range = -6
        self.max_range = 6
        self.minim = 0
        self.maxim = 100000
        self.step = self.calculateStep()
        self.pop_size = pop_size
        self.n_variables = n_variables
        self.population = self.initializePopulation()
        self.population_1 = self.initializePopulation()
        self.population_3 = self.initializePopulation()
        self.evaluatePopulation()
        self.count = 0

    def calculateStep(self):
        return((self.max_range - self.min_range)/(self.maxim - self.minim))

    def initializePopulation(self):
        return [[rnd.randint(self.minim, self.maxim) for j in range(self.n_variables)] 
                           for i in range(self.pop_size)]

    def evaluatePopulation(self):
        return [self.f(self.min_range + self.step*i[0], 
                    self.min_range + self.step*i[1]) for i in self.population]

    def nextGen(self):
        pairs_population = self.population
        old_results = self.evaluatePopulation()
        pairs_count = 0
        while pairs_count != self.pop_size/2:
            self.count +=1
            pairs_count += 1
            parent1,parent2 = rnd.choices(self.population, k = 2)

            #скрещивание
            if (np.random.randint(1,11) <= 9):
                p1 = [format(i, '020b') for i in parent1]
                p2 = [format(i, '020b') for i in parent2]
                gen_index = np.random.randint(1,len(p1[0]))
                #рекобинцаия
                genes = [[i[:gen_index] + j[gen_index:] for i,j in zip(p1,p2)],
                         [i[:gen_index] + j[gen_index:] for i,j in zip(p2,p1)]]
                #мутация
                if (np.random.randint(1,101) <= 15):
                    for var_s in genes:
                        mutate_index = np.random.randint(1,len(var_s[0]))
                        for x in var_s:
                            x = x[:mutate_index-1] + format(int(x[mutate_index])^1, 'b') + x[mutate_index:]
                children = [[int(i,2) for i in j] for j in genes]
                for child in children:
                    self.population.append(child)

        #редукция методом турнира
        results = self.evaluatePopulation();
        while len(self.population) > self.pop_size:
            playa1,playa2 = rnd.sample(range(0, len(self.population)),2)
            # внашем случае фитнес функция = основной функции т.к. ищем макс
            if results[playa1] > results[playa2]:
               self.population.pop(playa2)
               results.pop(playa2)
            else:
                self.population.pop(playa1)
                results.pop(playa1)

        #считаем прирощение прспособленности
        count, sum_old, sum_new = 0,0,0
        while count != len(old_results):
            sum_old += old_results[count]
            sum_new += results[count]
            count += 1
        dF = (sum_new-sum_old)/sum_new
        return(dF)
        



    def run(self):
        dF = self.nextGen()
        self.population_1 = self.population
        pop_count = 1
        while dF > 0.01:
            dF = self.nextGen()
            pop_count += 1
            if pop_count == 3:
                self.population_3 = self.population
        #ищем максимум среди последней популяции
        results = np.asarray(self.evaluatePopulation())
        index_max = np.argmax(results)
        return(self.population[index_max])



#main
f = lambda x, y:0.1*x + 0.2*y - 4*np.cos(x) - 4*np.cos(0.9*y) + 5
gen = Genetic(f)
results = gen.run()
result_x = gen.min_range+gen.step*results[0]
result_y = gen.min_range+gen.step*results[1]
result_z = f(result_x, result_y)
print ('Results are: X = ', result_x, ' Y = ', result_y)



xval = np.linspace(gen.min_range, gen.max_range, num= 150)
yval = np.linspace(gen.min_range, gen.max_range,num= 150)
x, y = np.meshgrid(xval, yval)
z = f(x, y)
fig = go.Figure()
fig = go.Figure(data=[go.Surface(x=x, y=y, z=z, colorbar_x=-0.07)])
fig.add_trace(go.Scatter3d(x = [result_x],y = [result_y], z = [result_z]))

fig.show()
