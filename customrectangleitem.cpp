#include "CustomRectangleItem.h" // 包含自定义矩形项头文件

// CustomRectangleItem构造函数，接受一个QRectF对象和一个父图形项
CustomRectangleItem::CustomRectangleItem(const QRectF &rect, QGraphicsItem *parent)
    : QGraphicsRectItem(rect, parent), // 调用基类QGraphicsRectItem的构造函数
    isDraggingTopLeftHandle(false), // 初始化左上角句柄拖动状态为false
    isDraggingBottomRightHandle(false), // 初始化右下角句柄拖动状态为false
    isDraggingRect(false)
{
    setFlag(QGraphicsItem::ItemIsMovable, true); // 设置矩形项可以移动
    setFlag(QGraphicsItem::ItemIsSelectable, true); // 设置矩形项可以被选择

    QPen pen(Qt::black); // 创建一个黑色画笔
    pen.setWidth(2); // 设置画笔宽度为2像素
    setPen(pen); // 应用画笔到矩形项

    this->LT=mapToScene(this->rect().topLeft());
    this->RB=mapToScene(this->rect().bottomRight());

    topLeftHandle = new QGraphicsEllipseItem(-3, -3, 6, 6, this); // 创建左上角句柄
    topLeftHandle->setBrush(Qt::green); // 设置句柄填充颜色为绿色
    topLeftHandle->setFlag(QGraphicsItem::ItemIsMovable, false); // 设置句柄不可移动
    topLeftHandle->setFlag(QGraphicsItem::ItemIsSelectable, false); // 设置句柄不可选择

    bottomRightHandle = new QGraphicsEllipseItem(-3, -3, 6, 6, this); // 创建右下角句柄
    bottomRightHandle->setBrush(Qt::green); // 设置句柄填充颜色为绿色
    bottomRightHandle->setFlag(QGraphicsItem::ItemIsMovable, false); // 设置句柄不可移动
    bottomRightHandle->setFlag(QGraphicsItem::ItemIsSelectable, false); // 设置句柄不可选择

    updateHandlesPosition(); // 更新句柄位置
}

// 更新句柄位置的函数
void CustomRectangleItem::updateHandlesPosition() {
    QRectF rect = this->rect(); // 获取矩形项的矩形
    topLeftHandle->setPos(rect.topLeft()); // 设置左上角句柄的位置
    bottomRightHandle->setPos(rect.bottomRight()); // 设置右下角句柄的位置
}

// 鼠标按下事件处理函数
void CustomRectangleItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    qDebug()<<QLineF(LT, event->scenePos()).length()<<"len"<<Qt::endl;
    // 如果鼠标位置接近左上角句柄，设置拖动状态为拖动左上角句柄
    if (QLineF(LT, event->scenePos()).length() < 7) {
        isDraggingTopLeftHandle = true;
    }
    else if (QLineF(RB, event->scenePos()).length() < 7) { // 如果鼠标位置接近右下角句柄，设置拖动状态为拖动右下角句柄
        isDraggingBottomRightHandle = true;
    }
    else if(isSelected())
    {
        isDraggingRect=true;
    }

    QGraphicsRectItem::mousePressEvent(event); // 调用基类的鼠标按下事件处理函数
}

// 鼠标移动事件处理函数
void CustomRectangleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isDraggingTopLeftHandle)
    { // 如果正在拖动左上角句柄，更新矩形的位置和大小
        QPointF tempLT=event->scenePos();
        if(tempLT.x()<RB.x()-10&&tempLT.y()<RB.y()-10)
        {
            LT=event->scenePos();
            QRectF newRect(mapFromScene(LT), mapFromScene(RB));
            setRect(newRect);
        }
    }
    else if (isDraggingBottomRightHandle)
    { // 如果正在拖动右下角句柄，更新矩形的位置和大小
        QPointF tempRB=event->scenePos();
        if(tempRB.x()-10>LT.x()&&tempRB.y()-10>LT.y())
        {
            RB=event->scenePos();
            QRectF newRect(mapFromScene(LT), mapFromScene(RB));
            setRect(newRect);
        }
    }
    else if (isDraggingRect)
    {
        RB=mapToScene(rect().bottomRight());
        LT=mapToScene(rect().topLeft());
        qDebug()<<"RB: "<<RB<<" LT: "<<LT<<" pos: "<<event->scenePos()<<Qt::endl;
    }
    updateHandlesPosition(); // 更新句柄位置
    update();
    QGraphicsRectItem::mouseMoveEvent(event); // 调用基类的鼠标移动事件处理函数
}

// 鼠标释放事件处理函数
void CustomRectangleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    isDraggingTopLeftHandle = false; // 重置左上角句柄拖动状态为false
    isDraggingBottomRightHandle = false; // 重置右下角句柄拖动状态为false
    isDraggingRect=false;
    QGraphicsRectItem::mouseReleaseEvent(event); // 调用基类的鼠标释放事件处理函数
}
