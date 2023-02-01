"""
An example function for training a neural network.
"""
from arknet.tensor import Tensor
from arknet.nn import NeuralNet
from arknet.loss import Loss, MSELoss
from arknet.optim import Optimizer, SGD
from arknet.data import DataIterator, BatchIterator


def train(net: NeuralNet,
          inputs: Tensor,
          targets: Tensor,
          n_epochs: int = 5000,
          iterator: DataIterator = BatchIterator(),
          loss: Loss = MSELoss(),
          optimizer: Optimizer = SGD(),
          log_every: int = 500) -> None:
    for epoch in range(n_epochs):
        epoch_loss = 0.0

        for batch in iterator(inputs, targets):
            predicted = net.forward(batch.inputs)
            epoch_loss += loss.loss(predicted, batch.targets)
            grad = loss.grad(predicted, batch.targets)
            net.backward(grad)
            optimizer.step(net)

        if (epoch + 1) % log_every == 0:
            print(epoch+1, epoch_loss)
