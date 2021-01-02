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

// DONE: Return the aggregate CPU utilization
// fraction between the active jiffies (non idle) and total jiffies (idle + non idle).

float Processor::Utilization() 
{
	long total = LinuxParser::Jiffies();
  	long active = LinuxParser::ActiveJiffies();
	float util = float(active)/float(total);
	return util;
}

