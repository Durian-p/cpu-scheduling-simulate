#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QUrl"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(timer,&QTimer::timeout,this,&MainWindow::refresh,Qt::UniqueConnection);

    this->setWindowTitle("CPU调度模拟");
    this->setWindowIcon(QIcon(":/icon.png"));

    ui->lcd_timeSlice->setStyleSheet("border: 1px solid black; color: black; background: rgbs(255,255,255,0.6);");
    ui->lcd_timeSlice->setDigitCount(4);
    ui->lcd_timeSlice->display(800);


    ui->table_ready->setShowGrid(true);
    ui->table_ready->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_ready->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_ready->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->table_ready->verticalHeader()->setVisible(true);
    ui->table_ready->horizontalHeader()->setVisible(true);
    ui->table_ready->setColumnWidth(0,140);
    ui->table_ready->setColumnWidth(1,70);
    ui->table_ready->setColumnWidth(2,80);
    ui->table_ready->setColumnWidth(3,140);
    ui->table_ready->setRowHeight(0,59);
    ui->table_ready->setRowHeight(1,59);
    ui->table_ready->setRowHeight(2,59);
    ui->table_ready->setRowHeight(3,59);
    ui->table_ready->setRowHeight(4,59);
    ui->table_ready->setRowHeight(5,59);

    ui->table_backup->setShowGrid(true);
    ui->table_backup->setSelectionMode(QAbstractItemView::NoSelection);
    ui->table_backup->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_backup->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->table_backup->verticalHeader()->setVisible(false);
    ui->table_backup->horizontalHeader()->setVisible(true);
    ui->table_backup->setColumnWidth(0,160);
    ui->table_backup->setColumnWidth(1,85);
    ui->table_backup->setColumnWidth(2,94);
    ui->table_backup->setColumnWidth(3,160);

    ui->table_waiting->setShowGrid(true);
    ui->table_waiting->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->table_waiting->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_waiting->verticalHeader()->setVisible(false);
    ui->table_waiting->horizontalHeader()->setVisible(true);
    ui->table_waiting->setColumnWidth(0,150);
    ui->table_waiting->setColumnWidth(1,60);
    ui->table_waiting->setColumnWidth(2,69);
    ui->table_waiting->setColumnWidth(3,110);

    ui->table_history->setShowGrid(true);
    ui->table_history->setSelectionMode(QAbstractItemView::NoSelection);
    ui->table_history->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table_history->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->table_history->verticalHeader()->setVisible(false);
    ui->table_history->horizontalHeader()->setVisible(true);
    ui->table_history->setColumnWidth(0,130);
    ui->table_history->setColumnWidth(1,60);
    ui->table_history->setColumnWidth(2,69);
    ui->table_history->setColumnWidth(3,130);
}

