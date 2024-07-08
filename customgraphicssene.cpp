#include "customgraphicssene.h"

void CustomGraphicsScene::DrawCircle(double x, double y, double Radius){
    circle = new CustomCircleItem(x, y, Radius);  // 创建自定义圆形项
    addItem(circle);  // 添加圆形项到场景
}

void CustomGraphicsScene::updateData(){
    //QString msg = QString("%1, %2, %3").arg(firstPoint.x()).arg(firstPoint.y()).arg(Radius);
    auto items = selectedItems();
    for (auto item : items) {
        if (CustomCircleItem *circleItem = dynamic_cast<CustomCircleItem*>(item)) {  // 更新圆形项的位置和大小
            //circleItem->center.setX(value);
            QString msg = QString("%1, %2, %3").arg(circleItem->center.x()).arg(circleItem->center.y()).arg(circleItem->radius);
            emit drawingCircleFinished(msg);
        } else if (CustomLineItem *lineItem = dynamic_cast<CustomLineItem*>(item)) {  // 更新圆形项的位置和大小
            //circleItem->center.setX(value);
            QString msg = QString("%1, %2, %3, %4").arg(lineItem->point1.x()).arg(lineItem->point1.y()).arg(lineItem->point2.x()).arg(lineItem->point2.y());
            //qDebug() << "[INFO] Pass the msg " << msg;
            emit drawingLineFinished(msg);
        }
    }
    //itemAt()
}

void CustomGraphicsScene::ReceivePara1ValueChanged(double value){
    qDebug() << "[INFO] CGS received value " << value << Qt::endl;
    auto items = selectedItems();
    for (auto item : items) {
        //auto circleItem = dynamic_cast<CustomCircleItem *>(item);
        if (CustomCircleItem *circleItem = dynamic_cast<CustomCircleItem*>(item)) {  // 更新圆形项的位置和大小
            //circleItem->center.setX(value);
            circleItem->changeByLineedit(value, circleItem->center.y(), circleItem->radius);
        } else if (CustomLineItem *lineItem = dynamic_cast<CustomLineItem*>(item)) {
            lineItem->changeByLineedit(value, lineItem->point1.y(), lineItem->point2.x(), lineItem->point2.y());
        }
    }
}

void CustomGraphicsScene::ReceivePara2ValueChanged(double value){
    //qDebug() << "[INFO] CGS received value " << value << Qt::endl;
    auto items = selectedItems();
    for (auto item : items) {
        //auto circleItem = dynamic_cast<CustomCircleItem *>(item);
        if (CustomCircleItem *circleItem = dynamic_cast<CustomCircleItem*>(item)) {  // 更新圆形项的位置和大小
            //circleItem->center.setX(value);
            circleItem->changeByLineedit(circleItem->center.x(), value, circleItem->radius);
        } else if (CustomLineItem *lineItem = dynamic_cast<CustomLineItem*>(item)) {
            lineItem->changeByLineedit(lineItem->point1.x(), value, lineItem->point2.x(), lineItem->point2.y());
        }
    }
}

void CustomGraphicsScene::ReceivePara3ValueChanged(double value){
    qDebug() << "[INFO] CGS received value " << value << Qt::endl;
    auto items = selectedItems();
    for (auto item : items) {
        //auto circleItem = dynamic_cast<CustomCircleItem *>(item);
        if (CustomCircleItem *circleItem = dynamic_cast<CustomCircleItem*>(item)) {  // 更新圆形项的位置和大小
            //circleItem->center.setX(value);
            circleItem->changeByLineedit(circleItem->center.x(), circleItem->center.y(), value);
        } else if (CustomLineItem *lineItem = dynamic_cast<CustomLineItem*>(item)) {
            lineItem->changeByLineedit(lineItem->point1.x(), lineItem->point1.y(), value, lineItem->point2.y());
        }
    }
}

void CustomGraphicsScene::ReceivePara4ValueChanged(double value){
    qDebug() << "[INFO] CGS received value " << value << Qt::endl;
    auto items = selectedItems();
    for (auto item : items) {
        //auto circleItem = dynamic_cast<CustomCircleItem *>(item);
        if (CustomLineItem *lineItem = dynamic_cast<CustomLineItem*>(item)) {
            lineItem->changeByLineedit(lineItem->point1.x(), lineItem->point1.y(), lineItem->point2.x(), value);
        }
    }
}

CustomGraphicsScene::CustomGraphicsScene(QObject *parent, QPen color)
    : QGraphicsScene(parent),
    circle(nullptr),  // 初始化圆形项为nullptr
    line(nullptr),
    centerPoint(nullptr),  // 初始化中心点项为nullptr
    tempLine(nullptr),  // 初始化临时线条项为nullptr
    drawing(false),  // 初始化绘制标志为false
    isAddingCircle(false),  // 初始化添加圆形标志为false
    isAddingText(false),  // 初始化添加文本标志为false
    LineColor(color)
{
    setSceneRect(0, 0, 640, 480); // 设置场景大小为640x480
}

void CustomGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (isAddingCircle) {  // 如果正在添加圆形
        if (firstPoint.isNull()) {  // 如果第一个点为空
            firstPoint = event->scenePos();  // 记录第一个点的场景位置
            centerPoint = addEllipse(firstPoint.x() - 2, firstPoint.y() - 2, 4, 4, QPen(Qt::NoPen), QBrush(Qt::red));  // 添加中心点标记（红色椭圆）
            drawing = true;  // 开始绘制
        } else {
            removeItem(tempLine);  // 移除临时线条
            qreal radius = QLineF(firstPoint, event->scenePos()).length();  // 计算半径
            circle = new CustomCircleItem(firstPoint.x(), firstPoint.y(), radius * 2);  // 创建自定义圆形项
            LineColor.setWidth(2);
            circle->setPen(LineColor);
            circle->R=LineColor.color().red();
            circle->G=LineColor.color().green();
            circle->B=LineColor.color().blue();
            Radius = radius;
            addItem(circle);  // 添加圆形项到场景
            removeItem(centerPoint);  // 移除中心点标记
            centerPoint = nullptr;  // 中心点标记置为空
            firstPoint = QPointF();  // 第一个点置为空点
            drawing = false;  // 结束绘制
            isAddingCircle = false;  // 结束添加圆形
            circle->setCursor(Qt::PointingHandCursor); // 当鼠标在图形上时，改变指针样式

            // emit circleAdded(circle->rect().center(),radius);  // 发送圆形添加信号，传递圆形的中心点
            updateData();
        }
    } else if (isAddingText) {  // 如果正在添加文本
        bool ok;
        QString textcontent = QInputDialog::getText(nullptr, "输入文字", "请输入文字:", QLineEdit::Normal, "", &ok);
        if (ok && !textcontent.isEmpty()) {
            text = new CustomTextItem(textcontent);
            text->setPos(event->scenePos());
            addItem(text);
            text->setMovable(true);  // 设置文本项可移动
            isAddingText = false;
        }
    }
    else if (isAddingLine)
    {  // 新增：如果正在添加直线
        if (firstPoint.isNull())
        {
            firstPoint = event->scenePos();
            centerPoint=addEllipse(firstPoint.x() - 2, firstPoint.y() - 2, 4, 4, QPen(Qt::NoPen), QBrush(Qt::red));
            drawing = true;
        }
        else
        {
            removeItem(tempLine);
            line = new CustomLineItem(QLineF(firstPoint, event->scenePos()));
            line->setPen(LineColor);
            line->R=LineColor.color().red();
            line->G=LineColor.color().green();
            line->B=LineColor.color().blue();
            addItem(line);
            removeItem(centerPoint);  // 移除中心点标记
            centerPoint = nullptr;  // 中心点标记置为空
            firstPoint = QPointF();
            drawing = false;
            isAddingLine = false;
            line->setCursor(Qt::PointingHandCursor); // 当鼠标在图形上时，改变指针样式
            updateData();
        }
    }
    else if (isAddingRectangle) { // 处理新增的矩形
        qDebug()<<"isAddingRec";
        if (firstPoint.isNull()) {
            firstPoint = event->scenePos();
            centerPoint=addEllipse(firstPoint.x() - 2, firstPoint.y() - 2, 4, 4, QPen(Qt::NoPen), QBrush(Qt::red));
            drawing = true;
        }
        else {
            removeItem(tempLine);
            removeItem(centerPoint);  // 移除中心点标记
            centerPoint = nullptr;  // 中心点标记置为空
            QRectF rect(firstPoint, event->scenePos());
            CustomRectangleItem *rectangle = new CustomRectangleItem(rect);
            addItem(rectangle);
            firstPoint = QPointF();
            drawing = false;
            isAddingRectangle = false;
        }
    }
    QGraphicsScene::mousePressEvent(event);  // 调用基类的鼠标按下事件处理函数
}

void CustomGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (drawing && centerPoint) {  // 如果正在绘制且中心点标记存在
        if (tempLine) {  // 如果临时线条存在
            removeItem(tempLine);  // 移除临时线条
        }
        tempLine = addLine(QLineF(firstPoint, event->scenePos()), QPen(Qt::blue));  // 添加临时线条（蓝色）
    }
    QGraphicsScene::mouseMoveEvent(event);  // 调用基类的鼠标移动事件处理函数
    updateData();
}

void CustomGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mouseReleaseEvent(event);  // 调用基类的鼠标释放事件处理函数
}

void CustomGraphicsScene::setAddingCircle(bool condition) {
    isAddingCircle = condition;  // 设置是否正在添加圆形的标志
}

void CustomGraphicsScene::setAddingText(bool condition) {
    isAddingText = condition;  // 设置是否正在添加文本的标志
}

void CustomGraphicsScene::setAddingLine(bool condition)
{
    isAddingLine=condition;
}

std::vector<std::pair<QPointF, qreal>> CustomGraphicsScene::getCircles()const {//遍历获取所有圆坐标和半径，已废置
    std::vector<std::pair<QPointF, qreal>> circles;
    for (QGraphicsItem *item : items()) {
        if (CustomCircleItem *circle = dynamic_cast<CustomCircleItem*>(item)) {
            circles.push_back(std::make_pair(circle->center, circle->radius));
        }
    }
    return circles;
}

void CustomGraphicsScene::setAddingRectangle(bool condition)
{ // 新增的设置函数实现
    isAddingRectangle = condition;
}
