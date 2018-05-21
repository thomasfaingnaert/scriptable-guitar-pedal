#include <chrono>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <pthread.h>
#include <thread>

#include "NE10.h"
#include "codec.h"
#include "delayeffect.h"
#include "distortioneffect.h"
#include "filesink.h"
#include "filesource.h"
#include "filtereffect.h"
#include "prudevice.h"
#include "sampledata.h"
#include "tremoloeffect.h"
#include "webserver.h"

int main(int argc, char *argv[])
{
    // NE10 Initialisation
    if (ne10_init() != NE10_OK)
    {
        std::cerr << "Could not initialise Ne10." << std::endl;
        return EXIT_FAILURE;
    }

    // Set thread priority
#if 1
    sched_param param;
    param.sched_priority = 99;

    if (pthread_setschedparam(pthread_self(), SCHED_FIFO, &param) != 0)
    {
        std::cerr << "Failed to set priority for thread: Error code " << errno << ": " << std::strerror(errno) << std::endl;
        return EXIT_FAILURE;
    }
#endif

    // Webserver
    WebServer server(8888);
    server.alsa_thread();

    //auto alsaDevice = WebServer::getAlsaDevice();

    /*
    float mainCoeff = 1.0;
    std::vector<unsigned int> delays;
    std::vector<float> coeffs;
    float decay = 0.4;

    int i = 1;
    while (decay >= 0.05)
    {
        delays.push_back(48000/2 * i++);
        coeffs.push_back(decay);
        decay *= decay;
    }

    auto effect = std::make_shared<DelayEffect>(mainCoeff, delays, coeffs);
    */

/*
    auto effect = std::make_shared<DistortionEffect>(8,2,1,0.5);

*/
    /*
    auto effect = std::make_shared<TremoloEffect>(0.4, 48000 / 6);
*/

/*
    SampleData filter("rev.wav");

    std::vector<float> reverb = filter.getSamples()[0];

    for(float& sample : reverb)
        sample /= 75;

    auto effect = std::make_shared<FilterEffect>(reverb);
   */
/*
    alsaDevice->connect(effect);
    effect->connect(alsaDevice);


    while(alsaDevice->generate_next());
*/

    return EXIT_SUCCESS;
}

