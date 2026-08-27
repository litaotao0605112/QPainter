#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QRandomGenerator>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);  // 创建定时器对象

    m_LiqNum_A = 0;
    ui->widget->Set_TextChangeEvent(0,QString::number(m_LiqNum_A));

    m_LiqNum_B = 0;
    ui->widget->Set_TextChangeEvent(1,QString::number(m_LiqNum_B));

    // A罐
    m_LiqA_Timer = new QTimer();
    connect(m_LiqA_Timer,SIGNAL(timeout()),this,SLOT(SlotLiq_A()));

    // B罐
    m_LiqB_Timer= new QTimer();
    connect(m_LiqB_Timer,SIGNAL(timeout()),this,SLOT(SlotLiq_B()));

    // 随机数
    m_RandomDataTimer = new QTimer();
    connect(m_RandomDataTimer,SIGNAL(timeout()),this,SLOT(SlotRandomData()));

    // 指示灯
    m_LightFlag = false;

    m_LightTimer = new QTimer();
    connect(m_LightTimer,SIGNAL(timeout()),this,SLOT(SlotRandomLight()));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_9_clicked()
{
    // 开启水流特效
    ui->widget->Set_DisplayEvent_IsShow(0,1);
    ui->widget->Set_DisplayEvent_IsShow(3,1);
    ui->widget->Set_DisplayEvent_IsShow(2,1);
    ui->widget->Set_DisplayEvent_IsShow(1,1);

    // 启动定时器
    m_LiqA_Timer->start(500);
    m_RandomDataTimer->start(1000);
    m_LightTimer->start(500);
}

void MainWindow::SlotLiq_A()
{
    m_LiqNum_A +=2;
    if(m_LiqNum_A <= 100)
    {
        ui->widget->Set_TextChangeEvent(0,QString::number(m_LiqNum_A));
    }
    else
    {
        qDebug()<<"m_LiqNum_A==="<<m_LiqNum_A;
        m_LiqA_Timer->stop();
        // 马上启动B液体操作
        ui->widget->Set_DisplayEvent_IsShow(4,1);
        ui->widget->Set_DisplayEvent_IsShow(5,1);
        ui->widget->Set_DisplayEvent_IsShow(6,1);
        m_LiqB_Timer->start(500);
    }
    ui->widget->update();

}

void MainWindow::SlotLiq_B()
{
    m_LiqNum_B +=2;
    if(m_LiqNum_B <= 100)
    {
        ui->widget->Set_TextChangeEvent(1,QString::number(m_LiqNum_B));
    }
    else
    {
        qDebug()<<"m_LiqNum_B==="<<m_LiqNum_B;
        m_LiqB_Timer->stop();
        ui->widget->Set_DisplayEvent_IsShow(7,1);
        ui->widget->Set_DisplayEvent_IsShow(8,1);
        ui->widget->Set_DisplayEvent_IsShow(9,1);
    }
    ui->widget->update();
}


// 放置随机数
void MainWindow::SlotRandomData()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 100);
    int int_part = dist(gen);
    int random = int_part + 5;

    double random_num = int_part / 100.0;

    ui->widget->Set_TextChangeEvent(2,QString::number(int_part));
    ui->widget->Set_TextChangeEvent(3,QString::number(random_num,10,2));
    ui->widget->Set_TextChangeEvent(4,QString::number(random));
}

void MainWindow::SlotRandomLight()
{
    if(m_LightFlag)
    {
        ui->widget->Set_DisplayEvent_IsShow(10,0);
        ui->widget->Set_DisplayEvent_IsShow(11,0);
        m_LightFlag = false;
    }
    else
    {
        ui->widget->Set_DisplayEvent_IsShow(10,1);
        ui->widget->Set_DisplayEvent_IsShow(11,1);
        m_LightFlag = true;
    }
}





