#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   
  std::vector<Process>& Processes();  
  float MemoryUtilization();          
  long UpTime();                      
  int TotalProcesses();               
  int RunningProcesses();             
  std::string Kernel();               
  std::string OperatingSystem();      

 private:
  Processor _cpu = {};
  std::vector<Process> _processes = {};
  float _memory_utilization;
  long _uptime;
  int _total_processes;
  int _running_processes;
  std::string _kernel;
  std::string _operating_system;
};

#endif