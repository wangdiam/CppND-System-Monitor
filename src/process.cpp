#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() { return _pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    long int hz = sysconf(_SC_CLK_TCK);
    return ((float)(LinuxParser::ActiveJiffies(_pid)/hz)/(LinuxParser::UpTime(_pid)));
}

string Process::Command() { return LinuxParser::Command(_pid); }

string Process::Ram() { return LinuxParser::Ram(_pid); }

string Process::User() { return LinuxParser::User(_pid); }

long int Process::UpTime() { return LinuxParser::UpTime(_pid); }

bool Process::operator<(Process const& a) const { return _cpu_utilization > a._cpu_utilization; }