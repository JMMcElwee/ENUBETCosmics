#include "io.hh"


//********************************************************************
//***** FUNCTIONS ****************************************************

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
