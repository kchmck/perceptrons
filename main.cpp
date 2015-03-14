#include "kernel.hpp"
#include "perceptron.hpp"
#include "vec.hpp"

int main() {
    const Phi phi({
        {0,   0, 0},
        {0,   1, 0},
        {1.5, 0, -1.5},
        {1.5, 1, -1.5},
        {1.5, 0, 0},
        {1.5, 1, 0},
        {0,   0, -1.5},
        {0,   1, -1.5},
    });

    const Labels labels({+1, +1, +1, +1, -1, -1, -1, -1});

    const perceptron::Basic bp(phi, labels, 10);
    const perceptron::Kernel kp(phi, labels, 10, kernel::poly(phi, 2));
}
