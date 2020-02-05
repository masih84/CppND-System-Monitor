#include <string>

#include "format.h"

using std::string;

// Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds[[maybe_unused]]) { 
    int hh, mm, ss;
    long int time;

    string hour, min, sec;

    // calculate sec, min and hour
    time = seconds;
   
    hh = time/3600;

    time = time%3600;

    mm = time/60;

    time = time%60;

    ss = time;

    // convert int to string

    if (ss<10) {
        sec = "0" + std::to_string(ss);
    }
    else
    {
        sec = std::to_string(ss);
    }
    
    if (mm<10) {
        min = "0" + std::to_string(mm);
    }
    else
    {
        min = std::to_string(mm);
    }
    if (hh<10) {
    hour = "0" + std::to_string(hh);
    }
    else
    {
        hour = std::to_string(hh);
    }
    

    return (hour + ":" + min + ":" + sec);    
    }