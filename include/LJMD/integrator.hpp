#ifndef INTEGRATOR_HPP
#define INTEGRATOR_HPP

#include "LJMD/interactions.hpp"
#include "LJMD/random_gens.hpp"
#include "LJMD/space.hpp"
#include "LJMD/system.hpp"

namespace LJMD {

double calc_velocity_scalef(int num_particles, double temp, double energy_kin);

class Integrator {
  public:
    virtual ~Integrator() = default;
    virtual void step() = 0;
    virtual void step_and_scale() = 0;
    virtual void init_velocities(RandomGens&) = 0;
    virtual double calc_energy_kin() = 0;
};

class VerletIntegrator: public Integrator {
  public:
    VerletIntegrator(
            SimpleCubicPBC space,
            System& system,
            Interactions& interactions,
            double Dt,
            double temp);
    void init_velocities(RandomGens&) final;
    void step() final;
    void step_and_scale() final;
    double calc_energy_kin() final;

  private:
    SimpleCubicPBC m_space;
    System& m_system;
    Config m_old_config;
    Config m_new_config;
    Config m_velocities;
    Interactions& m_interactions;
    double m_Dt;
    double m_i2Dt;
    double m_Dt2;
    double m_temp;
    double m_energy_kin;
};

class VelocityVerletIntegrator: Integrator {
  public:
    void step() final;
    void step_and_scale() final;
};

class EulerIntegrator: Integrator {
  public:
    void step() final;
    void step_and_scale() final;
};

} // namespace LJMD

#endif // INTEGRATOR_HPP
