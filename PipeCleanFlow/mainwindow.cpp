#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QRandomGenerator>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

int display[4] = {14,15,16,17};
QString state[3]  = {"停止","清洗中..","保修中.."};
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);  // 创建定时器对象

    static int count = 0;
    static bool flag = 0;
    static int colorflag = 0;
    connect(timer,&QTimer::timeout, [=]() {
        for(int i = 0; i<4; i++)
        {
            ui->widget->Set_DisplayEvent_IsShow(display[i],flag);
        }
        flag = !flag;
        int randomNumber = QRandomGenerator::global()->bounded(100); // 生成一个0到99之间的随机数（包括0和99）
        ui->widget->Set_TextChangeEvent(count,QString::number(randomNumber));
        count++;
        if(count > 3)
        {
          count = 0;
        }
        //传感器控制
        ui->widget->Set_TextChangeEvent(count + 4,QString::number(randomNumber));
        int stateRandomNumber = QRandomGenerator::global()->bounded(3); // 生成一个0到99之间的随机数（包括0和99）
        ui->widget->Set_TextChangeEvent(8,state[stateRandomNumber]);
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_9_clicked()
{
    timer->start(500);
    ui->widget->timer->start(50);
}





