#ifndef INTERACTIONS_HPP
#define INTERACTIONS_HPP

#include "LJMD/space.hpp"
#include "LJMD/system.hpp"

namespace LJMD {

constexpr double CUTOFF_FRACTION = 0.4999;

class Interactions {
  public:
    Interactions(SimpleCubicPBC space, System&);
    void calc_forces();
    double calc_energy_pot();
    inline Vector3& get_force(int i) { return m_forces[i]; }

  private:
    SimpleCubicPBC m_space;
    System& m_system;
    Config m_forces;
    const double m_cutoff;
    const double m_cutoff2;
    const double m_energy_cut;
};

} // namespace LJMD

#endif // INTERACTIONS_HPP
