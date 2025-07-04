#include "Random.h"
#include <chrono>

// Initialize static instance pointer
Random* Random::instance = nullptr;

Random::Random()
{
    // Seed with current time
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);
}

Random& Random::getInstance()
{
    // Create instance on first use (lazy initialization)
    if (instance == nullptr) {
        instance = new Random();
    }
    return *instance;
}

void Random::reseed(unsigned seed) 
{
    generator.seed(seed);
}

int Random::getInt(int min, int max) 
{
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

float Random::getFloat(float min, float max) 
{
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

double Random::getDouble(double min, double max) 
{
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

bool Random::getBool(double trueProbability) 
{
    std::bernoulli_distribution distribution(trueProbability);
    return distribution(generator);
}

unsigned char Random::getUChar(unsigned char min, unsigned char max) {
    std::uniform_int_distribution<unsigned int> distribution(min, max);
    return static_cast<unsigned char>(distribution(generator));
}