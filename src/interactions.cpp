#include <cmath>
#include <iostream>

#include "LJMD/interactions.hpp"
#include "LJMD/space.hpp"

namespace LJMD {

Interactions::Interactions(SimpleCubicPBC space, System& system):
        m_space {space},
        m_system {system},
        m_forces(system.num_particles, Vector3({0, 0, 0})),
        m_cutoff {m_system.boxl * CUTOFF_FRACTION},
        m_cutoff2 {std::pow(m_cutoff, 2)},
        m_energy_cut {
                4.0 * (std::pow(m_cutoff, -12.0) - std::pow(m_cutoff, -6.0))} {}

void Interactions::calc_forces() {
    double pressure {0};
    m_forces.assign(m_system.num_particles, Vector3({0, 0, 0}));
    //Vector3 rij {};
    for (int i {0}; i != m_system.num_particles - 1; ++i) {
        for (int j {i + 1}; j != m_system.num_particles; ++j) {

            // check if jnitializing once outside the loop is faster
            // apply periodic boundaries
            Vector3 rij {m_space.calc_diff(
                    m_system.get_pos(i), m_system.get_pos(j))};
            // start unroll
            //rij[0] = m_system.get_pos(i)[0] - m_system.get_pos(j)[0];
            //rij[1] = m_system.get_pos(i)[1] - m_system.get_pos(j)[1];
            //rij[2] = m_system.get_pos(i)[2] - m_system.get_pos(j)[2];

            //rij[0] -= m_space.boxl*std::round(rij[0]*m_space.m_iboxl);
            //rij[1] -= m_space.boxl*std::round(rij[1]*m_space.m_iboxl);
            //rij[2] -= m_space.boxl*std::round(rij[2]*m_space.m_iboxl);

            // end
            double r2 {sqnorm(rij)};
            if (r2 < m_cutoff2) {

                // consider not making a new variable
                double ir2 {1 / r2};
                double ir6 {pow(ir2, 3)};
                double Ff {ir6 * (ir6 - 0.5)};
                pressure += Ff;
                Vector3 Fij {rij * Ff * ir2};
                m_forces[i] += Fij;
                m_forces[j] -= Fij;
            }
        }
    }
    for (int i {0}; i != m_system.num_particles; ++i) {
        m_forces[i] *= 48;
    }
    pressure *= 48 / (3 * std::pow(m_space.boxl, 3));
//    std::cout << pressure << std::endl;
}

double Interactions::calc_energy_pot() {
    double energy_pot {0.0};
    for (int i {0}; i != m_system.num_particles - 1; ++i) {
        for (int j {i + 1}; j != m_system.num_particles; ++j) {
            Vector3 rij {m_space.calc_diff(
                    m_system.get_pos(i), m_system.get_pos(j))};
            double r2 {sqnorm(rij)};
            if (r2 < m_cutoff2) {
                double ir6 {1 / pow(r2, 3)};
                energy_pot += 4.0 * ir6 * (ir6 - 1.0) - m_energy_cut;
            }
        }
    }

    return energy_pot;
}

} // namespace LJMD
