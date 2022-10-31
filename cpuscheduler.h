#ifndef TEST_H
#define TEST_H

#include <pcb.h>
#include <queue>
#include <cpu.h>
#include <QObject>
#include <QDebug>

class CPUScheduler : public QObject
{
    Q_OBJECT
public:
    explicit CPUScheduler(QObject *parent = nullptr);
    void newProcess(PCB* newP);
    void newProcess(string PID,int time,int priority,string father = "");
    void exec1();

    CPU* cpu = new CPU();
    queue<PCB>* jobQueue = new queue<PCB>;
    priority_queue<PCB>* suspendQueue = new priority_queue<PCB>;
    priority_queue<PCB>* readyQueue = new priority_queue<PCB>;
    vector<PCB>* historyQueue = new vector<PCB>;

    void free(string fatherPID);

signals:

public slots:
};

#endif // TEST_H
