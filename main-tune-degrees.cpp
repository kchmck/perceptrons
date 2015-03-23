#include <fstream>

#include "best.hpp"
#include "confusion.hpp"
#include "digits.hpp"
#include "kernel.hpp"
#include "perceptron.hpp"
#include "util.hpp"
#include "vec.hpp"

// Polynomial degrees to test.
static constexpr double DEGREES[] = {2.0, 3.0, 4.0, 5.0, 6.0};

int main() {
    std::fstream trainStream("train.dat");
    const DigitData trainData(trainStream);

    std::fstream develStream("devel.dat");
    const DigitData develData(develStream);

    // The maximum accuracy for the normal and averaged kernels.
    MaxTracker<double, double> kernMaxAcc, avgMaxAcc;

    for (auto deg : DEGREES) {
        auto fn = CachedKernel(trainData.phi, kernel::normalize(kernel::poly(deg)));

        // Perceptrons trained for each digit class.
        std::vector<perceptron::Kernel> kernPerceps;
        std::vector<perceptron::AveragedKernel> avgPerceps;

        trainData.iterDigits([&](auto d, auto &phi, auto &labels) {
            printf("train %u\n", d);

            kernPerceps.emplace_back(phi, labels, best::EPOCHS, fn);
            avgPerceps.emplace_back(phi, labels, best::EPOCHS, fn);
        });

        // Test with the normal perceptrons.
        {
            const Confusion<perceptron::Kernel> c(develData, kernPerceps);
            printf("nrm\td:%f acc:%u/%u (%f)\n", deg, c.correct, c.total, c.acc());
            kernMaxAcc.consider(deg, c.acc());
        }

        // Test with the averaged perceptrons.
        {
            const Confusion<perceptron::AveragedKernel> c(develData, avgPerceps);
            printf("avg\td:%f acc:%u/%u (%f)\n", deg, c.correct, c.total, c.acc());
            avgMaxAcc.consider(deg, c.acc());
        }
    }

    printf("nrm\td:%f acc:%f\n", kernMaxAcc.key, kernMaxAcc.val);
    printf("avg\td:%f acc:%f\n", avgMaxAcc.key, avgMaxAcc.val);
}
