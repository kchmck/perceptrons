#include "kernel.hpp"
#include "perceptron.hpp"
#include "vec.hpp"

int main() {
    {
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

        auto fn = kernel::polyExt(2);
        auto alphas = perceptron::Kernel(phi, labels, 10, fn).run();

        for (auto x : phi)
            printf("%+i\n", sgn(kernel::eval(fn, phi, labels, alphas, x)));
    }

    // {
    //     const Phi phi {
    //         {1, 1},
    //         {1, 0},
    //     };
    //
    //     const Labels labels {-1, +1};
    //
    //     auto fn = kernel::poly(2);
    //
    //     auto alphas = perceptron::Kernel(phi, labels, 10, fn).run();
    //
    //     for (auto w : alphas)
    //         printf("%f\n", w);
    //
    //     printf("%f\n", kernel::eval(fn, phi, labels, alphas, phi[0]));
    //     printf("%f\n", kernel::eval(fn, phi, labels, alphas, phi[1]));
    // }
}
