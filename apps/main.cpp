#include <iostream>
#include <memory>

#include "LJMD/integrator.hpp"
#include "LJMD/interactions.hpp"
#include "LJMD/ofile.hpp"
#include "LJMD/params.hpp"
#include "LJMD/quantities.hpp"
#include "LJMD/random_gens.hpp"
#include "LJMD/space.hpp"
#include "LJMD/system.hpp"

using std::cerr;
using std::cout;
using std::endl;

using namespace LJMD;

int main(const int argc, const char* argv[]) {
    try {
        const Params params {argc, argv};
        auto random_gens = std::make_unique<RandomGens>();
        SimpleCubicPBC space {params.boxl};
        auto system = std::make_unique<System>(params.num_particles, space);
        auto interactions = std::make_unique<Interactions>(space, *system);
        std::unique_ptr<VerletIntegrator> integrator =
                std::make_unique<VerletIntegrator>(
                        space, *system, *interactions, params.Dt, params.temp);

        system->set_config_to_lattice();
        integrator->init_velocities(*random_gens);
        // cout << integrator->calc_energy_kin() << endl;
        auto quantities = std::make_unique<Quantities>(
                *system, *interactions, *integrator);
        VTFOutputFile vtf_ofile {"test.vtf", *system};
        QuantitiesOutputFile quantities_ofile {"test.dat", *quantities};
        //vtf_ofile.write_step(0);
        //quantities->update();
        //quantities_ofile.write_step(0);
        for (int step {0}; step != params.init_steps; ++step) {
            interactions->calc_forces();
//            cout << "step " << step << endl;
            integrator->step_and_scale();
        }

        vtf_ofile.write_step(0);
        quantities->update();
        quantities_ofile.write_step(0);

        for (int step {1}; step != params.steps + 1; ++step) {
            interactions->calc_forces();
            integrator->step();
            if (step % params.write_interval == 0) {
                vtf_ofile.write_step(step);
                quantities->update();
                quantities_ofile.write_step(step);
            }
        }

    } catch (const std::exception& e) {
        cout << endl;
        cout << "An exception occurred during the run" << endl;
        cout << endl;
        cerr << e.what() << endl;
        cout << endl;
        cout << "Ending run unsuccesfully" << endl;
        cout << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
