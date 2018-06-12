#include "painter.h"

static const int resizeHandleWidth = 6;

const QSizeF RouteRectangle::minSize(80, 50);

QMap<int,QString> PainterHelper::weekMap;

int PainterHelper::isGradient = 0;

void PainterHelper::initMap()
{
    PainterHelper::weekMap.insert(1,QString::fromUtf8("周一"));
    PainterHelper::weekMap.insert(2,QString::fromUtf8("周二"));
    PainterHelper::weekMap.insert(3,QString::fromUtf8("周三"));
    PainterHelper::weekMap.insert(4,QString::fromUtf8("周四"));
    PainterHelper::weekMap.insert(5,QString::fromUtf8("周五"));
    PainterHelper::weekMap.insert(6,QString::fromUtf8("周六"));
    PainterHelper::weekMap.insert(7,QString::fromUtf8("周日"));
}

Axis::Axis(int length)
    : m_rcBounding(0,0,0,0)
    , m_len(length)
{

}

Axis::~Axis()
{

}

void Axis::setStartPoint(const QPointF &pos)
{
    setPos(pos);
    m_startPosScene = pos;
}

void Axis::setEndPoint()
{
    m_endPosScene = QPointF(m_startPosScene.x() + m_len,m_startPosScene.y());
    qreal endX = m_endPosScene.x();
    qreal endY = m_endPosScene.y();
    qreal startX = m_startPosScene.x();
    qreal startY = m_startPosScene.y();

    m_rcBounding.setWidth(qAbs(endX-startX));
    m_rcBounding.setHeight(qAbs(endY-startY));

    QPointF startPoint;
    QPointF endPoint;

    startPoint.setX(0);
    endPoint.setX(m_rcBounding.width());

    startPoint.setY(0);
    endPoint.setY(0);

    m_line.setPoints(startPoint,endPoint);

    setPos(startX,startY);
}

void Axis::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->save();

    //painter->drawLine(m_startX,50,m_endX,50);

    QPen pen;

    pen.setStyle(Qt::DashLine);
    pen.setColor(Qt::red);           // 设置画笔为黄色
    painter->setPen(pen);

    painter->drawLine(m_line);

    painter->restore();

}

QRectF Axis::boundingRect() const
{
    return m_rcBounding;
}

XAxis::XAxis(int length)
    : m_rcBounding(0,0,0,0)
    , m_len(length)
{

}

XAxis::~XAxis()
{

}

void XAxis::setStartPoint(const QPointF &pos)
{
    setPos(pos);
    m_startPosScene = pos;
}

void XAxis::setEndPoint()
{
    m_endPosScene = QPointF(m_startPosScene.x() + m_len,m_startPosScene.y());
    qreal endX = m_endPosScene.x();
    qreal endY = m_endPosScene.y();
    qreal startX = m_startPosScene.x();
    qreal startY = m_startPosScene.y();

    m_rcBounding.setWidth(qAbs(endX-startX));
    m_rcBounding.setHeight(qAbs(endY-startY));

    m_rcBounding.adjust(0,-30,20,0);

    QPointF startPoint;
    QPointF endPoint;

    startPoint.setX(0);
    endPoint.setX(m_rcBounding.width());

    startPoint.setY(0);
    endPoint.setY(0);

    m_line.setPoints(startPoint,endPoint);

    setPos(startX,startY);
}

void XAxis::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->save();

    painter->drawLine(m_line);
    QFont font;
    font.setPixelSize(16);
    painter->setFont(font);

    for (int i = 1 ; i<=50 ; i++ ){

        if (m_endPosScene.x() < m_startPosScene.x() + i*100) {

            break;

        }else{
            painter->drawLine(m_rcBounding.bottomLeft().x()+i*100,0,m_rcBounding.bottomLeft().x()+i*100,-10);

            painter->drawText(m_rcBounding.bottomLeft().x()+i*100-10,-15,QString::number(i*100));
        }


    }

    painter->restore();

}

QRectF XAxis::boundingRect() const
{
    return m_rcBounding;
}

YAxis::YAxis(int day)
    : m_rcBounding(0,0,0,0)
    , m_day(day)
{

}

YAxis::~YAxis()
{

}

void YAxis::setStartPoint(const QPointF &pos)
{
    setPos(pos);
    m_startPosScene = pos;
}

