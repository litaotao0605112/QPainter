
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
typedef struct FLOWINFO
{
    double offset = 0;
    QColor arrowColor1;
    QColor arrowColor2;
    bool isMultiColor = false;
    double spacing;
    double speed = 0.01;
    int arrowCount = 10;
    bool directionFlag = false;
    int arrowType;
}FlowInfo;



class  CustomWidget : public QWidget
{
   Q_OBJECT
   public:
    explicit CustomWidget(QWidget *parent = nullptr);

    QTimer *timer;
private:
    void paintEvent(QPaintEvent *event) override;

public:
    void resizeEvent(QResizeEvent *event);
    void Set_DisplayEvent_IsShow(int id,bool state);
    void Set_TextChangeEvent(int id,QString data);

    QVector<bool> isSelect;
    QVector<QString> text;
    double m_offset = 0;
    QMap<int,FlowInfo>mapFlowInfo;
    double m_scale;
    QImage background;

    int  colorChange = 255;
};
#endif // SAMPLERACKWIDGET_H



