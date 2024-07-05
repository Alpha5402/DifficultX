#ifndef CUSTOMLINEITEM_H
#define CUSTOMLINEITEM_H

#include "communal.h"
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>

class CustomLineItem : public QGraphicsLineItem
{
public:
    CustomLineItem(const QLineF &line, QGraphicsItem *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override {
        setCursor(Qt::PointingHandCursor); // 鼠标悬停时变为手型光标
        QGraphicsItem::hoverEnterEvent(event);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override {
        unsetCursor(); // 鼠标离开时恢复默认光标
        QGraphicsItem::hoverLeaveEvent(event);
    }

public:
    enum DragState { NoDrag, DraggingStartPoint, DraggingEndPoint, DraggingLine } dragState;
    QPointF lastMousePos;
    QPointF point1;
    QPointF point2;//获取point1和point2
    // QGraphicsEllipseItem *startPoint;
    // QGraphicsEllipseItem *endPoint;
    void updateEndpoints();
    void changeByLineedit(double x1,double y1,double x2,double y2);
};

#endif // CUSTOMLINEITEM_H
