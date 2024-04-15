#ifndef IO_HH
#define IO_HH

#include <iostream>
#include <iomanip>
#include <sys/stat.h>

// --- Progress ---
void status(int evnt, int evntMax);

// --- Functions ---
void help();
bool is_alive(const std::string &file);
bool range_valid(double low, double high, const char *name);

#endif
