
#include "CustomWidget.h"
#include <math.h>
#include <QTimer>
#include <QRandomGenerator>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

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

void DrawText(QPainter &painter, const QRectF &rect, QString str, double angle, int size, QString fontFamily, int textAlignment)
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
void drawFlowLine(QPainter *painter, QPolygonF points,QColor backgroundColor,
                                    int bgLineWidth,QColor lineColor,int lineWidth,const FlowInfo &flowInfo)
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


CustomWidget::CustomWidget(QWidget *parent) : QWidget(parent)
{
    background = QImage(":/image/管道清洗.png").mirrored(false, true);

    isSelect.resize(24);
    text.resize(9);

    text[0] = "50";
    text[1] = "50";
    text[2] = "50";
    text[3] = "50";
    text[4] = "0";
    text[5] = "0";
    text[6] = "0";
    text[7] = "0";
    text[8] = "停止";

    mapFlowInfo.insert(0,{0,QColor(0, 255, 127,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(1,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(2,{0,QColor(255, 0, 0,200),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(3,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(4,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(5,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(6,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(7,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(8,{0,QColor(255, 85, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(9,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(10,{0,QColor(170, 0, 255,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(11,{0,QColor(170, 0, 255,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(12,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(13,{0,QColor(255, 0, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});
    mapFlowInfo.insert(14,{0,QColor(0, 255, 0,255),QColor(160, 160, 164,255),0,2,0.5,0,0,0});


    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
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

            colorChange  = QRandomGenerator::global()->bounded(255);
            count = 0;
        }
        update();
    });

}



void CustomWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setClipRegion(event->region());
    painter.setRenderHint(QPainter::Antialiasing, true);  // 设置抗锯齿（开启后线条会更加平滑）
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    painter.save();

    /*
    窗口坐标系
    (0,0)
      ┌──────────────────────────────→ X
      │
      │
      │
      │
      │
      ↓
      Y
    转换后的坐标系:
                           -Y
                           ↑
                           |
                           |
                           |
                           |
    X-  ───────────────────┼───────────────────→ X+
                           |
                           |
                           |
                           |
                           |
                           +Y
    */

    int centerX = width() / 2;
    int centerY = height() / 2;

    painter.translate(QPointF(centerX,centerY));
    painter.scale(m_scale, -m_scale);

    QTransform trans;
    trans.scale(m_scale, m_scale);

    QPen pen;

    DrawImage(painter, background,QRectF(-1403,-747,2806,1494),0);
    QPolygonF m_point1;
    m_point1.append(QPointF(-770.536,-552.199));
    m_point1.append(QPointF(-638.05,-653.948));
    m_point1.append(QPointF(-604.134,-663.487));
    m_point1.append(QPointF(-565.978,-658.823));
    m_point1.append(QPointF(-113.196,-466.772));
    drawFlowLine(&painter, m_point1,QColor(160,160,164,255),10,QColor(0,255,127,255), 15, mapFlowInfo.value(0));
    
    QPolygonF m_point2;
    m_point2.append(QPointF(298.388,-424.942));
    m_point2.append(QPointF(537.563,-574.717));
    m_point2.append(QPointF(560.784,-582.844));
    m_point2.append(QPointF(592.132,-581.683));
    m_point2.append(QPointF(632.769,-574.717));
    m_point2.append(QPointF(837.112,-477.189));
    drawFlowLine(&painter, m_point2,QColor(160,160,164,255),10,QColor(255,0,0,255), 25, mapFlowInfo.value(1));

    QPolygonF m_point3;
    m_point3.append(QPointF(-401.136,-82.19));
    m_point3.append(QPointF(-256.997,-182.781));
    drawFlowLine(&painter, m_point3,QColor(160,160,164,255),10,QColor(255,0,0,200), 25, mapFlowInfo.value(2));

    QPolygonF m_point4;
    m_point4.append(QPointF(-1197.67,-26.4971));
    m_point4.append(QPointF(-1355.92,-76.6698));
    m_point4.append(QPointF(-1377.61,-92.6171));
    m_point4.append(QPointF(-1376.38,-115.925));
    m_point4.append(QPointF(-1359.82,-131.872));
    m_point4.append(QPointF(-1160.08,-276.715));
    m_point4.append(QPointF(-1116.54,-284.456));
    m_point4.append(QPointF(-1062.36,-268.975));
    m_point4.append(QPointF(-533.123,-78.4314));
    drawFlowLine(&painter, m_point4,QColor(160,160,164,255),10,QColor(255,0,0,255), 25, mapFlowInfo.value(3));

    QPolygonF m_point5;
    m_point5.append(QPointF(-1024.91,-100.689));
    m_point5.append(QPointF(-886.065,-196.078));
    drawFlowLine(&painter, m_point5,QColor(160,160,164,255),10,QColor(255,0,0,255), 25, mapFlowInfo.value(4));

    QPolygonF m_point6;
    m_point6.append(QPointF(-980.969,98.3863));
    m_point6.append(QPointF(-912.387,122.877));
    drawFlowLine(&painter, m_point6,QColor(160,160,164,255),10,QColor(255,0,0,255), 25, mapFlowInfo.value(5));

    QPolygonF m_point7;
    m_point7.append(QPointF(-742.101,182.864));
    m_point7.append(QPointF(-681.18,205.453));
    drawFlowLine(&painter, m_point7,QColor(160,160,164,255),10,QColor(255,0,0,255), 20, mapFlowInfo.value(6));

    QPolygonF m_point8;
    m_point8.append(QPointF(-755.485,41.9714));
    m_point8.append(QPointF(-586.328,-72.4956));
    drawFlowLine(&painter, m_point8,QColor(160,160,164,255),10,QColor(255,0,0,255), 25, mapFlowInfo.value(7));

    QPolygonF m_point9;
    m_point9.append(QPointF(-350.125,270.9));
    m_point9.append(QPointF(-294.412,287.256));
    m_point9.append(QPointF(-146.694,196.275));
    m_point9.append(QPointF(-104.782,177.874));
    m_point9.append(QPointF(-81.27,177.363));
    m_point9.append(QPointF(7.156,207.008));
    drawFlowLine(&painter, m_point9,QColor(160,160,164,255),10,QColor(255,85,0,255), 25, mapFlowInfo.value(8));

    QPolygonF m_point10;
    m_point10.append(QPointF(117.011,120.827));
    m_point10.append(QPointF(62.321,73.7682));
    m_point10.append(QPointF(38.1557,53.4184));
    m_point10.append(QPointF(-118.283,0));
    drawFlowLine(&painter, m_point10,QColor(160,160,164,255),10,QColor(255,0,0,255), 25, mapFlowInfo.value(9));

    QPolygonF m_point11;
    m_point11.append(QPointF(557.911,267.915));
    m_point11.append(QPointF(658.011,301.772));
    m_point11.append(QPointF(671.26,307.661));
    m_point11.append(QPointF(693.341,303.244));
    drawFlowLine(&painter, m_point11,QColor(160,160,164,255),10,QColor(170,0,255,255), 25, mapFlowInfo.value(10));

    QPolygonF m_point12;
    m_point12.append(QPointF(377.583,98.628));
    m_point12.append(QPointF(451.186,57.4103));
    m_point12.append(QPointF(484.307,45.6338));
    m_point12.append(QPointF(509.332,41.9537));
    m_point12.append(QPointF(549.814,54.4662));
    m_point12.append(QPointF(679.355,108.932));
    m_point12.append(QPointF(786.816,156.038));
    m_point12.append(QPointF(821.409,172.231));
    drawFlowLine(&painter, m_point12,QColor(160,160,164,255),10,QColor(170,0,255,255), 25, mapFlowInfo.value(11));

    QPolygonF m_point13;
    m_point13.append(QPointF(653.087,-166.416));
    m_point13.append(QPointF(539.885,-212.858));
    drawFlowLine(&painter, m_point13,QColor(160,160,164,255),10,QColor(255,0,0,255), 25, mapFlowInfo.value(12));

    QPolygonF m_point14;
    m_point14.append(QPointF(985.946,157.202));
    m_point14.append(QPointF(1175.83,69.2058));
    m_point14.append(QPointF(1189.94,56.44));
    m_point14.append(QPointF(1183.89,46.361));
    m_point14.append(QPointF(995.085,-38.298));
    drawFlowLine(&painter, m_point14,QColor(160,160,164,255),10,QColor(255,0,0,255), 25, mapFlowInfo.value(13));

    QPolygonF m_point15;
    m_point15.append(QPointF(1122.34,-113.686));
    m_point15.append(QPointF(1398.89,13.706));
    drawFlowLine(&painter, m_point15,QColor(160,160,164,255),10,QColor(0,255,0,255), 25, mapFlowInfo.value(14));
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,0), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(-1202.15,35.5255,197.126,146.649),0);

    text[0].toDouble() > 30?painter.setBrush(QColor(255,0,0,200)):painter.setBrush(QColor(255,0,0,200));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(-1202.15,35.5255,197.126,146.649).bottomLeft().x(),QRectF(-1202.15,35.5255,197.126,146.649).bottomLeft().y()- (100 - text[0].toDouble() - 0) * (double)146.649 / (100 - 0))),QPointF(QRectF(-1202.15,35.5255,197.126,146.649).topRight())), 0, 0, 1, 1);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(0,255,0,colorChange), Qt::SolidPattern));
    painter.drawPolygon(QPolygonF({ QPointF(54.354 + 0,-199.198 + 0),QPointF(205.233 + 0,-138.847 + 0),QPointF(332.497 + 0,-220.19 + 0),QPointF(184.897 + 0,-280.542 + 0)  }));

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,0), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(-892.83,116.968,126.4,148.841),0);

    text[1].toDouble() > 30?painter.setBrush(QColor(255,85,0,200)):painter.setBrush(QColor(255,0,0,200));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(-892.83,116.968,126.4,148.841).bottomLeft().x(),QRectF(-892.83,116.968,126.4,148.841).bottomLeft().y()- (100 - text[1].toDouble() - 0) * (double)148.841 / (100 - 0))),QPointF(QRectF(-892.83,116.968,126.4,148.841).topRight())), 0, 0, 1, 1);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,0), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(-642.231,221.722,108.57,142.185),0);
    text[2].toDouble() > 30?painter.setBrush(QColor(0,255,0,200)):painter.setBrush(QColor(255,0,0,200));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(-642.231,221.722,108.57,142.185).bottomLeft().x(),QRectF(-642.231,221.722,108.57,142.185).bottomLeft().y()- (100 - text[2].toDouble() - 0) * (double)142.185 / (100 - 0))),QPointF(QRectF(-642.231,221.722,108.57,142.185).topRight())), 0, 0, 1, 1);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,0), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(-452.745,254.803,70.8077,127.678),0);
    text[3].toDouble() > 30?painter.setBrush(QColor(255,0,255,200)):painter.setBrush(QColor(255,0,0,200));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(-452.745,254.803,70.8077,127.678).bottomLeft().x(),QRectF(-452.745,254.803,70.8077,127.678).bottomLeft().y()- (100 - text[3].toDouble() - 0) * (double)127.678 / (100 - 0))),QPointF(QRectF(-452.745,254.803,70.8077,127.678).topRight())), 0, 0, 1, 1);
    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(144,144,176,255), Qt::SolidPattern));
    painter.drawPolygon(QPolygonF({ QPointF(-781.659 + 0,-210.181 + 0),QPointF(-783.103 + 0,-85.962 + 0),QPointF(-551.112 + 0,-7.724 + 0),QPointF(-545.527 + 0,-127.465 + 0) }));

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(80,120,32,255), Qt::SolidPattern));
    painter.drawPolygon(QPolygonF({ QPointF(12.0726 + 0,-9.48559 + 0),QPointF(242.315 + 0,75.8851 + 0),QPointF(238.003 + 0,-52.6021 + 0),QPointF(11.2103 + 0,-137.111 + 0) }));

    pen.setWidth(1);
    pen.setColor(QColor(0,255,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(11.3869 + 0,-93.9741 + 0,117.559,89.7569),R"(温度:)",19 + 0,28,R"(Microsoft YaHei)",132);
    pen.setWidth(1);
    pen.setColor(QColor(0,255,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(127.983 + 0,-48.4973 + 0,99.718,55.6452),text[5],19 + 0,28,R"(Microsoft YaHei)",129);
    pen.setWidth(1);
    pen.setColor(QColor(255,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(191.439 + 0,15.5802 + 0,44.7883,42.0979),R"(℃)",0 + 0,16,R"(Microsoft YaHei)",132);
    pen.setWidth(1);
    pen.setColor(QColor(0,0,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(-235.216 + 0,401.808 + 0,117.559,89.7569),R"(湿度:)",19 + 0,28,R"(Microsoft YaHei)",132);
    pen.setWidth(1);
    pen.setColor(QColor(0,0,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(-114.287 + 0,446.765 + 0,84.4955,55.6452),text[6],16 + 0,28,R"(Microsoft YaHei)",129);
    pen.setWidth(1);
    pen.setColor(QColor(255,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(216.208 + 0,506.195 + 0,117.559,89.7569),R"(转速:)",13 + 0,28,R"(Microsoft YaHei)",132);
    pen.setWidth(1);
    pen.setColor(QColor(255,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(339.11 + 0,550.52 + 0,84.4955,55.6452),text[7],11 + 0,28,R"(Microsoft YaHei)",129);
    pen.setWidth(1);
    pen.setColor(QColor(255,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(-773.413 + 0,-170.946 + 0,117.559,89.7569),R"(流速:)",20 + 0,28,R"(Microsoft YaHei)",132);
    pen.setWidth(1);
    pen.setColor(QColor(255,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(-653.018 + 0,-118.242 + 0,106.404,55.6452),text[4],17 + 0,28,R"(Microsoft YaHei)",129);
    pen.setWidth(1);
    pen.setColor(QColor(255,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(979.793 + 0,371.872 + 0,181.734,89.7569),text[8],-23 + 0,28,R"(Microsoft YaHei)",132);


    painter.restore();
}


void CustomWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    //窗口变化时缩放
    QSizeF widgetSize = size();
    // 计算缩放比例，使得内容宽度和高度均能适应窗口
    qreal scaleX = widgetSize.width() / 2700;
    qreal scaleY = widgetSize.height() / 1534;
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
