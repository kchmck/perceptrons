#include <cstdio>

#include "perceptron.hpp"
#include "vec.hpp"

int main() {
    const Phi phi {
        {0,   0, 0},
        {0,   1, 0},
        {1.5, 0, -1.5},
        {1.5, 1, -1.5},
        {1.5, 0, 0},
        {1.5, 1, 0},
        {0,   0, -1.5},
        {0,   1, -1.5},
    };

    const Labels labels {+1, +1, +1, +1, -1, -1, -1, -1};
    auto fn = CachedKernel(phi, kernel::poly(1.0));

    auto a = perceptron::Basic(phi, labels, 10);
    auto b = perceptron::Averaged(phi, labels, 10);
    auto c = perceptron::Kernel(phi, labels, 10, fn);
    auto d = perceptron::AveragedKernel(phi, labels, 10, fn);

    for (auto &x : phi) {
        printf("%f:%f:%f:%f\n", a.eval(x), c.eval(x), b.eval(x), d.eval(x));
    }
}
