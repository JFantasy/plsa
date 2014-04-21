#include "plsa.h"

#include <stdlib.h>

#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "usage: ./plsa <input file> <topic number> <epoch number> <output dir>";
    std::cerr << std::endl;
  } else {
    std::string file_name(argv[1]), output_dir(argv[4]);
    std::string theta_file = output_dir + "/" + "theta.out";
    std::string phi_file = output_dir + "/" + "phi.out";
    int topic = atoi(argv[2]), epoch = atoi(argv[3]);

    PLSA plsa;
    plsa.Initialize();
    plsa.LoadData(std::string(file_name));
    plsa.Train(epoch, topic);
    plsa.OutputTheta(theta_file);
    plsa.OutputPhi(phi_file);

    std::cout << "Train Finished!!" << std::endl;
  }
  return 0;
}
