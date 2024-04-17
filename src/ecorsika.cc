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
#include "cosmics.hh"

#include "distribute.hh"

int main (int argc, char *argv[]) {


 
  // === COMMAND LINE ===============================================

  // ----- Required Arguments -----
  int nReqArg = 1; // No. of required args
  std::string infile = argv[1];

  
  // ----- Optional Defaults -----
  Detector pdMuon;
  pdMuon.x[0] = -4.;
  pdMuon.x[1] = 4.;
  pdMuon.y[0] = -4.;
  pdMuon.y[1] = 4.;
  pdMuon.t = 2.;
  pdMuon.E[0] = 50;
  pdMuon.E[1] = 100000;

  int nprimaries = -1;

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
	  pdMuon.E[0] = std::stod(tempStr.substr(0,tempStr.find(",")));
	  pdMuon.E[1] = std::stod(tempStr.substr(tempStr.find(",")+1,tempStr.size()-1));
	  break;
	case 'x':
	  tempStr = optarg;
	  pdMuon.x[0] = std::stod(tempStr.substr(0,tempStr.find(",")));
	  pdMuon.x[1] = std::stod(tempStr.substr(tempStr.find(",")+1,tempStr.size()-1));
	  break;
	case 'y':
	  tempStr = optarg;
	  pdMuon.y[0] = std::stod(tempStr.substr(0,tempStr.find(",")));
	  pdMuon.y[1] = std::stod(tempStr.substr(tempStr.find(",")+1,tempStr.size()-1));
	  break;
	case 't':
	  pdMuon.t = std::stod(optarg);
	  break;
	case 'n':
	  nprimaries = std::stoi(optarg);
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

  // ----- Energy range -----
  if (!range_valid(pdMuon.E[0], pdMuon.E[1], "Energy")){
    double tempConst;
    tempConst = pdMuon.E[0];
    pdMuon.E[0] = pdMuon.E[1];
    pdMuon.E[1] = tempConst;
  }

  // ----- X Coords -----
  if (!range_valid(pdMuon.x[0], pdMuon.x[1], "x")){
    double tempConst;
    tempConst = pdMuon.x[0];
    pdMuon.x[0] = pdMuon.x[1];
    pdMuon.x[1] = tempConst;
  }
  
  // ----- Y Coords -----
  if (!range_valid(pdMuon.y[0], pdMuon.y[1], "y")){
    double tempConst;
    tempConst = pdMuon.y[0];
    pdMuon.y[0] = pdMuon.y[1];
    pdMuon.y[1] = tempConst;
  }
  
  // ================================================================

   
  
  // === DATABASE ===================================================

  // Number of showers to generate
  nprimaries = nshowers(&pdMuon, 1.8E4, nprimaries);
  
  DBReader *corsDB = new DBReader(infile.c_str());
  
  // Choose a random selection of showers from the database
  std::vector<int> primary_gen;
  primary_gen.reserve(nprimaries+1); // Reserve mem. to avoid reallocation


  std::cout << "\033[1;34m[INFO]\033[0m Creating database from file:\n\t"
	    << infile << std::endl;
  int rnd_shower;
  double shower_energy;
  for (int evnt = 1; evnt <= nprimaries; evnt++)
    {
      if ( (evnt % (nprimaries/100) == 0) || (evnt == nprimaries) )
	status(evnt, nprimaries);
      
      rnd_shower = gRandom->Integer(corsDB->GetNShowers());
      corsDB->GetShower(rnd_shower);
      shower_energy = corsDB->SE();
      
      if ((std::find(primary_gen.begin(),primary_gen.end(),rnd_shower)
	   == primary_gen.end()) && shower_energy < pdMuon.E[1] && shower_energy > pdMuon.E[0])
	primary_gen.push_back(rnd_shower);
      else evnt--;
    }
  
  // ================================================================ 



  // === EVENT LOOPS ================================================ 
  
  TFile corsOUT(outfile.c_str(), "RECREATE");

  std::cout << "\033[1;34m[INFO]\033[0m Saving ENUBET cosmics to output file:\n\t"
	    << outfile << std::endl;
  
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

  Particle parts;
  
  //  int pPDG, pParID;
  //  double pEK, pP[3], pVtx[2], pT;
  
  particleTree->Branch("parID", &parts.ParID, "parID/I");
  particleTree->Branch("pdg", &parts.PDG, "pdg/I");
  particleTree->Branch("eK", &parts.ek, "eK/D");
  particleTree->Branch("mom[3]", parts.mom, "mom[3]/D");
  particleTree->Branch("vtx[2]", parts.vtx, "vtx[2]/D");
  particleTree->Branch("t", &parts.t, "t/D");

  

  
  // Use this variable to speed up searching for particles
  int last_position = 0;

  // Sort the vector lowest to highest in order to speed up searching (we can skip many
  // of the early events this way...
  sort(primary_gen.begin(), primary_gen.end());
  int current_shower, newShowerID = 0; 
  
  
  for (int shower : primary_gen) {
    corsDB->GetShower(shower);

    sID = newShowerID;
    sE = corsDB->SE();
    sTheta = corsDB->STheta();
    sPhi = corsDB->SPhi();
    
    sVtx[0] = gRandom->Uniform(pdMuon.x[0], pdMuon.x[1]);
    sVtx[1] = gRandom->Uniform(pdMuon.y[0], pdMuon.y[1]);

    sT = gRandom->Uniform(0, pdMuon.t);
    
    showerTree->Fill();

    // I think the easiest way is to put a particle counter here, and then loop
    // over this afterwards. It's not efficient, but it's the only way to do it
    // with ROOT.

    std::vector<Particle> partList;
    
    for (int evnt = last_position; evnt < corsDB->GetNEvents(); evnt++){
      corsDB->GetEvent(evnt);
      current_shower = corsDB->ParID();
      parts.shift[0] = 0;
      parts.shift[1] = 0;
      
      if (current_shower == shower) {

	// Grab event information and store it per shower 
	parts.ParID = newShowerID;
	parts.PDG = corsDB->PDG();
	parts.ek = corsDB->EK();
	parts.mom[0] = corsDB->PX();
	parts.mom[1] = corsDB->PY();
	parts.mom[2] = corsDB->PZ();
	parts.vtx[0] = corsDB->X()/100 + sVtx[0];
	parts.vtx[1] = corsDB->Y()/100 + sVtx[1];

	while (parts.vtx[0] < pdMuon.x[0]) {
	  parts.vtx[0] += pdMuon.x[1] - pdMuon.x[0];
	  parts.shift[0]++;
	}
	while (parts.vtx[0] > pdMuon.x[1]) {
	  parts.vtx[0] -= pdMuon.x[1] - pdMuon.x[0];
	  parts.shift[0]--;
	}
	while (parts.vtx[1] < pdMuon.y[0]) {
	  parts.vtx[1] += pdMuon.y[1] - pdMuon.y[0];
	  parts.shift[1]++;
	}
	while (parts.vtx[1] > pdMuon.y[1]) {
	  parts.vtx[1] -= pdMuon.y[1] - pdMuon.y[0];
	  parts.shift[1]--;
	}

	parts.t = corsDB->T()/1E9 + sT;

	partList.push_back(parts);
       
	
      }
      else if (current_shower > shower) {

	for (Particle hit : partList){

	  parts.ParID = hit.ParID;
	  parts.PDG = hit.PDG;
	  parts.ek = hit.ek;
	  parts.mom[0] = hit.mom[0];
	  parts.mom[1] = hit.mom[1];
	  parts.mom[2] = hit.mom[2];
	  parts.vtx[0] = hit.vtx[0];
	  parts.vtx[1] = hit.vtx[1];
	  parts.t = hit.t;
	  particleTree->Fill();  
	}
	
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
