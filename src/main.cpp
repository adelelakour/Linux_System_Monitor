#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <ncurses.h>
#include <time.h>
#include <sstream>
#include <iomanip>
#include "util.h"
#include "SysInfo.h"
#include "ProcessContainer.h"

using namespace std;

// Function to escape '%' symbols
std::string escapePercent(std::string str) {
  std::string result;
  for (char ch : str) {
    if (ch == '%') {
      result += "%%";  // Escape '%' with '%%'
    } else {
      result += ch;
    }
  }
  return result;
}

// No need for manual C-style string conversion anymore, so getCString can be removed.

// Function to print system information to the console
void writeSysInfoToConsole(SysInfo sys, WINDOW *sys_win) {
  sys.setAttributes();

  mvwaddstr(sys_win, 2, 2, ("OS: " + sys.getOsName()).c_str());
  mvwaddstr(sys_win, 3, 2, ("Kernel version: " + sys.getKernelVersion()).c_str());
  mvwaddstr(sys_win, 5, 2, "CPU:    ");

  wattron(sys_win, COLOR_PAIR(2));
  mvwaddstr(sys_win, 5, 9, Util::getProgressBar(sys.getCpuPercent()).c_str());
  wattroff(sys_win, COLOR_PAIR(2));

  mvwaddstr(sys_win, 6, 2, "Other cores:");

  wattron(sys_win, COLOR_PAIR(2));
  std::vector<std::string> val = sys.getCoresStats();

  for (int i = 0; i < val.size(); i++) {
    mvwaddstr(sys_win, (7 + i), 2, val[i].c_str());
  }

  wattroff(sys_win, COLOR_PAIR(2));
  mvwaddstr(sys_win, 11, 2, "Memory: ");

  wattron(sys_win, COLOR_PAIR(2));
  mvwaddstr(sys_win, 11, 10, Util::getProgressBar(sys.getMemPercent()).c_str());
  wattroff(sys_win, COLOR_PAIR(2));

  mvwaddstr(sys_win, 12, 2, ("Total Processes: " + sys.getTotalProc()).c_str());
  mvwaddstr(sys_win, 13, 2, ("Running Processes: " + sys.getRunningProc()).c_str());
  mvwaddstr(sys_win, 14, 2, ("Up Time: " + Util::convertToTime(sys.getUpTime())).c_str());

  wrefresh(sys_win);
}

// Function to print process list to the console
void getProcessListToConsole(std::vector<string> processes, WINDOW *win) {
  wattron(win, COLOR_PAIR(2));
  mvwaddstr(win, 1, 2, "PID:");
  mvwaddstr(win, 1, 12, "User:");
  mvwaddstr(win, 1, 22, "CPU[%%]:  ");
  mvwaddstr(win, 1, 31, "RAM[MB]:");
  mvwaddstr(win, 1, 40, "Uptime:");
  mvwaddstr(win, 1, 49, "CMD:");
  wattroff(win, COLOR_PAIR(2));

  for (int i = 0; i < processes.size(); i++) {
    mvwaddstr(win, 2 + i, 2, processes[i].c_str());
  }
}

// Main loop for refreshing the screen and displaying system and process information
void printMain(SysInfo sys, ProcessContainer procs) {
  initscr();      /* Start curses mode */
  noecho();       // not printing input values
  cbreak();       // Terminating on classic ctrl + c
  start_color();  // Enabling color change of text

  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);  // getting size of window measured in lines and columns
  WINDOW *sys_win = newwin(17, xMax - 1, 0, 0);
  WINDOW *proc_win = newwin(15, xMax - 1, 18, 0);

  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  int counter = 0;

  while (1) {
    try {
      box(sys_win, 0, 0);
      box(proc_win, 0, 0);

      procs.refreshList();
      std::vector<std::vector<std::string>> processes = procs.getList();

      writeSysInfoToConsole(sys, sys_win);
      getProcessListToConsole(processes[counter], proc_win);

      wrefresh(sys_win);
      wrefresh(proc_win);
      refresh();
      sleep(1);

      if (counter == (processes.size() - 1)) {
        counter = 0;
      } else {
        counter++;
      }
    } catch (...) {}
  }

  endwin();
}

int main(int argc, char *argv[]) {
  ProcessContainer procs;  // Object that contains list of current processes
  SysInfo sys;             // Object for system information

  printMain(sys, procs);
  return 0;
}
