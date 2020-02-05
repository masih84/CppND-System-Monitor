#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // See src/processor.cpp

  // Declare any necessary private members
 private:
 double Total_;     // Total cpu usage
 double NonIdle_;   // Non idle usage
 double Idle_;      // Ide usage

 double PrevIdle_ = 0;  // previous Idle usage
 double PrevTotal_ = 0; //previous Total usage
};

#endif