#ifndef RANDOM_GENS_H
#define RANDOM_GENS_H

#include <random>

namespace LJMD {

using std::mt19937_64;
using std::uniform_real_distribution;

/** Class for storing and using instances of random number generators */
class RandomGens {
  public:
    RandomGens() {
        std::random_device true_random_engine {};
        auto seed {true_random_engine()};
        m_random_engine.seed(seed);
    };

    /** Draw a real number uniformly from 0 < x < 1 */
    double uniform_real() {
    return m_uniform_real_dist(m_random_engine);};

  private:
    mt19937_64 m_random_engine {};
    uniform_real_distribution<double> m_uniform_real_dist;
};
} // namespace random_gens

#endif // RANDOM_GENS_H
