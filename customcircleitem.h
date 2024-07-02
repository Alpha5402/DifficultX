#ifndef CUSTOMCIRCLEITEM_H
#define CUSTOMCIRCLEITEM_H

#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>

// 自定义圆形项类，继承自 QGraphicsEllipseItem
class CustomCircleItem : public QGraphicsEllipseItem
{
public:
    CustomCircleItem(const QPointF &center, qreal radius);

protected:
    // 重写鼠标拖动事件处理方法
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    // 重写鼠标按下事件处理方法
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    // 重写鼠标释放事件处理方法
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool isResizing;
    QPointF initialPos;
};

#endif // CUSTOMCIRCLEITEM_H
