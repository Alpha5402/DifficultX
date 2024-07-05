#include "customgraphicssene.h"

void CustomGraphicsScene::updateData(){
    //QString msg = QString("%1, %2, %3").arg(firstPoint.x()).arg(firstPoint.y()).arg(Radius);
    auto items = selectedItems();
    for (auto item : items) {
        auto circleItem = dynamic_cast<CustomCircleItem *>(item);
        if (circleItem) {  // 更新圆形项的位置和大小
            QString msg = QString("%1, %2, %3").arg(circleItem->center.x()).arg(circleItem->center.y()).arg(circleItem->radius);
            emit drawingFinished(msg);
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
        }
    }
}

void CustomGraphicsScene::ReceivePara2ValueChanged(double value){
    qDebug() << "[INFO] CGS received value " << value << Qt::endl;
    auto items = selectedItems();
    for (auto item : items) {
        //auto circleItem = dynamic_cast<CustomCircleItem *>(item);
        if (CustomCircleItem *circleItem = dynamic_cast<CustomCircleItem*>(item)) {  // 更新圆形项的位置和大小
            //circleItem->center.setX(value);
            circleItem->changeByLineedit(circleItem->center.x(), value, circleItem->radius);
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
        }
    }
}

CustomGraphicsScene::CustomGraphicsScene(QObject *parent, QPen color)
    : QGraphicsScene(parent),
    circle(nullptr),  // 初始化圆形项为nullptr
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
            circle->setPen(LineColor);
            Radius = radius;
            addItem(circle);  // 添加圆形项到场景
            removeItem(centerPoint);  // 移除中心点标记
            centerPoint = nullptr;  // 中心点标记置为空
            firstPoint = QPointF();  // 第一个点置为空点
            drawing = false;  // 结束绘制
            isAddingCircle = false;  // 结束添加圆形
            emit circleAdded(circle->rect().center(),radius);  // 发送圆形添加信号，传递圆形的中心点
            updateData();
        }
    } else if (isAddingText) {  // 如果正在添加文本
        bool ok;
        QString text = QInputDialog::getText(nullptr, "输入文字", "请输入文字:", QLineEdit::Normal, "", &ok);  // 弹出输入对话框获取文本
        if (ok && !text.isEmpty()) {  // 如果用户确认输入且文本非空
            QGraphicsTextItem *textItem = addText(text);  // 添加文本项到场景
            textItem->setPos(event->scenePos());  // 设置文本项位置为鼠标事件的场景位置
            isAddingText = false;  // 结束添加文本
            emit textAdded(text, event->scenePos());  // 发送文本添加信号，传递文本内容和位置
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
            addItem(line);
            removeItem(centerPoint);  // 移除中心点标记
            centerPoint = nullptr;  // 中心点标记置为空
            firstPoint = QPointF();
            drawing = false;
            isAddingLine = false;

        }
    }
    QGraphicsScene::mousePressEvent(event);  // 调用基类的鼠标按下事件处理函数
}

void CustomGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (drawing ) {  // 如果正在绘制且中心点标记存在
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

std::vector<std::pair<QPointF, qreal>> CustomGraphicsScene::getCircles()const {
    std::vector<std::pair<QPointF, qreal>> circles;
    for (QGraphicsItem *item : items()) {
        if (CustomCircleItem *circle = dynamic_cast<CustomCircleItem*>(item)) {
            circles.push_back(std::make_pair(circle->center, circle->radius));
        }
    }
    return circles;
}
