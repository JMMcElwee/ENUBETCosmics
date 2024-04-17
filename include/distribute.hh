#ifndef DISTRIBUTE_HH
#define DISTRIBUTE_HH


// Particle output struct
struct Particle {
  int ParID;
  int PDG;
  double ek;
  double mom[3];
  double vtx[2];
  double t;
  int shift[2];
};


#endif
