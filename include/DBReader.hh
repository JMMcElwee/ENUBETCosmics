/* ================== DBREADER ================== *
 * Created: 30-03-2024                            *
 * Author:  Jordan McElwee                        *
 * Email: mcelwee@lp2ib.in2p3.fr                  *
 *                                                *
 * Definition of the reader for the CORSIKA       *
 * database.                                      *
 *                                                *
 * Changelog:                                     * 
 * ============================================== */

#ifndef DBREADER_HH
#define DBREADER_HH


#include "TFile.h"
#include "TTree.h"



class DBReader
{

protected:

  // ----- MEMBER VARIABLES -----
  // File info.
  TFile m_dbfile {};
  TTree *m_showerTree {nullptr};
  TTree *m_particleTree {nullptr};

  // Shower variables
  int m_sId {};
  double m_sE {};
  double m_sTheta {};
  double m_sPhi {};

  // Particle variables
  int m_pId {};
  int m_pPDG {};
  double m_pEk {};
  double m_pPx {};
  double m_pPy {};
  double m_pPz {};
  double m_pX {};
  double m_pY {};
  double m_pT {};
  
  

public:

  // --- Constructors ---
  DBReader();
  DBReader(const char *dbfile);
  ~DBReader();

  // --- Access Functions ---
  int SID();
  double SE();
  double STheta();
  double SPhi();

  int ParID();
  int PDG();
  double EK();
  double PX();
  double PY();
  double PZ();
  double X();
  double Y();
  double T();

  // --- ROOT Functions ---
  void GetShower(int evnt);
  void GetEvent(int evnt);

  int GetNShowers();
  int GetNEvents();

};


#endif
