/* ================== COSMICS =================== *
 * Created: 07-04-2024                            *
 * Author:  Jordan McElwee                        *
 * Email: mcelwee@lp2ib.in2p3.fr                  *
 *                                                *
 * Functions to deal with the generation and      *
 * handling of the primary cosmics.               *
 *                                                *
 * Changelog:                                     * 
 * ============================================== */

#include "cosmics.hh"

//********************************************************************
//***** NPARTICLES ***************************************************

/* 
   Takes in the detector information and uses the shower calculation
   to calculate the number of showers that would be needed for such a
   detector area.
   kp is the shower constant, which can change based on the primary
   species. primOverride allows the user to specify the primary number
   for testing purposes.
 */
int nshowers(Detector *obs, double kp, int primOverride){

  // Calculate surface area and store
  double SA = (obs->x[1] - obs->x[0]) * (obs->y[1] - obs->y[0]);
  obs->SA = SA; 

  // Output information for user to determine if detector is correct
  std::cout << "\n\033[1m*********** COSMIC GEN ************\n\033[0m"
	    << "Detector dimensions\n"
	    << "\t x = [" << obs->x[0] << "," << obs->x[1] << "] m\n"
    	    << "\t y = [" << obs->y[0] << "," << obs->y[1] << "] m\n"
	    << "\t SA = " << obs->SA << " m2\n"
	    << "Spill time: \t" << obs->t << " s\n"
	    << "Constant: \t" << kp << "\n"
	    << "Energy Range: \t[" << obs->E[0] << "," << obs->E[1] << "]\n"
	    << "\033[1m***********************************\n\033[0m" << std::endl;;

  // Should be able to change the spectral index? --JM
  double nshowers = round(2 * M_PI * obs->SA * obs->t * kp * 
			  (pow(obs->E[0],-1.7) - pow(obs->E[1],-1.7))/1.7);

  // If user specifies a number, return the specified value (in primOverride)
  if (primOverride != -1){
    std::cout << "\033[1;34m[INFO]\033[0m Overwriting primary cosmic calculation."
	      << " Throwing " << primOverride << " instead." << std::endl;
    return primOverride; 
  }
  
  std::cout << "\033[34;1m[INFO]\033[0m Generating " << nshowers
	    << " primary cosmics." << std::endl;
  return nshowers;
  
}

//********************************************************************
