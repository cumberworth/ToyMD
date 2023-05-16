#ifndef QUANTITIES_HPP
#define QUANTITIES_HPP

#include "LJMD/integrator.hpp"
#include "LJMD/interactions.hpp"
#include "LJMD/system.hpp"

namespace LJMD {

class Quantities {
  public:
    Quantities(
            System& system,
            Interactions& interactions,
            Integrator& integrator);
    inline double calc_ave_energy_pot() { return m_sum_energy_pot / m_samples; }
    inline double calc_ave_energy_kin() { return m_sum_energy_kin / m_samples; }
    inline double calc_ave_energy_tot() { return m_sum_energy_tot / m_samples; }
    inline double calc_ave_pressure() { return m_sum_pressure / m_samples; }
    inline double calc_ave_temp() { return m_sum_temp / m_samples; }
    void update();

    double energy_pot;
    double energy_kin;
    double energy_tot;
    double pressure;
    double temp;

  private:
    System& m_system;
    Interactions& m_interactions;
    Integrator& m_integrator;
    int m_samples {0};
    double m_sum_energy_pot {0};
    double m_sum_energy_kin {0};
    double m_sum_energy_tot {0};
    double m_sum_pressure {0};
    double m_sum_temp {0};
};

} // namespace LJMD

#endif // INTEGRATOR_HPP
