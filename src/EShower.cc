/* =================== SHOWER =================== *
 * Created: 27-06-2024                            *
 * Author:  Jordan McElwee                        *
 * Email: mcelwee@lp2ib.in2p3.fr                  *
 *                                                *
 * Base class to hold the Tree information of the *
 * showers and the particles.                     * 
 *                                                *
 * Changelog:                                     *
 * ============================================== */

#include "EShower.hh"


//**********************************************************
//***** CONSTRUCTORS ***************************************

// - - - - - - - - - - - - - - -
EShower::EShower(DBReader *corsDB, Detector *pdMuon)
  : corsDB(corsDB), pdMuon(pdMuon)
{}
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
EShower::EShower(DBReader *corsDB, Detector *pdMuon, PType primary)
  : EShower(corsDB, pdMuon)
{
  m_primary = primary;
}
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
EShower::~EShower()
{}
// - - - - - - - - - - - - - - -

//**********************************************************



//**********************************************************
//***** STATIC VARIABLES ***********************************

// - - - - - - - - - - - - - - -
// Has to be initiated before the main function
double EShower::m_tspill = 0.1;
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
/*
  Globally set the spill time.
*/
void EShower::SetSpillT(double t)
{ m_tspill = t; }
// - - - - - - - - - - - - - - -

//**********************************************************


//**********************************************************
//***** ROOT METHODS ***************************************

// - - - - - - - - - - - - - - -
void EShower::CreateTree()
{

    m_saveAsROOT = true;

    m_tree = new TTree("shower", "Shower information");
    m_tree->Branch("id", &m_id, "id/I");
    m_tree->Branch("E", &m_E, "E/D");
    m_tree->Branch("theta", &m_theta, "theta/D");
    m_tree->Branch("phi", &m_phi, "phi/D");
    m_tree->Branch("t", &m_t, "t/D");
    m_tree->Branch("vtx[2]", m_vtx, "vtx[2]/D");

}
// - - - - - - - - - - - - - - -

//**********************************************************


//**********************************************************
//***** DATA HANDLING **************************************

// - - - - - - - - - - - - - - -
void EShower::Process(int shower)
{
    corsDB->GetShower(shower);

    IncrementShower();
    m_E = corsDB->SE();
    m_theta = corsDB->STheta();
    m_phi = corsDB->SPhi();

    m_vtx[0] = gRandom->Uniform(pdMuon->X()[0], pdMuon->X()[1]);
    m_vtx[1] = gRandom->Uniform(pdMuon->Y()[0], pdMuon->Y()[1]);

    m_t = gRandom->Uniform(0, m_tspill);

    if (m_saveAsROOT) m_tree->Fill();

}
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
void EShower::IncrementShower()
{
    m_id++;
}
// - - - - - - - - - - - - - - -

//**********************************************************



//**********************************************************
//***** ACCESS FUNCTIONS ***********************************

// - - - - - - - - - - - - - - -
TTree *EShower::GetTree()
{ return m_tree; }
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
double EShower::T()
{ return m_t; }
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
double *EShower::Vtx()
{ return m_vtx; }
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
int EShower::ID()
{ return m_id; }
// - - - - - - - - - - - - - - -


//**********************************************************



//**********************************************************
//***** CMC Model ******************************************

// - - - - - - - - - - - - - - -
/*
   Takes in the detector information and uses the shower calculation
   to calculate the number of showers that would be needed for such a
   detector area.
   kp is the shower constant, which can change based on the primary
   species.
 */
int EShower::NShowers()
{

  if (m_nshowers < 0) {

    // Output information for user to determine if detector is correct
    std::cout << "\n\033[1m*********** COSMIC GEN ************\n\033[0m"
	      << "Spill time: \t" << m_tspill << " s\n"
	      << "Constant: \t" << m_primary << "\n"
	      << "Energy Range: \t[" << pdMuon->E()[0] << "," << pdMuon->E()[1] << "]\n"
	      << "\033[1m***********************************\n\033[0m" << std::endl;
    
    double nshowers = round(2 * M_PI * pdMuon->SA() * m_tspill * m_primary *
			    (pow(pdMuon->E()[0],-1.7) - pow(pdMuon->E()[1],-1.7))/1.7);
    
    
    std::cout << "\033[34;1m[INFO]\033[0m Generating " << nshowers
	      << " primary cosmics." << std::endl;
    
    m_nshowers = nshowers;
  }

  std::cout << "This is the value " << m_nshowers << std::endl;
  return m_nshowers;
}
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -

/*
  Retrieve a list of the particles to be thrown from the database
  file. The number of primaries is only calculated within this
  function, and can only be accessed by the main function by
  recalculating it.
*/
std::vector<int> EShower::GetShowers()
{

  if ( m_primaries.empty() ){
    
    m_primaries.reserve(m_nshowers+1);

    std::cout << "\033[34;1m[INFO]\033[0m Loading primaries into vector" << std::endl;
    
    int rnd_shower;
    double shower_energy;
    for (int evnt = 1; evnt <= m_nshowers; evnt++){

      rnd_shower = gRandom->Integer(corsDB->GetNShowers());
      corsDB->GetShower(rnd_shower);
      shower_energy = corsDB->SE();

      if ((std::find(m_primaries.begin(),m_primaries.end(),rnd_shower)
           == m_primaries.end()) && shower_energy < pdMuon->E()[1] && shower_energy > pdMuon->E()[0])
        m_primaries.push_back(rnd_shower);
      else evnt--;
      
    }

    sort(m_primaries.begin(), m_primaries.end());

  }

  return m_primaries;
  
}
// - - - - - - - - - - - - - - -


//**********************************************************

