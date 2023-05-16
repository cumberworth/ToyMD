#ifndef PARAMS_HPP
#define PARAMS_HPP

namespace LJMD {

class Params {
  public:
    Params(const int argc, const char* argv[]);
    int init_steps;
    int steps;
    int write_interval;
    int num_particles;
    double boxl;
    double Dt;
    double temp;
};

} // namespace LJMD

#endif // PARAMS_HPP
