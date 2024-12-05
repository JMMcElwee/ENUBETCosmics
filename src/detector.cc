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

#include "detector.hh"


//**********************************************************
//***** CONSTRUCTORS ***************************************

Detector::Detector()
{}

Detector::Detector(double x[2], double y[2], double E[2])
{
  m_x[0] = x[0];
  m_x[1] = x[1];
  m_y[0] = y[0];
  m_y[1] = y[1];
  m_E[0] = E[0];
  m_E[1] = E[1];

  m_SA = (m_x[1] - m_x[0]) * (m_y[1] - m_y[0]);

  std::cout << "\n\033[1m*********** DETECTOR DIMEN ************\n\033[0m";
  DetectorInfo();
  std::cout << "\033[1m***************************************\n\033[0m" << std::endl;
}


bool Detector::IsRangeValid(double val[2])
{
  bool isValid = true;
  
  if (val[0] > val[1]) {
    std::cout << "\033[33;1m[WARNING]\033[0m Supplied range, ["
	      << val[0] << "," << val[1] << "], invalid. "
	      << "Swapping values..." << std::endl;

    double placeHold = val[0];
    val[0] = val[1];
    val[1] = placeHold;

    isValid = false;
  }

  return isValid;
}

void Detector::ValidateRange()
{
  std::cout << "\033[34;1m[INFO]\033[0m Validating ranges:\n"
	    << "       x-axis..." << std::endl;
  IsRangeValid(m_x);
  
  std::cout << "       y-axis..." << std::endl;
  IsRangeValid(m_y);

  std::cout << "       Energy..." << std::endl;
  IsRangeValid(m_E);
}

//**********************************************************



//**********************************************************
//***** I/O ************************************************

void Detector::DetectorInfo()
{
  std::cout << "x:\t[" << m_x[0] << "," << m_x[1] << "] m\n"
	    << "y:\t[" << m_y[0] << "," << m_y[1] << "] m\n"
	    << "Surface Area:\t" << m_SA << " m2" << std::endl;
}

//**********************************************************


//**********************************************************
//***** ACCESS FUNCTIONS ***********************************

double *Detector::E()
{ return m_E; }

double *Detector::X()
{ return m_x; }

double *Detector::Y()
{ return m_y; }

double Detector::SA()
{ return m_SA; }

//**********************************************************
