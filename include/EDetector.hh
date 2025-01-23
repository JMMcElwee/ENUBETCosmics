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

#ifndef EDETECTOR_HH
#define EDETECTOR_HH

#include <iostream>
#include <cmath>
#include <unistd.h>

#include "TVector3.h"

#include "DBReader.hh"


class EDetector
{

public:
  /*
    Definition of front and back is arbitrary.
    Here the y-z plane is the 'front' and back.
  */
  enum DetPlane {
    Front = 0,
    Back = 1,
    Left = 2, 
    Right = 3,
    Top = 4,
    Bottom = 5
  };
  
private:

  // ----- Variables -----

  struct Plane {
    TVector3 normal;
    double offset;
  };
  
  // Detector related
  double m_size[3] {12, 12, 12};
  double m_center[3] {0, 0, 0};
  
  double m_x[2] {};
  double m_y[2] {};
  double m_z[2] {};

  Plane PlaneInfo(DetPlane input);
  
public:
  
  EDetector();
  EDetector(double size[3]);
  EDetector(double size[3], double center[3]);
  
  // ----- I/O -----
  void DetectorInfo();

  // ----- Access Functions -----
  double *X();
  double *Y();
  double *Z();
  double XSize();
  double YSize();
  double ZSize();
  double *XYZSize();
  double *Center();
  
  //  void ValidateRange();


  // ----- Particle Checks -----
  bool InVolume(double pOffset[3], double pMom[3]);

  
};





#endif
