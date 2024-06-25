/* ================= DETECTOR =================== *
 * Created: 19-06-2024                            *
 * Author:  Jordan McElwee                        *
 * Email: mcelwee@lp2ib.in2p3.fr                  *
 *                                                *
 * Class to deal with the detector information    *
 * and the number of showers etc.                 *
 *                                                *
 * Changelog:                                     *
 * ============================================== */

#ifndef DETECTOR_HH
#define DETECTOR_HH

#include <iostream>
#include <cmath>
#include <unistd.h>
#include <vector>

#include "TRandom.h"

#include "DBReader.hh"


class Detector
{

private:

  // ----- Variables -----
  
  // Detector related
  double m_x[2] {};
  double m_y[2] {};
  double m_SA {};
  double m_E[2] {};

  // Shower related
  int m_nshowers {-1};
  std::vector<int> m_primaries {};

  void CalcNShowers(double kp, double t);
  void RetrievePrimaries(DBReader *corsDB, double kp = 0., double spillT = 0.);
  
public:
  Detector();
  Detector(double x[2], double y[2], double E[2]);

  // ----- I/O -----
  void DetectorInfo();

  // ----- Access Functions -----
  double *E();

  int NShowers(double kp = 1.8E4, double t = 2., bool primOverride = false);
  std::vector<int> GetPrimaries(DBReader *corsDB, double kp = 0., double spillT = 0.);
  
};





#endif
