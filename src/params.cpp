#include <fstream>
#include <iostream>
#include <string>

#include <boost/program_options.hpp>

#include "LJMD/params.hpp"

namespace LJMD {

namespace po = boost::program_options;

using std::cout;
using std::endl;
using std::string;

Params::Params(const int argc, const char* argv[]) {
    // Displayed options
    po::options_description displayed_options {"Allowed options"};

    // Command line options description
    po::options_description cl_options {"Command line options"};
    cl_options.add_options()("help,h", "Display available options")(
            "parameter_filename,i", po::value<string>(), "Input file")(
            "version,v", "Print program version");
    displayed_options.add(cl_options);

    // Options description
    po::options_description options {"Options and parameters"};

    // could add descriptions
    options.add_options()(
            "init_steps", po::value<int>(&init_steps)->default_value(0))(
            "steps", po::value<int>(&steps)->default_value(0))(
            "write_interval",
            po::value<int>(&write_interval)->default_value(0))(
            "num_particles", po::value<int>(&num_particles)->default_value(0))(
            "boxl", po::value<double>(&boxl)->default_value(0.0))(
            "Dt", po::value<double>(&Dt)->default_value(0.0))(
            "temp", po::value<double>(&temp)->default_value(0.0));
    //      "restart_traj_filebase",
    //       po::value<string>(&m_restart_traj_filebase)->default_value(""),
    //       "Trajectory restart filebase"
    displayed_options.add(options);

    // Command line variable map
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, displayed_options), vm);
    po::notify(vm);
    if (vm.count("help")) {
        cout << endl;
        cout << displayed_options;
        cout << endl;
        exit(1);
    }
    if (vm.count("version")) {
        cout << endl;
        // cout << "Git commit hash: " << GIT_COMMIT << endl;
        cout << "0.1.0";
        cout << endl;
        exit(1);
    }
    if (not vm.count("parameter_filename")) {
        cout << "Input parameter file must be provided" << endl
             << "Run with -h to see all options" << endl;
        cout << endl;
        exit(1);
    }

    // Parameter file variable map
    std::ifstream parameter_file {vm["parameter_filename"].as<string>()};
    po::store(po::parse_config_file(parameter_file, displayed_options), vm);
    po::notify(vm);
}

} // namespace LJMD