void YAxis::setEndPoint()
{
    m_endPosScene = QPointF(m_startPosScene.x(),m_startPosScene.y()+24*12*m_day);
    qreal endY = m_endPosScene.y();

    qreal startX = m_startPosScene.x();
    qreal startY = m_startPosScene.y();

    m_rcBounding.setWidth(-100);
    m_rcBounding.setHeight(qAbs(endY-startY));

    QPointF startPoint;
    QPointF endPoint;

    startPoint.setX(0);
    endPoint.setX(0);

    startPoint.setY(0);
    endPoint.setY(m_rcBounding.height());

    m_line.setPoints(startPoint,endPoint);

    setPos(startX,startY);
}

void YAxis::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->save();

    //painter->drawLine(100,m_startY,100,m_endY);
    painter->drawLine(m_line);
    QFont font;
    font.setPixelSize(16);
    painter->setFont(font);

    for (int i = 1 ; i<=100 ; i++ ){

        if (m_endPosScene.y() < m_startPosScene.y() + i * 24) {

            break;

        }else{

            painter->drawLine(-5,m_rcBounding.topRight().y() + i * 24, 0 ,m_rcBounding.topRight().y() + i*24);
            painter->drawText(-50,m_rcBounding.topRight().y() + i * 24 + 4,QString::number(i%12 * 2)+":00");
        }
    }

    painter->restore();

}

QRectF YAxis::boundingRect() const
{
    return m_rcBounding;
}

RouteTable::RouteTable(QGraphicsItem *parent,int rows,int cols,int cellWidth,int cellHeight,QPointF basePoint)
    : QGraphicsItem(parent)
    , m_rows(rows)
    , m_cols(cols)
    , m_cellWidth(cellWidth)
    , m_cellHeight(cellHeight)
    , m_basePoint(basePoint)
    , m_rcBounding(0,0,0,0)
{
    m_rcBounding.setTopLeft(basePoint);
    m_rcBounding.setHeight(rows*cellHeight);
    m_rcBounding.setWidth(cols*cellWidth);
}

void RouteTable::setContentList(const QList<QString> &contents)
{
    m_contents = contents;
}

RouteTable::~RouteTable()
{

}

QRectF RouteTable::boundingRect() const
{
    return m_rcBounding;
}

void RouteTable::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    Q_UNUSED(option);

    painter->save();


    qreal startX = m_basePoint.x();
    qreal startY = m_basePoint.y();
    int i = 0;

    for (int row = 0; row<=m_rows; row++){

        for (int col = 0; col<m_cols; col++ ){

            painter->drawLine(startX + col*m_cellWidth,startY + row*m_cellHeight,startX + col*m_cellWidth + m_cellWidth,startY + row*m_cellHeight);

            if (row > 0 && i < m_contents.count())
            {
                QFont font;
                font.setBold(true);
                font.setPixelSize(15);
                painter->setFont(font);
                painter->drawText(startX + col*m_cellWidth+4,startY + row*m_cellHeight-4,m_contents.at(i++));
            }

        }
    }

    for (int col = 0; col<=m_cols; col++){

        for (int row = 0; row<m_rows; row++ ){

            painter->drawLine(startX + col*m_cellWidth,startY + row*m_cellHeight,startX + col*m_cellWidth,startY + row*m_cellHeight + m_cellHeight);

        }
    }

    painter->restore();

}

RouteRectangle::RouteRectangle(QGraphicsItem *parent,QString text,QString id1,QString id2,QString id3,QPointF basePoint,int fontSize)
    : QGraphicsItem(parent)
    , m_text(text)
    , m_id_1(id1)
    , m_id_2(id2)
    , m_id_3(id3)
    , m_rcBounding(0,0,0,0)
    , m_basePoint(basePoint)
    , m_font_size(fontSize)
{
    setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemSendsGeometryChanges);

}

RouteRectangle::~RouteRectangle()
{

}

void RouteRectangle::setStartPoint(const QPointF &pos)
{
    //setPos(pos);
    m_startPosScene = pos;
}

void RouteRectangle::setEndPoint(const QPointF &pos)
{
    m_endPosScene = pos;
    qreal endX = m_endPosScene.x();
    qreal endY = m_endPosScene.y();
    qreal startX = m_startPosScene.x();
    qreal startY = m_startPosScene.y();

    m_rcBounding.setWidth(qAbs(endX-startX));
    m_rcBounding.setHeight(qAbs(endY-startY));

    if (endX < startX)
    {
        startX = endX;
    }

    if (endY < startY)
    {
        startY = endY;
    }

    setPos(startX,startY);

}

