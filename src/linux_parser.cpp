#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <cmath>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// ============================================================
// System

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem()
{
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel()
{
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids()
{
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization()
{
  // name => (init,value)
  std::unordered_map<std::string, std::pair<bool, float>> mem{
    {"total", {false, 0.0}},
    {"free" , {false, 0.0}},
  };
  string line, key;
  float value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal") {
        mem["total"].first = true;
        mem["total"].second = value;
      }
      else if (key == "MemFree") {
        mem["free"].first = true;
        mem["free"].second = value;
      }
      if (mem["total"].first && mem["free"].first) {
        break;
      }
    }
  }
  return (mem["total"].second - mem["free"].second) / mem["total"].second;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime()
{
  float up_time{0.0}, idle_time{0.0};
  std::string line;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> up_time >> idle_time;
  }
  return static_cast<long>(up_time);
}

// ============================================================
// CPU

// DONE: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization()
{
  std::vector<long> cpu(10, 0);
  std::string line;
  std::string key;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        for (int i = 0; i < 10; ++i) {
          linestream >> cpu[i];
        }
        break;
      }
      else {
        continue;
      }
    }
  }
  return cpu;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return IdleJiffies() + ActiveJiffies(); }

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
  auto cpu_stat = CpuUtilization();
  return cpu_stat[CPUStates::kIdle_] + cpu_stat[CPUStates::kIOwait_];
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  auto cpu_stat = CpuUtilization();
  return cpu_stat[CPUStates::kUser_] + cpu_stat[CPUStates::kNice_] +
         cpu_stat[CPUStates::kSystem_] + cpu_stat[CPUStates::kIRQ_] +
         cpu_stat[CPUStates::kSoftIRQ_] + cpu_stat[CPUStates::kSteal_];
}

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid)
{
  auto stats = Stat(pid);
  long utime = std::stol(stats[ProcessStat::kUtime_]);
  long stime = std::stol(stats[ProcessStat::kStime_]);
  long cutime = std::stol(stats[ProcessStat::kCutime_]);
  long cstime = std::stol(stats[ProcessStat::kCstime_]);
  return utime + stime + cutime + cstime;
}

// ============================================================

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
  std::string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        return value;
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses()
{
  std::string line, key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        return value;
      }
    }
  }
  return 0;
}

// ============================================================
// Processes

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid)
{
  std::string line, key, uid;
  std::string fp{kProcDirectory + std::to_string(pid) + "/" + kStatusFilename};
  std::ifstream filestream(fp);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "Uid") {
        linestream >> uid;
        return uid;
      }
    }
  }
  return string{"NA"};
}

// Read and return the user-name associated with a user-id
string LinuxParser::Username(const std::string& uid)
{
  std::string line{}, name{"NA"}, passwd{}, userid{};
  std::ifstream filestream{kPasswordPath};
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> passwd >> userid;
      if (userid == uid) {
        return name;
      }
    }
  }
  return name;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { return Username(Uid(pid)); }

// Return a path to a pid directory
string LinuxParser::PidDir(int pid)
{
  return kProcDirectory + std::to_string(pid);
}

// Read and return the stat of a process
vector<string> LinuxParser::Stat(int pid)
{
  std::vector<std::string> stats(52);
  std::string line{};
  std::ifstream filestream{PidDir(pid) + kStatFilename};
  if (filestream.is_open()) {
    std::getline(filestream, line);
    // replace empty space with _ in (<comm>)
    auto l = std::find(line.begin(), line.end(), '(');
    auto r = std::find(l, line.end(), ')');
    if (l != line.end() && r != line.end()) {
      std::replace(l, r, ' ', '_');
    }
    std::istringstream linestream(line);
    for (auto& s : stats) {
      linestream >> s;
    }
  }
  return stats;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid)
{
  long HZ = sysconf(_SC_CLK_TCK);
  auto starttime = Stat(pid)[ProcessStat::kStartTime_];
  return std::stol(starttime) / HZ;
}

// Read and return CPU utilization of a process
float LinuxParser::CpuUtilization(int pid)
{
  long HZ = sysconf(_SC_CLK_TCK);
  auto stats = Stat(pid);
  long starttime = std::stol(stats[ProcessStat::kStartTime_]);
  long uptime = UpTime();
  float total_time = static_cast<float>(ActiveJiffies(pid)) / HZ;
  float seconds = uptime - static_cast<float>(starttime) / HZ;
  return total_time / seconds;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid)
{
  std::string line{};
  std::ifstream filestream{PidDir(pid) + kCmdlineFilename};
  if (filestream.is_open()) {
    std::getline(filestream, line);
  }
  return line;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid)
{
  std::string line, key;
  long value;
  std::ifstream filestream{PidDir(pid) + kStatusFilename};
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "VmSize") {
        linestream >> value;
        return std::to_string(value / 1000);
      }
    }
  }
  return string{"0"};
}

// ============================================================
