//
// Created by adelelakour on 11.09.24.
//
#include "ProcessContainer.h"



void ProcessContainer::refreshList()
{
    vector<string> pids = ProcessParser::getPidList();
    this->_list.clear();
    for (auto pid : pids) {
        Process proc(pid);
        this->_list.push_back(proc);
    }
}

string ProcessContainer::printList()
{
    std::string result="";
    for (auto i : _list) {
        result += i.getProcess();
    }
    return result;
}

std::vector<std::vector<std::string>> ProcessContainer::getList() {
    std::vector<std::vector<std::string>> values;
    std::vector<std::string> stringifiedList;

    for (int i = 0; i < ProcessContainer::_list.size(); i++) {
        stringifiedList.push_back(ProcessContainer::_list[i].getProcess());
    }

    int lastIndex = 0;

    for (int i = 0; i < stringifiedList.size(); i++) {
        if (i % 10 == 0 && i > 0) {
            std::vector<std::string> sub(&stringifiedList[i - 10],
                                         &stringifiedList[i]);
            values.push_back(sub);
            lastIndex = i;
        }
        if (i == (ProcessContainer::_list.size() - 1) && (i - lastIndex) < 10) {
            std::vector<std::string> sub(&stringifiedList[lastIndex],
                                         &stringifiedList[i + 1]);
            values.push_back(sub);
        }
    }
    return values;
}

