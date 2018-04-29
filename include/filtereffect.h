#ifndef FILTEREFFECT_H_TJL7ARNQ
#define FILTEREFFECT_H_TJL7ARNQ

#include <atomic>
#include <pthread.h>
#include <deque>
#include <vector>

#include "sink.h"
#include "source.h"

class FilterEffect : public Source<float>, public Sink<float>
{
    public:
        FilterEffect();
        virtual void push(const std::array<float, Constants::BLOCK_SIZE>& data);

    private:
        // The worker threads
        std::vector<pthread_t> threads;

        // The main thread uses these to signal all worker threads when new data is available
        std::deque<pthread_mutex_t> main_to_workers_mutexes;
        std::deque<pthread_cond_t> main_to_workers_conds;

        // The worker threads use these to signal the main thread that all workers have finished
        std::deque<pthread_mutex_t> workers_to_main_mutexes;
        std::deque<pthread_cond_t> workers_to_main_conds;

        // The workers use these to know when all threads have finished
        std::deque<std::atomic<int>> counters;

        // This function is executed by each thread
        static void* thread_function(void* argument);

        // Used as a parameter for the thread function
        struct thread_param
        {
            unsigned int id; // The ID of the thread
            unsigned int period; // The scheduling period (in amount of process calls)
            bool inputAvailable; // Flag that is used by main to tell workers if input is available
            FilterEffect* filter; // This pointer
        };

        // The parameters for each thread
        std::deque<thread_param> params;

        // Used to remember how many blocks have arrived so far
        unsigned int numBlocksArrived;

        // The scheduling period
        unsigned int schedulingPeriod;
};

#endif /* end of include guard: FILTEREFFECT_H_TJL7ARNQ */
