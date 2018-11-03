#include "zero_crossing.h"
#include "routines.h"
#include <algorithm>
#include <cmath>

ZeroCrossing::ZeroCrossing(size_t index, std::vector<double> wdc) : index(index) {
    this->index = index;
    this->right_mm = ModulusMaxima(index, wdc);
    this->left_mm = ModulusMaxima(index, wdc);
    this->mm_amplitude = 0.0;
}
