#include <iostream>
#include <cmath>

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
        .channels = channels_in,
        .rate = rate,
        .period_size = period_size_in,
        .period_count = period_count_in,
        .format = PCM_FORMAT_S16_LE
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
        .channels = channels_out,
        .rate = rate,
        .period_size = period_size_out,
        .period_count = period_count_out,
        .format = PCM_FORMAT_S16_LE
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

void AlsaDevice::push(const std::shared_ptr<std::vector<float>> &samples_float, unsigned int channel)
{
    std::vector<int16_t> samples_int;

    samples_int.reserve(2 * BLOCK_SIZE);

    for (auto sample_float : *samples_float)
    {
        samples_int.push_back(static_cast<int16_t>(std::floor(INT16_MAX * sample_float)));
        samples_int.push_back(0);
    }

    pcm_writei(pcm_out, samples_int.data(), pcm_bytes_to_frames(pcm_out, sizeof(int16_t) * samples_int.size()));
}

void AlsaDevice::generate_next()
{
    std::vector<int16_t> samples_int(2 * BLOCK_SIZE);

    pcm_readi(pcm_in, samples_int.data(), pcm_bytes_to_frames(pcm_in, sizeof(int16_t) * samples_int.size()));

    auto block = std::make_shared<std::vector<float>>();

    block->reserve(BLOCK_SIZE);

    for (auto sample_int = samples_int.begin(); sample_int != samples_int.end(); sample_int += 2)
    {
        block->push_back(-static_cast<float>(*sample_int) / INT16_MIN);
    }

    generate(block);
}
