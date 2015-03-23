#include <fstream>

#include "best.hpp"
#include "confusion.hpp"
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
        auto fn = CachedKernel(trainData.phi, kernel::normalize(kernel::poly(width)));

        std::vector<perceptron::Kernel> kernPerceps;
        std::vector<perceptron::AveragedKernel> avgPerceps;

        trainData.iterDigits([&](auto d, auto &phi, auto &labels) {
            printf("train %u\n", d);

            kernPerceps.emplace_back(phi, labels, best::EPOCHS, fn);
            avgPerceps.emplace_back(phi, labels, best::EPOCHS, fn);
        });

        {
            const Confusion<perceptron::Kernel> c(develData, kernPerceps);
            printf("nrm\tw:%f acc:%u/%u (%f)\n", width, c.correct, c.total, c.acc());
            kernMaxAcc.consider(width, c.acc());
        }

        {
            const Confusion<perceptron::AveragedKernel> c(develData, avgPerceps);
            printf("avg\tw:%f acc:%u/%u (%f)\n", width, c.correct, c.total, c.acc());
            avgMaxAcc.consider(width, c.acc());
        }
    }

    printf("nrm: %f:%f\n", kernMaxAcc.key, kernMaxAcc.val);
    printf("avg: %f:%f\n", avgMaxAcc.key, avgMaxAcc.val);
}
