/* ================== PARTICLE ================== *
 * Created: 27-06-2024                            *
 * Author:  Jordan McElwee                        *
 * Email: mcelwee@lp2ib.in2p3.fr                  *
 *                                                *
 * Base class to hold the Tree information of the *
 * showers and the particles.                     *
 *                                                *
 * Changelog:                                     *
 * ============================================== */

#include "EParticle.hh"

//**********************************************************
//***** CONSTRUCTORS ***************************************

// - - - - - - - - - - - - - - - 
EParticle::EParticle(DBReader *corsDB, EDetector *pdMuon)
    : EShower(corsDB, pdMuon)
{}
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - - 
EParticle::EParticle(std::string directory, EDetector *pdMuon, std::vector<string> primaries)
  : EShower(directory, pdMuon, primaries)
{}
// - - - - - - - - - - - - - - -

//**********************************************************


//**********************************************************
//***** ROOT METHODS ***************************************

// - - - - - - - - - - - - - - - 
void EParticle::CreateTree()
{

    m_saveAsROOT = true;

    m_tree = new TTree("particles", "Particles crossing the detector");
    m_tree->Branch("parID", &m_id, "parID/I");
    m_tree->Branch("pdg", &currentParticle.m_pdg, "pdg/I");
    m_tree->Branch("eK", &currentParticle.m_eK, "eK/D");
    m_tree->Branch("mom[3]", currentParticle.m_mom, "mom[3]/D");
    m_tree->Branch("vtx[2]", currentParticle.m_vtx, "vtx[2]/D");
    m_tree->Branch("t", &currentParticle.m_t, "t/D");

}
// - - - - - - - - - - - - - - - 

//**********************************************************


//**********************************************************
//***** DATA HANDLING **************************************

// - - - - - - - - - - - - - - - 
void EParticle::Process(int shower, EShower *showerHandler, int pType)
{

  
  std::map<std::vector<int>, double> showerTiming;
  std::map<std::vector<int>, int> showerIDMap;

  DBReader *corsDB = m_dbVec.at(pType);
  
  for (int evnt = m_lastPos; evnt < corsDB->GetNEvents(); evnt++) {
    corsDB->GetEvent(evnt);
    
    if (corsDB->ParID() == shower) {
      std::vector<int> shift = {0, 0};
      showerTiming.insert( {shift, showerHandler->T()} );
      showerIDMap.insert ( {shift, showerHandler->ID() } );
      
      currentParticle.m_pdg = corsDB->PDG();
      currentParticle.m_eK = corsDB->EK();
      currentParticle.m_mom[0] = corsDB->PX();
      currentParticle.m_mom[1] = corsDB->PY();
      currentParticle.m_mom[2] = corsDB->PZ();
      
      currentParticle.m_vtx[0] = corsDB->X() / 100 + showerHandler->Vtx()[0];
      currentParticle.m_vtx[1] = corsDB->Y() / 100 + showerHandler->Vtx()[1];
      
      double surf_limX[2] = {pdMuon->X()[0] - showerHandler->Buffer()[0],
			     pdMuon->X()[1] + showerHandler->Buffer()[0]};
      double surf_limY[2] = {pdMuon->Y()[0] - showerHandler->Buffer()[1],
			     pdMuon->Y()[1] + showerHandler->Buffer()[1]};
      
      while (currentParticle.m_vtx[0] < surf_limX[0]) {
	currentParticle.m_vtx[0] += surf_limX[1] - surf_limX[0];
	shift[0]++;
      }
      while (currentParticle.m_vtx[0] > surf_limX[1]) {
	currentParticle.m_vtx[0] -= surf_limX[1] - surf_limX[0];
	shift[0]--;
      }
      while (currentParticle.m_vtx[1] < surf_limY[0]) {
	currentParticle.m_vtx[1] += surf_limY[1] - surf_limY[0];
	shift[1]++;
      }
      while (currentParticle.m_vtx[1] > surf_limY[1]) {
	currentParticle.m_vtx[1] -= surf_limY[1] - surf_limY[0];
	shift[1]--;
      }

      if ((std::abs(shift[0]) > 0 || std::abs(shift[1]) > 0) && !showerTiming.count(shift)) {
	showerTiming[shift] = gRandom->Uniform(0, m_tspill*1E9);
	showerHandler->IncrementShower();
	showerIDMap[shift] = showerHandler->ID();
      }
      
      currentParticle.m_t = corsDB->T() + showerTiming[shift];
      m_id = showerIDMap[shift];
      
      double testing[3] = {currentParticle.m_vtx[0],
			   currentParticle.m_vtx[1],
			   showerHandler->Offset()};
      
      if ( pdMuon->InVolume(testing,
			    currentParticle.m_mom) )
	particles.push_back(currentParticle);
      
      if (m_saveAsROOT) m_tree->Fill();
      
    } else if (corsDB->ParID() > shower) {
      m_lastPos = evnt;
      break;
    }
  }
  
}
// - - - - - - - - - - - - - - - 

// - - - - - - - - - - - - - - -
void EParticle::ResetParticle()
{
  m_lastPos = 0;
}
// - - - - - - - - - - - - - - - 

// - - - - - - - - - - - - - - - 
const std::vector<EParticle::Particle> &EParticle::GetParticles()
{ return particles; }
// - - - - - - - - - - - - - - - 

//**********************************************************
