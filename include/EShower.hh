/* =================== SHOWER =================== *
 * Created: 27-06-2024                            *
 * Author:  Jordan McElwee                        *
 * Email: mcelwee@lp2ib.in2p3.fr                  *
 *                                                *
 * Base class to hold the Tree nformation of the *
 * showers and the particles.                     * 
 *                                                *
 * Changelog:                                     *
 * ============================================== */
#ifndef ESHOWER_HH
#define ESHOWER_HH


#include <iostream>

#include "TRandom.h"

#include "EHandler.hh"

class EShower : public EHandler
{

private:
    int m_id;
    double m_E, m_theta, m_phi, m_t, m_vtx[2];


public: 
    EShower(DBReader *corsDB, Detector *pdMuon);

    void CreateTree() override;
    void Process(int shower, int newShowerID) override;

    double T();
    double *Vtx();

};

#endif // Header guard