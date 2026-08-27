#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QRandomGenerator>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->Set_DisplayEvent_IsShow(14,1);
    ui->widget->Set_DisplayEvent_IsShow(15,1);
    ui->widget->Set_DisplayEvent_IsShow(16,1);
    ui->widget->Set_DisplayEvent_IsShow(17,1);

    m_ABTimer = new QTimer();
    connect(m_ABTimer,SIGNAL(timeout()),this,SLOT(SlotTimeOut()));

    m_AB_CurrentNum = 100;
    m_C_CurrentNum = 0;
    m_D_CurrentNum = 0;

    // 警示灯
    m_ABLightTimer = new QTimer();
    connect(m_ABLightTimer,SIGNAL(timeout()),this,SLOT(SlotLightTimeOut()));
    m_ABLightFlag = true;

    m_CTimer = new QTimer();
    connect(m_CTimer,SIGNAL(timeout()),this,SLOT(Slot_C_TimeOut()));

    m_CLightTimer =  new QTimer();
    connect(m_CLightTimer,SIGNAL(timeout()),this,SLOT(Slot_C_Light()));
    m_CLightFlag = true;

    m_DTimer = new QTimer();
    connect(m_DTimer,SIGNAL(timeout()),this,SLOT(Slot_D_TimeOut()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 非阻塞等待指定毫秒数（这里固定30000ms=30s）
void WaitSeconds(int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, &QEventLoop::quit);
    loop.exec();
}

void MainWindow::on_pushButton_9_clicked()
{
    // 化学试剂A罐
    m_ABTimer->start(600);

    // 开启水流特效
    ui->widget->Set_DisplayEvent_IsShow(0,1);
    ui->widget->Set_DisplayEvent_IsShow(1,1);
    ui->widget->Set_DisplayEvent_IsShow(2,1);
    ui->widget->Set_DisplayEvent_IsShow(6,1);
    ui->widget->Set_DisplayEvent_IsShow(7,1);

    // 启动A罐、B罐警示灯闪烁效果
    m_ABLightTimer->start(500);

    WaitSeconds(5000);

     // 阀门开启之后水流效果
    ui->widget->Set_DisplayEvent_IsShow(3,1);
    ui->widget->Set_DisplayEvent_IsShow(4,1);

    ui->widget->Set_DisplayEvent_IsShow(8,1);
    ui->widget->Set_DisplayEvent_IsShow(9,1);

    // 试剂C罐容器上升效果
    m_CTimer->start(500);
}

void MainWindow::SlotTimeOut()
{
    // qDebug()<<"m_AB_CurrentNum=="<<m_AB_CurrentNum;
    m_AB_CurrentNum -=1;
    if(m_AB_CurrentNum <= 0)
    {
        m_ABTimer->stop();
    }
    else
    {
        ui->widget->Set_TextChangeEvent(0,QString::number(m_AB_CurrentNum));
        ui->widget->Set_TextChangeEvent(1,QString::number(m_AB_CurrentNum));
    }
    ui->widget->update();
}

void MainWindow::SlotLightTimeOut()
{
    if(m_ABLightFlag)
    {
        ui->widget->Set_DisplayEvent_IsShow(14,1);
        ui->widget->Set_DisplayEvent_IsShow(15,1);
        ui->widget->Set_DisplayEvent_IsShow(16,1);
        m_ABLightFlag = false;
    }
    else
    {
        ui->widget->Set_DisplayEvent_IsShow(14,0);
        ui->widget->Set_DisplayEvent_IsShow(15,0);
        ui->widget->Set_DisplayEvent_IsShow(16,0);
        m_ABLightFlag = true;
    }

}

void MainWindow::Slot_C_TimeOut()
{
    m_C_CurrentNum += 1;

    if(m_C_CurrentNum >=100)
    {
         m_CTimer->stop();

         // 开启C罐警告灯效果
         //qDebug()<<"m_CLightTimer=======";
         m_CLightTimer->start(500);
         ui->widget->Set_DisplayEvent_IsShow(10,1);
         // 等待水阀开门2s
         WaitSeconds(2000);
         ui->widget->Set_DisplayEvent_IsShow(11,1);
         ui->widget->Set_DisplayEvent_IsShow(12,1);
         ui->widget->Set_DisplayEvent_IsShow(13,1);
         // 试剂罐D开始上升效果
         m_DTimer->start(500);
    }
    else
    {
        ui->widget->Set_TextChangeEvent(2,QString::number(m_C_CurrentNum));
    }
    ui->widget->update();
}

void MainWindow::Slot_C_Light()
{
    if(m_CLightFlag)
    {
        ui->widget->Set_DisplayEvent_IsShow(17,1);
        ui->widget->Set_DisplayEvent_IsShow(18,1);
        m_CLightFlag = false;
    }
    else
    {
        ui->widget->Set_DisplayEvent_IsShow(17,0);
        ui->widget->Set_DisplayEvent_IsShow(18,0);
        m_CLightFlag = true;
    }
}

void MainWindow::Slot_D_TimeOut()
{
    m_D_CurrentNum +=1;

    if(m_D_CurrentNum >=100)
    {
        m_DTimer->stop();
        m_CLightTimer->stop();
        ui->widget->Set_DisplayEvent_IsShow(10,0);
        ui->widget->Set_DisplayEvent_IsShow(11,0);
        ui->widget->Set_DisplayEvent_IsShow(12,0);
        ui->widget->Set_DisplayEvent_IsShow(13,0);
    }

    ui->widget->Set_TextChangeEvent(3,QString::number(m_D_CurrentNum));
    QString percentage = QString("%1%").arg(QString::number(m_D_CurrentNum));
    ui->widget->Set_TextChangeEvent(4,percentage);
    // C罐下降感
    int ret = 100-m_D_CurrentNum;
    ui->widget->Set_TextChangeEvent(2,QString::number(ret));
    // 取消AB罐的流水特效
    for(int i = 0; i <= 9; i++)
    {
        ui->widget->Set_DisplayEvent_IsShow(i,0);
    }
    m_ABLightTimer->stop();
    ui->widget->update();
}




