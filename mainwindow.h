#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <cpuscheduler.h>
#include <newprocessdialog.h>
#include <QTimer>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QDebug>

enum QType{readyQ,jobQ,suspendQ};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void refresh();

    int timeSlice = 1000;
    bool running = false;
    bool select = false;
    QType selectType;
    int selectRow = 0;

    QTimer* timer = new QTimer;
    CPU* cpu = new CPU;
    CPUScheduler* cpus = new CPUScheduler;

    string bindFather(int fatherID);
    string bindFather(string fPID);
    void remove(string PID,QType type);

private slots:
    void on_btn_add_clicked();

    void on_btn_run_clicked();

    void on_btn_pre_clicked();

    void on_table_ready_cellClicked(int row, int column);

    void on_btn_suspend_clicked();

    void on_table_backup_cellClicked(int row, int column);

    void on_table_waiting_cellClicked(int row, int column);

    void on_slider_speed_valueChanged(int value);

    void on_slider_speed_sliderMoved(int position);

private:
    Ui::MainWindow *ui;

    int order = 0;
};

#endif // MAINWINDOW_H
