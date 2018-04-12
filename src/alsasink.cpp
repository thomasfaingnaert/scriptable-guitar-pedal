#include <cmath>
#include <iostream>

#include "tinyalsa/pcm.h"

#include "alsasink.h"

AlsaSink::AlsaSink(unsigned int card, unsigned int device, unsigned int sampling_frequency)
{
    unsigned int flags = PCM_OUT;

    const struct pcm_config config = {
            .channels = 2,
            .rate = sampling_frequency,
            .period_size = 1024,
            .period_count = 2,
            .format = PCM_FORMAT_S16_LE
    };

    pcm = pcm_open(card, device, flags, &config);

    if (pcm == nullptr) {
        std::cerr << "failed to allocate memory for PCM";

    } else if (!pcm_is_ready(pcm)){
        pcm_close(pcm);

        std::cerr << "failed to open PCM";
    }
}

void AlsaSink::push(const std::shared_ptr <std::vector<float>> &samples_float, unsigned int channel)
{
    std::vector<uint16_t> samples;

    samples.reserve(2 * samples_float->size());

    for (auto sample_float : *samples_float)
    {
        samples.push_back(static_cast<uint16_t>(std::floor(INT16_MAX * sample_float)));
        samples.push_back(0);
    }

    pcm_writei(pcm, samples.data(), pcm_bytes_to_frames(pcm, sizeof(uint16_t) * samples.size()));
}
