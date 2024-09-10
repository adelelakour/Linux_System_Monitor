#include "ncurses_display.h"
#include "system.h"
#include "ProcessParser.h"

#include <iostream>



int main() {

  std::cout << "Helloooooooooooooooooooo \n";

  ProcessParser p;

  std::cout << p.getSysRamPercent() << std::endl;


  /*
  System system;
  NCursesDisplay::Display(system);
  */




}