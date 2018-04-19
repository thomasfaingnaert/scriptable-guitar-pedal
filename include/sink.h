#ifndef SINK_H_D2GUWTG5
#define SINK_H_D2GUWTG5

#include <vector>

template<typename T>
class Sink
{
    public:
        virtual ~Sink() { }
        virtual void push(const std::vector<T>& data) = 0;
};

#endif /* end of include guard: SINK_H_D2GUWTG5 */
