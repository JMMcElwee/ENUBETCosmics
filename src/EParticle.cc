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


void EParticle::Process(int shower, int newShowerID)
{
    std::cout << "something" << std::endl;
/*    std::map<std::vector<int>, double> showerTiming;
    std::map<std::vector<int>, int> showerIDMap;

    for (int evnt = m_lastPos; evnt < corsDB->GetNEvents(); evnt++) {
        corsDB->GetEvent(evnt);

        if (corsDB->ParID() == shower) {
            std::vector<int> shift = {0, 0};
            showerTiming[shift] = gRandom->Uniform(0, spillT);
            showerIDMap[shift] = newShowerID;

            m_pdg = corsDB->PDG();
            m_eK = corsDB->EK();
            m_mom[0] = corsDB->PX();
            m_mom[1] = corsDB->PY();
            m_mom[2] = corsDB->PZ();
            m_vtx[0] = corsDB->X() / 100 + xVals[0];
            m_vtx[1] = corsDB->Y() / 100 + yVals[0];

            while (parts.vtx[0] < xVals[0]) {
                parts.vtx[0] += xVals[1] - xVals[0];
                shift[0]++;
            }
            while (parts.vtx[0] > xVals[1]) {
                parts.vtx[0] -= xVals[1] - xVals[0];
                shift[0]--;
            }
            while (parts.vtx[1] < yVals[0]) {
                parts.vtx[1] += yVals[1] - yVals[0];
                shift[1]++;
            }
            while (parts.vtx[1] > yVals[1]) {
                parts.vtx[1] -= yVals[1] - yVals[0];
                shift[1]--;
            }

            if ((std::abs(shift[0]) > 0 || std::abs(shift[1]) > 0) && !showerTiming.count(shift)) {
                double newTime = gRandom->Uniform(0, spillT);
                showerTiming[shift] = newTime;
                newShowerID++;
                showerIDMap[shift] = newShowerID;
            }

            parts.t = corsDB->T() / 1E9 + showerTiming[shift];
            parts.ParID = showerIDMap[shift];

            tree->Fill();
        } else if (corsDB->ParID() > shower) {
            last_position = evnt;
            break;
        }
    }
*/
}

//**********************************************************