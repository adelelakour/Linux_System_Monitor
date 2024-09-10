#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "ProcessParser.h"


class Process {
private:
    string pid;
    string user;
    string cmd;
    string cpu;
    string mem;
    string upTime;

public:
    Process(string pid)
    {
        this->pid = pid;
        this->user = ProcessParser::getProcUser(pid);
        this->mem = ProcessParser::getVmSize(pid);
        this->cmd = ProcessParser::getCmd(pid);
        this->upTime = ProcessParser::getProcUpTime(pid);
        this->cpu  = ProcessParser::getCpuPercent(pid);
    }
    void setPid(int pid);
    string getPid() const;
    string getUser() const;
    string getCmd() const;
    int getCpu() const;
    int getMem() const;
    string getUpTime() const;
    string getProcess();
};


#endif