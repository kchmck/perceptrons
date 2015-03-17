#ifndef VEC_HPP
#define VEC_HPP

#include <cassert>
#include <cmath>
#include <vector>

class Vec: public std::vector<double> {
public:
    using std::vector<double>::vector;

    void add_in(const Vec &other) {
        assert(size() == other.size());

        for (size_t i = 0; i < size(); i += 1)
            operator[](i) += other[i];
    }

    void div_in(double scalar) {
        for (size_t i = 0; i < size(); i += 1)
            operator[](i) /= scalar;
    }

    Vec sub(const Vec &other) const {
        assert(size() == other.size());

        Vec vec(*this);

        for (size_t i = 0; i < size(); i += 1)
            vec[i] -= other[i];

        return vec;
    }

    double norm() const {
        double sum = 0.0;

        for (size_t i = 0; i < size(); i += 1)
            sum += pow(operator[](i), 2);

        return sum;
    }

    void norm_mut() {
        double n = norm();

        for (size_t i = 0; i < size(); i += 1)
            (*this)[i] /= n;
    }

    double dot(const Vec &other) const {
        assert(size() == other.size());

        double sum = 0.0;

        for (size_t i = 0; i < size(); i += 1)
            sum += operator[](i) * other[i];

        return sum;
    }

    Vec mult(double scalar) const {
        Vec vec(*this);

        for (size_t i = 0; i < size(); i += 1)
            vec[i] *= scalar;

        return vec;
    }
};

typedef std::vector<Vec> Phi;
typedef std::vector<int> Labels;

#endif
