#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>

#include "linux_parser.h"

class Processor {
 private:
  long m_total_prev, m_idle_prev;

 public:
  Processor()
    : m_total_prev{LinuxParser::Jiffies()}
    , m_idle_prev{LinuxParser::IdleJiffies()}
  {}
  ~Processor() = default;

  float Utilization();
};

#endif