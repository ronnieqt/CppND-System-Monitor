#include <iostream>
#include "linux_parser.h"
#include "format.h"

int main() {
  std::cout << Format::ElapsedTime(165) << std::endl;
  return 0;
}