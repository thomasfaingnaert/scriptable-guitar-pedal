#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <iostream>

#include "alsadevice.h"

AlsaDevice::AlsaDevice(unsigned int card,
                       unsigned int device,
                       unsigned int rate,
                       unsigned int channels_in,
                       unsigned int channels_out,
                       unsigned int period_size_in,
                       unsigned int period_size_out,
                       unsigned int period_count_in,
                       unsigned int period_count_out)
{
    const struct pcm_config pcm_config_in = {
            channels_in,
            rate,
            period_size_in,
            period_count_in,
            PCM_FORMAT_S16_LE
    };

    pcm_in = pcm_open(card, device, PCM_IN, &pcm_config_in);

    if (pcm_in == nullptr)
    {
        std::cerr << "Failed to allocate memory for the input PCM!\n";
    }
    else if (!pcm_is_ready(pcm_in))
    {
        std::cerr << "Input PCM is not ready!\n";

        pcm_close(pcm_in);
    }

    const struct pcm_config pcm_config_out = {
            channels_out,
            rate,
            period_size_out,
            period_count_out,
            PCM_FORMAT_S16_LE
    };

    pcm_out = pcm_open(card, device, PCM_OUT, &pcm_config_out);

    if (pcm_out == nullptr)
    {
        std::cerr << "Failed to allocate memory for the output PCM!\n";
    }
    else if (!pcm_is_ready(pcm_out))
    {
        std::cerr << "Output PCM is not ready!\n";

        pcm_close(pcm_out);
    }
}

AlsaDevice::~AlsaDevice()
{
    pcm_close(pcm_in);
    pcm_close(pcm_out);
}

void AlsaDevice::push(const std::array<float, Constants::BLOCK_SIZE>& data)
{
    std::array<int16_t, Constants::BLOCK_SIZE * 2> samples_int;

    for (unsigned int i = 0; i < Constants::BLOCK_SIZE; ++i)
    {
        samples_int[2 * i] = static_cast<int16_t>(std::floor(INT16_MAX * data[i]));
        samples_int[2 * i + 1] = 0;
    }

    pcm_writei(pcm_out, samples_int.data(), pcm_bytes_to_frames(pcm_out, sizeof(int16_t) * samples_int.size()));
}

void AlsaDevice::generate_next()
{
    std::array<int16_t, Constants::BLOCK_SIZE * 2> samples_int;

    pcm_readi(pcm_in, samples_int.data(), pcm_bytes_to_frames(pcm_in, sizeof(int16_t) * samples_int.size()));

    std::array<float, Constants::BLOCK_SIZE> block;

    for (unsigned int i = 0; i < Constants::BLOCK_SIZE; ++i)
    {
        block[i] = static_cast<float>(-samples_int[2 * i] / INT16_MIN);
    }

    generate(block);
}
