#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

// 自定义场景类，继承自 QGraphicsScene
class CustomGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    CustomGraphicsScene(QObject *parent = nullptr);

signals:
    void sceneClicked(QGraphicsSceneMouseEvent *event); // 定义一个信号，传递鼠标事件

protected:
    // 重写鼠标按下事件处理方法
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // CUSTOMGRAPHICSSCENE_H
