#include <fstream>

#include "best.hpp"
#include "confusion.hpp"
#include "digits.hpp"
#include "kernel.hpp"
#include "perceptron.hpp"
#include "timer.hpp"
#include "util.hpp"
#include "vec.hpp"

int main() {
    std::fstream trainStream("train.dat");
    const DigitData trainData(trainStream);

    std::fstream testStream("test.dat");
    const DigitData testData(testStream);

    std::vector<perceptron::Basic> basicPerceps;
    std::vector<perceptron::Averaged> avgPerceps;
    std::vector<perceptron::Kernel> polyPerceps;
    std::vector<perceptron::Kernel> gausPerceps;
    std::vector<perceptron::AveragedKernel> avgPolyPerceps;
    std::vector<perceptron::AveragedKernel> avgGausPerceps;

    Timer basicTime, avgTime, polyTime, gausTime, avgPolyTime, avgGausTime;

    printf("precomputing poly kernel...\n");
    auto polyFn = CachedKernel(trainData.phi,
        kernel::normalize(kernel::poly(best::DEGREE)));
    printf("precomputing gaus kernel...\n");
    auto gausFn = CachedKernel(trainData.phi,
        kernel::normalize(kernel::gaussian(best::WIDTH)));

    printf("training perceptrons...\n");
    trainData.iterDigits([&](uint32_t d, const Phi &phi, const Labels &labels) {
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
            avgPolyPerceps.emplace_back(phi, labels, best::EPOCHS, polyFn);
        });

        avgGausTime.add([&] {
            avgGausPerceps.emplace_back(phi, labels, best::EPOCHS, gausFn);
        });
    });

    printf("basic time:    %" PRIuMAX "ns\n", basicTime.total);
    printf("avg time:      %" PRIuMAX "ns\n", avgTime.total);
    printf("poly time:     %" PRIuMAX "ns\n", polyTime.total);
    printf("gaus time:     %" PRIuMAX "ns\n", gausTime.total);
    printf("avg poly time: %" PRIuMAX "ns\n", avgPolyTime.total);
    printf("avg gaus time: %" PRIuMAX "ns\n", avgGausTime.total);

    printf("computing basic confusion...\n");
    const Confusion<perceptron::Basic> basicCon(testData, basicPerceps);
    basicCon.print(stdout);
    printf("computing avg confusion...\n");
    const Confusion<perceptron::Averaged> avgCon(testData, avgPerceps);
    avgCon.print(stdout);
    printf("computing poly confusion...\n");
    const Confusion<perceptron::Kernel> polyCon(testData, polyPerceps);
    polyCon.print(stdout);
    printf("computing gaus confusion...\n");
    const Confusion<perceptron::Kernel> gausCon(testData, gausPerceps);
    gausCon.print(stdout);
    printf("computing avg poly confusion...\n");
    const Confusion<perceptron::AveragedKernel> avgPolyCon(testData, avgPolyPerceps);
    avgPolyCon.print(stdout);
    printf("computing avg gaus confusion...\n");
    const Confusion<perceptron::AveragedKernel> avgGausCon(testData, avgGausPerceps);
    avgGausCon.print(stdout);
}
