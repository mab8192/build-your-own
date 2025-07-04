#pragma once

#include <random>
#include <limits>

class Random 
{
public:
    // Get the singleton instance
    static Random& getInstance();

    // Reseed the generator
    void reseed(unsigned seed);

    // Generate random integer in range [min, max]
    int getInt(int min = 0, int max = std::numeric_limits<int>::max());

    // Generate random float in range [min, max]
    float getFloat(float min = 0.0f, float max = 1.0f);

    // Generate random double in range [min, max]
    double getDouble(double min = 0.0, double max = 1.0);

    // Generate random boolean with given probability of true
    bool getBool(double trueProbability = 0.5);

    // Generate random unsigned char in range [min, max]
    unsigned char getUChar(unsigned char min = 0, unsigned char max = std::numeric_limits<unsigned char>::max());

private:
    std::mt19937 generator;

    // Private constructor for singleton pattern
    Random();

    // Deleted copy constructor and assignment operator
    Random(const Random&) = delete;
    Random& operator=(const Random&) = delete;

    // Static instance
    static Random* instance;
};