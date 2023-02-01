"""
A loss function measures how good some output is compared to the expected output.
Loss is used to adjust the internal parameters of a network.
"""
import numpy as np

from arknet.tensor import Tensor

class Loss:
    def loss(self, predictions: Tensor, expected: Tensor) -> float:
        raise NotImplementedError

    def grad(self, predictions: Tensor, expected: Tensor) -> Tensor:
        raise NotImplementedError


class MSELoss(Loss):
    def loss(self, predictions: Tensor, expected: Tensor) -> float:
        return np.mean((predictions - expected)**2)

    def grad(self, predictions: Tensor, expected: Tensor) -> Tensor:
        return 2 * (predictions - expected)
