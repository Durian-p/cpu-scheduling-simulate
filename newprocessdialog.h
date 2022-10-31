#ifndef NEWPROCESSDIALOG_H
#define NEWPROCESSDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <cpuscheduler.h>

namespace Ui {
class newProcessDialog;
}

class newProcessDialog : public QDialog
{
    Q_OBJECT

public:
    explicit newProcessDialog(QString* pid,int* time,int* priority,bool* add,int* fatherID,priority_queue<PCB> readyQ,queue<PCB> jobQ,QWidget *parent = nullptr);
    ~newProcessDialog();
    QString* pid;
    int* time;
    int* priority;
    bool* add;
    int* fatherID;
    queue<PCB>* allP = new queue<PCB>;
    vector<string> allPID;

    bool repeatPID(string s);

private slots:
    void on_btn_confirm_clicked();

    void on_btn_cancel_clicked();

private:
    Ui::newProcessDialog *ui;
};

#endif // NEWPROCESSDIALOG_H
