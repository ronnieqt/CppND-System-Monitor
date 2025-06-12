#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

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

  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // DONE: See src/system.cpp
  long UpTime();                      // DONE: See src/system.cpp
  int TotalProcesses();               // DONE: See src/system.cpp
  int RunningProcesses();             // DONE: See src/system.cpp
  std::string Kernel();               // DONE: See src/system.cpp
  std::string OperatingSystem();      // DONE: See src/system.cpp
};

#endif