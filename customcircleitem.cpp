#include "customcircleitem.h"

QTextStream test_is_code_right_circleitems(stdout);
using Qt::endl;

CustomCircleItem::CustomCircleItem(qreal x, qreal y, qreal diameter, QGraphicsItem *parent)
    : QGraphicsEllipseItem(x - diameter / 2, y - diameter / 2, diameter, diameter, parent),
    //版本1、2
    center(x, y),  // 设置圆心坐标

    radius(diameter / 2),  // 设置半径
    selected(false) {  // 初始化未被选中
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemSendsGeometryChanges | QGraphicsItem::ItemIsSelectable);  // 设置图形项的标志
    setAcceptHoverEvents(true);  // 允许接受悬停事件

    centerPoint = new QGraphicsEllipseItem(x - 6, y - 6, 12, 12, this);  // 创建圆心标记项
    centerPoint->setBrush(Qt::red);  // 设置圆心标记的颜色为红色
    centerPoint->setVisible(false);  // 初始隐藏圆心标记

    radiusPoint = new QGraphicsEllipseItem(x + radius - 4, y - 4, 8, 8, this);  // 创建半径端点标记项
    radiusPoint->setBrush(Qt::green);  // 设置半径端点标记的颜色为绿色
    radiusPoint->setVisible(false);  // 初始隐藏半径端点标记

    radiusLine = new QGraphicsLineItem(QLineF(center, QPointF(x + radius, y)), this);  // 创建半径线条项
    //radiusLine->setPen(QPen(RGB(255, 255, 255)));  // 设置半径线条的颜色为蓝色
    radiusLine->setVisible(false);  // 初始隐藏半径线条
    test_is_code_right_circleitems<<"centre is: "<<x<<','<<y<<endl;

    QString msg = QString("%1, %2, %3").arg(x).arg(y).arg(radius);
    //emit drawingFinished(msg);

}

void CustomCircleItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsEllipseItem::mousePressEvent(event);  // 调用基类的鼠标按下事件处理函数

    selected = isSelected();  // 切换选中状态
    // centerPoint->setVisible(selected);  // 设置圆心标记可见性
    // radiusPoint->setVisible(selected);  // 设置半径端点标记可见性
    // radiusLine->setVisible(selected);  // 设置半径线条可见性

}

void CustomCircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {

    if (!isSelected()) {  // 如果未选中
        QGraphicsEllipseItem::mouseMoveEvent(event);  // 调用基类的鼠标移动事件处理函数
        return;
    }
    if (isSelected()&&radiusPoint->isUnderMouse() ) {  // 如果选中并且鼠标位于半径端点标记上&& radiusPoint->isUnderMouse()
        //版本2
        QPointF newPoint = event->pos();  // 获取新的点位置
        QPointF tempcenter = mapFromScene(center);

        radius = QLineF(mapFromScene(center), newPoint).length();  // 计算新的半径

        setRect(tempcenter.x() - radius, tempcenter.y() - radius, radius * 2, radius * 2);  // 更新圆形项的位置和大小
        update();
        QString msg = QString("%1, %2, %3").arg(newPoint.x()).arg(newPoint.y()).arg(radius);
        //emit drawingFinished(msg);
    }
    QGraphicsEllipseItem::mouseMoveEvent(event);  // 调用基类的鼠标移动事件处理函数
}


void CustomCircleItem::wheelEvent(QGraphicsSceneWheelEvent *event) {
    // 检查 Ctrl 键是否按下
    if (event->modifiers() & Qt::ControlModifier) {
        // 查找当前选中的圆
        auto items = scene()->selectedItems();
        for (auto item : items) {
            auto circleItem = dynamic_cast<CustomCircleItem *>(item);
            if (circleItem) {
                if (radius < 0) radius = abs(radius);
                if (event->delta() > 0)
                    radius *= abs(1.02 * (event->delta() / 120));  // 计算新的半径
                else
                    radius *= abs(0.98 * (event->delta() / 120));

                setRect(mapFromScene(center).x() - radius, mapFromScene(center).y() - radius, radius * 2, radius * 2);  // 更新圆形项的位置和大小
            }
        }
        event->accept();
    } else {
        QGraphicsEllipseItem::wheelEvent(event);
    }
}

void CustomCircleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {

    qreal initialRadius = rect().width() / 2;

    QPointF initialPos = QPointF(rect().x() + rect().width() / 2, rect().y() + rect().height() / 2);


    center=mapToScene(initialPos);
    radius=initialRadius;        // 更新圆形项的位置和大小

    update();
    QGraphicsEllipseItem::mouseReleaseEvent(event);  // 调用基类的鼠标释放事件处理函数
}

// void CustomCircleItem::updateCircle() {//没用
//     // setRect(center.x() - radius, center.y() - radius, radius * 2, radius * 2);  // 更新圆形项的位置和大小
//     // centerPoint->setRect(center.x() - 3, center.y() - 3, 6, 6);  // 更新圆心标记的位置和大小
//     // radiusPoint->setRect(center.x() + radius - 3, center.y() - 3, 6, 6);  // 更新半径端点标记的位置和大小
//     // radiusLine->setLine(QLineF(center, QPointF(center.x() + radius, center.y())));  // 更新半径线条的位置和方向

// }
void CustomCircleItem::changeByLineedit(double cx,double cy,double r)
{
    if (!isSelected()) {  // 如果未选中
        return;
    }
    QPointF c=QPointF{cx,cy};
    center=c;
    radius = r;  // 计算新的半径
    setRect(center.x() - radius, center.y() - radius, radius * 2, radius * 2);  // 更新圆形项的位置和大小
    update();
}
