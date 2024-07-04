#include "CustomLineItem.h"
#include <QPen>
#include <QBrush>

CustomLineItem::CustomLineItem(const QLineF &line, QGraphicsItem *parent)
    : QGraphicsLineItem(line, parent), dragState(NoDrag)
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPen(QPen(Qt::black, 3));

    point1=mapToScene(line.p1());
    point2=mapToScene(line.p2());
    // 创建起点和终点的绿色标记
    startPoint = new QGraphicsEllipseItem(-5, -5, 10, 10, this);
    startPoint->setBrush(QBrush(Qt::green));
    qDebug()<<line.center();
    endPoint = new QGraphicsEllipseItem(-5, -5, 10, 10, this);
    endPoint->setBrush(QBrush(Qt::green));
    startPoint->setVisible(false);
    endPoint->setVisible(false);

    updateEndpoints();
}

void CustomLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF mousePos = event->pos();
    QLineF line = this->line();

    if (QLineF(line.p1(), mousePos).length() < 25) {
        dragState = DraggingStartPoint;
    } else if (QLineF(line.p2(), mousePos).length() < 25) {
        dragState = DraggingEndPoint;
    } else if (this->isSelected()) {
        dragState = DraggingLine;
    } else {
        dragState = NoDrag;
    }

    lastMousePos = mousePos;
    QGraphicsLineItem::mousePressEvent(event);
}

void CustomLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QPointF mousePos = event->scenePos();
    QLineF line = this->line();

    switch (dragState) {
    case DraggingEndPoint:
        line.setP2(mapFromScene(mousePos));
        point2=mapToScene(line.p2());
        break;
    case DraggingStartPoint:
        line.setP1(mapFromScene(mousePos));
        point1=mapToScene(line.p1());
        break;
    case DraggingLine:
        point1=mapToScene(line.p1());
        point2=mapToScene(line.p2());
        break;
    default:
        break;
    }

    setLine(QLineF(mapFromScene(point1),mapFromScene(point2)));//更新直线
    // startPoint->setRect(mapFromScene(point1).x()-6,mapFromScene(point1).y()-6,12,12);
    // endPoint->setRect(mapFromScene(point2).x()-6,mapFromScene(point2).y()-6,12,12);
    // startPoint->setRect(mapToParent(point1).x()-6,mapToParent(point1).y()-6,12,12);
    // endPoint->setRect(mapToScene(point2).x()-6,mapToScene(point2).y()-6,12,12);
    // startPoint->setRect(point1.x()-6,point1.y()-6,12,12);
    // endPoint->setRect(point2.x()-6,point2.y()-6,12,12);
    // startPoint->setRect(-6,-6,12,12);
    // endPoint->setRect(-6,-6,12,12);

    update();
    // updateEndpoints();
    lastMousePos = mousePos;
    // qDebug()<<dragState<<"line"<<line.p1()<<mapFromScene(mapToScene(line.p1()))<<','<<mapToParent(line.p2())<<',,'<<event->pos()<<",,"<<event->scenePos();
    QGraphicsLineItem::mouseMoveEvent(event);
}

void CustomLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {//这里，直接粘
    dragState = NoDrag;
    QGraphicsLineItem::mouseReleaseEvent(event);
}

QVariant CustomLineItem::itemChange(GraphicsItemChange change, const QVariant &value) {//这里
    if (change == QGraphicsItem::ItemPositionChange || change == QGraphicsItem::ItemScenePositionHasChanged) {
        updateEndpoints();
    }
    return QGraphicsLineItem::itemChange(change, value);
}

void CustomLineItem::updateEndpoints() {//这里
    QLineF line = this->line();
    startPoint->setPos(line.p1());
    endPoint->setPos(line.p2());
}

