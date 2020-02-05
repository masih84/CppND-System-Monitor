#include "processor.h"
#include "linux_parser.h"
#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::vector;

// Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<string> CPUStat;
    CPUStat = LinuxParser::CpuUtilization();
    string key;
    double kUser_, kNice_, kSystem_, kIdle_, kIOwait_, kIRQ_, KSoftIRQ_, kSteal_, kGuest_, kGuestNice_;
    for (int i = 0; i < 10 ; i++){
        key = CPUStat.back();
        CPUStat.pop_back();
        switch(i){
            case 0: 
                std::istringstream(key) >> kGuestNice_;
                break;
            case 1: 
                std::istringstream(key) >> kGuest_;
                break;
            case 2: 
                std::istringstream(key) >> kSteal_;
                break;
            case 3: 
                std::istringstream(key) >> KSoftIRQ_;
                break;
            case 4: 
                std::istringstream(key) >> kIRQ_;
                break;
            case 5: 
                std::istringstream(key) >> kIOwait_;
                break;
            case 6: 
                std::istringstream(key) >> kIdle_;
                break;
            case 7: 
                std::istringstream(key) >> kSystem_;
                break;
            case 8: 
                std::istringstream(key) >> kNice_;
                break;
            case 9: 
                std::istringstream(key) >> kUser_;
                break;
        }
    }

    // calcluate CPU usage

    Idle_ = kIdle_ + kIOwait_;

    NonIdle_ = kUser_ + kNice_ + kSystem_ + kIRQ_ + KSoftIRQ_ + kSteal_;

    Total_ = Idle_ + NonIdle_;

    // differentiate: actual value minus the previous one
    
    double totald = Total_ - PrevTotal_;
    
    double idled = Idle_ - PrevIdle_;

    float CPU_Percentage = (totald - idled)/totald;

    PrevIdle_ = Idle_;

    PrevTotal_ = Total_;

    return CPU_Percentage;; 
    }