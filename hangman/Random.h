#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace Random
{
    // Return a random number in range [min, max]
    template <typename T>
    T get(T min, T max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<T> dist(min, max);
        return dist(gen);
    }
}

#endif // RANDOM_H
