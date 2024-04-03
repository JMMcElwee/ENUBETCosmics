/* ================ CORSIKA2ROOT ================ *
 * Created: 30-03-2024                            *
 * Author:  Jordan McElwee                        *
 * Email: mcelwee@lp2ib.in2p3.fr                  *
 *                                                *
 * Take information from CORSIKA database and     *
 * generate correct spatial and timing            *
 * distributions.                                 *
 *                                                *
 * Changelog:                                     * 
 * ============================================== */

#include <iostream>
#include <unistd.h>

#include "io.hh"
#include "DBReader.hh"

int main (int argc, char *argv[]) {


  // === CALCULATE SHOWERS ==========================================

  // Surface area of detector
  double detXDim[2] = {-4.,4}; // m
  double detYDim[2] = {-4.,4}; // m

  double detSA = (detXDim[1] - detXDim[0]) * (detYDim[1] - detYDim[0]);


  // Spill window of the period
  double t = 2.0; // secs

  
  // Constant of the integral
  double k = 1.8E4;


  // Energy range;
  double E[2] = {10, 100};


  // Number of showers to generate
  double nShowers = 3.14 * detSA * t * k * (pow(E[1],3.7) - pow(E[0],3.7))/3.7;

  
  //  std::cout << "Detector Dimensions are: " << detSA << std::endl;
  std::cout << "Total number of showers to generate: " << nShowers << std::endl;
  
  // ================================================================


 
  // === COMMAND LINE ===============================================

  // ----- Required Arguments -----
  int nReqArg = 1; // No. of required args
  std::string infile = argv[1];

  
  // ----- Optional Defaults -----

  
  // ----- Read Command Line -----
  int opt;
  optind = nReqArg + 1;
  while ((opt = getopt(argc, argv, ":vh")) != -1)
    {
      switch (opt)
	{
	case 'h':
	  help();
	  return 0;
	case 'v':
	  //	  verbose = true;
	  break;
	case ':':
	  printf("\033[1;31m[ERROR]\033[0m -%c requires an argument.\n",optopt);
	  return 0;
	case '?':
	  printf("\033[1;33m[ERROR]\033[0m -%c is an unknown argument... just ignoring it.\n",optopt);
	  break;
	}
    }
  
  // ================================================================

  

  
  return 0;
}