void MainWindow::refresh()
{
    qDebug()<<"suspend size2:"<<cpus->suspendQueue->size();
    if(running)
        cpus->exec1();

    priority_queue<PCB>* temp0 = new priority_queue<PCB>;
    while(!cpus->readyQueue->empty())
    {
        PCB tempP = cpus->readyQueue->top();
        if(tempP.running_children == 0)
        {
            tempP.running_children = -1;
            tempP.cancelSuspend = true;
            tempP.suspend = false;
            tempP.color->setRgbF(200,200,0,0.7);
            tempP.priority = tempP.priority_pre;
            remove(tempP.PID,suspendQ);
        }
        temp0->push(tempP);
        cpus->readyQueue->pop();
    }
    priority_queue<PCB>* c =cpus->readyQueue;
    cpus->readyQueue = temp0;
    delete c;


    //jobQ上CPU至readyQ
    if(cpus->readyQueue->size() < 6 && cpus->jobQueue->size() > 0)
    {
        PCB temp = cpus->jobQueue->front();
        temp.color->setRgbF(0,255,0,0.7);
        cpus->readyQueue->push(temp);
        cpus->jobQueue->pop();
    }

    //显示预备队列
    qDebug()<<"----------------------------------------------";
    priority_queue<PCB>* temp1 = new priority_queue<PCB>;
    int size = cpus->readyQueue->size();
    string s = "";
       if(!cpus->readyQueue->empty())
        s += cpus->readyQueue->top().PID;
    ui->label_cpu->setText(QString::fromStdString(s));
    for(int i = 0;i < 6;i++)
    {
        if(i < size)
        {
            QTableWidgetItem* item;
            PCB tempP = cpus->readyQueue->top();
            qDebug()<<QString::fromStdString(tempP.PID)<<"  priority:"<<tempP.priority<<"  child:"<<tempP.running_children<<" cancle_suspend:"<<tempP.cancelSuspend;
            QString s = QString::fromStdString(tempP.PID);
            if(tempP.suspend)
                s += "(挂起)";
            item = new QTableWidgetItem(s);
            item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item->setBackground(*tempP.color);
            ui->table_ready->setItem(i,0,item);
            item = new QTableWidgetItem(QString::number(tempP.time));
            item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item->setBackground(*tempP.color);
            ui->table_ready->setItem(i,1,item);
            item = new QTableWidgetItem(QString::number(tempP.priority_pre));
            item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item->setBackground(*tempP.color);
            ui->table_ready->setItem(i,2,item);
            item = new QTableWidgetItem(QString::fromStdString(tempP.father));
            item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            item->setBackground(*tempP.color);
            ui->table_ready->setItem(i,3,item);
            if(!tempP.suspend)
                tempP.color->setRgbF(0,0,0,0);
            temp1->push(tempP);
            cpus->readyQueue->pop();
        }
        else
        {
            ui->table_ready->setItem(i,0,new QTableWidgetItem(""));
            ui->table_ready->setItem(i,1,new QTableWidgetItem(""));
            ui->table_ready->setItem(i,2,new QTableWidgetItem(""));
            ui->table_ready->setItem(i,3,new QTableWidgetItem(""));
        }
    }
    priority_queue<PCB>* b =cpus->readyQueue;
    cpus->readyQueue = temp1;
    delete b;
    qDebug()<<"----------------------------------------------";

    //显示后备队列
    queue<PCB> temp2 = *cpus->jobQueue;
    int size2 = (int)temp2.size();
    ui->table_backup->setRowCount(size2);
    for(int i = 0;i < size2;i++)
    {  
        QTableWidgetItem* item;
        item = new QTableWidgetItem(QString::fromStdString(temp2.front().PID));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->table_backup->setItem(i,0,item);
        item = new QTableWidgetItem(QString::number(temp2.front().time));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->table_backup->setItem(i,1,item);
        item = new QTableWidgetItem(QString::number(temp2.front().priority));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->table_backup->setItem(i,2,item);
        item = new QTableWidgetItem(QString::fromStdString(temp2.front().father));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->table_backup->setItem(i,3,item);
        temp2.pop();
    }

    //显示挂起队列
    priority_queue<PCB>* temp3 = new priority_queue<PCB>;
    int size3 = cpus->suspendQueue->size();
    if(size3 == 0)
        order = 0;
    qDebug()<<"suspend size3:"<<size3;
    ui->table_waiting->setRowCount(size3);
    for(int i = 0;i < size3;i++)
    {

        QTableWidgetItem* item;
        PCB tempP = cpus->suspendQueue->top();
        qDebug()<<QString::fromStdString(tempP.PID)<<"  priority:"<<tempP.priority<<"  child:"<<tempP.running_children<<" cancle_suspend:"<<tempP.cancelSuspend;
        QString s = QString::fromStdString(tempP.PID);
        if(!tempP.cancelSuspend)
            s += "(不可解挂)";
        item = new QTableWidgetItem(s);
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->table_waiting->setItem(i,0,item);
        item = new QTableWidgetItem(QString::number(tempP.time));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->table_waiting->setItem(i,1,item);
        item = new QTableWidgetItem(QString::number(tempP.priority));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->table_waiting->setItem(i,2,item);
        item = new QTableWidgetItem(QString::fromStdString(tempP.father));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->table_waiting->setItem(i,3,item);
        temp3->push(tempP);
        cpus->suspendQueue->pop();
    }
    priority_queue<PCB>* a = cpus->suspendQueue;
    cpus->suspendQueue = temp3;
    delete a;

    //显示历史队列
    int size4 = cpus->historyQueue->size();
    ui->table_history->setRowCount(size4);
    for(int i = 0;i < size4;i++)
    {
        QTableWidgetItem* item;
        item = new QTableWidgetItem(QString::fromStdString(cpus->historyQueue->at(i).PID));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->table_history->setItem(i,0,item);
        item = new QTableWidgetItem(QString::number(cpus->historyQueue->at(i).time_consume));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->table_history->setItem(i,1,item);
        item = new QTableWidgetItem(QString::number(cpus->historyQueue->at(i).priority_ori));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->table_history->setItem(i,2,item);
        item = new QTableWidgetItem(QString::fromStdString(cpus->historyQueue->at(i).father));
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->table_history->setItem(i,3,item);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_add_clicked()
{
    QString pid;
    int time;
    int priority;
    bool add = false;
    int fatherID = 0;
    newProcessDialog* a = new newProcessDialog(&pid,&time,&priority,&add,&fatherID,*cpus->readyQueue,*cpus->jobQueue,this);
    timer->stop();
    a->exec();
    timer->start(timeSlice);
    if(add)
    {
        cpus->newProcess(pid.toStdString(),time,priority,bindFather(fatherID));
        qDebug()<<"suspend size1:"<<cpus->suspendQueue->size();
        refresh();
    }
    select = false;
}

string MainWindow::bindFather(int fatherID)
{
    if(fatherID == 0)
        return "";
    string father;
    if(fatherID <= 6)
    {
        priority_queue<PCB>* tempQ = new priority_queue<PCB>;
        for(int i = 0;i < (fatherID-1);i++)
        {
            tempQ->push(cpus->readyQueue->top());
            cpus->readyQueue->pop();
        }
        PCB tempP = cpus->readyQueue->top();
        father = tempP.PID;
        if(tempP.running_children == -1)
            tempP.running_children = 1;
        else
            tempP.running_children++;
        tempP.cancelSuspend = false;
        if(!tempP.suspend)
        {
            cpus->suspendQueue->push(tempP);
            tempP.suspend = true;
            tempP.color->setRgbF(255,0,0,0.7);
            tempP.priority = 99 - order;
            order++;
        }
        tempQ->push(tempP);
        cpus->readyQueue->pop();
        while(!cpus->readyQueue->empty())
        {
            tempQ->push(cpus->readyQueue->top());
            cpus->readyQueue->pop();
        }
        priority_queue<PCB>* a = cpus->readyQueue;
        cpus->readyQueue = tempQ;
        delete a;
    }
    return father;
}

string MainWindow::bindFather(string fPID)
{
    qDebug()<<"1";
    priority_queue<PCB>* tempQ = new priority_queue<PCB>;
    qDebug()<<"2";
    while(!cpus->readyQueue->empty())
    {
        PCB tempP = cpus->readyQueue->top();
        if(tempP.PID == fPID)
        {
            tempP.cancelSuspend = false;
            if(!tempP.suspend)
            {
                tempP.running_children = 1;
                cpus->suspendQueue->push(tempP);
                tempP.suspend = true;
                tempP.color->setRgbF(255,0,0,0.7);
                tempP.priority = 99 - order;
                order++;
                tempQ->push(tempP);
            }
            else
            {
                tempP.running_children++;
                tempQ->push(tempP);
                tempP.priority = tempP.priority_pre;
                remove(tempP.PID,suspendQ);
                cpus->suspendQueue->push(tempP);

            }
        }
        else
        {
            tempQ->push(tempP);
        }
        cpus->readyQueue->pop();
    }
    qDebug()<<"3";
    priority_queue<PCB>* a = cpus->readyQueue;
    cpus->readyQueue = tempQ;
    delete a;
    return fPID;
}

void MainWindow::on_btn_run_clicked()
{
    if(running)
    {
        timer->stop();
        ui->btn_run->setText("继续");
        running = !running;
    }
    else
    {
        timer->start(timeSlice);
        ui->btn_run->setText("暂停");
        running = !running;
        select = false;
    }
}

void MainWindow::on_btn_pre_clicked()
{
    cpus->newProcess("Steam",8,110);
    cpus->newProcess("WeChat",5,130);
    cpus->newProcess("CS:GO",9,125,bindFather("Steam"));
    cpus->newProcess("WeChatRing",3,140,bindFather("WeChat"));
    cpus->newProcess("QQ",6,130);
    cpus->newProcess("HeyBox",12,110,bindFather("Steam"));

    cpus->newProcess("PerfectWorld",12,105,bindFather("CS:GO"));
    cpus->newProcess("Edge",11,120);
    cpus->newProcess("Qt",12,140);    
    cpus->newProcess("Apex Legend",7,120,bindFather("Steam"));
    cpus->newProcess("Chrome",10,135);
    cpus->newProcess("NetEasy",5,130); 
    cpus->newProcess("Edge",7,105);
    cpus->newProcess("Parsec",10,110);
    cpus->newProcess("PotPlayer",4,135);
    cpus->newProcess("DingTalk",5,125);
    cpus->newProcess("KOOK",10,130);
    cpus->newProcess("Sekiro",11,135);
    cpus->newProcess("PVZ",4,130);
    cpus->newProcess("AliDrive",9,125);

    refresh();
    select = false;
    ui->btn_pre->setDisabled(true);
}

void MainWindow::on_table_ready_cellClicked(int row, int column)
{
    ui->btn_suspend->setText("挂起");
    select = true;
    selectType = readyQ;
    selectRow = row;
    if(running)
    {
        timer->stop();
        ui->btn_run->setText("继续");
        running = !running;
    }
}

void MainWindow::on_table_backup_cellClicked(int row, int column)
{
    return;
}

void MainWindow::on_slider_speed_sliderMoved(int value)
{
    return;
}

void MainWindow::on_table_waiting_cellClicked(int row, int column)
{
    ui->btn_suspend->setText("解挂");
    select = true;
    selectType = suspendQ;
    selectRow = row;
    if(running)
    {
        timer->stop();
        ui->btn_run->setText("继续");
        running = !running;
    }
}

void MainWindow::on_btn_suspend_clicked()
{
    if(!select)
        return;
    if(selectType == readyQ)
    {
        priority_queue<PCB>* tempQ = new priority_queue<PCB>;
        if(selectRow + 1 > (int)cpus->readyQueue->size() && !running)
        {
            timer->start(timeSlice);
            ui->btn_run->setText("暂停");
            running = !running;
            return;
        }
        for(int i = 0;i < selectRow;i++)
        {
            tempQ->push(cpus->readyQueue->top());
            cpus->readyQueue->pop();
        }
        PCB tempP = cpus->readyQueue->top();
        if(!tempP.suspend)
        {
            cpus->suspendQueue->push(tempP);
            tempP.priority = 99 - order;
            order++;
            tempP.suspend = true;
            tempP.color->setRgbF(255,0,0,0.6);
            tempQ->push(tempP);
            cpus->readyQueue->pop();
        }
        while(!cpus->readyQueue->empty())
        {
            tempQ->push(cpus->readyQueue->top());
            cpus->readyQueue->pop();
        }
        priority_queue<PCB>* a = cpus->readyQueue;
        cpus->readyQueue = tempQ;
        delete a;
        refresh();
    }

    else if(selectType == suspendQ)
    {
        priority_queue<PCB>* temp = new priority_queue<PCB>;
        if(selectRow + 1 > (int)cpus->suspendQueue->size() && !running)
        {
            timer->start(timeSlice);
            ui->btn_run->setText("暂停");
            running = !running;
            return;
        }
        for(int i = 0;i < selectRow;i++)
        {
            PCB t0 = cpus->suspendQueue->top();
            temp->push(t0);
            cpus->suspendQueue->pop();
        }
        PCB t1 = cpus->suspendQueue->top();
        if(!t1.cancelSuspend)//--------------------------------------
        {
            QMessageBox::warning(this,"不可解挂","有子进程运行中，不可解挂");
            return;
        }
        //cpus->newProcess(&t1);
        remove(t1.PID,readyQ);
        t1.color->setRgbF(0,0,0,0);
        cpus->readyQueue->push(t1);
        cpus->suspendQueue->pop();        
        while(!cpus->suspendQueue->empty())
        {
            PCB t2 = cpus->suspendQueue->top();
            temp->push(t2);
            cpus->suspendQueue->pop();
        }
        priority_queue<PCB>* a = cpus->suspendQueue;
        cpus->suspendQueue = temp;
        delete a;

        refresh();
    }

    timer->start(timeSlice);
    ui->btn_run->setText("暂停");
    running = !running;
    select = false;
    return;
}


void MainWindow::remove(string PID,QType type)
{
    qDebug()<<"removing...";
    if(type == readyQ)
    {
        priority_queue<PCB>* tempQ = new priority_queue<PCB>;
        while(!cpus->readyQueue->empty())
        {
            PCB tempP = cpus->readyQueue->top();
            if(tempP.PID != PID)
                tempQ->push(tempP);
            cpus->readyQueue->pop();
        }
        priority_queue<PCB>* a = cpus->readyQueue;
        cpus->readyQueue = tempQ;
        delete a;
    }
    else if(type == suspendQ)
    {
        priority_queue<PCB>* tempQ = new priority_queue<PCB>;
        while(!cpus->suspendQueue->empty())
        {
            PCB tempP = cpus->suspendQueue->top();
            if(tempP.PID != PID)
                tempQ->push(tempP);
            cpus->suspendQueue->pop();
        }
        priority_queue<PCB>* a = cpus->suspendQueue;
        cpus->suspendQueue = tempQ;
        delete a;
    }

    qDebug()<<"removed!";
}


void MainWindow::on_slider_speed_valueChanged(int value)
{
    timer->stop();
    timeSlice = 3000 - value;
    ui->lcd_timeSlice->display(timeSlice);
    if(running)
        timer->start(timeSlice);
}
