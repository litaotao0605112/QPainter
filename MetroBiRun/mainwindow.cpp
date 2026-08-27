#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QRandomGenerator>
#include <QDateTime>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

// 预留的图元编号映射和状态文本。
int display[4] = {14,15,16,17};
QString state[3]  = {"停止","运行中..","保修中.."};

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 主窗口定时器只负责站点闪烁，不负责线路流动动画。
    m_Timer = new QTimer();
    connect(m_Timer,SIGNAL(timeout()),this,SLOT(SlotTimeOut()));
    m_StatusFlag = false;
    m_Count = 0;
    m_CurrentID = 0;
    m_isRunning = false;  // 是否正在闪烁（防重入）
    m_EventLoop  = nullptr; // 当前事件循环指针（用于退出）
    ui->widget->Set_TextChangeEvent(3,"停止中");

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotTimeOut()
{
    // 将真正的闪烁处理集中到独立函数中，便于正反向流程复用。
    HandleTimerTimeout(m_CurrentID);
}

void MainWindow::HandleTimerTimeout(int displayId)
{
    if(m_Count >= 12)
    {
       m_Count = 0;
       m_Timer->stop();
       m_isRunning = false;
       // 如果有事件循环，则退出
       if (m_EventLoop)
       {
           m_EventLoop->quit();
           m_EventLoop = nullptr;
       }
       return;
    }
    // 执行一次切换：根据 m_StatusFlag 决定显示或隐藏，然后翻转状态
    ui->widget->Set_DisplayEvent_IsShow(m_CurrentID, m_StatusFlag ? 1 : 0);
    m_StatusFlag = !m_StatusFlag;
    m_Count++;
}

// 通过局部事件循环等待一段时间，让界面在等待期间仍可刷新。
void WaitSeconds(int msec)
{
    QEventLoop loop;
    QTimer::singleShot(msec, &loop, &QEventLoop::quit);
    loop.exec();
}

