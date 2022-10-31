#include "newprocessdialog.h"
#include "ui_newprocessdialog.h"

newProcessDialog::newProcessDialog(QString* pid,int* time,int* priority,bool* add,int* fatherID,priority_queue<PCB> readyQ,queue<PCB> jobQ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newProcessDialog)
{
    ui->setupUi(this);
    this->pid = pid;
    this->time = time;
    this->priority = priority;
    this->add = add;
    this->fatherID = fatherID;
    while(!readyQ.empty())
    {
        allP->push(readyQ.top());
        readyQ.pop();
    }
    while(!jobQ.empty())
    {
        allP->push(jobQ.front());
        jobQ.pop();
    }
    int i = 0;
    while(!allP->empty())
    {
        if(i < 6)
            ui->com_father->addItem(QString::fromStdString(allP->front().PID));
        allPID.push_back(allP->front().PID);
        allP->pop();
        i++;
    }
}

newProcessDialog::~newProcessDialog()
{
    delete ui;
}

void newProcessDialog::on_btn_confirm_clicked()
{
    QString pidt = ui->edit_pid->text();
    int l = pidt.length();
    if(l == 0)
    {
        QMessageBox::warning(this,"重新输入PID","PID不可为空");
        return;
    }
    else if(l >= 64)
    {
        QMessageBox::warning(this,"重新输入PID","PID长度超出限制");
        ui->edit_pid->clear();
        return;
    }
    if(pidt == "无")
    {
        QMessageBox::warning(this,"重新输入PID","PID不可为\"无\"");
        ui->edit_pid->clear();
        return;
    }
    else if(repeatPID(pidt.toStdString()))
    {
        QMessageBox::warning(this,"重新输入PID","PID不可重复");
        ui->edit_pid->clear();
        return;
    }
    *pid = pidt;
    *priority = ui->spin_priority->value();
    *time = ui->spin_time->value();
    *add = true;
    *fatherID = ui->com_father->currentIndex();
    this->done(0);
}

bool newProcessDialog::repeatPID(string s)
{
    for(int i = 0;i < allPID.size();i++)
    {
        if(s == allPID.at(i))
            return true;
    }
    return false;
}
void newProcessDialog::on_btn_cancel_clicked()
{
    this->done(1);
}
