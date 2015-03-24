#ifndef VEC_HPP
#define VEC_HPP

#include <cassert>
#include <cmath>
#include <vector>

// Defines a C++ vector extended with vector algebra methods.
class Vec: public std::vector<double> {
public:
    using std::vector<double>::vector;

    // Add in the given vector elementwise.
    void addIn(const Vec &other) {
        assert(size() == other.size());

        for (size_t i = 0; i < size(); i += 1)
            (*this)[i] += other[i];
    }

    // Divide each element by the given scalar.
    void divIn(double scalar) {
        for (size_t i = 0; i < size(); i += 1)
            (*this)[i] /= scalar;
    }

    // Subtract the given vector elementwise and return the result.
    Vec sub(const Vec &other) const {
        assert(size() == other.size());

        Vec vec(*this);

        for (size_t i = 0; i < size(); i += 1)
            vec[i] -= other[i];

        return vec;
    }

    // Calculate the norm-squared.
    double normSquared() const {
        double sum = 0.0;

        for (size_t i = 0; i < size(); i += 1)
            sum += pow((*this)[i], 2);

        return sum;
    }

    // Divide each element by the norm.
    void normalize() {
        double n = sqrt(normSquared());

        for (size_t i = 0; i < size(); i += 1)
            (*this)[i] /= n;
    }

    // Calculate the dot product with the given vector.
    double dot(const Vec &other) const {
        assert(size() == other.size());

        double sum = 0.0;

        for (size_t i = 0; i < size(); i += 1)
            sum += (*this)[i] * other[i];

        return sum;
    }

    // Multiply each element by the given scalar and return the result.
    Vec mult(double scalar) const {
        Vec vec(*this);

        for (size_t i = 0; i < size(); i += 1)
            vec[i] *= scalar;

        return vec;
    }
};

// A vector of feature vectors/examples.
typedef std::vector<Vec> Phi;
// A vector of +1/-1.
typedef std::vector<int> Labels;

#endif
