#include "io.hh"


//********************************************************************
//***** FUNCTIONS ****************************************************

// ----- Help Function -----
void help()
{
  std::cout << "\n\033[1m************** CORSIKA2ROOT **************\033[0m\n\n";
  std::cout << "\033[1mSyntax:\033[0m \n\t./corsika2root <file> <opts>\n\n";
  std::cout << "\033[1mArgs:\033[0m \n";
  std::cout << "\t<file> Corsika file to analyse (no extension).\n";
  std::cout << "\t    -o ROOT output file name.\n";
  std::cout << "\t    -d Output directory.\n";
  std::cout << "\t    -h Return the help function.\n";
  std::cout << "\n\033[1m******************************************\033[0m\n"
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
bool range_valid(double lowVal, double highVal, const char *varName)
{
  bool isValid = true;
  
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
  std::cout << "\033[32;1m] " << std::setw(3) << "\033[0m" << percent
            << "%" << std::flush;
  if (evnt == evntMax) std::cout << std::endl;

}

//********************************************************************
