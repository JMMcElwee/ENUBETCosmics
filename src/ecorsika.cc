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
#include <cmath>
#include <algorithm>
#include <vector>

#include "TFile.h"
#include "TTree.h"
#include "TRandom.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TStyle.h"

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
  double E[2] = {50, 10000000};


  // Number of showers to generate
  int nShowers = round(-2 * 3.14 * detSA * t * k * (pow(E[1],-1.7) - pow(E[0],-1.7))/1.7);
  //  nShowers = 1000000;
  
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


  // === DATABASE ===================================================

  DBReader *corsDB = new DBReader(infile.c_str());
  
  // Choose a random selection of showers from the database
  std::vector<int> showers_gen;
  showers_gen.reserve(nShowers+1); // Reserve mem. to avoid reallocation
  
  int rnd_shower;
  double shower_energy;
  for (int evnt = 1; evnt <= nShowers; evnt++)
    {
      if ( (evnt % (nShowers/100) == 0) || (evnt == nShowers) )
	status(evnt, nShowers);
      
      rnd_shower = gRandom->Integer(corsDB->GetNShowers());
      corsDB->GetShower(rnd_shower);
      shower_energy = corsDB->SE();
      
      if ((std::find(showers_gen.begin(),showers_gen.end(),rnd_shower)
	   == showers_gen.end()) && shower_energy < E[1] && shower_energy > E[0])
	showers_gen.push_back(rnd_shower);
      else evnt--;
    }
  
  // ================================================================ 



  // === EVENT LOOPS ================================================ 

  TFile corsOUT("test.root", "RECREATE");
  
  // ----- Shower information -----
  TTree *showerTree = new TTree("shower", "Shower information");
  
  int sID;
  double sE, sTheta, sPhi, sT, sVtx[2];
  
  showerTree->Branch("id", &sID, "id/I");
  showerTree->Branch("E", &sE, "E/D");
  showerTree->Branch("theta", &sTheta, "theta/D");
  showerTree->Branch("phi", &sPhi, "phi/D");
  showerTree->Branch("t", &sT, "t/D");
  showerTree->Branch("vtx[2]", sVtx, "vtx[2]/D");


  // ----- Particle data -----
  TTree *particleTree = new TTree("particles", "Particles crossing the detector");
  
  int pPDG, pParID;
  double pEK, pP[3], pVtx[2], pT;
  
  particleTree->Branch("parID", &pParID, "parID/I");
  particleTree->Branch("pdg", &pPDG, "pdg/I");
  particleTree->Branch("eK", &pEK, "eK/D");
  particleTree->Branch("mom[3]", pP, "mom[3]/D");
  particleTree->Branch("vtx[2]", pVtx, "vtx[2]/D");
  particleTree->Branch("t", &pT, "t/D");

  

  
  // Use this variable to speed up searching for particles
  int last_position = 0;

  // Sort the vector lowest to highest in order to speed up searching (we can skip many
  // of the early events this way...
  sort(showers_gen.begin(), showers_gen.end());
  int current_shower, newShowerID = 0; 
  
  
  for (int shower : showers_gen) {
    corsDB->GetShower(shower);

    sID = newShowerID;
    sE = corsDB->SE();
    sTheta = corsDB->STheta();
    sPhi = corsDB->SPhi();
    
    sVtx[0] = gRandom->Uniform(detXDim[0], detXDim[1]);
    sVtx[1] = gRandom->Uniform(detYDim[0], detYDim[1]);

    sT = gRandom->Uniform(0, t);
    
    showerTree->Fill();
   
    
    for (int evnt = last_position; evnt < corsDB->GetNEvents(); evnt++){
      corsDB->GetEvent(evnt);
      current_shower = corsDB->ParID();

      if (current_shower == shower) {

	pParID = newShowerID;
	pPDG = corsDB->PDG();
	pEK = corsDB->EK();
	pP[0] = corsDB->PX();
	pP[1] = corsDB->PY();
	pP[2] = corsDB->PZ();
	pVtx[0] = corsDB->X()/100 + sVtx[0];
	pVtx[1] = corsDB->Y()/100 + sVtx[1];
	pT = corsDB->T()/1E9 + sT;

	particleTree->Fill();
       
	
      }
      else if (current_shower > shower) {
	last_position = evnt;
	break;
      }
      
    }

    newShowerID++;
    
  }
  // ================================================================ 
  
  corsOUT.cd();
  showerTree->Write();
  particleTree->Write(); 

  corsOUT.Close();


  return 0;
}
