#include <iostream>
#include <thread>
#include <chrono>
#include "linux_parser.h"
#include "format.h"
#include "processor.h"

using namespace std::chrono_literals;

int main() {
  // std::cout << Format::ElapsedTime(165) << std::endl;

  // auto cpu = LinuxParser::CpuUtilization();
  // std::cout << cpu[LinuxParser::CPUStates::kUser_] << std::endl;

  Processor p{};
  std::this_thread::sleep_for(3s);
  std::cout << p.Utilization() << std::endl;

  return 0;
}