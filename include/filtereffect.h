#ifndef FILTEREFFECT_H_TJL7ARNQ
#define FILTEREFFECT_H_TJL7ARNQ

#include <atomic>
#include <pthread.h>
#include <deque>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "NE10.h"
#include "frequencydelayline.h"
#include "sink.h"
#include "source.h"

class FilterEffect : public Source<float>, public Sink<float>
{
    public:
        FilterEffect();
        virtual void push(const std::array<float, Constants::BLOCK_SIZE>& data);

    private:
        // The main thread uses these to signal all worker threads when new data is available
        std::deque<pthread_mutex_t> main_to_workers_mutexes;
        std::deque<pthread_cond_t> main_to_workers_conds;

        // The worker threads use these to signal the main thread that all workers have finished
        std::deque<pthread_mutex_t> workers_to_main_mutexes;
        std::deque<pthread_cond_t> workers_to_main_conds;

        // The workers use these to know when all threads have finished
        std::deque<std::atomic<unsigned int>> counters;

        // The values to reset the counters to
        std::deque<unsigned int> counterDefaults;

        // This function is executed by each thread
        static void* thread_function(void* argument);

        // Used as a parameter for the thread function
        struct thread_param
        {
            thread_param(const FrequencyDelayLine& fdl) : fdl(fdl) { }

            std::string name; // The name of the thread
            unsigned int period; // The scheduling period (in amount of process calls)
            unsigned int priority; // The scheduling priority for this thread
            bool inputAvailable; // Flag that is used by main to tell workers if input is available
            FilterEffect* filter; // This pointer
            std::vector<float> input; // The input
            std::vector<float> outputBuffer; // Buffer for output
            std::shared_ptr<std::mutex> outputMutex; // Used to control access to outputBuffer
            FrequencyDelayLine fdl; // The FDL used by this thread
        };

        // The parameters for each thread
        std::deque<thread_param> params;

        // Used to remember how many blocks have arrived so far
        unsigned int numBlocksArrived;

        // The scheduling period
        unsigned int schedulingPeriod;

        // Buffer for input
        std::deque<float> inputBuffer;
};

#endif /* end of include guard: FILTEREFFECT_H_TJL7ARNQ */
