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

//**********************************************************


//**********************************************************
//***** SHOWER *********************************************


/*
   Takes in the detector information and uses the shower calculation
   to calculate the number of showers that would be needed for such a
   detector area.
   kp is the shower constant, which can change based on the primary
   species.
 */
void Detector::CalcNShowers(double kp, double t)
{

  // Output information for user to determine if detector is correct
  std::cout << "\n\033[1m*********** COSMIC GEN ************\n\033[0m"
            << "Spill time: \t" << t << " s\n"
            << "Constant: \t" << kp << "\n"
            << "Energy Range: \t[" << m_E[0] << "," << m_E[1] << "]\n"
            << "\033[1m***********************************\n\033[0m" << std::endl;

  // Should be able to change the spectral index? --JM
  double nshowers = round(2 * M_PI * m_SA * t * kp *
                          (pow(m_E[0],-1.7) - pow(m_E[1],-1.7))/1.7);


  std::cout << "\033[34;1m[INFO]\033[0m Generating " << nshowers
            << " primary cosmics." << std::endl;

  m_nshowers = nshowers;
 
}


/*
  Retrieve a list of the particles to be thrown from the database
  file. The number of primaries is only calculated within this
  function, and can only be accessed by the main function by
  recalculating it.
*/
void Detector::RetrievePrimaries(DBReader *corsDB, double kp, double spillT)
{

  int nprimaries = NShowers(kp, spillT); // This needs to change... lol --JM

  std::vector<int> primary_gen;
  primary_gen.reserve(nprimaries+1);

  
  int rnd_shower;
  double shower_energy;
  for (int evnt = 1; evnt <= nprimaries; evnt++)
    {
      //      if ( (evnt % (nprimaries/100) == 0) || (evnt == nprimaries) )
      //        status(evnt, nprimaries);

      rnd_shower = gRandom->Integer(corsDB->GetNShowers());
      corsDB->GetShower(rnd_shower);
      shower_energy = corsDB->SE();

      if ((std::find(primary_gen.begin(),primary_gen.end(),rnd_shower)
           == primary_gen.end()) && shower_energy < m_E[1] && shower_energy > m_E[0])
        primary_gen.push_back(rnd_shower);
      else evnt--;
    }

  m_primaries = primary_gen;
  //  return primary_gen;
}

//**********************************************************



//**********************************************************
//***** I/O ************************************************

void Detector::DetectorInfo()
{
  std::cout << "\t x = [" << m_x[0] << "," << m_x[1] << "] m\n"
	    << "\t y = [" << m_y[0] << "," << m_y[1] << "] m\n"
	    << "\t SA = " << m_SA << " m2\n" << std::endl;
}

//**********************************************************


//**********************************************************
//***** ACCESS FUNCTIONS ***********************************

int Detector::NShowers(double kp, double t, bool primOverride)
{

  if (m_nshowers < 0) CalcNShowers(kp, t);
  else if (primOverride) {
    std::cout << "\033[34;1m[INFO]\033[0m Recalculating primary shower number..." << std::endl;
    CalcNShowers(kp, t);
  }
  else {
    std::cout << "\033[34;1m[INFO]\033[0m Cosmic calculation already complete. Taking this value."
	      << std::endl;
  }
	
  return m_nshowers;
  
}

std::vector<int> Detector::GetPrimaries(DBReader *corsDB, double kp, double t)
{

  if (m_primaries.empty()) RetrievePrimaries(corsDB, kp, t);
  else {
    std::cout << "\033[34;1m[INFO]\033[0m Primaries already filled, using these values."
	      << std::endl;
  }
  
  return m_primaries;
  
}


double *Detector::E()
{ return m_E; }

//**********************************************************
