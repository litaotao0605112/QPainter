
#include "CustomWidget.h"
#include <math.h>
#include <QTimer>
#include <QRandomGenerator>

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif

// 绘制矩形类图元，当前工程中主要用于方向指示块。
void rotateAndPaintRect(QPainter &painter, const QRectF &rect,int pathsID, const QTransform &trans, int angle,bool isRound = false,int rx = 0,int ry = 0)
{
    painter.drawRect(rect.x(), rect.y(), rect.width(), rect.height());
}

// 以给定矩形中心为基准绘制文字，避免缩放后的文字方向颠倒。
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

// 绘制背景图片，坐标系与主画布保持一致。
void DrawImage(QPainter &painter, QImage &image, const QRectF &rect,double angle)
{
    QRectF rotatedRect(-rect.width()/2, -rect.height()/2, rect.width(), rect.height());
    painter.drawImage(rotatedRect, image);
}

// 先绘制底线，再通过虚线偏移营造列车行进效果。
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


void DrawPolygon(QPainter &painter,int pathsID, QVector<QPointF> points, const QTransform &trans,double angle)
{
    // 将三个顶点组织成多边形，用于绘制方向箭头。
    QPolygonF polygon(points);
    painter.drawPolygon(polygon);

}


// 初始化背景、默认文案和各段流动线的动画参数。
CustomWidget::CustomWidget(QWidget *parent) : QWidget(parent)
{
    background = QImage(":/image/subway.png").mirrored(false, true);

    // 0-5 为流动线，6-16 为站点/箭头/方向块等图元状态。
    isSelect.resize(17);

    // 文本区依次为当前站、行驶方向、剩余时间、运行状态。
    text.resize(5);

    text[0] = "人民广场";
    text[1] = "开往国际机场";
    text[2] = "2分钟";
    text[3] = "运行中";

    mapFlowInfo.insert(0,{0,QColor(85, 255, 0,255),QColor(170, 0, 255,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(1,{0,QColor(85, 255, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(2,{0,QColor(85, 255, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(3,{0,QColor(85, 255, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(4,{0,QColor(85, 255, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});
    mapFlowInfo.insert(5,{0,QColor(85, 255, 0,255),QColor(160, 160, 164,0),0,2,0.5,0,0,0});

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [=]() {
        // 仅更新当前启用线路的虚线偏移量。
        for(int i = 0;i< mapFlowInfo.count();i++)
            {
                if(!isSelect[i])
                {
                    continue;
                }
               if(mapFlowInfo[i].arrowType == 0)
            {
                // 根据运行方向决定流动效果的前进方向。
                if (directionFlag)
                {
                    mapFlowInfo[i].offset += mapFlowInfo[i].speed;
                }
                else
                {
                    mapFlowInfo[i].offset -= mapFlowInfo[i].speed;
                }
            }
          }
        update();
    });

    timer->start(50);
}


void CustomWidget::paintEvent(QPaintEvent *event)
{
    // 统一在逻辑坐标系中绘制，避免窗口尺寸变化后逐项重算坐标。
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

    // 背景图和 6 段流动线路。
    DrawImage(painter, background,QRectF(-886,-1182,1773,2364),0);
    QPolygonF m_point1;
    m_point1.append(QPointF(-640.833,-449.06));
    m_point1.append(QPointF(-587.82,-412.308));
    m_point1.append(QPointF(-548.49,-384.757));
    drawFlowLine(&painter, m_point1,QColor(170,0,255,0),40,QColor(85,255,0,255), 30, mapFlowInfo.value(0));

    QPolygonF m_point2;
    m_point2.append(QPointF(-442.726,-296.748));
    m_point2.append(QPointF(-399.382,-262.115));
    m_point2.append(QPointF(-362.96,-231.973));
    drawFlowLine(&painter, m_point2,QColor(160,160,164,0),20,QColor(85,255,0,255), 30, mapFlowInfo.value(1));

    QPolygonF m_point3;
    m_point3.append(QPointF(-226.391,-102.168));
    m_point3.append(QPointF(-188.858,-60.7329));
    m_point3.append(QPointF(-159.438,-30.224));
    m_point3.append(QPointF(-141.789,-9.472));
    drawFlowLine(&painter, m_point3,QColor(160,160,164,0),20,QColor(85,255,0,255), 30, mapFlowInfo.value(2));

    QPolygonF m_point4;
    m_point4.append(QPointF(-55.1992,85.7711));
    m_point4.append(QPointF(-45.0127,103.763));
    m_point4.append(QPointF(-11.235,139.719));
    m_point4.append(QPointF(29.0803,177.856));
    m_point4.append(QPointF(69.8794,206.155));
    drawFlowLine(&painter, m_point4,QColor(160,160,164,0),20,QColor(85,255,0,255), 30, mapFlowInfo.value(3));

    QPolygonF m_point5;
    m_point5.append(QPointF(168.757,271.886));
    m_point5.append(QPointF(220.321,303.137));
    m_point5.append(QPointF(282.824,335.951));
    m_point5.append(QPointF(328.138,359.39));
    drawFlowLine(&painter, m_point5,QColor(160,160,164,0),20,QColor(85,255,0,255), 30, mapFlowInfo.value(4));

    QPolygonF m_point6;
    m_point6.append(QPointF(446.401,421.853));
    m_point6.append(QPointF(487.52,439.08));
    m_point6.append(QPointF(553.147,468.769));
    m_point6.append(QPointF(612.525,493.77));
    drawFlowLine(&painter, m_point6,QColor(160,160,164,0),20,QColor(85,255,0,255), 30, mapFlowInfo.value(5));

    // 站点圆点，点亮时显示为绿色。
    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    isSelect[6]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[6]? painter.setBrush(QBrush(QColor(0,170,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(255,85,0,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(-686.735,-494.313));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),37.1567,37.1567);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    isSelect[7]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[7]? painter.setBrush(QBrush(QColor(0,170,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(255,85,0,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(-485.313,-347.808));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),37.1567,37.1567);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    isSelect[8]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[8]? painter.setBrush(QBrush(QColor(0,170,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(192,248,16,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(-290.974,-171.856));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),62.041,62.041);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    isSelect[9]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[9]? painter.setBrush(QBrush(QColor(0,170,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(255,85,0,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(-91.4869,28.7593));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),37.1567,37.1567);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    isSelect[10]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[10]? painter.setBrush(QBrush(QColor(0,170,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(255,85,0,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(136.608,217.629));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),37.1567,37.1567);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    isSelect[11]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[11]? painter.setBrush(QBrush(QColor(0,170,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(255,85,0,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(389.748,383.022));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),38.6658,38.6658);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    isSelect[12]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[12]? painter.setBrush(QBrush(QColor(0,170,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(255,85,0,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(672.882,506.266));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),39.3753,39.3753);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,0));
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(255,85,0,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(-57.3086,-969.426));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),38.8448,38.8448);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(85,170,255,255));
    painter.setPen(pen);
    // 固定站名和换乘信息。
    DrawText(painter,QRectF(-779  ,-655.15  ,256.844,92.1504),R"(知春路)",0  ,40,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(85,170,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(-580.627  ,-509.674  ,299.289,105.3),R"(人民广场)",0  ,40,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(85,170,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(-371  ,-366.07  ,322.584,121.07),R"(中央公园)",0  ,40,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(85,170,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(-165  ,-134.684  ,299.522,104.684),R"(金融中心)",0  ,40,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(85,170,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(20.9995  ,71.3384  ,244.383,97.6612),R"(大学城)",0  ,40,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(85,170,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(279  ,219  ,299.494,117),R"(体育中心)",0  ,40,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(85,170,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(544.354  ,336.708  ,296.36,117),R"(国际机场)",0  ,40,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(85,170,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(-535.103  ,-96.6804  ,335.512,117),R"(换乘二号线)",42  ,35,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,255));
    painter.setPen(pen);
    painter.setBrush(QBrush(QColor(192,248,16,255), Qt::SolidPattern));
    painter.save();
    painter.translate(QPointF(406.957,-969.925));
    painter.rotate(0);
    painter.drawEllipse(QPointF(0,0),53.9715,53.9715);
    painter.restore();

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,0));
    isSelect[13]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[13]? painter.setBrush(QBrush(QColor(255,0,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(255,255,255,255), Qt::SolidPattern));
    // 正向运行时使用的方向指示块。
    rotateAndPaintRect(painter,QRectF(-869.599  ,-934.245  ,150.071,34.6045),24,trans,0 - 0);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,0));
    isSelect[14]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[14]? painter.setBrush(QBrush(QColor(255,0,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(255,255,255,255), Qt::SolidPattern));
    DrawPolygon(painter, 25,{ QPointF(-740.178  ,-871.604  ),QPointF(-738.68  ,-957.393  ),QPointF(-640.915  ,-912.779  ) },trans,0 - 0);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,0));
    isSelect[16]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[16]? painter.setBrush(QBrush(QColor(255,0,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(255,255,255,255), Qt::SolidPattern));
    // 反向运行时使用的方向指示块。
    rotateAndPaintRect(painter,QRectF(-785.158  ,-1047.09  ,135.489,31.2419),26,trans,0 - 0);

    pen.setWidth(1);
    pen.setColor(QColor(0,0,0,0));
    isSelect[15]?1?painter.setPen(pen):painter.setPen(Qt::NoPen):painter.setPen(Qt::NoPen);
    isSelect[15]? painter.setBrush(QBrush(QColor(255,0,0,255), Qt::SolidPattern))
    :painter.setBrush(QBrush(QColor(255,255,255,255), Qt::SolidPattern));
    DrawPolygon(painter, 27,{ QPointF(-777.718  ,-1071.69  ),QPointF(-777.719  ,-994.228  ),QPointF(-866.674  ,-1032.96  ) },trans,0 - 0);

    pen.setWidth(1);
    pen.setColor(QColor(0,255,0,255));
    painter.setPen(pen);
    // 右上角动态状态信息。
    DrawText(painter,QRectF(468.153  ,-176.752  ,319.018,105.3),text[0],0  ,38,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,255,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(411.481  ,-298.468  ,376.115,105.3),text[1],0  ,36,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(85,170,255,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(527.487  ,-414.099  ,252.847,105.3),text[2],0  ,40,R"(Microsoft YaHei)",132);

    pen.setWidth(1);
    pen.setColor(QColor(0,255,0,255));
    painter.setPen(pen);
    DrawText(painter,QRectF(542.41  ,-536.772  ,234.881,105.3),text[3],0  ,36,R"(Microsoft YaHei)",132);

    painter.restore();
}


void CustomWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // 窗口变化时缩放
    QSizeF widgetSize = size();
    // 计算缩放比例，使得内容宽度和高度均能适应窗口
    qreal scaleX = widgetSize.width() / 1773;
    qreal scaleY = widgetSize.height() / 2364;
    m_scale = qMin(scaleX, scaleY);   // 保持宽高比
}


// 按编号切换指定图元的显示状态。
void CustomWidget::Set_DisplayEvent_IsShow(int id, bool state)
{
    isSelect[id] = state;
}

// 按编号更新界面文本内容。
void CustomWidget::Set_TextChangeEvent(int id, QString data)
{
    text[id] = data;
}