// 往国际机场
void MainWindow::on_pushButton_9_clicked()
{
    if (m_isRunning)
    {
        return;  // 正在执行，忽略重复点击
    }
    ui->widget->directionFlag = false;
    // 正向运行时点亮左侧方向箭头。
    ui->widget->Set_DisplayEvent_IsShow(13,1);
    ui->widget->Set_DisplayEvent_IsShow(14,1);

    // ----------知春路 ----------
    m_CurrentID = 6;
    m_Count = 0;
    m_StatusFlag = false;    // 从隐藏开始，第一次超时显示
    m_isRunning = true;

    // 创建局部事件循环，并将指针传给成员变量，供超时槽退出
    QEventLoop loop;
    m_EventLoop = &loop;

    m_Timer->start(500);     // 启动定时器
    loop.exec();             // 阻塞直到 loop.quit() 被调用

    // 第一站闪烁结束后，点亮对应线路段并更新状态文本。
    ui->widget->Set_DisplayEvent_IsShow(0, 1);

    ui->widget->Set_TextChangeEvent(3,"运行中");

    WaitSeconds(3000);

    // ---------- 人民广场 ----------
    ui->widget->Set_DisplayEvent_IsShow(0, 0);
    ui->widget->Set_TextChangeEvent(3,"停止中");
    ui->widget->Set_TextChangeEvent(0,"中央公园");
    m_CurrentID = 7;
    m_Count = 0;
    m_StatusFlag = false;
    m_isRunning = true;

    QEventLoop loop2;
    m_EventLoop = &loop2;

    m_Timer->start(500);
    loop2.exec();
    //
    ui->widget->Set_DisplayEvent_IsShow(1, 1);

    ui->widget->Set_TextChangeEvent(3,"运行中");

    WaitSeconds(3000);

    // ---------- 中央公园 ----------
    ui->widget->Set_DisplayEvent_IsShow(1, 0);
    ui->widget->Set_TextChangeEvent(3,"停止中");
    ui->widget->Set_TextChangeEvent(0,"金融中心");
    m_CurrentID = 8;
    m_Count = 0;
    m_StatusFlag = false;
    m_isRunning = true;

    QEventLoop loop3;
    m_EventLoop = &loop3;

    m_Timer->start(500);
    loop3.exec();

    //
    ui->widget->Set_DisplayEvent_IsShow(2, 1);

    ui->widget->Set_TextChangeEvent(3,"运行中");

    WaitSeconds(3000);

    // ---------- 金融中心 ----------
    ui->widget->Set_DisplayEvent_IsShow(2, 0);
    ui->widget->Set_TextChangeEvent(3,"停止中");
    ui->widget->Set_TextChangeEvent(0,"大学城");
    m_CurrentID = 9;
    m_Count = 0;
    m_StatusFlag = false;
    m_isRunning = true;

    QEventLoop loop4;
    m_EventLoop = &loop4;

    m_Timer->start(500);
    loop4.exec();

    //
    ui->widget->Set_DisplayEvent_IsShow(3, 1);

    ui->widget->Set_TextChangeEvent(3,"运行中");

    WaitSeconds(3000);

    // ---------- 大学城 ----------
    ui->widget->Set_DisplayEvent_IsShow(3, 0);
    ui->widget->Set_TextChangeEvent(3,"停止中");
    ui->widget->Set_TextChangeEvent(0,"体育中心");
    m_CurrentID = 10;
    m_Count = 0;
    m_StatusFlag = false;
    m_isRunning = true;

    QEventLoop loop5;
    m_EventLoop = &loop5;

    m_Timer->start(500);
    loop5.exec();

    //
    ui->widget->Set_DisplayEvent_IsShow(4, 1);

    ui->widget->Set_TextChangeEvent(3,"运行中");

    WaitSeconds(3000);

    // ---------- 体育中心 ----------
    ui->widget->Set_DisplayEvent_IsShow(4, 0);
    ui->widget->Set_TextChangeEvent(3,"停止中");
    ui->widget->Set_TextChangeEvent(0,"国际机场");
    m_CurrentID = 11;
    m_Count = 0;
    m_StatusFlag = false;
    m_isRunning = true;

    QEventLoop loop6;
    m_EventLoop = &loop6;

    m_Timer->start(500);
    loop6.exec();

    //
    ui->widget->Set_DisplayEvent_IsShow(5, 1);

    ui->widget->Set_TextChangeEvent(3,"运行中");

    WaitSeconds(3000);

    // ---------- 国际机场 ----------
    ui->widget->Set_DisplayEvent_IsShow(5, 0);
    ui->widget->Set_TextChangeEvent(3,"停止中");
    ui->widget->Set_TextChangeEvent(0,"已到终点站");
    m_CurrentID = 12;
    m_Count = 0;
    m_StatusFlag = false;
    m_isRunning = true;

    QEventLoop loop7;
    m_EventLoop = &loop7;

    m_Timer->start(500);
    loop7.exec();

    // 到达终点后关闭正向方向箭头。
    ui->widget->Set_DisplayEvent_IsShow(13,0);
    ui->widget->Set_DisplayEvent_IsShow(14,0);
}

