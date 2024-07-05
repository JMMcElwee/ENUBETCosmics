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

#include "EHandler.hh"

class EShower : public EHandler
{

private:

    // Shower variables to output to the TTree
    int m_id = 0;
    double m_E, m_theta, m_phi, m_t, m_vtx[2];

public: 

    // --- Constructors ---
    EShower(DBReader *corsDB, Detector *pdMuon);

    // --- ROOT Methods --- 
    void CreateTree() override;
 
    // --- Data Handling ---
    void Process(int shower);
    void IncrementShower();

    // --- Access Functions ---
    int ID();
    double T();
    double *Vtx();

};

#endif // Header guard