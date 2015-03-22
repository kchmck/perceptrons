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

    auto bp = perceptron::Basic(phi, labels, 10).run();
    auto ap = perceptron::Averaged(phi, labels, 10).run();
    auto kp = perceptron::Kernel(phi, labels, 10, kernel::poly(phi, 2)).run();
    auto akp = perceptron::AveragedKernel(phi, labels, 10, kernel::poly(phi, 2)).run();
}
