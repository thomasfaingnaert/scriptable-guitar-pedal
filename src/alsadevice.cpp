#include <algorithm>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "alsadevice.h"

struct pcm {
    /** The PCM's file descriptor */
    int fd;
    /** Flags that were passed to @ref pcm_open */
    unsigned int flags;
    /** Whether the PCM is running or not */
    int running:1;
    /** Whether or not the PCM has been prepared */
    int prepared:1;
    /** The number of underruns that have occured */
    int underruns;
    /** Size of the buffer */
    unsigned int buffer_size;
    /** The boundary for ring buffer pointers */
    unsigned int boundary;
    /** Description of the last error that occured */
    char error[128];
    /** Configuration that was passed to @ref pcm_open */
    struct pcm_config config;
    struct snd_pcm_mmap_status *mmap_status;
    struct snd_pcm_mmap_control *mmap_control;
    struct snd_pcm_sync_ptr *sync_ptr;
    void *mmap_buffer;
    unsigned int noirq_frames_per_msec;
    /** The delay of the PCM, in terms of frames */
    long pcm_delay;
    /** The subdevice corresponding to the PCM */
    unsigned int subdevice;
};

AlsaDevice::AlsaDevice(unsigned int card,
                       unsigned int device,
                       unsigned int rate,
                       unsigned int channels_in,
                       unsigned int channels_out,
                       unsigned int period_size_in,
                       unsigned int period_size_out,
                       unsigned int period_count_in,
                       unsigned int period_count_out)
    : rate(rate)
{
    pcm_config pcm_config_in{};

    pcm_config_in.channels = channels_in;
    pcm_config_in.rate = rate;
    pcm_config_in.period_size = period_size_in;
    pcm_config_in.period_count = period_count_in;
    pcm_config_in.format = PCM_FORMAT_S16_LE;

    pcm_in = pcm_open(card, device, PCM_IN, &pcm_config_in);

    if (pcm_in == nullptr)
    {
        throw std::runtime_error("Failed to allocate memory for the input PCM");
    }
    else if (!pcm_is_ready(pcm_in))
    {
        pcm_close(pcm_in);

        throw std::runtime_error("Input PCM is not ready");
    }

    pcm_config pcm_config_out{};

    pcm_config_out.channels = channels_out;
    pcm_config_out.rate = rate;
    pcm_config_out.period_size = period_size_out;
    pcm_config_out.period_count = period_count_out;
    pcm_config_out.format = PCM_FORMAT_S16_LE;

    pcm_out = pcm_open(card, device, PCM_OUT, &pcm_config_out);

    if (pcm_out == nullptr)
    {
        throw std::runtime_error("Failed to allocate memory for the output PCM");
    }
    else if (!pcm_is_ready(pcm_out))
    {
        pcm_close(pcm_out);

        throw std::runtime_error("Output PCM is not ready");
    }
}

AlsaDevice::~AlsaDevice()
{
    pcm_close(pcm_in);
    pcm_close(pcm_out);
}

void AlsaDevice::push(const std::array<float, Constants::BLOCK_SIZE> &data)
{
    std::array < int16_t, Constants::BLOCK_SIZE * 2 > samples_int;

    for (unsigned int i = 0; i < Constants::BLOCK_SIZE; ++i)
    {
        samples_int[2 * i] = std::min(static_cast<int16_t>(std::floor(INT16_MAX * data[i])), static_cast<int16_t>(INT16_MAX));
        samples_int[2 * i + 1] = 0;
    }

    pcm_writei(pcm_out, samples_int.data(), pcm_bytes_to_frames(pcm_out, sizeof(int16_t) * samples_int.size()));
}

bool AlsaDevice::generate_next()
{
    std::array < int16_t, Constants::BLOCK_SIZE * 2 > samples_int;

    pcm_readi(pcm_in, samples_int.data(), pcm_bytes_to_frames(pcm_in, sizeof(int16_t) * samples_int.size()));

    std::array<float, Constants::BLOCK_SIZE> block;

    for (unsigned int i = 0; i < Constants::BLOCK_SIZE; ++i)
    {
        block[i] = -static_cast<float>(samples_int[2 * i]) / INT16_MIN;
    }

    generate(block);

    if (pcm_out->underruns)
    {
        std::cout << "underruns: " << pcm_out->underruns << std::endl;
    }

    return true;
}
