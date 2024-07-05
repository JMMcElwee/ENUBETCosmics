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

    // --- Constructors ---
    EHandler(DBReader *corsDB, Detector *pdMuon);
    virtual ~EHandler();

    // --- Base only methods --- 
    TTree *GetTree();
    static void SetSpillT(double t);

    // --- For inheritance --- 
    virtual void CreateTree() = 0;
    

protected:

    // --- Members --- 
    DBReader *corsDB {nullptr};
    Detector *pdMuon {nullptr};
    TTree *m_tree {nullptr};
    
    // --- Static members --- 
    static double m_tspill;


};


#endif // Header guard