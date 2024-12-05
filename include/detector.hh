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

  bool IsRangeValid(double val[2]);
  
public:
  
  Detector();
  Detector(double x[2], double y[2], double E[2]);

  // ----- I/O -----
  void DetectorInfo();

  // ----- Access Functions -----
  double *E();
  double *X();
  double *Y();
  double SA();
  
  void ValidateRange();
  
};





#endif
