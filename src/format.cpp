#include <string>
#include <iostream>
#include <sstream>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) {
	long h, m, s;
	h = (seconds/3600);
	m = (seconds - (3600*h))/60;
	s = (seconds - (3600*h)-(m*60));

std::ostringstream stream;
if (h > 99) { return "99:59:59"; }
    if (h < 10) { stream << "0"; }
    stream << h << ":" ;
    if (m < 10) { stream << "0"; }
    stream << m << ":";
    if (s < 10) { stream << "0"; }
    stream << s;
    return stream.str();  
}
