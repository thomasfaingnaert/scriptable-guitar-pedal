#include "filtereffect.h"

#include <cstring>
#include <cmath>
#include <iostream>

FilterEffect::FilterEffect()
    : numBlocksArrived(0)
{
    constexpr unsigned int numThreads = 3;
    schedulingPeriod = 4;

    // Initialise all data
    for (unsigned int i = 0; i < schedulingPeriod; ++i)
    {
        // Create cond_vars and mutexes
        main_to_workers_mutexes.push_back(PTHREAD_MUTEX_INITIALIZER);
        workers_to_main_mutexes.push_back(PTHREAD_MUTEX_INITIALIZER);
        main_to_workers_conds.push_back(PTHREAD_COND_INITIALIZER);
        workers_to_main_conds.push_back(PTHREAD_COND_INITIALIZER);

        // Initialise counter to zero
        counters.emplace_back(0);
    }

    // TODO: Set priorities for each thread
    for (unsigned int i = 0; i < numThreads; ++i)
    {
        // Create parameters for thread
        thread_param param;
        param.id = i;
        param.period = std::pow(2,i);
        param.inputAvailable = false;
        param.filter = this;
        params.push_back(param);

        // Start thread
        pthread_t thread;
        if (pthread_create(&thread, nullptr, thread_function, &params[i]) != 0)
        {
            std::string errorMsg = "Could not create thread in FilterEffect: ";
            errorMsg += std::strerror(errno);
            throw std::runtime_error(errorMsg);
        }
        threads.push_back(thread);
    }
}

void FilterEffect::push(const std::array<float, Constants::BLOCK_SIZE>& data)
{
    // Wait for workers that have deadline
    pthread_mutex_lock(&workers_to_main_mutexes[numBlocksArrived]);
    while (counters[numBlocksArrived] != 0)
        pthread_cond_wait(&workers_to_main_conds[numBlocksArrived], &workers_to_main_mutexes[numBlocksArrived]);
    pthread_mutex_unlock(&workers_to_main_mutexes[numBlocksArrived]);

    // Calculate output

    // Generate output

    // Signal all threads that have to be started
    pthread_mutex_lock(&main_to_workers_mutexes[numBlocksArrived]);

    for (unsigned int i = 0; i < params.size(); ++i)
    {
        if (((numBlocksArrived + 1) % params[i].period) == 0)
        {
            ++counters[numBlocksArrived];
            params[i].inputAvailable = true;
        }
    }

    pthread_cond_broadcast(&main_to_workers_conds[numBlocksArrived]);
    pthread_mutex_unlock(&main_to_workers_mutexes[numBlocksArrived]);

    // New block arrived
    ++numBlocksArrived;

    if (numBlocksArrived == schedulingPeriod)
        numBlocksArrived = 0;
}

void* FilterEffect::thread_function(void* argument)
{
    thread_param* param = static_cast<thread_param*>(argument);

    // Used to remember what condvar to wait on (main -> worker)
    unsigned int waitIndex = param->period - 1;

    while (true)
    {
        // Wait until main signals there is input available
        pthread_mutex_lock(&param->filter->main_to_workers_mutexes[waitIndex]);
        while (!param->inputAvailable)
            pthread_cond_wait(&param->filter->main_to_workers_conds[waitIndex], &param->filter->main_to_workers_mutexes[waitIndex]);
        pthread_mutex_unlock(&param->filter->main_to_workers_mutexes[waitIndex]);

        // Calculate output

        // Decrement count
        unsigned int signalIndex = (waitIndex + param->period) % param->filter->schedulingPeriod;
        if (--param->filter->counters[signalIndex] == 0)
        {
            // All threads with this deadline have finished: signal main
            pthread_mutex_lock(&param->filter->workers_to_main_mutexes[signalIndex]);
            pthread_cond_signal(&param->filter->workers_to_main_conds[signalIndex]);
            pthread_mutex_unlock(&param->filter->workers_to_main_mutexes[signalIndex]);
        }

        // Update waitIndex
        waitIndex = signalIndex;
    }

    return nullptr;
}
