#include "cpuscheuler.h"


void CPUScheuler::newProcess(string PID,int time,int priority,pstate state,PCB* father)
{
    PCB* newP = new PCB(PID,time,priority,state,father);
    jobQueue.push(*newP);
}

void CPUScheuler::exec()
{
    PCB temp = readyQueue.top();
    cpu->exec(temp);
    readyQueue.pop();
}
