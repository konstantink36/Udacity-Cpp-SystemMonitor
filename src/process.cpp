#include <unistd.h>
#include <iostream>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) {
  pid_ = pid;
}

// DONE: Return this process's ID
int Process::Pid() const { return pid_; }

// DONE: Return this process's CPU utilization
// Process CPU utilization =  process active time/ process uptime
float Process::CpuUtilization() {
  float util;
  long active = LinuxParser::ActiveJiffies(Pid()); 
  long uptime = LinuxParser::UpTime(Pid());	
  long activetime = active / sysconf(_SC_CLK_TCK); //convert jiffies to seconds
  util = float(activetime) / float(uptime);
  return util;
}

// DONE: Return the command that generated this process
string Process::Command() { 
	return LinuxParser::Command(Process::Pid());
}

// DONE: Return this process's memory utilization
string Process::Ram() { 
	return LinuxParser::Ram(Process::Pid());
}

// DONE: Return the user (name) that generated this process
string Process::User() { 
	return LinuxParser::User(Process::Pid());
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() 
{ 
	return LinuxParser::UpTime(Process::Pid());  
}

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator< (Process & a) {
    return Process::CpuUtilization() > a.Process::CpuUtilization();
}
