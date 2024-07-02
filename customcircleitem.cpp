#include "CustomCircleItem.h"

CustomCircleItem::CustomCircleItem(const QPointF &center, qreal radius)
    : QGraphicsEllipseItem(center.x() - radius, center.y() - radius, radius * 2, radius * 2), isResizing(false)
{
    setFlag(QGraphicsItem::ItemIsMovable);  // 设置图形项可移动
    setFlag(QGraphicsItem::ItemSendsGeometryChanges);  // 设置图形项发送几何变化
    setFlag(QGraphicsItem::ItemIsSelectable);  // 设置图形项可选中
}

void CustomCircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (isResizing) {
        qreal radius = QLineF(initialPos, event->scenePos()).length();  // 计算新半径
        setRect(initialPos.x() - radius, initialPos.y() - radius, radius * 2, radius * 2);  // 设置新的椭圆边界矩形
    } else {
        QGraphicsEllipseItem::mouseMoveEvent(event);  // 调用父类的鼠标移动事件处理
    }
}

void CustomCircleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) {  // 如果按下了Ctrl键
        isResizing = true;  // 开始调整大小
        initialPos = rect().center();  // 记录初始中心点位置
    } else {
        isResizing = false;  // 否则停止调整大小
        QGraphicsEllipseItem::mousePressEvent(event);  // 调用父类的鼠标按下事件处理
    }
}

void CustomCircleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    isResizing = false;  // 停止调整大小
    QGraphicsEllipseItem::mouseReleaseEvent(event);  // 调用父类的鼠标释放事件处理
}
