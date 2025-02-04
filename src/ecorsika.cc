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
#include "EDetector.hh"
#include "EShower.hh"
#include "EParticle.hh"

#include "distribute.hh"

int main (int argc, char *argv[]) {


 
  // === COMMAND LINE ===============================================

  // ----- Required Arguments -----
  int nReqArg = 1; // No. of required args
  std::string infile = argv[1];

  
  // ----- Optional Defaults -----
  double EVals[2] = {80., 1.E8};
  double spillT = 2.;
  
  double det_size[3] = {12., 12., 12.};
  double det_center[3] = {0., 0., 0.};
  double gen_buffer[2] = {0., 0.};
  double gen_offset = 0.;
  
  int primOverride = -1;

  std::string outfile = "enubet_cosmics.root";
  std::string directory = "/Users/jmcelwee/Documents/NDTF/cosmics/database";
  
  // ----- Read Command Line -----
  int opt;
  optind = nReqArg + 1;
  while ((opt = getopt(argc, argv, ":vhE:b:c:d:t:n:o:O:D:")) != -1)
    {
      std::string tempStr;
      switch (opt)
	{
	case 'h':
	  IO::help();
	  return 0;
	case 'E':
	  tempStr = optarg;
	  EVals[0] = IO::return_arg(tempStr,2)[0];
	  EVals[1] = IO::return_arg(tempStr,2)[1];
	  break;
	case 'D':
	  directory = optarg;
	  break;
	case 'b':
	  tempStr = optarg;
	  gen_buffer[0] = IO::return_arg(tempStr,2)[0];
	  gen_buffer[1] = IO::return_arg(tempStr,2)[1];
	  break;
	case 'c':
	  tempStr = optarg;
	  det_center[0] = IO::return_arg(tempStr,3)[0];
	  det_center[1] = IO::return_arg(tempStr,3)[1];
	  det_center[2] = IO::return_arg(tempStr,3)[2];
	  break;
	case 'd':
	  tempStr = optarg;
	  det_size[0] = IO::return_arg(tempStr,3)[0];
	  det_size[1] = IO::return_arg(tempStr,3)[1];
	  det_size[2] = IO::return_arg(tempStr,3)[2];
	case 't':
	  spillT = std::stod(optarg);
	  break;
	case 'n':
	  primOverride = std::stoi(optarg);
	  break;
	case 'O':
	  gen_offset = std::stod(optarg);
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

  
  // ================================================================

  
  
  // === DATABASE ===================================================

  // Read the database using the reader class 
  //  DBReader *corsDB = new DBReader(infile.c_str());

  //	Setup a detector level to read from 
  EDetector *pdMuon = new EDetector(det_size, det_center);
  
  
  // ================================================================ 

  

  // === EVENT LOOPS ================================================ 
  
  // Outfile for saving the events 
  TFile corsOUT(outfile.c_str(), "RECREATE");

  std::cout << "\033[1;34m[INFO]\033[0m Saving ENUBET cosmics to output file:\n\t"
	    << outfile << std::endl;
  
  // Set the spill time for all EHandler objects
  EShower::SetSpillT(spillT);

  //  std::vector<std::string> primaries = {"H", "He", "C", "O"};
  std::vector<std::string> primaries = {"H", "He", "C"};
  
  // Create a handler for the shower
  //  EShower showerHandler(corsDB, pdMuon, EVals, EShower::H);
  EShower showerHandler(directory, pdMuon, EVals, primaries);
  showerHandler.CreateTree();
  showerHandler.SetBuffer(gen_buffer);
  showerHandler.SetOffset(gen_offset);
  showerHandler.NShowers();
  //  std::vector<int> testVec = showerHandler.GetShowers();
  
  // Create a handler for the particle 
  //  EParticle particleHandler(corsDB, pdMuon);
  EParticle particleHandler(directory, pdMuon, primaries);
  particleHandler.CreateTree();
  
  for (int i=0; i < primaries.size(); i++) {

    std::cout << "Generating primary... " << primaries.at(i) << std::endl;
    
    // Loop through the showers selected and process them 
    for ( int shower : showerHandler.GetShowers(i) ) {
    
      showerHandler.Process(shower, i);
      particleHandler.Process(shower, &showerHandler, i);
      
    }

    particleHandler.ResetParticle();
    
  }
  // ================================================================ 
  
  corsOUT.cd();
  
  std::cout << "Number of particles: " << particleHandler.GetParticles().size() << std::endl;
  
  // Write everything to the output file 
  showerHandler.GetTree()->Write();
  particleHandler.GetTree()->Write();

  
  corsOUT.Close();

  return 0;
}
