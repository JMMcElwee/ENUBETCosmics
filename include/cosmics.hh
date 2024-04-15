#ifndef COSMICS_HH
#define COSMICS_HH

#include <iostream>
#include <cmath>
#include <unistd.h>

struct Detector {

  char *name;
  double x[2];
  double y[2];
  double SA;
  double t;
  double E[2];

};

int nshowers(Detector *obs, double kp);

#endif
