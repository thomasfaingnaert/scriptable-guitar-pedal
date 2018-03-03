#include <vector>
#include "NE10.h"

#ifndef DSPMATH_H_ACFNYYOF
#define DSPMATH_H_ACFNYYOF

typedef ne10_fft_cpx_float32_t Complex;

std::vector<Complex> fft(std::vector<float> x, const unsigned int n, ne10_fft_r2c_cfg_float32_t cfg);
std::vector<float> ifft(std::vector<Complex> x, const unsigned int n, ne10_fft_r2c_cfg_float32_t cfg);
std::vector<Complex> multiplyComplex(const std::vector<Complex>& x, const std::vector<Complex>& y);

#endif /* end of include guard: DSPMATH_H_ACFNYYOF */
