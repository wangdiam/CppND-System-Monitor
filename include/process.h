#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) {
      _pid = pid;
      _cpu_utilization = Process::CpuUtilization();
      _command = LinuxParser::Command(_pid);
  }
  int Pid();                              
  std::string User();                     
  std::string Command();                  
  float CpuUtilization();                 
  std::string Ram();                      
  long int UpTime();                      
  bool operator<(Process const& a) const; 

 private:
 int _pid;
 std::string _user;
 std::string _command;
 float _cpu_utilization;
 std:: string _ram;
 long _uptime;
};

#endif