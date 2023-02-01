"""
Canonical example of a function that cannot be learned with a linear model is XOR.
"""
import numpy as np

from arknet.train import train, seed_everything
from arknet.nn import NeuralNet
from arknet.layers import Linear, Tanh, ReLU
from arknet import optim

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

seed_everything(42)

net = NeuralNet([
    Linear(2, 2),
    ReLU(),
    Linear(2, 2)
])


train(net, inputs, targets,
      optimizer=optim.Adam(lr=0.01),
      log_every=500)

for x, y in zip(inputs, targets):
    pred = net.forward(x)
    print(x, pred, y)
