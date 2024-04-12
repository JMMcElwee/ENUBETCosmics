/* ================== DBREADER ================== *
 * Created: 02-04-2024                            *
 * Author:  Jordan McElwee                        *
 * Email: mcelwee@lp2ib.in2p3.fr                  *
 *                                                *
 * Definitions of functions for the database      *
 * reader.                                        *
 *                                                *
 * Changelog:                                     *
 * ============================================== */

#include "DBReader.hh"


// ********************************************************************
// ***** CONSTRUCTORS *************************************************

// --- Default Constructor ---
DBReader::DBReader()
{}
// ---------------------------

// --- ROOT Constructor ------
DBReader::DBReader(const char *dbfile) : m_dbfile(dbfile, "READ")
{
  m_dbfile.GetObject("shower", m_showerTree);
  m_dbfile.GetObject("particles", m_particleTree);

  m_showerTree->SetBranchAddress("id", &m_sId);
  m_showerTree->SetBranchAddress("E", &m_sE);
  m_showerTree->SetBranchAddress("theta", &m_sTheta);
  m_showerTree->SetBranchAddress("phi", &m_sPhi);

  m_particleTree->SetBranchAddress("parID",&m_pId);
  m_particleTree->SetBranchAddress("pdg",&m_pPDG);
  m_particleTree->SetBranchAddress("eK",&m_pEk);
  m_particleTree->SetBranchAddress("px",&m_pPx);
  m_particleTree->SetBranchAddress("py",&m_pPy);
  m_particleTree->SetBranchAddress("pz",&m_pPz);
  m_particleTree->SetBranchAddress("x",&m_pX);
  m_particleTree->SetBranchAddress("y",&m_pY);
  m_particleTree->SetBranchAddress("t",&m_pT);
  
}
// ---------------------------

// --- Destructor ------------
DBReader::~DBReader()
{
  m_dbfile.Close();
}
// ---------------------------

// ********************************************************************



// ********************************************************************
// ***** ROOT FUNCTIONS ***********************************************

// ---------------------------
void DBReader::GetShower(int evnt)
{ m_showerTree->GetEntry(evnt); }
// ---------------------------

// ---------------------------
void DBReader::GetEvent(int evnt)
{ m_particleTree->GetEntry(evnt); }
// ---------------------------

// ---------------------------
int DBReader::GetNShowers()
{ return m_showerTree->GetEntries(); }
// ---------------------------

// ---------------------------
int DBReader::GetNEvents()
{ return m_particleTree->GetEntries(); }
// ---------------------------

// ********************************************************************




// ********************************************************************
// ***** ACCESS FUNCTIONS *********************************************

// ---------------------------
int DBReader::SID()
{ return m_sId; }
// ---------------------------

// ---------------------------
double DBReader::SE()
{ return m_sE; }
// ---------------------------

// ---------------------------
double DBReader::STheta()
{ return m_sTheta; }
// ---------------------------

// ---------------------------
double DBReader::SPhi()
{ return m_sPhi; }
// ---------------------------


// ---------------------------
int DBReader::ParID()
{ return m_pId; }
// ---------------------------

// ---------------------------
int DBReader::PDG()
{ return m_pPDG; }
// ---------------------------

// ---------------------------
double DBReader::EK()
{ return m_pEk; }
// ---------------------------

// ---------------------------
double DBReader::PX()
{ return m_pPx; }
// ---------------------------

// ---------------------------
double DBReader::PY()
{ return m_pPy; }
// ---------------------------

// ---------------------------
double DBReader::PZ()
{ return m_pPz; }
// ---------------------------

// ---------------------------
double DBReader::X()
{ return m_pX; }
// ---------------------------

// ---------------------------
double DBReader::Y()
{ return m_pY; }
// ---------------------------

// ---------------------------
double DBReader::T()
{ return m_pT; }
// ---------------------------

// ********************************************************************
