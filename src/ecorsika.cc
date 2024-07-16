/* ================= ECORSIKA =================== *
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
#include <cmath>
#include <algorithm>
#include <vector>

#include <map>

#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"

#include "io.hh"
#include "DBReader.hh"
#include "detector.hh"
#include "EHandler.hh"
#include "EShower.hh"
#include "EParticle.hh"

#include "distribute.hh"

int main (int argc, char *argv[]) {


 
  // === COMMAND LINE ===============================================

  // ----- Required Arguments -----
  int nReqArg = 1; // No. of required args
  std::string infile = argv[1];

  
  // ----- Optional Defaults -----
  double xVals[2] = {-4.,4.};
  double yVals[2] = {-4.,4.};
  double EVals[2] = {50, 100000};
  double spillT = 2.;
  
  int primOverride = -1;


  std::string outfile = "enubet_cosmics.root";
  
  // ----- Read Command Line -----
  int opt;
  optind = nReqArg + 1;
  while ((opt = getopt(argc, argv, ":vhE:x:y:t:n:o:")) != -1)
    {
      std::string tempStr;
      switch (opt)
	{
	case 'h':
	  help();
	  return 0;
	case 'E':
	  tempStr = optarg;
	  EVals[0] = std::stod(tempStr.substr(0,tempStr.find(",")));
	  EVals[1] = std::stod(tempStr.substr(tempStr.find(",")+1,tempStr.size()-1));
	  break;
	case 'x':
	  tempStr = optarg;
	  xVals[0] = std::stod(tempStr.substr(0,tempStr.find(",")));
	  xVals[1] = std::stod(tempStr.substr(tempStr.find(",")+1,tempStr.size()-1));
	  break;
	case 'y':
	  tempStr = optarg;
	  yVals[0] = std::stod(tempStr.substr(0,tempStr.find(",")));
	  yVals[1] = std::stod(tempStr.substr(tempStr.find(",")+1,tempStr.size()-1));
	  break;
	case 't':
	  spillT = std::stod(optarg);
	  break;
	case 'n':
	  primOverride = std::stoi(optarg);
	  break;
	case 'o':
	  outfile = optarg;
	  break;
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
      tempStr.clear(); // To be safe
    }


  // ----- File exists -----
  if (!is_alive(infile)) {
    std::cout << "\033[31;1m[ERROR]\033[0m File \'" << infile << "\' does not exist." << std::endl;
    return 0;
  }

  
  // ================================================================


  
  // === DATABASE ===================================================

  // Read the database using the reader class 
  DBReader *corsDB = new DBReader(infile.c_str());

  //	Setup a detector level to read from 
  Detector *pdMuon = new Detector(xVals, yVals, EVals);
  pdMuon->ValidateRange();
  
  // Collect primaries from the detector. Should probably put this in the function
  std::vector<int> primary_gen = pdMuon->GetPrimaries(corsDB, 1.8E4, 2.);
  
  // ================================================================ 



  // === EVENT LOOPS ================================================ 
  
  // Outfile for saving the events 
  TFile corsOUT(outfile.c_str(), "RECREATE");

  std::cout << "\033[1;34m[INFO]\033[0m Saving ENUBET cosmics to output file:\n\t"
	    << outfile << std::endl;
  
	// Set the spill time for all EHandler objects
  EHandler::SetSpillT(spillT);

  // Create a handler for the shower
	EShower showerHandler(corsDB, pdMuon);
	showerHandler.CreateTree();

	// Create a handler for the particle 
	EParticle particleHandler(corsDB, pdMuon);
	particleHandler.CreateTree();

	// Sort the vector lowest to highest in order to speed up searching (we can skip many
  // of the early events this way...
  sort(primary_gen.begin(), primary_gen.end());
  
  // Loop through the showers selected and process them 
  for (int shower : primary_gen) {

    showerHandler.Process(shower);
    particleHandler.Process(shower, &showerHandler);
 
  }
  // ================================================================ 
  
  corsOUT.cd();
  
  // Write everything to the output file 
  showerHandler.GetTree()->Write();
  particleHandler.GetTree()->Write();



  corsOUT.Close();


  return 0;
}