QPointF RouteRectangle::getStartPosScene()
{
    return m_startPosScene;
}

QPointF RouteRectangle::getBasePoint()
{
    return m_basePoint;
}

QPointF RouteRectangle::getEndPosScene()
{
    return m_endPosScene;
}
void RouteRectangle::setFillColor(const QColor &color,const int alpha)
{
    m_fillColor = color;
    m_fillColor.setAlpha(alpha);

}

QRectF RouteRectangle::boundingRect() const
{
    return m_rcBounding;
}

QVariant RouteRectangle::itemChange(GraphicsItemChange change, const QVariant &value)
{
    //qDebug() << change;
    if(change == QGraphicsItem::ItemPositionChange && scene()){
        QPointF newPos = value.toPointF();

        if (this->pos().y() != newPos.y() ){

            newPos.setY(this->pos().y());
            //setPos(newPos);

        }
        return newPos;

    }

    return QGraphicsItem::itemChange(change,value);
}

QRectF RouteRectangle::resizeHandle() const
{
    QPointF br = m_rcBounding.bottomRight();
    return QRectF(br - QPointF(resizeHandleWidth, resizeHandleWidth), br);
}

void RouteRectangle::setFontSize(int fontSize)
{
    m_font_size = fontSize;
}

int RouteRectangle::getFontSize()
{
    return m_font_size;
}

void RouteRectangle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);

    painter->save();
    if (PainterHelper::isGradient)
        painter->setBrush(PainterHelper::gradient(m_fillColor, m_rcBounding));
    else
        painter->setBrush(m_fillColor);

    painter->setPen((option->state & QStyle::State_Selected) ? Qt::DashDotDotLine : Qt::SolidLine);


    painter->drawRect(m_rcBounding);

    QString text = m_text;

    QFont font;
    font.setBold(true);
    if (m_rcBounding.width() > 300 && m_rcBounding.height() > 200) {

        font.setPixelSize(m_font_size+5);

    }else{

        font.setPixelSize(m_font_size);

    }

    painter->setFont(font);

    QFontMetrics fm = painter->fontMetrics();

    QRect rect = option->rect;

    rect.adjust(4,4,-4,-4);

    int writeWidth =  ((rect.height()-8) / fm.height() -1) * (rect.width()-8) ;

    rect.adjust(-10,-10,10,10);

    QString strElidedText = fm.elidedText(text, Qt::ElideRight, writeWidth, Qt::TextShowMnemonic);

    painter->drawText(rect, Qt::TextWordWrap | Qt::AlignVCenter |Qt::AlignHCenter, strElidedText);

    painter->setBrush(Qt::white);
    painter->drawRect(resizeHandle().adjusted(0, 0, 1, 1));

    painter->restore();

}

void RouteRectangle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //Q_UNUSED(event);

    m_resizeHandlePressed = resizeHandle().contains(event->pos());

    if (m_resizeHandlePressed){

        setFlag(QGraphicsItem::ItemIsMovable,false);
        m_mousePressOffset = m_rcBounding.bottomRight() - event->pos();
        return;
    }

    if (boundingRect().contains(event->pos())){


        setFlag(QGraphicsItem::ItemIsMovable,true);
        qDebug() << " in item ";
        setSelected(true);

        m_mousePressOffset = event->pos() - m_rcBounding.topLeft();

    }else{

        setSelected(false);

        QGraphicsItem::mousePressEvent(event);
        event->accept();

    }




}

void RouteRectangle::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if (m_resizeHandlePressed) {

        //setStartPoint(m_startPosScene);
        //setEndPoint(event->pos());

        //m_rcBounding = QRectF(m_rcBounding.topLeft(), event->pos() + m_mousePressOffset);

        m_rcBounding = QRectF(m_rcBounding.topLeft(), QPointF(event->pos().x()+m_mousePressOffset.x(),m_rcBounding.bottomRight().y() ));

    }else{



        //m_rcBounding.moveLeft(event->pos().x() - m_mousePressOffset.x());
        //m_rcBounding.moveTopLeft(event->pos() - m_mousePressOffset);

    }

    QSizeF size = m_rcBounding.size().expandedTo(RouteRectangle::minSize);
    m_rcBounding.setSize(size);

    QGraphicsItem::mouseMoveEvent(event);

}

