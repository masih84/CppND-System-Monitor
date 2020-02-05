#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"


using std::string;
using std::to_string;
using std::vector;

// Constructor to get the ID and Parse user and cmd.
Process::Process(int pid){
    pid_ = pid;
    user_ = LinuxParser::User(pid_);
    cmd_ = LinuxParser::Command(pid_);
}

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() { 
    string key;
    double utime;     //#14  - CPU time spent in user code, measured in clock ticks
    double stime;     //#15 - CPU time spent in kernel code, measured in clock ticks
    double cutime;    //#16 Waited-for children's CPU time spent in user code (in clock ticks)
    double cstime;    // #17  - Waited-for children's CPU time spent in kernel code (in clock ticks)
    double starttime; // #22  - Time when the process started, measured in clock ticks
    int Hertz = sysconf(_SC_CLK_TCK); // from getconf CLK_TCK
    vector<string> CPUStat = LinuxParser::CpuUtilization(pid_);

        for (int i = 0; i < 5 ; i++){
        key = CPUStat.back();
        CPUStat.pop_back();
        switch(i){
            case 0: 
                std::istringstream(key) >> starttime;
                break;
            case 1: 
                std::istringstream(key) >> cstime;
                break;
            case 2: 
                std::istringstream(key) >> cutime;
                break;
            case 3: 
                std::istringstream(key) >> stime;
                break;
            case 4: 
                std::istringstream(key) >> utime;
                break;
            }
        }

    double total_time = utime + stime;
    //We also have to decide whether we want to include the time from children processes. 
    //If we do, then we add those values to total_time:
    
    total_time = total_time + cutime + cstime;
    
    // Next we get the total elapsed time in seconds since the process started:
    long uptime =  LinuxParser::ReadUpTime();
    float seconds = uptime - (starttime / Hertz);

    //Finally we calculate the CPU usage percentage:

    cpu_ = (total_time / Hertz) / seconds;
    return cpu_;
}

// Return the command that generated this process
string Process::Command() { return cmd_; }

// Return this process's memory utilization
string Process::Ram() { 
    string ram_str = LinuxParser::Ram(pid_); 
    std::istringstream(ram_str) >> ram_;
    return ram_str;
    }

// Return the user (name) that generated this process
string Process::User() { 
    return user_; 
    }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_);
 }

// Overload the "less than" comparison operator for Process objects
bool Process::operator>(Process const& a) const { 
    return (cpu_ > a.cpu_);
}

bool Process::operator<(Process const& a) const { 
    return (cpu_ < a.cpu_);
}