"""
Canonical example of a function that cannot be learned with a linear model is XOR.
"""
import numpy as np

from arknet.train import train
from arknet.nn import NeuralNet
from arknet.layers import Linear, Tanh

inputs = np.array([
    [0, 0],
    [0, 1],
    [1, 0],
    [1, 1]
])

targets = np.array([
    [1, 0],
    [0, 1],
    [0, 1],
    [1, 0]
])


# Example showing a linear function cannot represent XOR
"""
net = NeuralNet([
    Linear(input_size=2, output_size=2)
])

train(net, inputs, targets)

for x, y in zip(inputs, targets):
    pred = net.forward(x)
    print(x, pred, y)
"""

net = NeuralNet([
    Linear(2, 2),
    Tanh(),
    Linear(2, 2)
])

train(net, inputs, targets)

for x, y in zip(inputs, targets):
    pred = net.forward(x)
    print(x, pred, y)
