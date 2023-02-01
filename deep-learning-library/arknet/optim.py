"""
Optimizers are used to adjust the parameters of a network according to the computed
gradients during backprop.
"""
from typing import Tuple

import numpy as np

from arknet.nn import NeuralNet

class Optimizer:
    def step(self, net: NeuralNet) -> None:
        raise NotImplementedError


class SGD(Optimizer):
    def __init__(self, lr: float = 0.001) -> None:
        super().__init__()
        self.lr = lr

    def step(self, net: NeuralNet) -> None:
        for param, grad in net.params_and_grads():
            param -= self.lr * grad


class Adam(Optimizer):
    def __init__(self, lr: float = 0.001, betas: Tuple[float, float] = (0.9, 0.999), eps: float = 1e-8) -> None:
        super().__init__()
        self.lr = lr
        self.beta1 = betas[0]
        self.beta2 = betas[1]
        self.eps = eps

        self.m_w = 0
        self.m_b = 0
        self.v_w = 0
        self.v_b = 0

        self.t = 1

    def step(self, net):
        for param, grad in net.params_and_grads():
            if len(param.shape) > 1:
                # Weights (n, m) matrix
                self.m_w = self.beta1 * self.m_w + (1 - self.beta1) * grad
                self.v_w = self.beta2 * self.v_w + (1 - self.beta2) * (grad**2)

                m_corr = self.m_w / (1 - self.beta1**self.t)
                v_corr = self.v_w / (1 - self.beta2**self.t)

                param -= self.lr*(m_corr / (np.sqrt(v_corr) + self.eps))
            else:
                # Biases (m,) matrix
                self.m_b = self.beta1 * self.m_b + (1 - self.beta1) * grad
                self.v_b = self.beta2 * self.v_b + (1 - self.beta2) * (grad**2)

                m_corr = self.m_b / (1 - self.beta1**self.t)
                v_corr = self.v_b / (1 - self.beta2**self.t)

                param -= self.lr*(m_corr / (np.sqrt(v_corr) + self.eps))

        self.t += 1
