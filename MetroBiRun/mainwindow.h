#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEventLoop>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    // 控制定时闪烁过程中的单步切换。
    void HandleTimerTimeout(int displayId);
private slots:


    void on_pushButton_9_clicked();

    void on_pushButton_clicked();

    void SlotTimeOut();

private:
    Ui::MainWindow *ui;
    // 站点闪烁动画共用的定时器。
    QTimer *m_Timer;

    // 当前闪烁次数。
    int m_Count;

    // 当前正在处理的站点图元编号。
    int m_CurrentID;

    // 控制本轮闪烁显示/隐藏的翻转状态。
    bool m_StatusFlag;

    // 防止重复点击按钮导致流程重入。
    bool m_isRunning;

    // 用于把顺序流程拆成若干个“闪烁后继续”的步骤。
    QEventLoop *m_EventLoop;

};
#endif // MAINWINDOW_H
