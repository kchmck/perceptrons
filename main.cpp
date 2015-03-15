#include <cstdio>
#include <fstream>

#include "kernel.hpp"
#include "optdigits.hpp"
#include "perceptron.hpp"
#include "vec.hpp"

int main() {
    std::fstream trainStream("train.dat");

    if (!trainStream)
        abort();

    std::fstream develStream("devel.dat");

    if (!develStream)
        abort();

    std::fstream testStream("test.dat");

    if (!testStream)
        abort();

    OptDigits od(std::move(trainStream), std::move(develStream),
                 std::move(testStream));
}
