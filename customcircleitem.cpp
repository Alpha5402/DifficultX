#include "CustomCircleItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QBrush>
#include <QGraphicsLineItem>

QTextStream test_is_code_right_circleitems(stdout);
using Qt::endl;

CustomCircleItem::CustomCircleItem(qreal x, qreal y, qreal diameter, QGraphicsItem *parent)
    : QGraphicsEllipseItem(x - diameter / 2, y - diameter / 2, diameter, diameter, parent),
    center(x, y),  // 设置圆心坐标
    radius(diameter / 2),  // 设置半径
    selected(false) {  // 初始化未被选中
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemIsSelectable);  // 设置图形项的标志
    setAcceptHoverEvents(true);  // 允许接受悬停事件

    centerPoint = new QGraphicsEllipseItem(x - 3, y - 3, 6, 6, this);  // 创建圆心标记项
    centerPoint->setBrush(Qt::red);  // 设置圆心标记的颜色为红色
    centerPoint->setVisible(false);  // 初始隐藏圆心标记

    radiusPoint = new QGraphicsEllipseItem(x + radius - 3, y - 3, 6, 6, this);  // 创建半径端点标记项
    radiusPoint->setBrush(Qt::green);  // 设置半径端点标记的颜色为绿色
    radiusPoint->setVisible(false);  // 初始隐藏半径端点标记

    radiusLine = new QGraphicsLineItem(QLineF(center, QPointF(x + radius, y)), this);  // 创建半径线条项
    radiusLine->setPen(QPen(Qt::blue));  // 设置半径线条的颜色为蓝色
    radiusLine->setVisible(false);  // 初始隐藏半径线条
    test_is_code_right_circleitems<<"centre is: "<<x<<','<<y<<endl;
}

void CustomCircleItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsEllipseItem::mousePressEvent(event);  // 调用基类的鼠标按下事件处理函数
    selected = !selected;  // 切换选中状态
    centerPoint->setVisible(selected);  // 设置圆心标记可见性
    radiusPoint->setVisible(selected);  // 设置半径端点标记可见性
    radiusLine->setVisible(selected);  // 设置半径线条可见性
}

void CustomCircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (selected && centerPoint->isUnderMouse()) {  // 如果选中并且鼠标位于圆心标记上
        center = event->scenePos();  // 更新圆心位置
        setRect(center.x() - radius, center.y() - radius, radius * 2, radius * 2);  // 更新圆形项的位置和大小
        centerPoint->setRect(center.x() - 3, center.y() - 3, 6, 6);  // 更新圆心标记的位置和大小
        radiusLine->setLine(QLineF(center, radiusPoint->rect().center()));  // 更新半径线条的位置和方向
    } else if (selected && radiusPoint->isUnderMouse()) {  // 如果选中并且鼠标位于半径端点标记上
        QPointF newPoint = event->scenePos();  // 获取新的点位置
        radius = QLineF(center, newPoint).length();  // 计算新的半径
        setRect(center.x() - radius, center.y() - radius, radius * 2, radius * 2);  // 更新圆形项的位置和大小
        radiusPoint->setRect(newPoint.x() - 3, newPoint.y() - 3, 6, 6);  // 更新半径端点标记的位置和大小
        radiusLine->setLine(QLineF(center, newPoint));  // 更新半径线条的位置和方向
        test_is_code_right_circleitems<<"centre is: "<<newPoint.x()<<','<<newPoint.y()<<endl;
    }
    QGraphicsEllipseItem::mouseMoveEvent(event);  // 调用基类的鼠标移动事件处理函数
}

void CustomCircleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsEllipseItem::mouseReleaseEvent(event);  // 调用基类的鼠标释放事件处理函数
}

void CustomCircleItem::updateCircle() {
    setRect(center.x() - radius, center.y() - radius, radius * 2, radius * 2);  // 更新圆形项的位置和大小
    centerPoint->setRect(center.x() - 3, center.y() - 3, 6, 6);  // 更新圆心标记的位置和大小
    radiusPoint->setRect(center.x() + radius - 3, center.y() - 3, 6, 6);  // 更新半径端点标记的位置和大小
    radiusLine->setLine(QLineF(center, QPointF(center.x() + radius, center.y())));  // 更新半径线条的位置和方向
}
