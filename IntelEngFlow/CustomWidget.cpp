
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

//绘制图片
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
                  int bgLineWidth, QColor lineColor, int lineWidth,
                  const FlowInfo &flowInfo)
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


void rotateAndPaintRect(QPainter &painter, const QRectF &rect,int pathsID, const QTransform &trans, int angle,bool isRound = false,int rx = 0,int ry = 0)
{
    int dir = 6;
    qreal cx = 0;
    qreal cy = 0;

    cx = rect.bottomRight().x();
    cy = rect.bottomRight().y();

    cx = rect.center().x();
    cy = rect.center().y();

    painter.save();//save()函数会将当前QPainter的所有状态保存到一个栈中，以便后续恢复
    // 1. 将原点平移到
    painter.translate(cx ,cy);
    // 2. 旋转
    painter.rotate(angle);
    // 3. 计算绘制偏移量
    // 绘制起点 = 矩形左上角 - 旋转中心顶点
    QPointF drawOffset = rect.topLeft() - QPointF(cx,cy);

    if (isRound)
    {
        painter.drawRoundedRect(drawOffset.x(), drawOffset.y(), rect.width(), rect.height(), rx, ry);//绘制圆角矩形
    }
    else
    {
        painter.drawRect(drawOffset.x(), drawOffset.y(), rect.width(), rect.height());
    }
    //恢复上面保存的状态
    painter.restore();
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
    isSelect.resize(19);
    text.resize(5);
    text[0] = "100";
    text[1] = "100";
    text[2] = "0";
    text[3] = "0";
    text[4] = "0%";

    mapFlowInfo.insert(0,{0,QColor(255, 255, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(1,{0,QColor(255, 255, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(2,{0,QColor(255, 255, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(3,{0,QColor(255, 255, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(4,{0,QColor(255, 255, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(5,{0,QColor(255, 255, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(6,{0,QColor(0, 0, 255,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(7,{0,QColor(0, 0, 255,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(8,{0,QColor(0, 0, 255,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(9,{0,QColor(0, 0, 255,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(10,{0,QColor(0, 170, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(11,{0,QColor(0, 170, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(12,{0,QColor(0, 170, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(13,{0,QColor(0, 170, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});

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

    DrawImage(painter, background,QRectF(-1488,-768,2976,1536),0);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,125), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(-1394.52,424.785,202.345,180.745),0);

    text[0].toDouble() > 30?painter.setBrush(QColor(255,255,0,125)):painter.setBrush(QColor(255,255,0,125));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(-1394.52,424.785,202.345,180.745).bottomLeft().x(),QRectF(-1394.52,424.785,202.345,180.745).bottomLeft().y()- (100 - text[0].toDouble() - 0) * (double)180.745 / (100 - 0))),QPointF(QRectF(-1394.52,424.785,202.345,180.745).topRight())), 0, 0, 1, 1);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,125), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(-644.639,425.161,202.345,180.745),0);

    text[1].toDouble() > 30?painter.setBrush(QColor(0,0,255,125)):painter.setBrush(QColor(0,0,255,125));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(-644.639,425.161,202.345,180.745).bottomLeft().x(),QRectF(-644.639,425.161,202.345,180.745).bottomLeft().y()- (100 - text[1].toDouble() - 0) * (double)180.745 / (100 - 0))),QPointF(QRectF(-644.639,425.161,202.345,180.745).topRight())), 0, 0, 1, 1);

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,125), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(-403.268,-691.916,335.812,471.898),0);

    text[2].toDouble() > 30?painter.setBrush(QColor(0,170,0,125)):painter.setBrush(QColor(0,170,0,125));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(-403.268,-691.916,335.812,471.898).bottomLeft().x(),QRectF(-403.268,-691.916,335.812,471.898).bottomLeft().y()- (100 - text[2].toDouble() - 0) * (double)471.898 / (100 - 0))),QPointF(QRectF(-403.268,-691.916,335.812,471.898).topRight())), 0, 0, 1, 1);

    QPolygonF m_point4;
    m_point4.append(QPointF(-1294.27,241.457));
    m_point4.append(QPointF(-1294.91,109.525));
    drawFlowLine(&painter, m_point4,QColor(160,160,164,0),20,QColor(255,255,0,255), 15, mapFlowInfo.value(0));

    QPolygonF m_point5;
    m_point5.append(QPointF(-1229.87,45.6353));
    m_point5.append(QPointF(-1188.8,46.7761));
    drawFlowLine(&painter, m_point5,QColor(160,160,164,0),20,QColor(255,255,0,255), 15, mapFlowInfo.value(1));

    QPolygonF m_point6;
    m_point6.append(QPointF(-1078.93,51.168));
    m_point6.append(QPointF(-962.84,48.9672));
    drawFlowLine(&painter, m_point6,QColor(160,160,164,0),20,QColor(255,255,0,255), 15, mapFlowInfo.value(2));

    QPolygonF m_point7;
    m_point7.append(QPointF(-766.527,54.1568));
    m_point7.append(QPointF(-446.271,51.0998));
    drawFlowLine(&painter, m_point7,QColor(160,160,164,0),20,QColor(255,255,0,255), 15, mapFlowInfo.value(3));

    QPolygonF m_point8;
    m_point8.append(QPointF(-378.057,-23.954));
    m_point8.append(QPointF(-378.139,-95.3863));
    m_point8.append(QPointF(-378.057,-99.9819));
    drawFlowLine(&painter, m_point8,QColor(160,160,164,0),20,QColor(255,255,0,255), 15, mapFlowInfo.value(4));

    QPolygonF m_point9;
    m_point9.append(QPointF(-409.301,49.991));
    m_point9.append(QPointF(-400.969,47.908));
    m_point9.append(QPointF(-388.472,40.6176));
    m_point9.append(QPointF(-379.098,27.0784));
    m_point9.append(QPointF(-377.015,13.5392));
    drawFlowLine(&painter, m_point9,QColor(160,160,164,0),20,QColor(255,255,0,255), 15, mapFlowInfo.value(5));

    QPolygonF m_point10;
    m_point10.append(QPointF(-539.19,247.037));
    m_point10.append(QPointF(-539.902,194.364));
    drawFlowLine(&painter, m_point10,QColor(160,160,164,0),20,QColor(0,0,255,255), 15, mapFlowInfo.value(6));

    QPolygonF m_point11;
    m_point11.append(QPointF(-469.715,136.775));
    m_point11.append(QPointF(-399.528,136.775));
    drawFlowLine(&painter, m_point11,QColor(160,160,164,0),20,QColor(0,0,255,255), 15, mapFlowInfo.value(7));

    QPolygonF m_point12;
    m_point12.append(QPointF(-185.383,133.31));
    m_point12.append(QPointF(-129.143,133.31));
    drawFlowLine(&painter, m_point12,QColor(160,160,164,0),20,QColor(0,0,255,255), 15, mapFlowInfo.value(8));

    QPolygonF m_point13;
    m_point13.append(QPointF(-63.5301,69.779));
    m_point13.append(QPointF(-65.7591,-101.412));
    drawFlowLine(&painter, m_point13,QColor(160,160,164,0),20,QColor(0,0,255,255), 15, mapFlowInfo.value(9));

    QPolygonF m_point14;
    m_point14.append(QPointF(79.8617,-682.248));
    m_point14.append(QPointF(268.107,-683.389));
    drawFlowLine(&painter, m_point14,QColor(160,160,164,0),20,QColor(0,170,0,255), 15, mapFlowInfo.value(10));

    QPolygonF m_point15;
    m_point15.append(QPointF(352.946,-569.92));
    m_point15.append(QPointF(355.976,203.021));
    drawFlowLine(&painter, m_point15,QColor(160,160,164,0),20,QColor(0,170,0,255), 15, mapFlowInfo.value(11));

    QPolygonF m_point16;
    m_point16.append(QPointF(420.569,280.599));
    m_point16.append(QPointF(971.863,279.279));
    drawFlowLine(&painter, m_point16,QColor(160,160,164,0),20,QColor(0,170,0,255), 15, mapFlowInfo.value(12));

    QPolygonF m_point17;
    m_point17.append(QPointF(1040.48,207.641));
    m_point17.append(QPointF(1039.34,1.141));
    drawFlowLine(&painter, m_point17,QColor(160,160,164,0),20,QColor(0,170,0,255), 15, mapFlowInfo.value(13));

    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(QColor(180,180,180,125), Qt::SolidPattern));
    rotateAndPaintRect(&painter,QRectF(894.731,-482.311,351.339,347.764),0);

    text[3].toDouble() > 30?painter.setBrush(QColor(0,170,0,125)):painter.setBrush(QColor(0,170,0,125));
    rotateAndPaintRect(&painter,QRectF((QPointF(QRectF(894.731,-482.311,351.339,347.764).bottomLeft().x(),QRectF(894.731,-482.311,351.339,347.764).bottomLeft().y()- (100 - text[3].toDouble() - 0) * (double)347.764 / (100 - 0))),QPointF(QRectF(894.731,-482.311,351.339,347.764).topRight())), 0, 0, 1, 1);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(986.194 + 0,-556.039 + 0,137.032,62.1138),text[4],0 + 0,27,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    isSelect[17]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[17]? painter.setBrush(QBrush(QColor(255,0,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(0,170,0,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(355.748,-679.857));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),31.8421,31.8421);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    isSelect[15]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[15]? painter.setBrush(QBrush(QColor(255,0,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(0,170,0,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(-865.953,79.4968));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),20.1017,20.1017);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    isSelect[16]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[16]? painter.setBrush(QBrush(QColor(255,0,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(0,170,0,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(-287.035,164.429));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),19.1579,19.1579);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    isSelect[14]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[14]? painter.setBrush(QBrush(QColor(255,0,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(0,170,0,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(-1142.52,61.6584));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),20.6028,20.6028);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(-1417.92 + 0,598.883 + 0,234.73,75.4155),R"(化学试剂A罐)",0 + 0,26,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(-684.332 + 0,608.883 + 0,276.585,59.8098),R"(化学试剂B罐)",0 + 0,26,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(-422.817 + 0,-213.057 + 0,382.195,71.2785),R"(化学混合试剂C罐)",0 + 0,30,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(918.29 + 0,-117.928 + 0,296.119,71.2785),R"(存储试剂D罐)",0 + 0,30,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    isSelect[18]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[18]? painter.setBrush(QBrush(QColor(255,85,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(0,170,0,255), Qt::SolidPattern));
    rotateAndPaintRect(painter,QRectF(19.6747 + 0,-472.134 + 0,22.3858,155.53),28,trans,0  - 0,1,2,51);
    painter.restore();
}


void CustomWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    QSizeF widgetSize = size();
    qreal scaleX = widgetSize.width() / 2976;
    qreal scaleY = widgetSize.height() / 1536;
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



