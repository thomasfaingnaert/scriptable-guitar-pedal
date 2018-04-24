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

    for (unsigned int i = 0; i < x.size() / 4; i += 4)
    {
        // Manual 4x loop unrolling to hint g++ to use NEON SIMD instructions
        result[i + 0].r = x[i + 0].r * y[i + 0].r - x[i + 0].i * y[i + 0].i;
        result[i + 0].i = x[i + 0].r * y[i + 0].i + x[i + 0].i * y[i + 0].r;

        result[i + 1].r = x[i + 1].r * y[i + 1].r - x[i + 1].i * y[i + 1].i;
        result[i + 1].i = x[i + 1].r * y[i + 1].i + x[i + 1].i * y[i + 1].r;

        result[i + 2].r = x[i + 2].r * y[i + 2].r - x[i + 2].i * y[i + 2].i;
        result[i + 2].i = x[i + 2].r * y[i + 2].i + x[i + 2].i * y[i + 2].r;

        result[i + 3].r = x[i + 3].r * y[i + 3].r - x[i + 3].i * y[i + 3].i;
        result[i + 3].i = x[i + 3].r * y[i + 3].i + x[i + 3].i * y[i + 3].r;
    }

    return result;
}

