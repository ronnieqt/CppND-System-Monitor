#include "format.h"

#include <sstream>
#include <string>
#include <iomanip>

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
std::string Format::ElapsedTime(long seconds) {
  std::stringstream sstream;
  long hh = seconds / (60 * 60);
  seconds = seconds % (60 * 60);
  long mm = seconds / 60;
  long ss = seconds % 60;
  sstream << std::setfill('0') << std::setw(2) << hh << ":"
          << std::setfill('0') << std::setw(2) << mm << ":"
          << std::setfill('0') << std::setw(2) << ss;
  return sstream.str();
}
