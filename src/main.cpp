#include "ncurses_display.h"
#include "system.h"
#include "SysInfo.h"

int main() {

  // Object which contains list of current processes, Container for Process Class
  ProcessContainer procs;
  // Object which contains relevant methods and attributes regarding system details
  SysInfo sys;

  // Call the printMain function within the NCursesDisplay namespace
  NCursesDisplay::printMain(sys, procs);

  return 0;
}

