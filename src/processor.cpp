#include "processor.h"
#include "ProcessParser.h"
#include <vector>
#include <string>
#include "std_includes.h"

float getSysActiveCpuTime(vector<string> values) {
    return (stof(values[S_USER]) +
            stof(values[S_NICE]) +
            stof(values[S_SYSTEM]) +
            stof(values[S_IRQ]) +
            stof(values[S_SOFTIRQ]) +
            stof(values[S_STEAL]) +
            stof(values[S_GUEST]) +
            stof(values[S_GUEST_NICE]));
}


float getSysIdleCpuTime(vector<string> values) {
    return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}



// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    // Fetch current CPU utilization values
    std::vector<std::string> current_cpu_values = ProcessParser::getSysCpuPercent("");

    // Calculate active and idle times from the current CPU data
    float current_active_time = getSysActiveCpuTime(current_cpu_values);
    float current_idle_time = getSysIdleCpuTime(current_cpu_values);

    // Total time is the sum of active and idle times
    float total_time = current_active_time + current_idle_time;

    // Calculate utilization as a fraction of active time over total time
    if (total_time == 0) {
        return 0.0; // Prevent division by zero
    }

    return (current_active_time / total_time);
}