#pragma once

#include <algorithm>
#include <cfloat>
#include <random>

/**
 * random
 */

namespace VKernel
{
    // type alias
    template<typename NumericType>
    using uniform_distribution = typename std::conditional<std::is_integral<NumericType>::value,
                                                           std::uniform_int_distribution<NumericType>,
                                                           std::uniform_real_distribution<NumericType>>::type;

    // Random Number Generator, template class
    //-----------------------------------------------------------------------
    template<typename RandomEngine = std::default_random_engine> ///< Default type
    class RandomNumberGenerator
    {

    private:
        // random engine
        RandomEngine m_engine;

    public:
        // Constructor
        template<typename... Params>
        explicit RandomNumberGenerator(Params&&... params) : m_engine(std::forward<Params>(params)...)
        {}

        // set seed
        template<typename... Params>
        void seed(Params&&... seeding)
        {
            m_engine.seed(std::forward<Params>(seeding)...);
        }

        // Underlying distribution
        template<typename DistributionFunc, typename... Params>
        typename DistributionFunc::result_type distribution(Params&&... params) 
        {
            DistributionFunc dist(std::forward<Params>(params)...);
            return dist(m_engine);
        }

        // Return a random number from the range
        template<typename NumericType>
        NumericType uniformDistribution(NumericType lower, NumericType upper)
        {
            if (lower == upper)
            {
                return lower;
            }
            return distribution<uniform_distribution<NumericType>>(lower, upper);
        }

        // Return a random number between 0 and 1
        float uniformUnit() { return uniformDistribution(0.f, std::nextafter(1.f, FLT_MAX)); }

        // Return a random number between -1 and 1
        float uniformSymmetry() { return uniformDistribution(-1.f, std::nextafter(1.f, FLT_MAX)); }

        // bernoulli: The specified probability returns true
        bool bernoulliDistribution(float probability) { return distribution<std::bernoulli_distribution>(probability); }

        // Generate floating-point numbers that conform to a normal distribution, including mean and standard deviation stddev
        float normalDistribution(float mean, float stddev)
        {
            return distribution<std::normal_distribution<float>>(mean, stddev);
        }

        // Fill the entire container/range with random numbers
        template<typename DistributionFunc, typename Range, typename... Params>
        void generator(Range&& range, Params&&... params)
        {
            DistributionFunc dist(std::forward<Params>(params)...);
            return std::generate(std::begin(range), std::end(range), [&] { return dist(m_engine); });
        }
    };

    // Dist Random Number Generator, template class
    //-----------------------------------------------------------------------
    template<typename DistributionFunc,
             typename RandomEngine = std::default_random_engine,
             typename SeedType     = std::seed_seq>
    class DistRandomNumberGenerator
    {
        using ResultType = typename DistributionFunc::result_type;

    private:
        RandomEngine      m_engine; ///< random engine
        DistributionFunc* m_dist = nullptr; ///< distribution func

    public:
        // Constructor
        template<typename... Params>
        explicit DistRandomNumberGenerator(SeedType&& seeding, Params&&...) : m_engine(seeding){}

        // destruct
        ~DistRandomNumberGenerator() { CHAOS_DELETE_T(m_dist); }

        // set seed
        template<typename... Params>
        void seed(Params&&... params)
        {
            m_engine.seed(std::forward<Params>(params)...);
        }

        // Return the next random number that conforms to the distribution
        ResultType next() { return (*m_dist)(m_engine); }
    };

    using DefaultRNG = RandomNumberGenerator<std::mt19937>;
}
