#include "filtereffect.h"
#include "sampledata.h"

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <iterator>

FilterEffect::FilterEffect()
    : numBlocksArrived(0)
{
    constexpr unsigned int numThreads = 1;
    SampleData impulse("impulse.wav");
    std::vector<float> impulseData = impulse.getSamples()[0];
    std::cout << "samples: " << impulseData.size() << "\n";

    // Create parameters for thread
    for (unsigned int i = 0; i < numThreads; ++i)
    {
        std::vector<std::vector<float>> impulseResponses;
        for (unsigned int j = 0; j < 500; ++j)
        {
            impulseResponses.emplace_back(impulseData.begin() + j * Constants::BLOCK_SIZE, impulseData.begin() + (j+1) * Constants::BLOCK_SIZE);
        }
        FrequencyDelayLine fdl(Constants::BLOCK_SIZE, impulseResponses);

        thread_param param(fdl);
        param.name = "FDL-" + std::to_string(i);
        param.period = std::pow(2,i);
        param.priority = 98 - i;
        param.inputAvailable = false;
        param.filter = this;
        param.outputMutex = std::make_shared<std::mutex>();
        param.outputBuffer = std::vector<float>((2 * param.period - 1) * Constants::BLOCK_SIZE);
        params.push_back(param);
    }

    // The scheduling period is the lcm of all thread periods.
    // Because all thread periods are powers of two, this is also the maximum period.
    // TODO: Since we are adding threads in increasing order of period, this is also the last period.
    //       Maybe change this later?
    schedulingPeriod = std::max_element(params.begin(), params.end(), [](const thread_param& par1, const thread_param& par2) { return par1.period < par2.period; })->period;

    // Remember enough input blocks
    inputBuffer = std::deque<float>(schedulingPeriod * Constants::BLOCK_SIZE);

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

        // Set name of thread
        if (pthread_setname_np(thread, params[i].name.c_str()) != 0)
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
    std::array<float, Constants::BLOCK_SIZE> result{};
    for (unsigned int i = 0; i < params.size(); ++i)
    {
        std::unique_lock<std::mutex> l(*params[i].outputMutex);
        if (!params[i].outputBuffer.empty())
        {
            ne10_add_float(result.data(), result.data(), params[i].outputBuffer.data(), Constants::BLOCK_SIZE);
            params[i].outputBuffer.erase(params[i].outputBuffer.begin(), params[i].outputBuffer.begin() + Constants::BLOCK_SIZE);
        }
        else
        {
            std::cerr << "outputBuffer was empty!" << std::endl;
        }
    }

    // Generate output
    generate(result);

    // Remove old input and add new input block
    inputBuffer.erase(inputBuffer.begin(), inputBuffer.begin() + Constants::BLOCK_SIZE);
    inputBuffer.insert(inputBuffer.end(), data.begin(), data.end());

    // Signal all threads that have to be started
    pthread_mutex_lock(&main_to_workers_mutexes[numBlocksArrived]);

    for (unsigned int i = 0; i < params.size(); ++i)
    {
        if (((numBlocksArrived + 1) % params[i].period) == 0)
        {
            params[i].inputAvailable = true;
            params[i].input = std::vector<float>(inputBuffer.end() - (Constants::BLOCK_SIZE * params[i].period), inputBuffer.end());
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
        std::vector<float> res(param->input.size());
        param->fdl.process(param->input.begin(), param->input.end(), res.begin());
        {
            std::lock_guard<std::mutex> l(*param->outputMutex);
            param->outputBuffer.insert(param->outputBuffer.end(), res.begin(), res.end());
        }

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
