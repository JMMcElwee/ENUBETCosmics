/* ==================== IO ====================== *
 * Created: 30-03-2024                            *
 * Author:  Jordan McElwee                        *
 * Email: mcelwee@lp2ib.in2p3.fr                  *
 *                                                *
 * Input/output functions that are useful for     *
 * controlling and testing the data. Anything     *
 * specific should be kept elsewhere!             *
 *                                                *
 * Changelog:                                     *
 * ============================================== */

#include "io.hh"


//********************************************************************
//***** FUNCTIONS ****************************************************

// ----- Help Function -----
void help()
{
  std::cout << "\n\033[1m**************** ECORSIKA ****************\033[0m\n\n"
	    << "\033[1mSyntax:\033[0m \n\t./corsika2root <file> <opts>\n\n"
	    << "\033[1mArgs:\033[0m \n"
	    << "<file> Database file for analysis.\n"
	    << "\n--- Data steering\n"
	    << "    -E Energy range \t\t<min>,<max>\n"
    	    << "    -x Detector x coords \t<min>,<max>\n"
	    << "    -y Detector y coords \t<min>,<max>\n"
	    << "    -t Cosmic spill time \t<t>\n"
	    << "    -n Override n cosmics calc \t<n>\n"
	    << "\n--- I/O\n"
	    << "    -o ROOT output file name \t<name>\n"
	    << "    -d Output directory \t<pathtodir> \n"
	    << "    -h Help function \n"
	    << "\n\033[1m******************************************\033[0m\n"
            << std::endl;
}
// -------------------------

// ----- File Exists -------
bool is_alive(const std::string &file)
{
  struct stat buffer;
  return (stat (file.c_str(), &buffer) == 0);
}
// -------------------------

// ----- Range correct -----
bool range_valid(double lowVal, double highVal, const char *name)
{
  // Assume range is valid
  bool isValid = true;

  // If it's not, inform of the swap!
  if (lowVal > highVal) {

    std::cout << "\033[33;1m[WARNING]\033[0m " << varName
	      << " range supplied [" << lowVal << ","
	      << highVal << "] is invalid.\n";

    std::cout << "          Swapping range to: [" << highVal
              << "," << lowVal << "]" << std::endl;

    isValid = false;
  } // Actually have to swap outside the code... not elegant

  return isValid;
}
// -------------------------

//********************************************************************



//********************************************************************
//***** PROGRESS *****************************************************

void status(int evnt, int evntMax)
{
  int barWidth = 50;

  // Calculate the number of '=' for the bar
  int progBar = barWidth * evnt / evntMax;
  int percent = evnt * 100 / evntMax;

  // Sort out cursor position
  std::cout << "\r" << std::flush;

  // Display the progress bar
  std::cout << "\033[32;1m[";
  for (int i = 0; i < barWidth; ++i) {
    if (i < progBar) {
      std::cout << "=";
    } else {
      std::cout << "\033[0m\033[31m-";
    }
  }

  // Display percentage at the end of the bar
  std::cout << "\033[32;1m] " << std::setw(3) << "\033[0m" << percent
            << "%" << std::flush;
  
  // Include this to make sure it reaches 100%
  if (evnt == evntMax) std::cout << std::endl;

}

//********************************************************************
