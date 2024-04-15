#include "cosmics.hh"


//********************************************************************
//***** NPARTICLES ***************************************************

int nshowers(Detector *obs, double kp){

  double SA = (obs->x[1] - obs->x[0]) * (obs->y[1] - obs->y[0]);
  obs->SA = SA; 
  
  std::cout << "\n\033[1m*********** COSMIC GEN ************\n\033[0m"
	    << "Detector dimensions\n"
	    << "\t x = [" << obs->x[0] << "," << obs->x[1] << "] m\n"
    	    << "\t y = [" << obs->y[0] << "," << obs->y[1] << "] m\n"
	    << "\t SA = " << obs->SA << " m2\n"
	    << "Spill time: \t" << obs->t << " s\n"
	    << "Constant: \t" << kp << "\n"
	    << "Energy Range: \t[" << obs->E[0] << "," << obs->E[1] << "]\n"
	    << "\033[1m***********************************\n\033[0m" << std::endl;;

  double nshowers = round(2 * M_PI * obs->SA * obs->t * kp * 
			  (pow(obs->E[0],-1.7) - pow(obs->E[1],-1.7))/1.7);
  
  
  std::cout << "\033[34;1m[INFO]\033[0m Generating " << nshowers
	    << " primary cosmics." << std::endl;
  
  
  return nshowers;
  
}

//********************************************************************
