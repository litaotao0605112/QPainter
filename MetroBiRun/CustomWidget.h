
#ifndef SAMPLERACKWIDGET_H
#define SAMPLERACKWIDGET_H
#include <QWidget>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QVector>
#include <QDebug>
#include <QMouseEvent>

#define M_PI 3.14159
// 流动线段的运行参数。
typedef struct FLOWINFO
{
    // 虚线偏移量，定时器中持续修改以形成流动效果。
    double offset = 0;
    // 预留的双色箭头颜色参数。
    QColor arrowColor1;
    QColor arrowColor2;
    // 是否启用多色显示。
    bool isMultiColor = false;
    // 虚线间隔与移动速度。
    double spacing;
    double speed = 0.01;
    // 预留的箭头数量、方向和类型参数。
    int arrowCount = 10;
    bool directionFlag = false;
    int arrowType;
}FlowInfo;



class  CustomWidget : public QWidget
{
   Q_OBJECT
   public:
    explicit CustomWidget(QWidget *parent = nullptr);

    // 驱动线路流动动画的定时器。
    QTimer *timer;
private:
    // 负责绘制整张线路图和状态元素。
    void paintEvent(QPaintEvent *event) override;

public:
    // 根据窗口大小重新计算缩放比例。
    void resizeEvent(QResizeEvent *event);
    // 控制某个图元是否点亮显示。
    void Set_DisplayEvent_IsShow(int id,bool state);
    // 更新界面上的动态文本。
    void Set_TextChangeEvent(int id,QString data);


    // 保存各个图元的选中/显示状态。
    QVector<bool> isSelect;
    // 保存站名、方向、倒计时、运行状态等文本。
    QVector<QString> text;
    double m_offset = 0;
    // 每段流动线对应一组动画参数。
    QMap<int,FlowInfo>mapFlowInfo;
    // 当前控件缩放比例。
    double m_scale;
    // 地铁线路背景图。
    QImage background;

    int  colorChange = 255;

    // true 表示反向运行，false 表示正向运行。
    bool  directionFlag = false;
};
#endif // SAMPLERACKWIDGET_H



