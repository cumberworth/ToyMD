#include <cmath>

#include "LJMD/system.hpp"

namespace LJMD {

void System::set_config_to_lattice() {
    auto sites_per_side {static_cast<int>(std::ceil(std::cbrt(num_particles)))};
    double spacing {m_space.boxl / sites_per_side};
    int i {0};
    for (double xi {0.0}; xi != sites_per_side; ++xi) {
        for (double yi {0.0}; yi != sites_per_side; ++yi) {
            for (double zi {0.0}; zi != sites_per_side; ++zi) {
                if (i == num_particles) {
                    return;
                }
                Vector3 pos = {xi, yi, zi};
                pos = spacing * (pos + 0.5);
                set_pos(i, pos);
                ++i;
            }
        }
    }
}

} // namespace LJMD
