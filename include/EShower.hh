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
#ifndef ESHOWER_HH
#define ESHOWER_HH

#include <iostream>
#include <map>

#include "TRandom.h"
#include "TTree.h"

#include "DBReader.hh"
#include "EDetector.hh"
#include "EShower.hh"

using namespace std;

class EShower
{

private:
  
  // Shower variables to output to the TTree
  double m_E, m_theta, m_phi, m_t, m_vtx[2];
  double m_ERange[2] {};
  double m_buffer[2] {0, 0};
  double m_offset = 0.;
  
public: 

  // --- CMC Model Params ---
  const map<string, int> PConst = {
    {"H",  17200},
    {"He", 9200},
    {"C",  6200},
    {"N",  6200},
    {"O",  6200},
    {"Mg", 9200},
    {"Fe", 6200}
  };
  vector<int> m_nshowers {};
  vector<vector<int>> m_primaries {};  
  void NShowers();
  void GetShowers(); 
  vector<int> GetShowers(string pType);
  vector<int> GetShowers(int vec);
  
  // --- ROOT Methods ---
  TTree *GetTree();
  virtual void CreateTree();
  
  // --- Data Handling ---
  void Process(int shower, int vec);
  void IncrementShower();
  virtual void SetBuffer(double buffer[2]) final;
  virtual void SetOffset(double offset) final;
  double *Buffer();
  double Offset();
  
  static void SetSpillT(double t);
  
  // --- Access Functions ---
  int ID();
  virtual double T() final;
  virtual double *Vtx() final;
  
  // --- Constructors ---
  EShower(DBReader *corsDB, EDetector *pdMuon);
  EShower(DBReader *corsDB, EDetector *pdMuon, double ERange[2]);
  EShower(DBReader *corsDB, EDetector *pdMuon, double ERange[2], std::string primary);
  EShower(string directory, EDetector *pdMuon, vector<string> primaries);
  EShower(string directory, EDetector *pdMuon, double ERange[2], vector<string> primaries);
  virtual ~EShower();

  
protected:

  // --- Members ---
  //  DBReader *corsDB {nullptr};
  EDetector *pdMuon {nullptr};
  TTree *m_tree {nullptr};

  bool m_saveAsROOT = false;

  int m_id = 0;

  // --- Static members ---
  static double m_tspill;

  // --- CMC Model ---
  vector<string> m_pTypeVec = { "H" };
  vector<DBReader*> m_dbVec = { nullptr };
};

#endif // Header guard
