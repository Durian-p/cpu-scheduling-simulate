#include "pcb.h"

PCB::PCB(string PID,int time,int priority,string father)
{
    this->PID = PID;
    this->time = time;
    time_consume = time;
    this->priority = priority;
    priority_pre = priority;
    priority_ori = priority;
    this->father = father;
}

