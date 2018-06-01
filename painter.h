#ifndef PAINTER_H
#define PAINTER_H

#include <QtWidgets>
#include <QVariant>

class PainterHelper
{
public:

    static QMap<int, QString> weekMap;

    static void initMap();

    static QGradient gradient(const QColor &color, const QRectF &rect)
    {
        QColor c = color;
        c.setAlpha(200);
        QLinearGradient result(rect.topLeft(), rect.bottomRight());
        result.setColorAt(0, c.dark(150));
        result.setColorAt(0.5, c.light(200));
        result.setColorAt(1, c.dark(150));
        return result;
    }

    static int convertTimeToMinute(const QString &time,const int day)
    {
        int hour;
        int minute;

        hour = (time.left(2).left(1) == "0") ? time.left(2).mid(1,1).toInt() : time.left(2).toInt();
        minute = (time.right(2).left(1)=="0") ? time.right(2).mid(1,1).toInt() : time.right(2).toInt();

        return hour*60+minute+(day-1)*24*60;
    }

    static QPointF convertToScreenTopLeft(const int startBerthPonit, const QString &startTime,const int day,const QPointF &basePoint)
    {
        QPointF result;
        result.setX(basePoint.x()+startBerthPonit);

        result.setY(basePoint.y()+0.2*convertTimeToMinute(startTime,day));
        return result;
    }

    static QPointF convertToScreenBottomRight(const int startBerthPoint, const int length, const QString &endTime,const int day,const QPointF &basePoint)
    {
        QPointF result;
        result.setX(basePoint.x()+startBerthPoint+length);

        result.setY(basePoint.y()+0.2*convertTimeToMinute(endTime,day));
        return result;
    }

    static int convertScreenToLength(const QPointF &topLeft, const QPointF &bottomRight)
    {
        return bottomRight.x() - topLeft.x();
    }

    static QString convertScreenToStartTime(const QPointF &topLeft,const QPointF &basePoint)
    {
        int passByMinute;
        passByMinute = (qRound((topLeft.y())-basePoint.y())*5) % (24 * 60);



        int minute = passByMinute % 60;

        int hour = passByMinute / 60;

        qDebug() << "pass by m:" << passByMinute;
        qDebug() << "minute:" << minute;
        qDebug() << "hour:" << hour;

        return QString("%1").arg(hour,2,10,QLatin1Char('0'))+":"+QString("%1").arg(minute,2,10,QLatin1Char('0'));


    }

    static int convertScreenToStartBerthPoint(const QPointF &topLeft,const QPointF &basePoint)
    {
        return topLeft.x() - basePoint.x();
    }

    static QString convertScreenToEndTime(const QPointF &bottomRight,const QPointF &basePoint)
    {
        int passByMinute;
        passByMinute = (qRound((bottomRight.y())-basePoint.y())*5) % (24 * 60);

        int minute = passByMinute % 60;

        int hour = passByMinute / 60;

        return QString("%1").arg(hour,2,10,QLatin1Char('0'))+":"+QString("%1").arg(minute,2,10,QLatin1Char('0'));
    }

    static int convertScreenToStartWeekDay(const QPointF &topLeft,const QPointF &basePoint)
    {
        return (qRound((topLeft.y())-basePoint.y())*5) / (24 * 60) + 1;
    }

    static int convertScreenToEndWeekDay(const QPointF &bottomRight,const QPointF &basePoint)
    {
        return (qRound((bottomRight.y())-basePoint.y())*5) / (24 * 60) + 1;
    }

};



class Axis : public QGraphicsItem
{

public:
    Axis(int length=1500);
    ~Axis();

    QRectF boundingRect() const override;
    void setStartPoint(const QPointF &pos);
    void setEndPoint();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
protected:

    QPointF m_startPosScene;
    QPointF m_endPosScene;
    QRectF m_rcBounding;
    QLineF m_line;
    int m_len;

};

class XAxis : public QGraphicsItem
{

public:
    XAxis(int length=1500);
    ~XAxis();

    QRectF boundingRect() const override;

    void setStartPoint(const QPointF &pos);
    void setEndPoint();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
protected:

    QPointF m_startPosScene;
    QPointF m_endPosScene;
    QRectF m_rcBounding;
    QLineF m_line;
    int m_len;

};

class YAxis : public QGraphicsItem
{

public:
    YAxis(int day=8);
    ~YAxis();

    QRectF boundingRect() const override;
    void setStartPoint(const QPointF &pos);
    void setEndPoint();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
protected:

    QPointF m_startPosScene;
    QPointF m_endPosScene;
    QRectF m_rcBounding;
    QLineF m_line;

    int m_day;

};

class RouteRectangle : public QGraphicsItem
{
public:
    RouteRectangle(QGraphicsItem *parent,QString text,QString id1,QString id2,QString id3, QPointF basePoint);
    ~RouteRectangle();

    QRectF boundingRect() const override;

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

    void setStartPoint(const QPointF &pos);
    void setEndPoint(const QPointF &pos);
    void setFillColor(const QColor &color);

    QString getText();

    QString getId1();
    QString getId2();
    QString getId3();

    QPointF getStartPosScene();

    QPointF getEndPosScene();

    QRectF resizeHandle() const;

    QPointF getBasePoint();

    static const QSizeF minSize;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:

    QString m_text;
    QString m_id_1;
    QString m_id_2;
    QString m_id_3;
    QPointF m_startPosScene;
    QPointF m_endPosScene;
    QRectF m_rcBounding;
    QColor m_fillColor;

    QPointF m_basePoint;

    bool m_bResizing;
    bool m_resizeHandlePressed;
    QPointF m_mousePressOffset;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    int type() const;

};

class PainterView : public QGraphicsView
{
    Q_OBJECT

public:
    PainterView(QWidget *parent = 0);
    ~PainterView();

    void setZoomDelta(qreal delta);
    qreal zoomDelta() const;
    void closeEvent(QCloseEvent *event);
    void setHeight(int height);
signals:

    void exit(int type);

protected:
    void resizeEvent(QResizeEvent * event);
    void wheelEvent(QWheelEvent *event) override;
public slots:
    void zoomIn();  // 放大
    void zoomOut();  // 缩小
    void zoom(float scaleFactor);
private:
    qreal m_zoomDelta;  // 缩放的增量
    qreal m_scale;  // 缩放值
    int m_height;

};

class PainterScene : public QGraphicsScene
{
    Q_OBJECT
public:
    PainterScene(QObject * parent = 0);
    ~PainterScene();

    void setAxis(XAxis * xAxis,YAxis * yAxis);

    XAxis * getXAxis();

    YAxis * getYAxis();

    void setCurrentRect(RouteRectangle * routeRectangle);

protected:

    XAxis * m_xAxis;
    YAxis * m_yAxis;

    RouteRectangle *m_currentRect;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

};

#endif // PAINTER_H
