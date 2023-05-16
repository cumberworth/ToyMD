#include "LJMD/interactions.hpp"
#include "LJMD/ofile.hpp"
#include "LJMD/system.hpp"

namespace LJMD {

using std::endl;

VTFOutputFile::VTFOutputFile(string filename, System& system):
        m_file {filename}, m_system {system}, m_filename {filename} {

    write_structure(system);
}

VTFOutputFile::~VTFOutputFile() { m_file.close(); }

void VTFOutputFile::write_step(int step) {
    m_file << "# " << step << endl;
    m_file << "t" << endl;
    for (int i {0}; i != m_system.num_particles; ++i) {
        Vector3& v {m_system.get_pos(i)};
        for (auto& c: v) {
            m_file << c << " ";
        }
        m_file << endl;
    }
    m_file << endl;
}

void VTFOutputFile::write_structure(System& system) {
    for (int i {0}; i != system.num_particles; ++i) {
        m_file << "atom " << i << " ";
        m_file << "type " << 0 << " ";
        m_file << "resid " << i << " ";
        m_file << "radius " << 1.0 << endl;
    }
    m_file << endl;
    double x {system.boxl};
    m_file << "pbc " << x << " " << x << " " << x << endl;
    m_file << endl;
}

QuantitiesOutputFile::QuantitiesOutputFile(
        string filename,
        Quantities& quantities):
        m_file {filename},
        m_quantities {quantities},
        m_filename {filename} {

    m_file << "step energy_pot energy_kin energy_tot energy_diff pressure temp" << endl;
}

void QuantitiesOutputFile::write_step(int step) {
    m_file << step << " ";
    m_file << m_quantities.energy_pot << " ";
    m_file << m_quantities.energy_kin << " ";
    m_file << m_quantities.energy_tot << " ";
    m_file << endl;
}

QuantitiesOutputFile::~QuantitiesOutputFile() { m_file.close(); }

}
