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
#ifndef EPARTICLE_HH
#define EPARTICLE_HH


#include <iostream>
#include <map>
#include <vector>

#include "EShower.hh"

class EParticle : public EShower
{

public: 
  
  struct Particle {
    int m_pdg;
    double m_eK, m_mom[3], m_vtx[2], m_t;
  };
  
  // --- Constructors ---
  EParticle(DBReader *corsDB, EDetector *pdMuon);
  EParticle(std::string directory, EDetector *pdMuon, std::vector<std::string> primaries);
  
  // --- ROOT Methods ---
  void CreateTree() override;
  
  // --- Data Handling ---
  void Process(int shower, EShower *showerHandler, int pType);
  const std::vector<EParticle::Particle> &GetParticles(); // This should change to an iterator
  void ResetParticle();
  
private:
  
  // --- Members ---
  Particle currentParticle;
  std::vector<Particle> particles;
  
  //int m_id;
  int m_lastPos = 0;
  


};

#endif // Header guard
