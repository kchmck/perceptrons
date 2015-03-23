#include <cstdio>
#include <fstream>

#include "best.hpp"
#include "confusion.hpp"
#include "digits.hpp"
#include "kernel.hpp"
#include "perceptron.hpp"
#include "timer.hpp"
#include "util.hpp"
#include "vec.hpp"

// Print out the support vector elements of the given perceptrons.
template<typename P>
void printSupportVectors(const std::vector<P> &perceps) {
    for (uint32_t p = 0; p < DIGITS; p += 1)
        printf("%02u:%03u\n", p, perceps[p].countSupports());
}

// Print out the given confusion matrix.
template<typename P>
void printConfusion(const Confusion<P> &c) {
    for (auto &row : c) {
        for (auto cell : row)
            printf("%03u ", cell);

        putchar('\n');
    }
}

int main() {
    std::fstream trainStream("train.dat");
    const DigitData trainData(trainStream);

    std::fstream testStream("test.dat");
    const DigitData testData(testStream);

    std::vector<perceptron::Linear> basicPerceps;
    std::vector<perceptron::Averaged> avgPerceps;
    std::vector<perceptron::Kernel> polyPerceps;
    std::vector<perceptron::Kernel> gausPerceps;
    std::vector<perceptron::AveragedKernel> avgPolyPerceps;
    std::vector<perceptron::AveragedKernel> avgGausPerceps;

    Timer basicTime, avgTime, polyTime, gausTime, avgPolyTime, avgGausTime;

    printf("precomputing poly kernels...\n");
    auto polyFn = CachedKernel(trainData.phi,
        kernel::normalize(kernel::poly(best::NRM_DEGREE)));
    auto avgPolyFn = CachedKernel(trainData.phi,
        kernel::normalize(kernel::poly(best::AVG_DEGREE)));
    printf("precomputing gaus kernels...\n");
    auto gausFn = CachedKernel(trainData.phi,
        kernel::normalize(kernel::gaussian(best::NRM_WIDTH)));
    auto avgGausFn = CachedKernel(trainData.phi,
        kernel::normalize(kernel::gaussian(best::AVG_WIDTH)));

    printf("training perceptrons...\n");
    trainData.iterDigits([&](auto d, auto &phi, auto &labels) {
        printf("  digit %u\n", d);

        basicTime.add([&] {
            basicPerceps.emplace_back(phi, labels, best::EPOCHS);
        });

        avgTime.add([&] {
            avgPerceps.emplace_back(phi, labels, best::EPOCHS);
        });

        polyTime.add([&] {
            polyPerceps.emplace_back(phi, labels, best::EPOCHS, polyFn);
        });

        gausTime.add([&] {
            gausPerceps.emplace_back(phi, labels, best::EPOCHS, gausFn);
        });

        avgPolyTime.add([&] {
            avgPolyPerceps.emplace_back(phi, labels, best::EPOCHS, avgPolyFn);
        });

        avgGausTime.add([&] {
            avgGausPerceps.emplace_back(phi, labels, best::EPOCHS, avgGausFn);
        });
    });

    printf("basic time:    %" PRIuMAX "ns\n", basicTime.total);
    printf("avg time:      %" PRIuMAX "ns\n", avgTime.total);
    printf("poly time:     %" PRIuMAX "ns\n", polyTime.total);
    printf("gaus time:     %" PRIuMAX "ns\n", gausTime.total);
    printf("avg poly time: %" PRIuMAX "ns\n", avgPolyTime.total);
    printf("avg gaus time: %" PRIuMAX "ns\n", avgGausTime.total);

    printf("computing basic confusion...\n");
    const Confusion<perceptron::Linear> basicCon(testData, basicPerceps);
    printConfusion(basicCon);

    printf("computing avg confusion...\n");
    const Confusion<perceptron::Averaged> avgCon(testData, avgPerceps);
    printConfusion(avgCon);

    printf("computing poly confusion...\n");
    const Confusion<perceptron::Kernel> polyCon(testData, polyPerceps);
    printConfusion(polyCon);
    printSupportVectors(polyPerceps);

    printf("computing gaus confusion...\n");
    const Confusion<perceptron::Kernel> gausCon(testData, gausPerceps);
    printConfusion(gausCon);
    printSupportVectors(gausPerceps);

    printf("computing avg poly confusion...\n");
    const Confusion<perceptron::AveragedKernel> avgPolyCon(testData, avgPolyPerceps);
    printConfusion(avgPolyCon);
    printSupportVectors(avgPolyPerceps);

    printf("computing avg gaus confusion...\n");
    const Confusion<perceptron::AveragedKernel> avgGausCon(testData, avgGausPerceps);
    printConfusion(avgGausCon);
    printSupportVectors(avgGausPerceps);
}
