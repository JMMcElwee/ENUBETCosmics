#include <sys/stat.h>

#include <crsRead/MCorsikaReader.h>

#include <crs/MRunHeader.h>
#include <crs/MEventHeader.h>
#include <crs/MEventEnd.h>

#include <crs/MParticleBlock.h>
#include <crs/MParticle.h>

#include <string>
#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#include "TNtupleD.h"

void status(int evnt, int evntMax);

int main (int argc, char **argv) {

  // === INPUT INFORMATION ===================================================

  std::string fname (argv[1]);  
  // ----- Check if file exists -----
  {
    struct stat buffer;
    if ( stat (fname.c_str(),&buffer) != 0) {
      std::cout << "\033[31;1m[ERROR]\033[0m File \'" << fname
		<< "\' not found." << std::endl;
      return 0;
    }
  }
  
  // ----- Corsika Read -----
  crsRead::MCorsikaReader corsRead(fname);

  // Check if correct file 
  if (fname.rfind(".long") != std::string::npos){
    std::cout << "\033[31;1m[ERROR]\033[0m Please use non \'.long\' file." << std::endl;
    return 0;
  }

  // Remove preceding file path
  std::string corsFile; 
  if (fname.rfind('/') == std::string::npos)
    corsFile = fname;
  else 
    corsFile = fname.substr(fname.rfind('/')+1, fname.size()-fname.rfind('/'));
  
  // =========================================================================


  // ----- Loop over runs -----
  crs::MRunHeader Run;
  while (corsRead.GetRun(Run)){

    const int nRun = Run.GetRunID();
    const int nShowers = Run.GetNumberOfShowers();
    std::cout << "\033[1m************ RUN INFO ************\033[0m\n"  
	      << "Run number: \t" << nRun << "\n"
	      << "No. showers: \t" << nShowers << "\n"
	      << "Primary slope: \t" << Run.GetSpectralSlope() << "\n"
	      << "Energy range: \t[" << Run.GetEMin() << "," << Run.GetEMax() << "]\n"
	      << "\033[1m**********************************\033[0m"<< std::endl;

    std::string outfile = corsFile + ".root";  
    TFile corsROOT(outfile.c_str(),"RECREATE");
    std::cout << "\033[34;1m[INFO]\033[0m Converting to ROOT file named:\n\t"
	      << outfile << std::endl;
    

    // === CREATE TREES ======================================================
    
    // ----- Particle data -----
    TTree *corsTree = new TTree("particles", "Particles crossing the detector");

    int pdg;
    double w, eK, px, py, pz, x, y, t;

    corsTree->Branch("pdg", &pdg, "pdg/I");
    corsTree->Branch("w", &w, "w/D");
    corsTree->Branch("eK", &eK, "eK/D");
    corsTree->Branch("px", &px, "px/D");
    corsTree->Branch("py", &py, "py/D");
    corsTree->Branch("pz", &pz, "pz/D");
    corsTree->Branch("x", &x, "x/D");
    corsTree->Branch("y", &y, "y/D");
    corsTree->Branch("t", &t, "t/D");


    // ----- Shower information -----
    TTree *showerTree = new TTree("shower", "Shower information");
    
    int sID, sDate;
    double sE, sTheta, sPhi, sFirst;

    showerTree->Branch("id", &sID, "id/I");
    showerTree->Branch("date", &sDate, "date/I");
    showerTree->Branch("E", &sE, "E/D");
    showerTree->Branch("theta", &sTheta, "theta/D");
    showerTree->Branch("phi", &sPhi, "phi/D");
    showerTree->Branch("first", &sFirst, "first/D");

    // =======================================================================



    // === READ SHOWER =======================================================

    std::cout << "\033[34;1m[INFO]\033[0m Analysing " << nShowers
	      << " showers..." << std::endl;

    // ----- Pull Shower Info ----- 
    crs::MEventHeader Shower;
    while (corsRead.GetShower(Shower)){

      // ----- Fill Shower Tree -----
      sID = Shower.GetEventNumber(); 
      sE = Shower.GetEnergy();
      sTheta = Shower.GetTheta();
      sPhi = Shower.GetPhi();
      sFirst = Shower.GetZFirst();
      sDate = Shower.GetDateStart();

      showerTree->Fill();


      // Conversion status
      if (sID % (nShowers/100) == 0) status(sID, nShowers);


      // ----- Pull Data Info -----
      crs::TSubBlock corsData; 
      while (corsRead.GetData(corsData)) {

	// Check if particle or not
	if (corsData.GetBlockType() == crs::TSubBlock::ePARTDATA) {

	  // Pull data
	  const crs::MParticleBlock &ParticleData = corsData;
	  crs::MParticleBlock::ParticleListConstIterator iEntry; 
	  // They ban you from accessing them without the iterator... WHAT?!?!

	  // Loop through entries
	  for (iEntry = ParticleData.FirstParticle(); 
	       iEntry != ParticleData.LastParticle(); ++iEntry)
	    {
	  
	      // Check if particle
	      if (iEntry->IsParticle()) {

		// Grab specific particle
		crs::MParticle iPart(*iEntry);

		// ----- Fill Particle Tree -----
		pdg = iPart.GetPDGCode();
		w = iPart.GetWeight();
		eK = iPart.GetKinEnergy();
		px = iPart.GetPx();
		py = iPart.GetPy();
		pz = iPart.GetPz();
		x = iPart.GetX();
		y = iPart.GetY();
		t = iPart.GetTime();      
	
		corsTree->Fill();

	      } // If particle

	    } // Particle Loop 
	  
	} // If particle

      } // Data loop
      
      //      crs::MEventEnd ShowerSummary;
      //      corsRead.GetShowerSummary(ShowerSummary);
  


    } // Shower Loop
         
    // =======================================================================

    std::cout << "\033[34;1m[INFO]\033[0m Writing trees to file." << std::endl;

    corsROOT.cd();
    showerTree->Write();
    corsTree->Write();

    std::cout << "\033[34;1m[INFO]\033[0m Complete! Have fun with your easier format." << std::endl;
    corsROOT.Close();

  } // Run Loop

 

  return 0;

}





//********************************************************************
//***** PROGRESS *****************************************************

void status(int evnt, int evntMax)
{
  int barWidth = 50;

  // Calculate the number of '=' for the bar
  int progBar = barWidth * evnt / evntMax;
  int percent = evnt * 100 / evntMax;

  // Sort out cursor position
  std::cout << "\r" << std::flush;

  // Display the progress bar
  std::cout << "\033[32;1m[";
  for (int i = 0; i < barWidth; ++i) {
    if (i < progBar) {
      std::cout << "=";
    } else {
      std::cout << "\033[0m\033[31m-";
    }
  }
  std::cout << "\033[32;1m] " << std::setw(3) << "\033[0m" << percent
            << "%" << std::flush;
  if (evnt == evntMax) std::cout << std::endl;

}

//********************************************************************
