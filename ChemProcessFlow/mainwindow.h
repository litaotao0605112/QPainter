#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:


    void on_pushButton_9_clicked();


    void SlotLiq_A();

    void SlotLiq_B();

    void SlotRandomData();

    void SlotRandomLight();

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    int m_LiqNum_A;

    int m_LiqNum_B;

    QTimer *m_LiqA_Timer;

    QTimer *m_LiqB_Timer;

    QTimer *m_RandomDataTimer;

    QTimer *m_LightTimer;

    bool  m_LightFlag;

};
#endif // MAINWINDOW_H
