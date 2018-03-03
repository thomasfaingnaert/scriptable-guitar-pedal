#include "dspmath.h"

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
    std::vector<Complex> result(x.size());

    for (unsigned int i = 0; i < x.size(); ++i)
    {
        result[i].r = x[i].r * y[i].r - x[i].i * y[i].i;
        result[i].i = x[i].r * y[i].i + x[i].i * y[i].r;
    }

    return result;
}

