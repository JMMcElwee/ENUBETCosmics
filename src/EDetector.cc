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

#include "EDetector.hh"


//**********************************************************
//***** CONSTRUCTORS ***************************************

// - - - - - - - - - - - - - - -
EDetector::EDetector()
{}
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
EDetector::EDetector(double size[3])
{
  for (int i = 0; i < 3; ++i)
    m_size[i] = size[i];
  
  DetectorInfo();
}
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
EDetector::EDetector(double size[3], double center[3])
{
  for (int i = 0; i < 3; ++i) {
    m_size[i] = size[i];
    m_center[i] = center[i];
  }
  DetectorInfo();
}
// - - - - - - - - - - - - - - -


//**********************************************************



//**********************************************************
//***** I/O ************************************************

// - - - - - - - - - - - - - - -
void EDetector::DetectorInfo()
{

  m_x[0] = m_center[0] - m_size[0]/2;
  m_x[1] = m_center[0] + m_size[0]/2;
  m_y[0] = m_center[1] - m_size[1]/2;
  m_y[1] = m_center[1] + m_size[1]/2;
  m_z[0] = m_center[2] - m_size[2]/2;
  m_z[1] = m_center[2] + m_size[2]/2;
  
  std::cout << "\n\033[1m*********** DETECTOR DIMEN ************\n\033[0m"
	    << "x:\t[" << m_x[0] << "," << m_x[1] << "] m\n"
	    << "y:\t[" << m_y[0] << "," << m_y[1] << "] m\n"
	    << "z:\t[" << m_z[0] << "," << m_z[1] << "] m\n"
	    << "\033[1m***************************************\n\033[0m" << std::endl;
}
// - - - - - - - - - - - - - - -


//**********************************************************


//**********************************************************
//***** ACCESS FUNCTIONS ***********************************

// - - - - - - - - - - - - - - -
double *EDetector::X()
{ return m_x; }
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
double *EDetector::Y()
{ return m_y; }
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
double *EDetector::Z()
{ return m_z; }
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
double EDetector::XSize()
{ return m_size[0]; }
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
double EDetector::YSize()
{ return m_size[1]; }
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
double EDetector::ZSize()
{ return m_size[2]; }
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
double *EDetector::XYZSize()
{ return m_size; }
// - - - - - - - - - - - - - - -

// - - - - - - - - - - - - - - -
double *EDetector::Center()
{ return m_center; }
// - - - - - - - - - - - - - - -

//**********************************************************


//**********************************************************
//***** PARTICLE CHECKS ************************************

// - - - - - - - - - - - - - - -
/*
  Return the plane information of a particular
  detector surface.
*/
EDetector::Plane EDetector::PlaneInfo(DetPlane input)
{
  Plane outplane;
  
  switch (input)
    {
    case DetPlane::Front:
      outplane.normal.SetXYZ(1, 0, 0);
      outplane.offset = m_x[1];
      break;
    case DetPlane::Back:
      outplane.normal.SetXYZ(1, 0, 0);
      outplane.offset = -m_x[0];
      break;
    case DetPlane::Right:
      outplane.normal.SetXYZ(0, -1, 0);
      outplane.offset = m_y[1];
      break;
    case DetPlane::Left:
      outplane.normal.SetXYZ(0, 1, 0);
      outplane.offset = -m_y[0];
      break;
    case DetPlane::Top:
      outplane.normal.SetXYZ(0, 0, 1);
      outplane.offset = m_z[1];
      break;
    case DetPlane::Bottom:
      outplane.normal.SetXYZ(0, 0, 1);
      outplane.offset = m_z[0];
      break;
    }
  
  return outplane;
}
// - - - - - - - - - - - - - - -


// - - - - - - - - - - - - - - -
/*
  To check if the particle passes through the detector, 
*/
bool EDetector::InVolume(double pOffset[3], double pMom[3])
{
  // Particle position
  TVector3 pR0 = {pOffset[0], pOffset[1], m_z[1] + pOffset[2]};
  TVector3 pP0 = {pMom[0], pMom[1], pMom[2]};

  DetPlane plList[5] = {DetPlane::Front,
			DetPlane::Back,
  			DetPlane::Left,
  			DetPlane::Right,
  			DetPlane::Bottom};

  for (int iPl = 0; iPl < 5; iPl++) {
    
    Plane plane = PlaneInfo(plList[iPl]);
    
    double tconst = - ( plane.offset + plane.normal.Dot(pR0) ) / plane.normal.Dot(pP0);
    
    // Determine crossing point
    TVector3 pR = pR0 + tconst * pP0;
    
    if ( pR.X() > 1.001*m_x[0] && pR.X() < 1.001*m_x[1] &&
	 pR.Y() > 1.001*m_y[0] && pR.Y() < 1.001*m_y[1] &&
	 pR.Z() > 1.001*m_z[0] && pR.Z() < 1.001*m_z[1] )
      return true;
    
  } // plane loop

  return false;
    
}
// - - - - - - - - - - - - - - -

//**********************************************************
