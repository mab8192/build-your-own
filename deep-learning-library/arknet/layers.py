"""
Neural networks are made up of layers.
Each layer passes it's input forward and propagates its gradients backward.
"""
from typing import Dict, Callable

import numpy as np

from arknet.tensor import Tensor


class Layer:
    def __init__(self) -> None:
        self.params: Dict[str, Tensor] = {}
        self.grads: Dict[str, Tensor] = {}

    def forward(self, inputs: Tensor) -> Tensor:
        """Compute outputs for the given inputs"""
        raise NotImplementedError

    def backward(self, grad: Tensor) -> Tensor:
        """Backprop the given grad through the layer"""
        raise NotImplementedError


class Linear(Layer):
    """Computes outputs as inputs @ w + b"""
    def __init__(self, input_size: int, output_size: int) -> None:
        super().__init__()
        # Inputs are (batch_size, input_size)
        # Outputs are (batch_size, output_size)
        self.params["w"] = np.random.randn(input_size, output_size)
        self.params["b"] = np.random.randn(output_size)

    def forward(self, inputs: Tensor) -> Tensor:
        """Compute outputs for the given inputs"""
        self.inputs = inputs  # Saved for backprop later
        return inputs @ self.params["w"] + self.params["b"]

    def backward(self, grad: Tensor) -> Tensor:
        """
        y = f(x)
        x = a @ b + c
        dy/da = f'(x) @ b.T
        dy/db = a.T @ f'(x)
        dy/dc = f'(x)
        """
        self.grads["b"] = np.sum(grad, axis=0)
        self.grads["w"] = self.inputs.T @ grad
        return grad @ self.params["w"].T


# Activation Layers
F = Callable[[Tensor], Tensor]


class Activation(Layer):
    """
    An activation layer applies a function elementwise to some inputs.
    """
    def __init__(self, f: F, f_prime: F) -> None:
        super().__init__()
        self.f = f
        self.f_prime = f_prime

    def forward(self, inputs: Tensor) -> Tensor:
        self.inputs = inputs
        return self.f(inputs)

    def backward(self, grad: Tensor) -> Tensor:
        """
        y = f(x), x = g(z)
        dy/dz = f'(x) * g'(z)
        """
        return self.f_prime(self.inputs) * grad


# ------------------ Hyperbolic Tangent ------------------
def tanh(x: Tensor) -> Tensor:
    return np.tanh(x)


def tanh_prime(x: Tensor) -> Tensor:
    y = tanh(x)
    return 1 - y**2


class Tanh(Activation):
    def __init__(self) -> None:
        super().__init__(tanh, tanh_prime)


# ------------------ Rectified Linear Unit (ReLU) ------------------
def relu(x: Tensor) -> Tensor:
    return np.maximum(x, 0)


def relu_prime(x: Tensor) -> Tensor:
    """
    f(x) = max(x, 0)
    f'(x) = 1 if x > 0, 0 otherwise
    """
    r = np.zeros_like(x)
    r[x > 0] = 1
    return r


class ReLU(Activation):
    def __init__(self) -> None:
        super().__init__(relu, relu_prime)


# ------------------ Softmax ------------------
"""
def softmax(x: Tensor) -> Tensor:
    exp = np.exp(x - np.max(x))
    return exp / np.sum(exp, axis=1, keepdims=True)


def softmax_prime(x: Tensor) -> Tensor:
    s = softmax(x)
    t1 = np.einsum('ij,jk->ijk', s, np.eye(s.shape[-1]))
    t2 = np.einsum('ij,ik->ijk', s, s)
    jac = t1-t2
    return jac
"""
