/* ================== HANDLER =================== *
 * Created: 26-06-2024                            *
 * Author:  Jordan McElwee                        *
 * Email: mcelwee@lp2ib.in2p3.fr                  *
 *                                                *
 * Base class to hold the Tree information of the *
 * showers and the particles.                     * 
 *                                                *
 * Changelog:                                     *
 * ============================================== */
#ifndef EHANDLER_HH
#define EHANDLER_HH


#include "DBReader.hh"
#include "detector.hh"

#include "TTree.h"

class EHandler
{
public:
    EHandler(DBReader *corsDB, Detector *pdMuon);
    virtual ~EHandler();

    TTree *GetTree();
    static void SetSpillT(double t);
    double GetSpillT();
    
    virtual void CreateTree() = 0;
    virtual void Process(int shower, int newShowerID) = 0;
    //virtual void Process(EHandler shower);
    

protected:
    DBReader *corsDB {nullptr};
    TTree *m_tree {nullptr};
    Detector *pdMuon {nullptr};
    
    static double m_tspill;

};



#endif // Header guard