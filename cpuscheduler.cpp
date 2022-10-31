#include "cpuscheduler.h"

CPUScheduler::CPUScheduler(QObject *parent) : QObject(parent)
{

}

void CPUScheduler::newProcess(PCB* newP)
{
    if(readyQueue->size() < 6)
        readyQueue->push(*newP);
     else
        jobQueue->push(*newP);
}

void CPUScheduler::newProcess(string PID,int time,int priority,string father)
{
    PCB* newP = new PCB(PID,time,priority,father);
    if(readyQueue->size() < 6)
        readyQueue->push(*newP);
    else
    {
        if(father == "")
            jobQueue->push(*newP);
        else
        {
            queue<PCB>* tempQ = new queue<PCB>;
            tempQ->push(*newP);
            while(!jobQueue->empty())
            {
                tempQ->push(jobQueue->front());
                jobQueue->pop();
            }
            queue<PCB>* a = jobQueue;
            jobQueue = tempQ;
            delete a;
        }

    }
}

void CPUScheduler::exec1()
{    
    if(readyQueue->empty())
        return;
    PCB tempE = readyQueue->top();
    if(tempE.suspend)
        return;
    cpu->exec1(tempE);
    readyQueue->pop();

    //其余进程优先度增加
    int inc = 1;
    int size = (int)readyQueue->size();
    priority_queue<PCB>* tempQ = new priority_queue<PCB>;
    for(int i = 0;i < size;i++)
    {
        PCB temp2 = readyQueue->top();
        if(temp2.suspend)
        {
            tempQ->push(temp2);
            readyQueue->pop();
            continue;
        }
        if(temp2.priority + inc <= 140)       
        {
            temp2.priority += inc;
            temp2.priority_pre += inc;
        }
        else
        {
            temp2.priority = 140;
            temp2.priority_pre = 140;
        }

        tempQ->push(temp2);
        readyQueue->pop();
    }
    priority_queue<PCB>* a;
    a = readyQueue;
    readyQueue = tempQ;
    delete a;

    //进程未完成push回 已完成push历史队列
    if(tempE.time > 0)
    {
        readyQueue->push(tempE);
    }
    else
    {
        //运行完毕队列
        free(tempE.father);
        historyQueue->push_back(tempE);
    }
}

void CPUScheduler::free(string fatherPID)
{
    priority_queue<PCB>* tempQ = new priority_queue<PCB>;

    while(!readyQueue->empty())
    {
       PCB tempP = readyQueue->top();
       if(tempP.PID == fatherPID)
       {
           tempP.running_children--;
           qDebug()<<"father --";
       }
       tempQ->push(tempP);
       readyQueue->pop();
    }
    priority_queue<PCB>* a = readyQueue;
    readyQueue = tempQ;
    delete a;
}
