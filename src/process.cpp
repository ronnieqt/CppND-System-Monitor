#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() const { return m_pid; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const
{
  return LinuxParser::CpuUtilization(m_pid);
}

// DONE: Return the command that generated this process
string Process::Command() const { return LinuxParser::Command(m_pid); }

// DONE: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(m_pid); }

// DONE: Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(m_pid); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(m_pid); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& other) const
{
  return CpuUtilization() < other.CpuUtilization();
}
