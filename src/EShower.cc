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

EShower::EShower(DBReader *corsDB, Detector *pdMuon)
    : EHandler(corsDB, pdMuon)
{}

//**********************************************************


//**********************************************************
//***** ROOT METHODS ***************************************

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

//**********************************************************


//**********************************************************
//***** DATA HANDLING **************************************

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

void EShower::IncrementShower()
{
    m_id++;
}

//**********************************************************



//**********************************************************
//***** ACCESS FUNCTIONS ***********************************

double EShower::T()
{ return m_t; }

double *EShower::Vtx()
{ return m_vtx; }

int EShower::ID()
{ return m_id; }


//**********************************************************
