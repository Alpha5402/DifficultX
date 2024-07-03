#include "CustomGraphicsScene.h"
#include "CustomCircleItem.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>

CustomGraphicsScene::CustomGraphicsScene(QObject *parent)
    : QGraphicsScene(parent),
    circle(nullptr),
    centerPoint(nullptr),
    edgePoint(nullptr),
    tempLine(nullptr),
    drawing(false) {}

void CustomGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (!drawing) {
        // 第一次点击，设置圆心位置
        centerPoint = addEllipse(event->scenePos().x() - 2, event->scenePos().y() - 2, 4, 4,
                                 QPen(Qt::NoPen), QBrush(Qt::red));
        drawing = true;
    } else {
        // 第二次点击，完成圆的绘制
        removeItem(tempLine);
        qreal radius = QLineF(centerPoint->rect().center(), event->scenePos()).length();
        circle = new CustomCircleItem(centerPoint->rect().center().x(), centerPoint->rect().center().y(), 2 * radius);
        addItem(circle);
        removeItem(centerPoint);
        centerPoint = nullptr;
        drawing = false;
    }
    QGraphicsScene::mousePressEvent(event);
}

void CustomGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (drawing && centerPoint) {
        // 移除之前的临时线条
        if (tempLine) {
            removeItem(tempLine);
        }
        // 绘制新的临时线条
        tempLine = addLine(QLineF(centerPoint->rect().center(), event->scenePos()), QPen(Qt::blue));
    }
    QGraphicsScene::mouseMoveEvent(event);
}
