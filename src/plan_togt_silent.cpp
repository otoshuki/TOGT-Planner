#include "drolib/race/race_track.hpp"
#include "drolib/race/race_params.hpp"
#include "drolib/race/race_planner.hpp"
#include <filesystem>
#include <iostream>

using namespace drolib;
namespace fs = std::filesystem;

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <base_name>\n";
    return 1;
  }
  const std::string base_name = argv[1];
  const std::string quad_name = "crazyflie";
  const std::string config_name = quad_name + "_setups.yaml";
  const std::string track_name = "custom_track.yaml";
  const std::string traj_name = base_name + "_traj.csv";
  const std::string wpt_name = base_name + "_wpt.yaml";

  try {
    fs::path root(PROJECT_ROOT);
    fs::path config_path = root / "parameters" / quad_name;
    fs::path track_path = root / "resources/racetrack" / track_name;
    fs::path traj_path = root / "resources/trajectory" / traj_name;
    fs::path wpt_path = root / "resources/trajectory" / wpt_name;

    auto raceparams = std::make_shared<RaceParams>(config_path, config_name);
    auto raceplanner = std::make_shared<RacePlanner>(*raceparams);
    auto racetrack = std::make_shared<RaceTrack>(track_path);

    bool success = raceplanner->planTOGT(racetrack);
    if (!success) {
      std::cerr << "Trajectory planning failed." << std::endl;
      return 1;
    }

    TrajExtremum extremum = raceplanner->getExtremum();
    //std::cout << extremum << std::endl;

    MincoSnapTrajectory traj = raceplanner->getTrajectory();
    traj.save(traj_path);
    traj.saveSegments(wpt_path, 1);

    //std::cout << "Trajectory saved to " << traj_path << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
