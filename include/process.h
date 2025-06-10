#ifndef PROCESS_H
#define PROCESS_H

#include <string>

#include "linux_parser.h"

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 private:
  int m_pid;

 public:
  Process(int pid) : m_pid{pid} {}
  ~Process() = default;

  int Pid() const;
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(Process const& other) const;
};

#endif