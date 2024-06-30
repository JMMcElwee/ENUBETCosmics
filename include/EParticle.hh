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

#include "EHandler.hh"

class EParticle : public EHandler
{

private:

    int m_id, m_pdg;
    int m_lastPos = 0;
    double m_eK, m_mom[3], m_vtx[2], m_t;

public: 
    EParticle(DBReader *corsDB, Detector *pdMuon);

    void CreateTree() override;
    void Process(int shower, int newShowerID) override;
    //void Process(EHandler temp);

};

#endif // Header guard