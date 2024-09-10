#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"
#include "SysInfo.h"
#include "ProcessContainer.h"


namespace NCursesDisplay {

    char* getCString(const std::string& str);
    void writeSysInfoToConsole(SysInfo sys, WINDOW* sys_win);
    void getProcessListToConsole(ProcessContainer procs, WINDOW* win);
    void printMain(SysInfo sys, ProcessContainer procs);

};  // namespace NCursesDisplay

#endif