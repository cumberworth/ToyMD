#ifndef SYSTEM_HPP
#define SYSTEM_HPP
// consider chaning file name

#include <vector>

#include "LJMD/space.hpp"

namespace LJMD {

typedef std::vector<Vector3> Config;

class System {
  public:
    System(int num_particles, SimpleCubicPBC space):
            num_particles {num_particles},
            m_space {space},
            boxl {space.boxl},
            m_config(num_particles, Vector3({0, 0, 0})) {};

    void set_config_to_lattice();

    inline Vector3& get_pos(int i) { return m_config[i]; }
    inline void set_pos(int i, Vector3 v) {
        m_space.wrap(v);
        m_config[i] = v;
    }
    inline Config get_config() { return m_config; }

    const int num_particles;
    const double boxl;

  private:
    Config m_config;
    SimpleCubicPBC m_space;
};

} // namespace LJMD

#endif // SYSTEM_HPP
