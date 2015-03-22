#include <fstream>

#include "best.hpp"
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
        auto fn = kernel::normalize(kernel::poly(deg));

        // Perceptrons trained for each digit class.
        std::vector<perceptron::Kernel> kernPerceps;
        std::vector<perceptron::AveragedKernel> avgPerceps;

        trainData.iterDigits([&](uint32_t d, const Phi &phi, const Labels &labels) {
            printf("train %u\n", d);

            kernPerceps.emplace_back(phi, labels, best::EPOCHS, fn);
            avgPerceps.emplace_back(phi, labels, best::EPOCHS, fn);
        });

        // Test with the normal perceptrons.
        {
            const AccTester<perceptron::Kernel> at(kernPerceps, develData);
            printf("nrm\td:%f acc:%u/%u (%f)\n", deg, at.correct, at.total, at.acc());
            kernMaxAcc.consider(deg, at.acc());
        }

        // Test with the averaged perceptrons.
        {
            const AccTester<perceptron::AveragedKernel> at(avgPerceps, develData);
            printf("avg\td:%f acc:%u/%u (%f)\n", deg, at.correct, at.total, at.acc());
            avgMaxAcc.consider(deg, at.acc());
        }
    }

    printf("nrm\td:%f acc:%f\n", kernMaxAcc.key, kernMaxAcc.val);
    printf("avg\td:%f acc:%f\n", avgMaxAcc.key, avgMaxAcc.val);
}
