#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

// DONE: Return the system's CPU
Processor& System::Cpu() { return m_cpu; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes()
{
  m_processes.clear();
  for (auto pid : LinuxParser::Pids()) {
    m_processes.emplace_back(pid);
  }
  std::sort(m_processes.begin(), m_processes.end(),
            [](const Process& p1, const Process p2) { return p2 < p1; });
  return m_processes;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return m_kernel; }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return m_os; }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
