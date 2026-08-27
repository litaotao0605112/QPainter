
#include "CustomWidget.h"
#include <math.h>
#include <QTimer>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif


// 绘制文本
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
    painter.rotate(-angle);
    painter.drawText(rotatedRect,textAlignment,str);
    painter.restore();
}

// 绘制图片
void DrawImage(QPainter &painter, QImage &image, const QRectF &rect,double angle)
{
    QRectF rotatedRect(-rect.width()/2, -rect.height()/2, rect.width(), rect.height());
    qreal cx = rect.x() + rect.width() / 2;
    qreal cy = rect.y() + rect.height() / 2;

    painter.save();
    painter.translate(cx, cy);
    painter.rotate(angle);
    painter.drawImage(rotatedRect, image);

    painter.restore();
}

// 绘制流动曲线
void drawFlowLine(QPainter *painter, QPolygonF points, QColor backgroundColor,
                  int bgLineWidth, QColor lineColor, int lineWidth, const FlowInfo &flowInfo)
{
    QPen pen(backgroundColor);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(bgLineWidth);
    pen.setCapStyle(Qt::RoundCap);
    painter->setPen(pen);
    painter->drawPolyline(points);

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

// 绘制矩形
void rotateAndPaintRect(QPainter *painter, const QRectF &rect, int angle,bool isRound = false,int rx = 0,int ry = 0)
{
    qreal cx = 0;
    qreal cy = 0;

    cx = rect.center().x();
    cy = rect.center().y();

    painter->save();
    painter->translate(cx ,cy);
    painter->rotate(angle);
    QPointF drawOffset = rect.topLeft() - QPointF(cx,cy);
    painter->drawRect(drawOffset.x(), drawOffset.y(), rect.width(), rect.height());
    painter->restore();
}


CustomWidget::CustomWidget(QWidget *parent) : QWidget(parent)
{
    background = QImage(":/image/1.png").mirrored(false, true);

    isSelect.resize(12);
    text.resize(5);
    text[0] = "50";
    text[1] = "50";
    text[2] = "3";
    text[3] = "3";
    text[4] = "36";


    mapFlowInfo.insert(0,{0,QColor(0, 170, 0,255),QColor(170, 255, 255,0),0,2,0.5,0,1,0});
    mapFlowInfo.insert(2,{0,QColor(170, 85, 255,255),QColor(160, 160, 164,0),0,2,0.5,0,1,0});
    mapFlowInfo.insert(1,{0,QColor(255, 85, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,1,0});
    mapFlowInfo.insert(3,{0,QColor(0, 170, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,1,0});
    mapFlowInfo.insert(4,{0,QColor(170, 85, 255,255),QColor(160, 160, 164,0),0,2,0.5,0,1,0});
    mapFlowInfo.insert(5,{0,QColor(170, 85, 255,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(6,{0,QColor(0, 85, 255,255),QColor(160, 160, 164,0),0,2,0.5,0,1,0});
    mapFlowInfo.insert(7,{0,QColor(0, 85, 255,255),QColor(160, 160, 164,0),0,2,0.5,0,1,0});
    mapFlowInfo.insert(8,{0,QColor(0, 85, 255,255),QColor(160, 160, 164,0),0,2,0.5,0,1,0});
    mapFlowInfo.insert(9,{0,QColor(0, 85, 255,255),QColor(160, 160, 164,0),0,2,0.5,0,1,0});

    timer = new QTimer(this);
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        for(int i = 0;i< mapFlowInfo.count();i++)
        {
            if(!isSelect[i])
            {
                continue;
            }
           if(mapFlowInfo[i].arrowType == 0)
            {
                if (mapFlowInfo[i].directionFlag)
                {
                    mapFlowInfo[i].offset += mapFlowInfo[i].speed;
                }
                else
                {
                    mapFlowInfo[i].offset -= mapFlowInfo[i].speed;
                }
            }
            else if(mapFlowInfo[i].arrowType == 1)
            {
                mapFlowInfo[i].offset += mapFlowInfo[i].speed;

                if (mapFlowInfo[i].offset > 1.0)
                {
                 mapFlowInfo[i].offset -= 1.0;
                }
            }
        }
       update();
    });
    timer->start(50);
}



void CustomWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setClipRegion(event->region());
    painter.setRenderHint(QPainter::Antialiasing, true);  // 设置抗锯齿（开启后线条会更加平滑）
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    painter.save();

    int centerX = width() / 2;
    int centerY = height() / 2;

    painter.translate(QPointF(centerX,centerY));

    painter.scale(m_scale, -m_scale);

    QTransform trans;
    trans.scale(m_scale, m_scale);

    QPen pen;

    DrawImage(painter, background,QRectF(-1367,-766.5,2734,1533),0);

    QPolygonF m_point1;
    m_point1.append(QPointF(-1075.55,-188.589));
    m_point1.append(QPointF(-1072.6,-565.766));
    drawFlowLine(&painter, m_point1,QColor(170,255,255,0),40,QColor(0,170,0,255), 25, mapFlowInfo.value(0));

    QPolygonF m_point2;
    m_point2.append(QPointF(-729.026,-188.689));
    m_point2.append(QPointF(-728.449,-290.852));
    m_point2.append(QPointF(-722.734,-324.869));
    m_point2.append(QPointF(-706.481,-349.726));
    m_point2.append(QPointF(-676.605,-373.451));
    m_point2.append(QPointF(-631.465,-383.81));
    m_point2.append(QPointF(-468.298,-382.679));
    m_point2.append(QPointF(-353.643,-383.423));
    drawFlowLine(&painter, m_point2,QColor(160,160,164,0),25,QColor(170,85,255,255), 25, mapFlowInfo.value(2));

    QPolygonF m_point3;
    m_point3.append(QPointF(-510.415,-16.3989));
    m_point3.append(QPointF(-548.646,-15.6521));
    drawFlowLine(&painter, m_point3,QColor(160,160,164,0),25,QColor(255,85,0,255), 25, mapFlowInfo.value(1));

    QPolygonF m_point4;
    m_point4.append(QPointF(-891.673,-14.8282));
    m_point4.append(QPointF(-905.513,-14.8282));
    drawFlowLine(&painter, m_point4,QColor(160,160,164,0),25,QColor(0,170,0,255), 25, mapFlowInfo.value(3));

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,100), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(-390.4645,-218.4217,218.347,490.107),0);

    text[0].toDouble() > 30?painter.setBrush(QColor(170,85,255,200)):painter.setBrush(QColor(255,85,0,200));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(-390.4645,-218.4217,218.347,490.107).bottomLeft().x(),QRectF(-390.4645,-218.4217,218.347,490.107).bottomLeft().y()- (100 - text[0].toDouble() - 0) * (double)490.107 / (100 - 0))),QPointF(QRectF(-390.4645,-218.4217,218.347,490.107).topRight())), 0, 0, 1, 1);

    QPolygonF m_point6;
    m_point6.append(QPointF(-45.2695,-19.4012));
    m_point6.append(QPointF(-60.5791,-20.8894));
    drawFlowLine(&painter, m_point6,QColor(160,160,164,0),25,QColor(170,85,255,255), 25, mapFlowInfo.value(4));

    QPolygonF m_point7;
    m_point7.append(QPointF(272.691,-13.3485));
    m_point7.append(QPointF(274.85,-9.7006));
    drawFlowLine(&painter, m_point7,QColor(160,160,164,0),25,QColor(170,85,255,255), 25, mapFlowInfo.value(5));

    QPolygonF m_point8;
    m_point8.append(QPointF(103.959,-188.425));
    m_point8.append(QPointF(103.959,-233.907));
    m_point8.append(QPointF(105.258,-297.582));
    m_point8.append(QPointF(109.267,-322.08));
    m_point8.append(QPointF(126.429,-350.112));
    m_point8.append(QPointF(152.745,-374.711));
    m_point8.append(QPointF(187.641,-385.009));
    m_point8.append(QPointF(440.525,-384.648));
    drawFlowLine(&painter, m_point8,QColor(160,160,164,0),25,QColor(0,85,255,255), 25, mapFlowInfo.value(6));

    QPolygonF m_point9;
    m_point9.append(QPointF(801.106,-153.015));
    m_point9.append(QPointF(799.183,-297.582));
    m_point9.append(QPointF(794.317,-324.83));
    m_point9.append(QPointF(782.828,-349.374));
    m_point9.append(QPointF(764.55,-369.742));
    m_point9.append(QPointF(743.138,-381.231));
    m_point9.append(QPointF(730.31,-383.348));
    m_point9.append(QPointF(588.035,-383.842));
    drawFlowLine(&painter, m_point9,QColor(160,160,164,0),25,QColor(0,85,255,255), 25, mapFlowInfo.value(7));

    QPolygonF m_point10;
    m_point10.append(QPointF(772.036,-12.6206));
    m_point10.append(QPointF(743.142,-12.5863));
    drawFlowLine(&painter, m_point10,QColor(160,160,164,0),25,QColor(0,85,255,255), 25, mapFlowInfo.value(8));

    QPolygonF m_point11;
    m_point11.append(QPointF(860.006,-11.7315));
    m_point11.append(QPointF(843.952,-12.9341));
    drawFlowLine(&painter, m_point11,QColor(160,160,164,0),25,QColor(0,85,255,255), 25, mapFlowInfo.value(9));

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,100), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(406.346,-219.9091,209.44,491.75),0);

    text[1].toDouble() > 30?painter.setBrush(QColor(0,85,255,200)):painter.setBrush(QColor(170,85,255,200));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(406.346,-219.9091,209.44,491.75).bottomLeft().x(),QRectF(406.346,-219.9091,209.44,491.75).bottomLeft().y()- (100 - text[1].toDouble() - 0) * (double)491.75 / (100 - 0))),QPointF(QRectF(406.346,-219.9091,209.44,491.75).topRight())), 0, 0, 1, 1);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,0));
    isSelect[10]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[10]? painter.setBrush(QBrush(QColor(255,0,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(0,255,0,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(-279.564,-483.039));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),25.5532,25.5532);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    isSelect[11]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[11]? painter.setBrush(QBrush(QColor(255,0,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(0,255,0,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(514.863,-484.185));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),23.0096,23.0096);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(255,255,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(821.6 + 0,444.595 + 0,131.831,83.6503),R"(温度)",0 + 0,35,R"(Microsoft YaHei UI)",132);

    pen.setWidth(1);
    pen.setColor(QColor(255,255,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1004.89 + 0,444.595 + 0,189.574,83.6503),R"(36℃)",0 + 0,35,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(255,255,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(821.6 + 0,341.201 + 0,131.831,83.6503),R"(压力)",0 + 0,35,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(255,255,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1209.06 + 0,341.201 + 0,131.831,83.6503),R"(MPa)",0 + 0,35,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(255,255,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(821.6 + 0,238.616 + 0,131.831,83.6503),R"(流量)",0 + 0,35,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(255,255,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(821.6 + 0,117.892 + 0,131.831,83.6503),R"(浓度)",0 + 0,35,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(255,255,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1001.66 + 0,341.201 + 0,189.574,83.6503),text[2],0 + 0,35,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(255,255,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(998.433 + 0,238.616 + 0,189.574,83.6503),text[3],0 + 0,35,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(255,255,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1004.89 + 0,117.892 + 0,189.574,83.6503),text[4],0 + 0,35,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(255,255,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1209.06 + 0,238.616 + 0,131.831,83.6503),R"(m/h)",0 + 0,35,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(255,255,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(1209.06 + 0,117.892 + 0,131.831,83.6503),R"(%)",0 + 0,35,R"(Microsoft YaHei)",132);

    painter.restore();
}


void CustomWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QSizeF widgetSize = size();
    qreal scaleX = widgetSize.width() / 2700;
    qreal scaleY = widgetSize.height() / 1534;
    m_scale = qMin(scaleX, scaleY);
}


void CustomWidget::Set_DisplayEvent_IsShow(int id, bool state)
{
    isSelect[id] = state;
}


void CustomWidget::Set_TextChangeEvent(int id, QString data)
{
    text[id] = data;
}




