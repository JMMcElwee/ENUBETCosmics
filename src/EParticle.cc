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
//**********************************************************

void EParticle::CreateTree()
{

    m_tree = new TTree("particles", "Particles crossing the detector");
    m_tree->Branch("parID", &m_id, "parID/I");
    m_tree->Branch("pdg", &m_pdg, "pdg/I");
    m_tree->Branch("eK", &m_eK, "eK/D");
    m_tree->Branch("mom[3]", m_mom, "mom[3]/D");
    m_tree->Branch("vtx[2]", m_vtx, "vtx[2]/D");
    m_tree->Branch("t", &m_t, "t/D");

}


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

            m_pdg = corsDB->PDG();
            m_eK = corsDB->EK();
            m_mom[0] = corsDB->PX();
            m_mom[1] = corsDB->PY();
            m_mom[2] = corsDB->PZ();

            // This is also wrong.. should be showerHandler.X/Y
            m_vtx[0] = corsDB->X() / 100 + showerHandler->Vtx()[0];
            m_vtx[1] = corsDB->Y() / 100 + showerHandler->Vtx()[1];

            while (m_vtx[0] < pdMuon->X()[0]) {
                m_vtx[0] += pdMuon->X()[1] - pdMuon->X()[0];
                shift[0]++;
            }
            while (m_vtx[0] > pdMuon->X()[1]) {
                m_vtx[0] -= pdMuon->X()[1] - pdMuon->X()[0];
                shift[0]--;
            }
            while (m_vtx[1] < pdMuon->Y()[0]) {
                m_vtx[1] += pdMuon->Y()[1] - pdMuon->Y()[0];
                shift[1]++;
            }
            while (m_vtx[1] > pdMuon->Y()[1]) {
                m_vtx[1] -= pdMuon->Y()[1] - pdMuon->Y()[0];
                shift[1]--;
            }

            if ((std::abs(shift[0]) > 0 || std::abs(shift[1]) > 0) && !showerTiming.count(shift)) {
                showerTiming[shift] = gRandom->Uniform(0, m_tspill);
                showerHandler->IncrementShower();
                showerIDMap[shift] = showerHandler->ID();
            }

            m_t = corsDB->T() / 1E9 + showerTiming[shift];
            m_id = showerIDMap[shift];

            m_tree->Fill();
        } else if (corsDB->ParID() > shower) {
            m_lastPos = evnt;
            break;
        }
    }

}

//**********************************************************