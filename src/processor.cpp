#include "processor.h"

// DONE: Return the aggregate CPU utilization
// Ref: https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization()
{
  long total_curr = LinuxParser::Jiffies();
  long idle_curr = LinuxParser::IdleJiffies();
  float total_delta = static_cast<float>(total_curr - m_total_prev);
  float idle_delta = static_cast<float>(idle_curr - m_idle_prev);
  m_total_prev = total_curr;
  m_idle_prev = idle_curr;
  float cpu_usage = (total_delta - idle_delta) / total_delta;
  return (cpu_usage > 0.0) ? cpu_usage : 0.0;
}
