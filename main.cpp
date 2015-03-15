#include <cstdio>
#include <fstream>

#include "kernel.hpp"
#include "digits.hpp"
#include "perceptron.hpp"
#include "vec.hpp"

int main() {
    std::fstream trainStream("train.dat");
    const DigitData trainData(trainStream);

    std::fstream develStream("devel.dat");
    const DigitData develData(develStream);

    std::fstream testStream("test.dat");
    const DigitData testData(testStream);

    if (!testStream)
        abort();

}
