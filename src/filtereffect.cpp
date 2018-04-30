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
        unsigned int initialCounters[] = { 0, 1, 1, 2 };
        counters.emplace_back(initialCounters[i]);
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
    std::cout << "----- BEGIN PROCESS -----" << std::endl;

    // Wait for workers that have deadline

    std::cout << "[main] waiting for t->m " << numBlocksArrived << std::endl;

    pthread_mutex_lock(&workers_to_main_mutexes[numBlocksArrived]);
    while (counters[numBlocksArrived] != 0)
        pthread_cond_wait(&workers_to_main_conds[numBlocksArrived], &workers_to_main_mutexes[numBlocksArrived]);
    pthread_mutex_unlock(&workers_to_main_mutexes[numBlocksArrived]);

    std::cout << "[main] got t->m " << numBlocksArrived << std::endl;

    // Set counter
    unsigned int countVals[] = { 1, 2, 1, 3};
    std::cout << "[main] set counter " << numBlocksArrived << " to " << countVals[numBlocksArrived] << std::endl;
    counters[numBlocksArrived] = countVals[numBlocksArrived];

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

    std::cout << "[main] broadcasting m->t " << numBlocksArrived << std::endl;
    pthread_cond_broadcast(&main_to_workers_conds[numBlocksArrived]);
    pthread_mutex_unlock(&main_to_workers_mutexes[numBlocksArrived]);

    // New block arrived
    ++numBlocksArrived;

    if (numBlocksArrived == schedulingPeriod)
        numBlocksArrived = 0;

    std::cout << "----- END PROCESS -----" << std::endl;
}

void* FilterEffect::thread_function(void* argument)
{
    thread_param* param = static_cast<thread_param*>(argument);

    // Used to remember what condvar to wait on (main -> worker)
    unsigned int waitIndex = param->period - 1;

    while (true)
    {
        std::cout << "[thread " << param->id << "] waiting for m->t " << waitIndex << std::endl;
        // Wait until main signals there is input available
        pthread_mutex_lock(&param->filter->main_to_workers_mutexes[waitIndex]);
        while (!param->inputAvailable)
            pthread_cond_wait(&param->filter->main_to_workers_conds[waitIndex], &param->filter->main_to_workers_mutexes[waitIndex]);
        std::cout << "[thread " << param->id << "] got m->t " << waitIndex << std::endl;

        // Reset flag
        param->inputAvailable = false;

        pthread_mutex_unlock(&param->filter->main_to_workers_mutexes[waitIndex]);

        // Calculate output

        // Decrement count
        unsigned int signalIndex = (waitIndex + param->period) % param->filter->schedulingPeriod;
        std::cout << "[thread " << param->id << "] decremented counter " << signalIndex << " from " << param->filter->counters[signalIndex] << " to " << param->filter->counters[signalIndex] - 1 << std::endl;

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
