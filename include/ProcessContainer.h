//
// Created by adelelakour on 11.09.24.
//

#ifndef PROCESSCONTAINER_H
#define PROCESSCONTAINER_H


#include "process.h"

#include <string>
#include <vector>
using std::string;
using std::vector;

class ProcessContainer {
public:
    ProcessContainer()
    {
        this->refreshList();
    }
    void refreshList();
    string printList();
    vector<string> getList();

private:
    vector<Process>_list;
};



#endif //PROCESSCONTAINER_H
