import numpy as np
import matplotlib.pyplot as plt
from scipy.stats import norm
import math

# Dados fornecidos
data = [32.3, 32.7, 31.9, 32.9, 33.2, 32.5, 32.7, 32.6, 32.7, 33.6, 33.9, 33.7, 33.1, 33.2, 
        33.2, 33.3, 32.7, 33, 33, 33.5, 33, 32.6, 33.2, 32.5, 32.6, 32.8, 33, 32.4, 32.6, 
        33, 33, 33.4, 33.2, 33.3, 32.7, 33, 33.3, 32.7, 32.8, 32.9]

# Calcular o número de barras
num_bins = math.floor(math.sqrt(len(data)))

# Criar o histograma dos dados
count, bins, ignored = plt.hist(data, bins=num_bins, alpha=0.6, color='g')

# Calcular a média e o desvio padrão dos dados
mu, std = norm.fit(data)

# Calcular a curva normal escalada para a frequência dos valores
xmin, xmax = plt.xlim()
x = np.linspace(xmin, xmax, 100)
p = norm.pdf(x, mu, std) * len(data) * (bins[1] - bins[0])

# Sobrepor a curva normal ao histograma
plt.plot(x, p, 'k', linewidth=2)

# Adicionar título e rótulos aos eixos
title = "Histograma e Curva Normal \nMédia = {:.2f}, Desvio Padrão = {:.2f}".format(mu, std)
plt.title(title)
plt.xlabel('Valor')
plt.ylabel('Ocorrência')

# Exibir o gráfico
plt.show()
