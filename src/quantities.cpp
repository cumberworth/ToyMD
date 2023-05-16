#include "LJMD/quantities.hpp"

#include "LJMD/interactions.hpp"
#include "LJMD/system.hpp"

namespace LJMD {

Quantities::Quantities(System& system, Interactions& interactions, Integrator& integrator):
        m_system {system}, m_interactions {interactions}, m_integrator {integrator} {

    update();
}

void Quantities::update() {
    energy_pot = m_interactions.calc_energy_pot();
    m_sum_energy_pot += energy_pot;
    energy_kin = m_integrator.calc_energy_kin();
    m_sum_energy_kin += energy_kin;
    energy_tot = energy_pot + energy_kin;
    m_sum_energy_tot += energy_tot;
}

} // namespace LJMD
