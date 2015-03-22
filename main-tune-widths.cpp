#include <fstream>

#include "best.hpp"
#include "digits.hpp"
#include "kernel.hpp"
#include "perceptron.hpp"
#include "util.hpp"
#include "vec.hpp"

// The Gaussian widths to test.
static constexpr double WIDTHS[] = {0.5, 2.0, 3.0, 5.0, 10.0};

int main() {
    std::fstream trainStream("train.dat");
    const DigitData trainData(trainStream);

    std::fstream develStream("devel.dat");
    const DigitData develData(develStream);

    MaxTracker<double, double> kernMaxAcc, avgMaxAcc;

    for (auto width : WIDTHS) {
        auto fn = kernel::normalize(kernel::gaussian(width));

        std::vector<perceptron::Kernel> kernPerceps;
        std::vector<perceptron::AveragedKernel> avgPerceps;

        trainData.iterDigits([&](uint32_t d, const Phi &phi, const Labels &labels) {
            printf("train %u\n", d);

            kernPerceps.emplace_back(phi, labels, best::EPOCHS, fn);
            avgPerceps.emplace_back(phi, labels, best::EPOCHS, fn);
        });

        {
            const AccTester<perceptron::Kernel> at(kernPerceps, develData);
            printf("nrm\tw:%f acc:%u/%u (%f)\n", width, at.correct, at.total, at.acc());
            kernMaxAcc.consider(width, at.acc());
        }

        {
            const AccTester<perceptron::AveragedKernel> at(avgPerceps, develData);
            printf("avg\tw:%f acc:%u/%u (%f)\n", width, at.correct, at.total, at.acc());
            avgMaxAcc.consider(width, at.acc());
        }
    }

    printf("nrm: %f:%f\n", kernMaxAcc.key, kernMaxAcc.val);
    printf("avg: %f:%f\n", avgMaxAcc.key, avgMaxAcc.val);
}
