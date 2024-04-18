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

#ifndef IO_HH
#define IO_HH

#include <iostream>
#include <iomanip>
#include <sys/stat.h>

// --- Progress --------------
void status(int evnt, int evntMax);
// ---------------------------

// --- Functions -------------
void help();
bool is_alive(const std::string &file);
bool range_valid(double low, double high, const char *name);
// ---------------------------


#endif
