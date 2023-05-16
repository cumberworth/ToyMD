#include <cmath>
#include <iostream>

#include "LJMD/integrator.hpp"
#include "LJMD/space.hpp"

namespace LJMD {

using std::cout;
using std::endl;

double calc_velocity_scalef(int num_particles, double temp, double energy_kin) {
    return std::sqrt(3 * (num_particles - 1) * temp / energy_kin);
};

VerletIntegrator::VerletIntegrator(
        SimpleCubicPBC space,
        System& system,
        Interactions& interactions,
        double Dt,
        double temp):
        m_space {space},
        m_system {system},
        m_old_config {system.get_config()},
        m_new_config {system.get_config()},
        m_velocities(system.num_particles, Vector3({0, 0, 0})),
        m_interactions {interactions},
        m_Dt {Dt},
        m_Dt2 {std::pow(Dt, 2)},
        m_i2Dt {1 / (2 * m_Dt)},
        m_temp {temp} {}

void VerletIntegrator::init_velocities(RandomGens& random_gens) {
    Vector3 momentum {0, 0, 0};
    double energy_kin {0};
    for (int i {0}; i != m_system.num_particles; ++i) {
        for (int j {0}; j != 3; ++j) {
            m_velocities[i][j] = random_gens.uniform_real() - 0.5;
        }
        momentum += m_velocities[i];
        energy_kin += sqnorm(m_velocities[i]);
    }
    momentum /= m_system.num_particles;

    double scalef {
            calc_velocity_scalef(m_system.num_particles, m_temp, energy_kin)};

    for (int i {0}; i != m_system.num_particles; ++i) {
        m_velocities[i] -= momentum;
        m_velocities[i] *= scalef;
        m_old_config[i] = m_system.get_pos(i) - m_velocities[i] * m_Dt;
        m_space.wrap(m_old_config[i]);
    }
}

void VerletIntegrator::step() {
    for (int i {0}; i != m_system.num_particles; ++i) {
        Vector3 Dr {
                m_space.calc_diff(2 * m_system.get_pos(i), m_old_config[i])};
        Vector3 v {Dr + m_interactions.get_force(i) * m_Dt2};
        m_old_config[i] = m_system.get_pos(i);
        m_system.set_pos(i, v);
    }
}

void VerletIntegrator::step_and_scale() {
    Vector3 momentum {0, 0, 0};
    double energy_kin {0};
    for (int i {0}; i != m_system.num_particles; ++i) {
        Vector3 Dr {
                m_space.calc_diff(2 * m_system.get_pos(i), m_old_config[i])};
        m_new_config[i] = Dr + m_interactions.get_force(i) * m_Dt2;
        Vector3 Dr_new_old {
                m_space.calc_diff(m_new_config[i], m_old_config[i])};
        m_velocities[i] = Dr_new_old * m_i2Dt;
        energy_kin += sqnorm(m_velocities[i]);
        momentum += m_velocities[i];

        Vector3& F {m_interactions.get_force(i)};
        F *= m_Dt2;
    }
    momentum /= m_system.num_particles;

    double scalef {
            calc_velocity_scalef(m_system.num_particles, m_temp, energy_kin)};
    for (int i {0}; i != m_system.num_particles; ++i) {
        m_velocities[i] *= scalef;
        m_new_config[i] = 2 * m_Dt * m_velocities[i] + m_old_config[i];
        m_old_config[i] = m_system.get_pos(i);
        m_system.set_pos(i, m_new_config[i]);
    }
}

double VerletIntegrator::calc_energy_kin() {
    double energy_kin {0};
    for (int i {0}; i != m_system.num_particles; ++i) {
        Vector3 Dr {
                m_space.calc_diff(2 * m_system.get_pos(i), m_old_config[i])};
        m_new_config[i] = Dr + m_interactions.get_force(i) * m_Dt2;
        Vector3 Dr_new_old {
                m_space.calc_diff(m_new_config[i], m_old_config[i])};
        m_velocities[i] = Dr_new_old * m_i2Dt;
        energy_kin += sqnorm(m_velocities[i]);
    }

    return energy_kin / 2;
}

} // namespace LJMD
