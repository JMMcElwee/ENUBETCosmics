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

#include "TRandom.h"
#include "TTree.h"

#include "DBReader.hh"
#include "detector.hh"
#include "EShower.hh"

class EShower
{

private:
  
  // Shower variables to output to the TTree
  double m_E, m_theta, m_phi, m_t, m_vtx[2];

  
public: 

  // --- CMC Model Params ---
  enum PType {
    H =	17200,
    He = 9200,
    C =	6200,
    N =	6200,
    O = 6200,
    Mg = 9200,
    Fe = 6200,
    Complete = 0
  };
  int m_nshowers {-1};
  std::vector<int> m_primaries {};  
  int NShowers();
  std::vector<int> GetShowers(); 

  
  // --- ROOT Methods ---
  TTree *GetTree();
  virtual void CreateTree();
  
  // --- Data Handling ---
  void Process(int shower);
  void IncrementShower();

  static void SetSpillT(double t);
  
  // --- Access Functions ---
  int ID();
  virtual double T() final;
  virtual double *Vtx() final;
  
  // --- Constructors ---
  EShower(DBReader *corsDB, Detector *pdMuon);
  EShower(DBReader *corsDB, Detector *pdMuon, PType primary);
  virtual ~EShower();

  
protected:

  // --- Members ---
  DBReader *corsDB {nullptr};
  Detector *pdMuon {nullptr};
  TTree *m_tree {nullptr};

  bool m_saveAsROOT = false;

  int m_id = 0;

  // --- Static members ---
  static double m_tspill;

  // --- CMC Model ---
  PType m_primary = PType::H;
  
};

#endif // Header guard
