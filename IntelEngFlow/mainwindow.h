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


    void SlotTimeOut();

    void SlotLightTimeOut();

    void Slot_C_TimeOut();

    void Slot_C_Light();

    void Slot_D_TimeOut();

private:
    Ui::MainWindow *ui;
    QTimer* m_ABTimer;

    QTimer* m_ABLightTimer;

    QTimer* m_CTimer;

    QTimer* m_DTimer;

    QTimer* m_CLightTimer;

    int m_AB_CurrentNum;

    int m_C_CurrentNum;

    int m_D_CurrentNum;

    bool m_ABLightFlag;

    bool m_CLightFlag;

};
#endif // MAINWINDOW_H
