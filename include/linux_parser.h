#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <unistd.h>

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// ============================================================
// System

float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// ============================================================
// CPU

enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
// NOTE: the return type is updated from std::vector<string> to std::vector<long>
// so that these numbers are easier to manipulate in downstream code
std::vector<long> CpuUtilization();
long Jiffies();
long IdleJiffies();
long ActiveJiffies();

// ============================================================
// Processes

enum ProcessStat {
  kUtime_ = 13,
  kStime_ = 14,
  kCutime_ = 15,
  kCstime_ = 16,
  kStartTime_ = 21,
};
std::string Username(const std::string& uid);
std::string PidDir(int pid);
std::vector<std::string> Stat(int pid);
long ActiveJiffies(int pid);
float CpuUtilization(int pid);
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
};  // namespace LinuxParser

#endif