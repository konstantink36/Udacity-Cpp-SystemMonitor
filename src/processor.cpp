#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>
#include <vector>
#include <string>
#include <chrono>
#include <thread>

#include <iostream>

using std::vector;
using std::string;

// Return the aggregate CPU utilization

float Processor::Utilization() 
{
	long total = LinuxParser::Jiffies();
  	long active = LinuxParser::ActiveJiffies();
	float util = float(active)/float(total);
	return util;
}

