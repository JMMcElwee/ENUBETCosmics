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

#include <sys/stat.h>
#include <iostream>

#include "TFile.h"
#include "TTree.h"


/*
  Class to read the data from the ROOT
  database. If we include a reader for
  the CORSIKA files too, this will need
  to be reworked.. but it will likely
  be faster than the ROOT reader. 
*/

class DBReader
{

protected:

  // ----- MEMBER VARIABLES -----
  // File info.
  TFile m_dbfile {};
  TTree *m_showerTree {nullptr};
  TTree *m_particleTree {nullptr};
  std::string m_filename {};

  
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
  std::string File();

  // --- ROOT Functions ---
  void GetShower(int evnt);
  void GetEvent(int evnt);

  int GetNShowers();
  int GetNEvents();

  // --- General ---
  inline bool FileExist(const std::string &name);
  
};


#endif
