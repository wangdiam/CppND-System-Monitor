#include "processor.h"
#include <vector>
#include <string>
#include "linux_parser.h"

float Processor::Utilization() { 
    std::vector<std::string> cpu_util = LinuxParser::CpuUtilization();
    float usage;
    for (auto i=0;i<cpu_util.size();i++) {
        if (i!=LinuxParser::kGuest_ and i!=LinuxParser::kGuestNice_) {
            total_jiffies += std::stof(cpu_util[i]);
        }
        if (i==LinuxParser::kIdle_ or i==LinuxParser::kIOwait_) {
            idle_jiffies += std::stof(cpu_util[i]);
        }
    }
    util_jiffies = total_jiffies - idle_jiffies;
    usage = (util_jiffies-util_jiffies_prev)/(total_jiffies-total_jiffies_prev);
    return usage; 
}