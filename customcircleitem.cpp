#include "CustomCircleItem.h"
#include <QGraphicsSceneMouseEvent>

CustomCircleItem::CustomCircleItem(qreal x, qreal y, qreal diameter, QGraphicsItem *parent)
    : QGraphicsEllipseItem(x - diameter / 2, y - diameter / 2, diameter, diameter, parent),
    center(x, y), radius(diameter / 2) {
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
}

QVariant CustomCircleItem::itemChange(GraphicsItemChange change, const QVariant &value) {
    if (change == ItemPositionChange && scene()) {
        // 圆形项位置改变时的处理逻辑
    }
    return QGraphicsItem::itemChange(change, value);
}

void CustomCircleItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mousePressEvent(event);
    // 处理鼠标按下事件
}

void CustomCircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseMoveEvent(event);
    // 处理鼠标移动事件
}

void CustomCircleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
    // 处理鼠标释放事件
}
