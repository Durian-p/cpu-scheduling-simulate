#ifndef PCB_H
#define PCB_H
#include <string>
#include <QDebug>
#include <QColor>

using namespace std;

enum pstate{RUNNING,WAITING,READY,SUSPEND};

class PCB
{
public:
    PCB(string PID,int time,int priority,string father = "");
    friend bool operator < (PCB pcb1,PCB pcb2)
    {
        if(pcb1.priority == pcb2.priority)
            return pcb1.PID < pcb2.PID;
        return pcb1.priority < pcb2.priority;
    }

    string PID;
    int time;
    int time_consume;
    int priority;
    int priority_pre;
    int priority_ori;
    bool cancelSuspend = true;
    bool suspend = false;
    QColor* color = new QColor(0,0,0,0);
    int running_children = -1;

    pstate state;
    string father;

};

#endif // PCB_H
