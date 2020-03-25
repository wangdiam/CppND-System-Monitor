#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"
#include "format.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() { 
  float memory_utilization;
  std::ifstream stream(kProcDirectory+kMeminfoFilename);
  vector<float> memory_data;
  std::string title, data;
  if (stream.is_open()) {
    std::string line; 
    for (int i=0;i<2;i++) {
        std::getline(stream,line);
        std::istringstream linestream(line);
        linestream >> title >> data;
        memory_data.push_back(std::stof(data));
    }
  }
  memory_utilization = (memory_data[0]-memory_data[1]) / memory_data[0];
  return memory_utilization; 
}

long LinuxParser::UpTime() { 
  long uptime;
  std::ifstream stream(kProcDirectory+kUptimeFilename);
  string uptime_str;
  if (stream.is_open()) {
    std::string line;
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime_str;
    uptime = std::stol(uptime_str);
  }
  return uptime; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return 0; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line, value;
  std::vector<std::string> jiffies_list;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (stream.is_open()) {
    std::getline(stream,line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      jiffies_list.push_back(value);
    }
  }
  return std::stol(jiffies_list[13]) + std::stol(jiffies_list[14]) + std::stol(jiffies_list[15]) + std::stol(jiffies_list[16]);
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

vector<string> LinuxParser::CpuUtilization() { 
  std::ifstream stream(kProcDirectory+kStatFilename);
  std::string line, key, value;
  vector<string> values;
  if (stream.is_open()) {
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> key;
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  return values; 
}

int LinuxParser::TotalProcesses() { 
  std::ifstream stream(kProcDirectory+kStatFilename);
  std::string line, key, value;
  int int_value;
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          int_value = std::stoi(value);
          break;
        }
      }
    }
  }
  return int_value;
}

int LinuxParser::RunningProcesses() { 
  std::ifstream stream(kProcDirectory+kStatFilename);
  std::string line, key, value;
  int int_value;
  if (stream.is_open()) {
    while (std::getline(stream,line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          int_value = std::stoi(value);
        }
      }
    }
    
  }
  return int_value;
}

string LinuxParser::Command(int pid) { 
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kCmdlineFilename);
  std::string command;
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}

string LinuxParser::Ram(int pid) { 
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  std::string key, value, line, memory_usage;
  if (stream.is_open()) {
    while (std::getline(stream,line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          memory_usage = std::to_string(std::stoi(value)/1000);
          break;
        }
      }
    }
  }
  return memory_usage; 
}

string LinuxParser::Uid(int pid) { 
  std::string uid, line, key, value;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream,line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
          break;
        }
      }
    }
  }
  return uid; 
}

string LinuxParser::User(int pid) { 
  std::string user, line, key, value, what;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream,line)) {
      std::replace(line.begin(),line.end(),':', ' ');
      std::istringstream linestream(line);
      linestream >> key >> what >> value;
      if (value == LinuxParser::Uid(pid)) {
        user = key;
        break;
      }
    }
  }
  return user; 
}

long LinuxParser::UpTime(int pid) { 
  std::string value, line;
  std::ifstream stream(kProcDirectory+std::to_string(pid)+kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i=0;i<22;i++) {
      linestream >> value;
    }
  }
  return std::stol(value); 
}