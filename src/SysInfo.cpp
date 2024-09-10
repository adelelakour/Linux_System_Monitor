//
// Created by adelelakour on 10.09.24.
//
#include "SysInfo.h"
#include "std_includes.h"
#include "ProcessParser.h"

string SysInfo::getCpuPercent() const
{
    return this->cpuPercent;
}

string SysInfo::getMemPercent() const
{
    return to_string(this->memPercent);
}

long SysInfo::getUpTime() const
{
    return this->upTime;
}

string SysInfo::getKernelVersion() const
{
    return this->kernelVer;
}

string SysInfo::getTotalProc() const
{
    return to_string(this->totalProc);
}

string SysInfo::getRunningProc() const
{
    return to_string(this->runningProc);
}

string SysInfo::getThreads() const
{
    return to_string(this->threads);
}

string SysInfo::getOsName() const
{
    return this->osName;
}

void SysInfo::setLastCpuMeasures()
{
    this->lastCpuStats = ProcessParser::getSysCpuPercent();
}


void SysInfo::getOtherCores(int _size)
{
    //when number of cores is detected, vectors are modified to fit incoming data
    this->coresStats = vector<string>();
    this->coresStats.resize(_size);
    this->lastCpuCoresStats = vector<vector<string>>();
    this->lastCpuCoresStats.resize(_size);
    this->currentCpuCoresStats = vector<vector<string>>();
    this->currentCpuCoresStats.resize(_size);
    for (int i = 0; i < _size; i++) {
        this->lastCpuCoresStats[i] = ProcessParser::getSysCpuPercent(to_string(i));
    }
}


void SysInfo::setCpuCoresStats()
{
    // Getting data from files (previous data is required)
    for(int i = 0; i < this->currentCpuCoresStats.size(); i++) {
        this->currentCpuCoresStats[i] = ProcessParser::getSysCpuPercent(to_string(i));
    }
    for(int i = 0; i < this->currentCpuCoresStats.size(); i++) {
        // after acquirement of data we are calculating every core percentage of usage
        this->coresStats[i] = ProcessParser::printCpuStats(this->lastCpuCoresStats[i],this->currentCpuCoresStats[i]);
    }
    this->lastCpuCoresStats = this->currentCpuCoresStats;
}

