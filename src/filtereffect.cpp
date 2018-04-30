#include "filtereffect.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iterator>

FilterEffect::FilterEffect()
    : numBlocksArrived(0)
{
    constexpr unsigned int numThreads = 4;

    // Create parameters for thread
    for (unsigned int i = 0; i < numThreads; ++i)
    {
        thread_param param;
        param.id = i;
        param.period = std::pow(2,i);
        param.priority = 98 - i;
        param.inputAvailable = false;
        param.filter = this;
        params.push_back(param);
    }

    // The scheduling period is the lcm of all thread periods.
    // Because all thread periods are powers of two, this is also the maximum period.
    // TODO: Since we are adding threads in increasing order of period, this is also the last period.
    //       Maybe change this later?
    schedulingPeriod = std::max_element(params.begin(), params.end(), [](const thread_param& par1, const thread_param& par2) { return par1.period < par2.period; })->period;

    // Initialise all data
    for (unsigned int i = 0; i < schedulingPeriod; ++i)
    {
        // Create cond_vars and mutexes
        main_to_workers_mutexes.push_back(PTHREAD_MUTEX_INITIALIZER);
        workers_to_main_mutexes.push_back(PTHREAD_MUTEX_INITIALIZER);
        main_to_workers_conds.push_back(PTHREAD_COND_INITIALIZER);
        workers_to_main_conds.push_back(PTHREAD_COND_INITIALIZER);

        // Initialise counter to zero
        unsigned int counter = std::count_if(params.begin(), params.end(), [i](const thread_param& par) { return (i+1) % par.period == 0; });
        unsigned int initialCounter = counter - std::count_if(params.begin(), params.end(), [i](const thread_param& par) { return i == par.period - 1; });

        counters.emplace_back(initialCounter);
        counterDefaults.emplace_back(counter);
    }

    // Start all threads
    for (unsigned int i = 0; i < numThreads; ++i)
    {
        // Create thread
        pthread_t thread;
        if (pthread_create(&thread, nullptr, thread_function, &params[i]) != 0)
        {
            std::string errorMsg = "Could not create thread in FilterEffect: ";
            errorMsg += std::strerror(errno);
            throw std::runtime_error(errorMsg);
        }

        // Set priority
        sched_param param;
        param.sched_priority = params[i].priority;
        if (pthread_setschedparam(thread, SCHED_FIFO, &param) != 0)
        {
            std::string errorMsg = "Could not set priority for thread in FilterEffect: ";
            errorMsg += std::strerror(errno);
            throw std::runtime_error(errorMsg);
        }
    }
}

void FilterEffect::push(const std::array<float, Constants::BLOCK_SIZE>& data)
{
    // Wait for workers that have deadline
    pthread_mutex_lock(&workers_to_main_mutexes[numBlocksArrived]);
    while (counters[numBlocksArrived] != 0)
        pthread_cond_wait(&workers_to_main_conds[numBlocksArrived], &workers_to_main_mutexes[numBlocksArrived]);
    pthread_mutex_unlock(&workers_to_main_mutexes[numBlocksArrived]);

    // Set counter
    counters[numBlocksArrived] = counterDefaults[numBlocksArrived];

    // Calculate output

    // Generate output

    // Signal all threads that have to be started
    pthread_mutex_lock(&main_to_workers_mutexes[numBlocksArrived]);

    for (unsigned int i = 0; i < params.size(); ++i)
    {
        if (((numBlocksArrived + 1) % params[i].period) == 0)
        {
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

        // Reset flag
        param->inputAvailable = false;

        pthread_mutex_unlock(&param->filter->main_to_workers_mutexes[waitIndex]);

        // Calculate output

        // Decrement count
        unsigned int signalIndex = (waitIndex + param->period) % param->filter->schedulingPeriod;

        if (param->filter->counters[signalIndex] == 1)
        {
            // We are last to finish, signal main
            pthread_mutex_lock(&param->filter->workers_to_main_mutexes[signalIndex]);
            --param->filter->counters[signalIndex];
            pthread_cond_signal(&param->filter->workers_to_main_conds[signalIndex]);
            pthread_mutex_unlock(&param->filter->workers_to_main_mutexes[signalIndex]);
        }
        else
        {
            // Just decrement
            --param->filter->counters[signalIndex];
        }

        // Update waitIndex
        waitIndex = signalIndex;
    }

    return nullptr;
}
