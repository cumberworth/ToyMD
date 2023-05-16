#ifndef OFILE_H
#define OFILE_H

#include <fstream>
#include <string>

#include "LJMD/interactions.hpp"
#include "LJMD/quantities.hpp"
#include "LJMD/system.hpp"

namespace LJMD {

using std::string;

class VTFOutputFile {
  public:
    VTFOutputFile(string filename, System& system);
    ~VTFOutputFile();
    void write_step(int step);

  private:
    void write_structure(System& system);

    System& m_system;
    std::ofstream m_file;
    string m_filename;
};

// I don't like that the quantities are actually calculated here
class QuantitiesOutputFile {
  public:
    QuantitiesOutputFile(string filename, Quantities& quantities);
    ~QuantitiesOutputFile();
    void write_step(int step);

  private:
    Quantities& m_quantities;
    std::ofstream m_file;
    string m_filename;
};

} // namespace LJMD

#endif // OFILE_H
