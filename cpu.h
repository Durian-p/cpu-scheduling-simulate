#ifndef CPU_H
#define CPU_H

#include <pcb.h>
#include <QObject>

class CPU : public QObject
{
    Q_OBJECT
public:
    explicit CPU(QObject *parent = nullptr);
    void exec1(PCB &p);

signals:

public slots:
};

#endif // CPU_H
