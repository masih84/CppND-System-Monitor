#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);                        // Process constructor functions
  int Pid();                               // See src/process.cpp
  std::string User();                      // See src/process.cpp
  std::string Command();                   // See src/process.cpp
  float CpuUtilization();                  // See src/process.cpp
  std::string Ram();                       // See src/process.cpp
  long int UpTime();                       // See src/process.cpp
  bool operator>(Process const& a) const;  // See src/process.cpp
  bool operator<(Process const& a) const;  // See src/process.cpp

  // TODO: Declare any necessary private members
 private:
 int pid_;                  // Process ID
 std::string user_ = "";    // User ID
 std::string cmd_ = "";     // Process cmd
 long int ram_ = 0;         // Process ram
 float cpu_ = 0.0;          // Process cpu usage
};

#endif