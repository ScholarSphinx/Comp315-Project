#pragma once
#include <random>

class RandomGenerator
{
public:
	static int getInt(int min, int max) //Generates a random integer between the specified minimum and maximum values (inclusive), using a random number generator and a uniform distribution to ensure that the generated numbers are uniformly distributed within the specified range, allowing for flexible generation of random integers for various purposes in the application
    {
        static std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dist(min, max);
        return dist(gen);
    }
};