// 往知春路
void MainWindow::on_pushButton_clicked()
{
    if (m_isRunning)
    {
         return;  // 正在执行，忽略重复点击
    }

     // 切换到反向运行前，先恢复所有站点默认状态。
    for(int i = 6;i<12;i++)
    {
       ui->widget->Set_DisplayEvent_IsShow(i, 0);
    }

    ui->widget->directionFlag = true;

    // 反向运行时点亮另一组方向箭头。
    ui->widget->Set_DisplayEvent_IsShow(15,1);
    ui->widget->Set_DisplayEvent_IsShow(16,1);

    ui->widget->Set_TextChangeEvent(1,"开往知春路");

    // ---------- 国际机场 ----------
    ui->widget->Set_TextChangeEvent(3,"停止中");
    ui->widget->Set_TextChangeEvent(0,"体育中心");
    m_CurrentID = 12;
    m_Count = 0;
    m_StatusFlag = false;
    m_isRunning = true;

    QEventLoop loop1;
    m_EventLoop = &loop1;

    m_Timer->start(500);
    loop1.exec();

    //
    ui->widget->Set_DisplayEvent_IsShow(5, 1);

    ui->widget->Set_TextChangeEvent(3,"运行中");

    WaitSeconds(3000);

    // ---------- 体育中心 ----------
    ui->widget->Set_DisplayEvent_IsShow(5, 0);
    ui->widget->Set_TextChangeEvent(3,"停止中");
    ui->widget->Set_TextChangeEvent(0,"大学城");
    m_CurrentID = 11;
    m_Count = 0;
    m_StatusFlag = false;
    m_isRunning = true;

    QEventLoop loop2;
    m_EventLoop = &loop2;

    m_Timer->start(500);
    loop2.exec();

    //
    ui->widget->Set_DisplayEvent_IsShow(4, 1);

    ui->widget->Set_TextChangeEvent(3,"运行中");

    WaitSeconds(3000);

    // ---------- 大学城 ----------
    ui->widget->Set_DisplayEvent_IsShow(4, 0);
    ui->widget->Set_TextChangeEvent(3,"停止中");
    ui->widget->Set_TextChangeEvent(0,"金融中心");
    m_CurrentID = 10;
    m_Count = 0;
    m_StatusFlag = false;
    m_isRunning = true;

    QEventLoop loop3;
    m_EventLoop = &loop3;

    m_Timer->start(500);
    loop3.exec();

    //
    ui->widget->Set_DisplayEvent_IsShow(3, 1);

    ui->widget->Set_TextChangeEvent(3,"运行中");

    WaitSeconds(3000);

    // ---------- 金融中心 ----------
    ui->widget->Set_DisplayEvent_IsShow(3, 0);
    ui->widget->Set_TextChangeEvent(3,"停止中");
    ui->widget->Set_TextChangeEvent(0,"中央公园");
    m_CurrentID = 9;
    m_Count = 0;
    m_StatusFlag = false;
    m_isRunning = true;

    QEventLoop loop4;
    m_EventLoop = &loop4;

    m_Timer->start(500);
    loop4.exec();

    //
    ui->widget->Set_DisplayEvent_IsShow(2, 1);

    ui->widget->Set_TextChangeEvent(3,"运行中");

    WaitSeconds(3000);

    // ---------- 中央公园 ----------
    ui->widget->Set_DisplayEvent_IsShow(2, 0);
    ui->widget->Set_TextChangeEvent(3,"停止中");
    ui->widget->Set_TextChangeEvent(0,"人民广场");
    m_CurrentID = 8;
    m_Count = 0;
    m_StatusFlag = false;
    m_isRunning = true;

    QEventLoop loop5;
    m_EventLoop = &loop5;

    m_Timer->start(500);
    loop5.exec();

    //
    ui->widget->Set_DisplayEvent_IsShow(1, 1);

    ui->widget->Set_TextChangeEvent(3,"运行中");

    WaitSeconds(3000);

    // ---------- 人民广场 ----------
    ui->widget->Set_DisplayEvent_IsShow(1, 0);
    ui->widget->Set_TextChangeEvent(3,"停止中");
    ui->widget->Set_TextChangeEvent(0,"知春路");
    m_CurrentID = 7;
    m_Count = 0;
    m_StatusFlag = false;
    m_isRunning = true;

    QEventLoop loop6;
    m_EventLoop = &loop6;

    m_Timer->start(500);
    loop6.exec();

    //
    ui->widget->Set_DisplayEvent_IsShow(0, 1);

    ui->widget->Set_TextChangeEvent(3,"运行中");

    WaitSeconds(3000);

    // ----------知春路 ----------
    ui->widget->Set_DisplayEvent_IsShow(0, 0);
    ui->widget->Set_TextChangeEvent(3,"停止中");
    ui->widget->Set_TextChangeEvent(0,"已到终点站");
    m_CurrentID = 6;
    m_Count = 0;
    m_StatusFlag = false;
    m_isRunning = true;

    QEventLoop loop;
    m_EventLoop = &loop;

    m_Timer->start(500);     // 启动定时器
    loop.exec();

    // 当前实现末尾保持反向箭头常亮，表示列车已停靠在终点侧。
    ui->widget->Set_DisplayEvent_IsShow(15,1);
    ui->widget->Set_DisplayEvent_IsShow(16,1);
}
