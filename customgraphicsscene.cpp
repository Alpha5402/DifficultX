#include "CustomGraphicsScene.h"

CustomGraphicsScene::CustomGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
}

void CustomGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit sceneClicked(event); // 触发自定义信号，将鼠标事件传递出去
    QGraphicsScene::mousePressEvent(event); // 调用父类的方法处理默认行为
}
