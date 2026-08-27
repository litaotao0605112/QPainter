
#include "CustomWidget.h"
#include <math.h>
#include <QTimer>
#include <QTime>
#include <QtGlobal>
#include <QRandomGenerator>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

void rotateAndPaintRect(QPainter *painter, const QRectF &rect, int angle,bool isRound = false,int rx = 0,int ry = 0)
{
    qreal cx = 0;
    qreal cy = 0;

    // 以矩形中心作为旋转中心。
    cx = rect.center().x();
    cy = rect.center().y();

    painter->save();
    /*
        平移坐标系
        1. 默认情况下，画图原点是控件左上角 (0, 0)
        2. 执行这句后，画图的原点被移动到了 (cx, cy)。
    */
    painter->translate(cx ,cy);
    // 把当前坐标系旋转 angle 度
    // 注意，它旋转的不是已经画出的图形，而是“后续绘图所使用的坐标轴”。
    painter->rotate(angle);

    // 将原始左上角转换到以中心为原点的局部坐标中，便于旋转后按原尺寸绘制。
    QPointF drawOffset = rect.topLeft() - QPointF(cx,cy);

    painter->drawRect(drawOffset.x(), drawOffset.y(), rect.width(), rect.height());

    painter->restore();
}

void DrawText(QPainter &painter,const QRectF &rect,QString str,double angle,int size,QString fontFamily,int textAlignment)
{
    QRectF rotatedRect(-rect.width()/2, -rect.height()/2, rect.width(), rect.height());
    qreal cx = rect.x() + rect.width() / 2;
    qreal cy = rect.y() + rect.height() / 2;

    QFont font = QFont(fontFamily, size);
    painter.setFont(font);

    painter.save();


    painter.translate(cx, cy);
    painter.scale(1, -1);
    painter.rotate(-angle);//旋转角度

    painter.drawText(rotatedRect,textAlignment,str);

    //恢复上面保存的状态
    painter.restore();

}

void DrawImage(QPainter &painter, QImage &image, const QRectF &rect,double angle)
{
    QRectF rotatedRect(-rect.width()/2, -rect.height()/2, rect.width(), rect.height());
    painter.drawImage(rotatedRect, image);
}

// 绘制流动线段
void drawFlowLine(QPainter *painter, QPolygonF points,
                  QColor backgroundColor, int bgLineWidth,
                  QColor lineColor, int lineWidth,
                  const FlowInfo &flowInfo)
{

    QPen pen(backgroundColor);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(bgLineWidth);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
    painter->drawPolyline(points);

    // 设置虚线模式: "画3个单位"-->"空2个单位"-->"画3个单位"-->"空2个单位" 实际效果: 短线段-->空隙-->短线段-->空隙
    QPen pen1(lineColor);
    QVector<qreal> dashes;
    qreal space = 2;
    dashes << 3 << space << 3 << space;
    pen1.setDashPattern(dashes);
    pen1.setWidth(lineWidth);
    pen1.setJoinStyle(Qt::RoundJoin);
    pen1.setCapStyle(Qt::RoundCap);
    pen1.setDashOffset(flowInfo.offset);
    painter->setPen(pen1);
    painter->drawPolyline(points);
}

void DrawPolygon(QPainter &painter,int pathsID, QVector<QPointF> points, const QTransform &trans,double angle)
{
    // 直接从初始化列表构造 QVector 或 QPolygonF
    QPolygonF polygon(points);
    // 1. 计算旋转中心（通常为多边形的几何中心）
    painter.drawPolygon(polygon);

}

