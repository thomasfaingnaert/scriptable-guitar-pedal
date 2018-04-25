#include "dspmath.h"
#include "arm_neon_complex.h"

std::vector<Complex> fft(std::vector<float> x, const unsigned int n, ne10_fft_r2c_cfg_float32_t cfg)
{
    std::vector<Complex> out(n / 2 + 1);

    // first resize x by adding zeros
    x.resize(n);
    ne10_fft_r2c_1d_float32(out.data(), x.data(), cfg);

    return out;
}

std::vector<float> ifft(std::vector<Complex> x, const unsigned int n, ne10_fft_r2c_cfg_float32_t cfg)
{
    std::vector<float> out(n);
    ne10_fft_c2r_1d_float32(out.data(), x.data(), cfg);
    return out;
}

std::vector<Complex> multiplyComplex(const std::vector<Complex>& x, const std::vector<Complex>& y)
{
    // TODO: Make this work with sizes which are not a multiple of 8
    std::vector<Complex> result(x.size());
    arm_neon_complex_multiply((float*)result.data(), (float*)x.data(), (float*)y.data(), x.size() - 1);
    return result;
}

