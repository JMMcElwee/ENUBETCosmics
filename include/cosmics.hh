/* ================== COSMICS =================== *
 * Created: 07-04-2024                            *
 * Author:  Jordan McElwee                        *
 * Email: mcelwee@lp2ib.in2p3.fr                  *
 *                                                *
 * Functions to deal with the generation and      *
 * handling of the primary cosmics.               *
 *                                                *
 * Changelog:                                     *
 * ============================================== */

#ifndef COSMICS_HH
#define COSMICS_HH

#include <iostream>
#include <cmath>
#include <unistd.h>


// --- Detector information ---
/*
   Information on the detector.
   Should only need one, but a
   struct can easily be parsed
   to other functions.
*/

struct Detector {

  char *name;   // for primary
  double x[2];  // xmin, xmax
  double y[2];  // ymin, ymax
  double SA;    // surface area
  double t;     // spill time
  double E[2];  // energy min, max

};
// ----------------------------

// --- Shower calculation -----
int nshowers(Detector *obs, double kp, int primOverride);
// ----------------------------


#endif
