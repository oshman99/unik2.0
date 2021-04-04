import numpy as np
import math as math
class Genetic(object):

    def __init__(self, f, pop_size = 10, n_variables = 2):
        self.f = f
        self.min_range = -5
        self.max_range = 5
        self.minim = 0
        self.maxim = 1000000
        self.step = self.calculateStep()
        self.pop_size = pop_size
        self.n_variables = n_variables
        self.population = self.initializePopulation()
        self.evaluatePopulation()

    def calculateStep(self):
        return((self.max_range - self.min_range)/(self.maxim - self.minim))

    def initializePopulation(self):
        return [np.random.randint(self.minim, self.maxim, size=(self.n_variables)) 
                           for i in range(self.pop_size)]

    def evaluatePopulation(self):
        return [self.f(self.min_range + self.step*i[0], 
                    self.min_range + self.step*i[1]) for i in self.population]

    def 




f = lambda x, y:0.2*x + 0.2*y - 2*math.cos(x) - 2* math.cos(0.9*y) + 4
gen = Genetic(f)
print(gen.evaluatePopulation())
print(gen.step)

   