
#include "output.hpp"

output::output(parameters * &par){
  dx = par->get_dx();
  dy = par->get_dy();
  Nxcell = par->get_Nxcell();
  Nycell = par->get_Nycell();
  outputDirectory = par->get_outputDirectory();
  namefile = outputDirectory + "huz_evolution";

}//end constructor

output::~output(){

}

