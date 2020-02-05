#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <algorithm>
#include <unistd.h>


#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Defined tag to keep systemup time and send it to processes
long int sysuptime{0};

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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

  double MemTotal, MemFree, Buffers, Cached_memory, Total_used_memory, util_memory;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          std::istringstream(value) >> MemTotal;
        }
        if (key == "MemFree:") {
          std::istringstream(value) >> MemFree;
        }
        if (key == "Buffers:") {
          std::istringstream(value) >> Buffers;
        }
        if (key == "Cached:") {
          std::istringstream(value) >> Cached_memory;
        }
      }
    }
  }
  // Total use memory
  Total_used_memory = MemTotal - MemFree;

  // util memory 
  util_memory = Total_used_memory - (Buffers + Cached_memory);

  // memory percentage in use
  float mem_prc = util_memory/Total_used_memory;

  return (mem_prc); }

// Read and return the system uptime
long int LinuxParser::UpTime() { 
  string line;
  float key1, key2;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key1 >> key2) {
        sysuptime = key1;
        return key1;
      }
    }
  } 
  return 0; }
// Return the System UpTime instead of parsing
  long int LinuxParser::ReadUpTime(){
    return sysuptime;
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, key;
  vector<string> keys;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key){
        if (key == "cpu") {
          for(int i = 0; i<10 ; i++){
            linestream >> key;
            keys.push_back(key);
          }
        return keys;
        }
      }
    } 
  }
  return {}; }

  // Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization(int pid) {
  vector<int> my_vec = {13,14,15,16,21};
  string KPIDFilename = "/" + std::to_string(pid) + "/stat";
  string line, key;
  vector<string> keys;
  std::ifstream filestream(kProcDirectory + KPIDFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
        for(int i = 0; i<23 ; i++){
            linestream >> key;
            if (std::count(my_vec.begin(), my_vec.end(), i)){
            keys.push_back(key);}
          }
          return keys;
      }
    } 
  
  return{};}


// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  } 
  return 0; }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string key1;
  int value = 1;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key1 >> value) {
        if (key1 =="procs_running") {
            return value;
        }
      }
    }
  } 
  return 0; }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  string KCMDFilename = "/" + std::to_string(pid) + "/cmdline";
  string key = "";

  // step1 read cmdline file and find cmd
  std::ifstream filestream(kProcDirectory + KCMDFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      //linestream >> key;
       return line;
    }    
  }
  return key;   

}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string KPIDFilename = "/" + std::to_string(pid) + "/status";
  string key1;
  long int value = 0;

  // step1 read status file and find user id
  std::ifstream filestream(kProcDirectory + KPIDFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key1 >> value) {
        if (key1 =="VmSize:") {
            return std::to_string(value/1000);
        }
      }
    }
  } 
  
  return string(); }

// Read and return the user ID associated with a process
int LinuxParser::Uid(int pid) { 
  string line;
  string KPIDFilename = "/" + std::to_string(pid) + "/status";
  string key1;
  int value = 0;

  // step1 read status file and find user id
  std::ifstream filestream(kProcDirectory + KPIDFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key1 >> value) {
        if (key1 =="Uid:") {
            return value;
        }
      }
    }
  } 

  return 0; }

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
 string line;
 string KPIDFilename = "/" + std::to_string(pid) + "/status";
  string key1, key2;
  int value = 0;
  int uid = LinuxParser::Uid(pid);

  // step1 read password path and find user name

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open() && uid != -1) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key1 >> key2 >> value) {
        if (value == uid) {
          std::replace(key1.begin(), key1.end(), '_', ' ');
          return key1;
        }
      }
    }
  }

  return string(); }

// Read and return the uptime of a process
long int LinuxParser::UpTime(int pid) { 
  string line;
  string KPIDFilename = "/" + std::to_string(pid) + "/stat";
  string key;
  long int value = 0;
  int Hertz = sysconf(_SC_CLK_TCK);
  std::ifstream filestream(kProcDirectory + KPIDFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        for(int i = 0; i<22 ; i++){
          linestream >> key;
          }
          std::istringstream(key) >> value;
          return value/Hertz;
      }
    }
  return 0; }