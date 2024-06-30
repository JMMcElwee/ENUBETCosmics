/* ================== HANDLER =================== *
 * Created: 26-06-2024                            *
 * Author:  Jordan McElwee                        *
 * Email: mcelwee@lp2ib.in2p3.fr                  *
 *                                                *
 * Base class to hold the Tree nformation of the *
 * showers and the particles.                     * 
 *                                                *
 * Changelog:                                     *
 * ============================================== */

#include "EHandler.hh"

//**********************************************************
//***** CONSTRUCTORS ***************************************

EHandler::EHandler(DBReader *corsDB, Detector *pdMuon)
    : corsDB(corsDB), pdMuon(pdMuon)
{}

EHandler::~EHandler()
{}


//**********************************************************


//**********************************************************
//***** ACCESS FUNCTIONS ***********************************

TTree *EHandler::GetTree()
{ return m_tree; }

double EHandler::GetSpillT()
{ return m_tspill; }

//**********************************************************


//**********************************************************
//***** STATIC VARIABLES ***********************************

void EHandler::SetSpillT(double t)
{ m_tspill = t; }

//**********************************************************