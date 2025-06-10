#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "linux_parser.h"

class System
{
private:
  std::string m_os{""}, m_kernel{""};
  Processor m_cpu{};
  std::vector<Process> m_processes{};

public:
  System()
    : m_os{ LinuxParser::OperatingSystem() }
    , m_kernel{ LinuxParser::Kernel() }
  {}
  ~System() = default;

  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();
};

#endif