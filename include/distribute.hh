/* ================= DISTRIBUTE ================= * 
 * Created: 16-04-2024                            * 
 * Author:  Jordan McElwee                        * 
 * Email: mcelwee@lp2ib.in2p3.fr                  * 
 *                                                * 
 * Contains everything necessary for uniformly    * 
 * distributing the particles in both time and    * 
 * space. Includes declarations of the maps and   *
 * structs relevant.                              *
 *                                                * 
 * Changelog:                                     * 
 * ============================================== */

#ifndef DISTRIBUTE_HH
#define DISTRIBUTE_HH

#include <map>

// --- Output particle struct ---
struct Particle {
  int ParID;
  int PDG;
  double ek;
  double mom[3];
  double vtx[2];
  double t;
};
// ------------------------------


#endif
