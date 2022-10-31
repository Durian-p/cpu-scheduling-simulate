#include "cpu.h"

CPU::CPU(QObject *parent) : QObject(parent)
{

}

void CPU::exec1(PCB &p)
{
    p.time--;
    int dec = 3;
    if(p.priority - dec >= 100)
     {
        p.priority-= dec;
        p.priority_pre-= dec;
     }
    else
    {
        p.priority = 100;
        p.priority_pre = 100;
    }
}