CustomWidget::CustomWidget(QWidget *parent) : QWidget(parent)
{
    //qsrand(QTime::currentTime().msec());
    background = QImage(":/image/1.png").mirrored(false, true);

    isSelect.resize(33);
    text.resize(12);

    text[0] = "50";
    text[1] = "50";
    text[2] = "50";
    text[3] = "50";
    text[4] = "50";
    text[5] = "0";
    text[6] = "0";
    text[7] = "0";
    text[8] = "0";
    text[9] = "0";
    text[10] = "停止";
    text[11] = "2026-08-11 ";

    mapFlowInfo.insert(0,{0,QColor(255, 0, 0,200),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(1,{0,QColor(255, 0, 0,200),QColor(160, 160, 164,255),0,2,0.5,0,0,0});

    mapFlowInfo.insert(2,{0,QColor(0, 255, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(3,{0,QColor(0, 255, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(4,{0,QColor(0, 255, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(5,{0,QColor(0, 255, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});

    mapFlowInfo.insert(6,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(7,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});

    mapFlowInfo.insert(8,{0,QColor(255, 0, 255,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});

    mapFlowInfo.insert(9,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(10,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(11,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(12,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(13,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(14,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(15,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(16,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(17,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(18,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});

    mapFlowInfo.insert(19,{0,QColor(0, 0, 255,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});

    mapFlowInfo.insert(20,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(21,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]()
    {
        for(int i = 0;i< mapFlowInfo.count();i++)
        {
            if(mapFlowInfo[i].directionFlag > 0)
            {
                mapFlowInfo[i].offset += mapFlowInfo[i].speed;
            }
            else
            {
                mapFlowInfo[i].offset -= mapFlowInfo[i].speed;
            }
        }
        static int count = 0;
        count++;
        if(count > 5)
        {
            //colorChange = qrand() % 255;
            colorChange  = QRandomGenerator::global()->bounded(255);
            count = 0;
        }

        update();
    });

}



void CustomWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setClipRegion(event->region());                         // 只重绘本次事件要求更新的区域，减少无效绘制。
    painter.setRenderHint(QPainter::Antialiasing, true);            // 设置抗锯齿（开启后线条会更加平滑）
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);   // 图片平滑缩放
    painter.setRenderHint(QPainter::TextAntialiasing, true);        // 文字抗锯齿
    // 先保存当前画笔状态，最后 restore() 时回到初始状态，避免变换和画笔设置污染外部。
    painter.save();
    // 先求出当前控件中心。
    int centerX = width() / 2;
    int centerY = height() / 2;

    painter.translate(QPointF(centerX,centerY));        // 把绘图原点从左上角移动到控件中心。
    // 一是按 m_scale 统一缩放整张图
    // 二是把 Y 轴翻转，让逻辑坐标变成“向上为正”，更适合按设计图坐标绘制。
    painter.scale(m_scale, -m_scale);

    QPen pen;
    // 把背景图铺在设计坐标中的固定区域里。可以把它理解成“底图”或“工艺流程图底板”。后面的管道、液位、文字，全都叠加在这张底图上。
    DrawImage(painter, background,QRectF(-1403,-747,2806,1494),0);
    // 绘制右上角的文本: 日期
    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(881.63 + 0,602.71 + 0,512.919,62.4453),text[11],0 + 0,23,R"(Microsoft YaHei)",132);

    // 绘制红色的线
    QPolygonF m_point2;
    m_point2.append(QPointF(523.053,-613.596));
    m_point2.append(QPointF(516.14,-639.031));
    m_point2.append(QPointF(497.611,-653.255));
    m_point2.append(QPointF(458.046,-659.14));
    m_point2.append(QPointF(-1141.77,-654.672));
    drawFlowLine(&painter, m_point2,QColor(160,160,164,255),10,QColor(255,0,0,255), 25, mapFlowInfo.value(0));
    // 绘制红色的线
    QPolygonF m_point3;
    m_point3.append(QPointF(-1186.42,-657.801));
    m_point3.append(QPointF(-1295.2,-649.303));
    m_point3.append(QPointF(-1322.74,-612.218));
    m_point3.append(QPointF(-1320.84,126.3));
    drawFlowLine(&painter, m_point3,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(1));
    // 绘制绿色的线
    QPolygonF m_point4;
    m_point4.append(QPointF(-1176.06,115.984));
    m_point4.append(QPointF(-1202.87,109.748));
    m_point4.append(QPointF(-1223.45,84.8055));
    m_point4.append(QPointF(-1226.67,13.406));
    drawFlowLine(&painter, m_point4,QColor(160,160,164,255),10,QColor(0,255,0,255), 23, mapFlowInfo.value(2));
    // 绘制绿色的线
    QPolygonF m_point5;
    m_point5.append(QPointF(-1226.67,-13.4062));
    m_point5.append(QPointF(-1226.67,-223.437));
    drawFlowLine(&painter, m_point5,QColor(160,160,164,255),10,QColor(0,255,0,255), 23, mapFlowInfo.value(3));
    // 绘制绿色的线
    QPolygonF m_point6;
    m_point6.append(QPointF(-1227.35,-256.022));
    m_point6.append(QPointF(-1224.25,-400.325));
    m_point6.append(QPointF(-1214.94,-418.945));
    m_point6.append(QPointF(-1190.11,-437.565));
    m_point6.append(QPointF(-1072.18,-439.116));
    drawFlowLine(&painter, m_point6,QColor(160,160,164,255),10,QColor(0,255,0,255), 23, mapFlowInfo.value(4));
    // 绘制绿色的线
    QPolygonF m_point7;
    m_point7.append(QPointF(-830.132,-436.013));
    m_point7.append(QPointF(-636.643,-437.299));
    drawFlowLine(&painter, m_point7,QColor(160,160,164,255),10,QColor(0,255,0,255), 23, mapFlowInfo.value(5));
    // 绘制液位槽
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,0), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(-1140.33,75.05,267.054,137.744),0);
    // 液位填充
    text[0].toDouble() > 30 ? painter.setBrush(QColor(0,255,0,200)) : painter.setBrush(QColor(0,0,0,255));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(-1140.33,75.05,267.054,137.744).bottomLeft().x(),QRectF(-1140.33,75.05,267.054,137.744).bottomLeft().y()- (100 - text[0].toDouble() - 0) * (double)137.744 / (100 - 0))),QPointF(QRectF(-1140.33,75.05,267.054,137.744).topRight())), 0, 0, 1, 1);
    // 绘制液位槽
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,0), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(-558.697,68.8425,253.719,167.319),0);
    // 液位填充
    text[1].toDouble() > 30?painter.setBrush(QColor(255,0,0,200)):painter.setBrush(QColor(0,0,0,255));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(-558.697,68.8425,253.719,167.319).bottomLeft().x(),QRectF(-558.697,68.8425,253.719,167.319).bottomLeft().y()- (100 - text[1].toDouble() - 0) * (double)167.319 / (100 - 0))),QPointF(QRectF(-558.697,68.8425,253.719,167.319).topRight())), 0, 0, 1, 1);
    // 绘制液位槽
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,0), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(-579.073,-484.716,191.194,120.441),0);
    // 液位填充
    text[3].toDouble() > 30?painter.setBrush(QColor(0,170,0,200)):painter.setBrush(QColor(0,0,0,255));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(-579.073,-484.716,191.194,120.441).bottomLeft().x(),QRectF(-579.073,-484.716,191.194,120.441).bottomLeft().y()- (100 - text[3].toDouble() - 0) * (double)120.441 / (100 - 0))),QPointF(QRectF(-579.073,-484.716,191.194,120.441).topRight())), 0, 0, 1, 1);
    // 绘制液位槽
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,0), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(22.106,82.986,199.64,179.856),0);
    // 液位填充
    text[2].toDouble() > 30 ? painter.setBrush(QColor(255,0,255,200)) : painter.setBrush(QColor(0,0,0,255));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(22.106,82.986,199.64,179.856).bottomLeft().x(),QRectF(22.106,82.986,199.64,179.856).bottomLeft().y()- (100 - text[2].toDouble() - 0) * (double)179.856 / (100 - 0))),QPointF(QRectF(22.106,82.986,199.64,179.856).topRight())), 0, 0, 1, 1);
    // 绘制液位槽
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,0), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(744.139,-633.719,318.569,199.091),0);
    // 液位填充
    text[4].toDouble() > 30 ? painter.setBrush(QColor(0,0,255,200)) : painter.setBrush(QColor(0,0,0,255));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(744.139,-633.719,318.569,199.091).bottomLeft().x(),QRectF(744.139,-633.719,318.569,199.091).bottomLeft().y()- (100 - text[4].toDouble() - 0) * (double)199.091 / (100 - 0))),QPointF(QRectF(744.139,-633.719,318.569,199.091).topRight())), 0, 0, 1, 1);
    // 绘制红色线
    QPolygonF m_point13;
    m_point13.append(QPointF(-819.787,129.304));
    m_point13.append(QPointF(-853.406,129.304));
    drawFlowLine(&painter, m_point13,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(6));
    // 绘制红色线
    QPolygonF m_point14;
    m_point14.append(QPointF(-593.505,128.011));
    m_point14.append(QPointF(-640.055,130.597));
    drawFlowLine(&painter, m_point14,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(7));
    // 绘制红色线
    QPolygonF m_point15;
    m_point15.append(QPointF(133.692,363.238));
    m_point15.append(QPointF(137.475,447.742));
    m_point15.append(QPointF(112.591,480.997));
    m_point15.append(QPointF(34.481,486.151));
    m_point15.append(QPointF(-392.223,485.073));
    m_point15.append(QPointF(-418.083,460.29));
    m_point15.append(QPointF(-426.704,437.662));
    m_point15.append(QPointF(-425.626,370.855));
    drawFlowLine(&painter, m_point15,QColor(160,160,164,255),10,QColor(255,0,255,255), 23, mapFlowInfo.value(8));
    // 绘制红色线
    QPolygonF m_point16;
    m_point16.append(QPointF(1395.12,-161.107));
    m_point16.append(QPointF(1253.47,-159.843));
    drawFlowLine(&painter, m_point16,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(9));
    // 绘制红色线
    QPolygonF m_point17;
    m_point17.append(QPointF(1210.79,-162.911));
    m_point17.append(QPointF(892.363,-161.476));
    drawFlowLine(&painter, m_point17,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(10));
    // 绘制红色线
    QPolygonF m_point18;
    m_point18.append(QPointF(711.178,-164.269));
    m_point18.append(QPointF(265.876,-162.835));
    drawFlowLine(&painter, m_point18,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(11));
    // 绘制红色线
    QPolygonF m_point19;
    m_point19.append(QPointF(219.267,-161.476));
    m_point19.append(QPointF(-622.107,-163.176));
    m_point19.append(QPointF(-620.407,-161.476));
    drawFlowLine(&painter, m_point19,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(12));
    QPolygonF m_point20;
    // 绘制红色线
    m_point20.append(QPointF(-665.733,-164.073));
    m_point20.append(QPointF(-707.047,-175.877));
    m_point20.append(QPointF(-714.129,-224.272));
    m_point20.append(QPointF(-714.872,-275.608));
    m_point20.append(QPointF(-715.821,-346.763));
    drawFlowLine(&painter, m_point20,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(13));
    // 绘制红色线
    QPolygonF m_point21;
    m_point21.append(QPointF(-715.593,-387.542));
    m_point21.append(QPointF(-715.347,-416.495));
    drawFlowLine(&painter, m_point21,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(14));
    // 绘制红色线
    QPolygonF m_point22;
    m_point22.append(QPointF(-134.76,-99.3485));
    m_point22.append(QPointF(-141.646,-404.279));
    m_point22.append(QPointF(-151.482,-423.952));
    m_point22.append(QPointF(-172.139,-432.805));
    drawFlowLine(&painter, m_point22,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(15));
    // 绘制红色线
    QPolygonF m_point23;
    m_point23.append(QPointF(-233.42,-630.137));
    m_point23.append(QPointF(-228.026,-614.444));
    m_point23.append(QPointF(-226.064,-573.743));
    m_point23.append(QPointF(-226.281,-536.612));
    drawFlowLine(&painter, m_point23,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(16));
    // 绘制红色线
    QPolygonF m_point24;
    m_point24.append(QPointF(611.369,-146.222));
    m_point24.append(QPointF(611.985,-107.228));
    drawFlowLine(&painter, m_point24,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(17));
    // 绘制红色线
    QPolygonF m_point25;
    m_point25.append(QPointF(614.6,-70.6136));
    m_point25.append(QPointF(609.075,112.977));
    m_point25.append(QPointF(586.753,136.21));
    m_point25.append(QPointF(549.217,138.611));
    drawFlowLine(&painter, m_point25,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(18));
    // 绘制红色线
    QPolygonF m_point26;
    m_point26.append(QPointF(1402.67,-546.63));
    m_point26.append(QPointF(1072.63,-546.63));
    drawFlowLine(&painter, m_point26,QColor(160,160,164,255),10,QColor(0,0,255,255), 23, mapFlowInfo.value(19));
    // 绘制红色线
    QPolygonF m_point27;
    m_point27.append(QPointF(679.811,-547.723));
    m_point27.append(QPointF(524.468,-546.54));
    m_point27.append(QPointF(524.468,-579.122));
    drawFlowLine(&painter, m_point27,QColor(160,160,164,255),10,QColor(255,0,0,255), 23, mapFlowInfo.value(20));
    // 绘制红色线
    QPolygonF m_point28;
    m_point28.append(QPointF(-473.974,343.487));
    m_point28.append(QPointF(-476.852,364.595));
    m_point28.append(QPointF(-483.569,377.068));
    m_point28.append(QPointF(-497.96,379.947));
    m_point28.append(QPointF(-587.19,379.947));
    m_point28.append(QPointF(-603.501,372.271));
    m_point28.append(QPointF(-611.177,358.838));
    m_point28.append(QPointF(-612.136,151.595));
    drawFlowLine(&painter, m_point28,QColor(160,160,164,255),10,QColor(255,0,0,255), 5, mapFlowInfo.value(21));
    //
    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1010.45 + 0,411.097 + 0,217.247,60.1511),text[5],0 + 0,23,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1010.45 + 0,331.311 + 0,217.247,60.1511),text[6],0 + 0,23,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1010.45 + 0,251.564 + 0,217.247,60.1511),text[7],0 + 0,23,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1010.45 + 0,177.334 + 0,217.247,60.1511),text[8],0 + 0,23,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1010.45 + 0,100.825 + 0,217.247,60.1511),text[9],0 + 0,23,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1015.45 + 0,27.7255 + 0,217.247,60.1511),text[10],0 + 0,23,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1219.92 + 0,325.535 + 0,84.6672,69.5337),R"(MPa)",0 + 0,23,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1215.24 + 0,404.069 + 0,84.6672,69.5337),R"(%)",0 + 0,23,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1215.45 + 0,254.973 + 0,84.6672,69.5337),R"(℃)",0 + 0,23,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1213 + 0,171.753 + 0,84.6672,69.5337),R"(%)",0 + 0,23,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1213.21 + 0,96.0862 + 0,84.6672,69.5337),R"(%)",0 + 0,23,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));


    isSelect[22] ? painter.setPen(pen) : painter.setPen(Qt::NoPen);
    isSelect[22] ? painter.setBrush(QBrush(QColor(255,0,0,255), Qt::SolidPattern)) : painter.setBrush(QBrush(QColor(170,0,255,255), Qt::SolidPattern));

    QTransform trans;
    trans.scale(m_scale, m_scale);

    DrawPolygon(painter, 40,{ QPointF(-965.229 + 0,-256.174 + 0),QPointF(-965.229 + 0,-202.805 + 0),QPointF(-940.831 + 0,-202.195 + 0),QPointF(-940.831 + 0,-255.869 + 0),QPointF(-924.058 + 0,-256.479 + 0),QPointF(-952.725 + 0,-306.799 + 0),QPointF(-981.392 + 0,-255.564 + 0) },trans,0 - 0);
    painter.restore();
}


void CustomWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // 窗口变化时缩放
    QSizeF widgetSize = size();
    // 计算缩放比例，使得内容宽度和高度均能适应窗口
    qreal scaleX = widgetSize.width() / 2806;
    qreal scaleY = widgetSize.height() / 1494;
    m_scale = qMin(scaleX, scaleY);   // 保持宽高比
}

void CustomWidget::Set_DisplayEvent_IsShow(int id, bool state)
{
    isSelect[id] = state;
}

void CustomWidget::Set_TextChangeEvent(int id, QString data)
{
    text[id] = data;
}
