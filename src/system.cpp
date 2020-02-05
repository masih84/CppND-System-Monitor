#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pids = LinuxParser::Pids();

    set<int> extant_pids;
    for (Process process : processes_){
        extant_pids.insert(process.Pid());
    }

    // loop through all Process ids and add new processes
    for (int pid:pids){
        if(extant_pids.find(pid) == extant_pids.end()){
            processes_.emplace_back(pid);
        }
    }

    // Sort processes based on cpu usage
    std::sort(processes_.begin(), processes_.end(),std::greater<Process>());
    
    return processes_; }

// Return the system's kernel identifier (string)
std::string System::Kernel() { 
    if (kernel_ == ""){
        kernel_ = LinuxParser::Kernel();
    }
    return kernel_;}

// Return the system's memory utilization
float System::MemoryUtilization() {
     uptime_ =  LinuxParser::UpTime();
     return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() {
    // check if os_ is empty, and parse it first time
    if (os_ == ""){
        os_ = LinuxParser::OperatingSystem();
    }
    return  os_;}

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
// Read and return the system uptime
long int System::UpTime() {  
    return uptime_;}