void RouteRectangle::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    m_resizeHandlePressed = false;
    setFlag(QGraphicsItem::ItemIsMovable,false);
    //


    m_startPosScene = this->pos();
    m_endPosScene.setX(this->pos().x() + m_rcBounding.width());
    m_endPosScene.setY(this->pos().y() + m_rcBounding.height());

    QGraphicsItem::mouseReleaseEvent(event);

}

int RouteRectangle::type() const
{
    return UserType + 1;
}

QString RouteRectangle::getText()
{
    return m_text;
}

QString RouteRectangle::getId1()
{
    return m_id_1;
}

QString RouteRectangle::getId2()
{
    return m_id_2;
}

QString RouteRectangle::getId3()
{
    return m_id_3;
}


PainterView::PainterView(QWidget *parent)
    : QGraphicsView(parent)
    , m_zoomDelta(0.1)
    , m_scale(1.0)
    , m_height(2500)

{
    setRenderHint(QPainter::Antialiasing,true);
    setAttribute(Qt::WA_DeleteOnClose);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

PainterView::~PainterView()
{
    scene()->clear();
}

void PainterView::setZoomDelta(qreal delta)
{
    // 建议增量范围
    Q_ASSERT_X(delta >= 0.0 && delta <= 1.0,
               "InteractiveView::setZoomDelta", "Delta should be in range [0.0, 1.0].");
    m_zoomDelta = delta;
}

void PainterView::setHeight(int height)
{
    m_height = height;
}

void PainterView::wheelEvent(QWheelEvent *event)
{
    // 滚轮的滚动量
    //this->centerOn(event->pos());
    QPoint scrollAmount = event->angleDelta();
    // 正值表示滚轮远离使用者（放大），负值表示朝向使用者（缩小）
    scrollAmount.y() > 0 ? zoomIn() : zoomOut();
    QGraphicsView::wheelEvent(event);
}

// 放大
void PainterView::zoomIn()
{
    zoom(1 + m_zoomDelta);
}

// 缩小
void PainterView::zoomOut()
{
    zoom(1 - m_zoomDelta);
}

// 缩放 - scaleFactor：缩放的比例因子
void PainterView::zoom(float scaleFactor)
{
    // 防止过小或过大
    qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
    if (factor < 0.1 || factor > 100)
        return;
    //
    scale(scaleFactor, scaleFactor);



    m_scale *= scaleFactor;

    //this->resetMatrix();

}


void PainterView::resizeEvent(QResizeEvent *event)
{

    QGraphicsView::resizeEvent(event);

    //const QSize &size = event->size();
    PainterScene * s = static_cast<PainterScene *> (scene());
    if(s != NULL){
        //QRectF rc(0,0,size.width(),size.height());

        QRectF rc(0,0,s->width(),m_height);

        setSceneRect(rc);
        //s->setSceneRect(rc);
        //XAxis * xAxis = s->getXAxis();
        //YAxis * yAxis = s->getYAxis();
        //xAxis->setXAxisLength(size.width()-30);
        //yAxis->setYAxisLength(size.height()-30);
        s->update();
    }


}

void PainterView::closeEvent(QCloseEvent *event) {

    Q_UNUSED(event);
    emit exit(0);

}


PainterScene::PainterScene(QObject * parent)
    : QGraphicsScene(parent)
    , m_currentRect(nullptr)
{

}

PainterScene::~PainterScene()
{

}

void PainterScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsScene::mousePressEvent(event);
    qDebug() << " in scene ";

}

void PainterScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsScene::mouseMoveEvent(event);

}

void PainterScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseReleaseEvent(event);
    update();
}

void PainterScene::setCurrentRect(RouteRectangle * routeRectangle)
{
    m_currentRect = routeRectangle;
}

void PainterScene::setAxis(XAxis * xAxis,YAxis * yAxis)
{
    m_xAxis = xAxis;
    m_yAxis = yAxis;
}

XAxis * PainterScene::getXAxis()
{
    return m_xAxis;
}

YAxis * PainterScene::getYAxis()
{
    return m_yAxis;
}

void PainterScene::textSize(const QString &p)
{
    QList<QGraphicsItem *> list = this->selectedItems();
    foreach(QGraphicsItem *item,list){
        if (item->type() == QGraphicsItem::UserType + 1){
            RouteRectangle * routeItem = static_cast<RouteRectangle *> (item);
            routeItem->setFontSize(p.toInt());
            routeItem->update();
        }
    }

}
