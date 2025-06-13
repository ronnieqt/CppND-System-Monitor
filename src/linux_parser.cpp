#include <dirent.h>
#include <unistd.h>
#include <cmath>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <iostream>  // TMP: for dev

#include "linux_parser.h"

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
  return std::lround(up_time);
}

// ============================================================
// CPU

// DONE: Read and return CPU utilization
std::vector<long> LinuxParser::CpuUtilization()
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
long LinuxParser::Jiffies()
{
  return IdleJiffies() + ActiveJiffies();
}

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

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

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

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

// ============================================================
