#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

void Process::setPid(int pid)
{
    this->pid = pid;
}
string Process::getPid() const
{
    return this->pid;
}
string Process::getProcess()
{
    this->mem = ProcessParser::getVmSize(this->pid);
    this->upTime = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);

    return (this->pid + "   "
                    + this->user
                    + "   "
                    + this->mem.substr(0,5)
                    + "     "
                    + this->cpu.substr(0,5)
                    + "     "
                    + this->upTime.substr(0,5)
                    + "    "
                    + this->cmd.substr(0,30)
                    + "...");
}