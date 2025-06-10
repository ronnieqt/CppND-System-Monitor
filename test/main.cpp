#include <iostream>
#include <thread>
#include <chrono>
#include "linux_parser.h"
#include "format.h"
#include "processor.h"
#include "system.h"
#include "process.h"

using namespace std::chrono_literals;

int main() {
  // std::cout << LinuxParser::MemoryUtilization() << std::endl;

  // std::cout << Format::ElapsedTime(165) << std::endl;

  // auto cpu = LinuxParser::CpuUtilization();
  // std::cout << cpu[LinuxParser::CPUStates::kUser_] << std::endl;

  // Processor p{};
  // std::this_thread::sleep_for(3s);
  // std::cout << p.Utilization() << std::endl;

  // for (auto pid : LinuxParser::Pids()) {
  //   std::cout << LinuxParser::Uid(pid) << std::endl;
  // }

  // System sys{};
  // for (auto& p : sys.Processes()) {
  //   std::cout << p.Pid() << " " << p.User() << " " << LinuxParser::UpTime(p.Pid()) << std::endl;
  // }

  // std::cout << LinuxParser::Username(0) << std::endl;
  // std::cout << LinuxParser::Username(1000) << std::endl;

  // int pid{21302};
  // std::cout << LinuxParser::UpTime(pid) << std::endl;
  // std::cout << LinuxParser::CpuUtilization(pid) << std::endl;
  // std::cout << LinuxParser::Ram(pid) << std::endl;
  // std::cout << LinuxParser::Command(pid) << std::endl;

  Process process1{1};
  std::cout << process1.CpuUtilization() << std::endl;
  Process process2{82};
  std::cout << process2.CpuUtilization() << std::endl;

  return 0;
}