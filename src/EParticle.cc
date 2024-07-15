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

#include "EParticle.hh"

//**********************************************************
//***** CONSTRUCTORS ***************************************

EParticle::EParticle(DBReader *corsDB, Detector *pdMuon)
    : EHandler(corsDB, pdMuon)
{}

//**********************************************************


//**********************************************************
//***** ROOT METHODS ***************************************

void EParticle::CreateTree()
{

    m_saveAsROOT = true;

    m_tree = new TTree("particles", "Particles crossing the detector");
    m_tree->Branch("parID", &m_id, "parID/I");
    m_tree->Branch("pdg", &currentParticle.m_pdg, "pdg/I");
    m_tree->Branch("eK", &currentParticle.m_eK, "eK/D");
    m_tree->Branch("mom[3]", currentParticle.m_mom, "mom[3]/D");
    m_tree->Branch("vtx[2]", currentParticle.m_vtx, "vtx[2]/D");
    m_tree->Branch("t", &currentParticle.m_t, "t/D");

}

//**********************************************************


//**********************************************************
//***** DATA HANDLING **************************************

void EParticle::Process(int shower, EShower *showerHandler)
{

    std::map<std::vector<int>, double> showerTiming;
    std::map<std::vector<int>, int> showerIDMap;

    for (int evnt = m_lastPos; evnt < corsDB->GetNEvents(); evnt++) {
        corsDB->GetEvent(evnt);

        if (corsDB->ParID() == shower) {
            std::vector<int> shift = {0, 0};
            showerTiming.insert( {shift, showerHandler->T()} );
            showerIDMap.insert ( {shift, showerHandler->ID() } );

            currentParticle.m_pdg = corsDB->PDG();
            currentParticle.m_eK = corsDB->EK();
            currentParticle.m_mom[0] = corsDB->PX();
            currentParticle.m_mom[1] = corsDB->PY();
            currentParticle.m_mom[2] = corsDB->PZ();

            // This is also wrong.. should be showerHandler.X/Y
            currentParticle.m_vtx[0] = corsDB->X() / 100 + showerHandler->Vtx()[0];
            currentParticle.m_vtx[1] = corsDB->Y() / 100 + showerHandler->Vtx()[1];

            while (currentParticle.m_vtx[0] < pdMuon->X()[0]) {
                currentParticle.m_vtx[0] += pdMuon->X()[1] - pdMuon->X()[0];
                shift[0]++;
            }
            while (currentParticle.m_vtx[0] > pdMuon->X()[1]) {
                currentParticle.m_vtx[0] -= pdMuon->X()[1] - pdMuon->X()[0];
                shift[0]--;
            }
            while (currentParticle.m_vtx[1] < pdMuon->Y()[0]) {
                currentParticle.m_vtx[1] += pdMuon->Y()[1] - pdMuon->Y()[0];
                shift[1]++;
            }
            while (currentParticle.m_vtx[1] > pdMuon->Y()[1]) {
                currentParticle.m_vtx[1] -= pdMuon->Y()[1] - pdMuon->Y()[0];
                shift[1]--;
            }

            if ((std::abs(shift[0]) > 0 || std::abs(shift[1]) > 0) && !showerTiming.count(shift)) {
                showerTiming[shift] = gRandom->Uniform(0, m_tspill);
                showerHandler->IncrementShower();
                showerIDMap[shift] = showerHandler->ID();
            }

            currentParticle.m_t = corsDB->T() / 1E9 + showerTiming[shift];
            m_id = showerIDMap[shift];

            particles.push_back(currentParticle);

            if (m_saveAsROOT) m_tree->Fill();

        } else if (corsDB->ParID() > shower) {
            m_lastPos = evnt;
            break;
        }
    }

}


const std::vector<EParticle::Particle> &EParticle::GetParticles()
{ return particles; }

//**********************************************************