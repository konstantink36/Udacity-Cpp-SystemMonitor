#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

#include "linux_parser.h"

using std::stoi;
using std::stol;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Example of how to read data from the filesystem
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


string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel; 
  } 
  return kernel;
}

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

// Read and return the system memory utilization

float LinuxParser::MemoryUtilization() { 
  string key, value, line;				
  float memTotal = 0.0, memFree = 0.0;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);  
  if (stream.is_open()) {				
    while (std::getline(stream, line)) {		
      std::istringstream linestream(line);		
      linestream >> key >> value;			
      if (key == "MemTotal:") {				
        memTotal = stof(value);				
      }
      if (key == "MemFree:") {				
        memFree = stof(value);				
      }
    }
    return (memTotal - memFree) / memTotal;		
  }
return 0; 
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime, line;			
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;		
    return stol(uptime);		
  }
  return 0; 
 }

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> cpu_str = LinuxParser::CpuUtilization();
  vector<long> cpu_long(10, 0);
  long jiffies = 0;
  for (int i=0; i<10; i++ ) { 	
    cpu_long[i] = stol(cpu_str[i]);
    jiffies += cpu_long[i];   
  };
  return jiffies;
} 

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  long sum = 0;
  vector<string> values;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    }
  }
  sum = stol(values[13]) + stol(values[14]);
  return sum; 
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> jiffies = CpuUtilization();
  long sum = 0;
  sum = stol(jiffies[0]) + stol(jiffies[1]) + stol(jiffies[2]);
  return sum;
}

// Read and return the number of idle jiffies for the system
// columns from left to right: user, nice, system, idle, iowait
// idle jiffies = idle + iowait
long LinuxParser::IdleJiffies() {
 vector<string> jiffies = CpuUtilization();
  long sum = 0;
  sum = stol(jiffies[3]) + stol(jiffies[4]); 
  return sum;
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string value, line, key;
  vector<string> cpuValues;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
     linestream >> key;  
    while (linestream >> value) {
        cpuValues.push_back(value);
    }
  }
  return cpuValues;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return stoi(value);
        }
      }
    }
  }
  return 0;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  std::stringstream ram;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmData:") {
       ram << std::fixed << std::setprecision(3) << stof(value) / 1024; 
       return ram.str();
      }
    }
  }
  return string();
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  }
  return string();
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string uid = LinuxParser::Uid(pid);
  string line, key, x, value;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
   while (std::getline(stream, line)) {
     std::replace(line.begin(), line.end(), ':', ' ');
     std::istringstream linestream(line);
     linestream >> key >> x >> value;
     if (value == uid) {
       return key;
     }
   }
  }
  return string();
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, value;
  long starttime, uptime;
  vector<string> values;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.push_back(value);
    };
  }
 starttime = stol(values[21]) / sysconf(_SC_CLK_TCK);  
 uptime = LinuxParser::UpTime() - starttime;
 return uptime;
}
