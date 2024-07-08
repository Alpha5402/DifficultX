#ifndef CUSTOMRECTANGLEITEM_H
#define CUSTOMRECTANGLEITEM_H

#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QWheelEvent>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QRectF>
#include "communal.h"

class CustomRectangleItem : public QGraphicsRectItem {
public:
    CustomRectangleItem(const QRectF &rect, QGraphicsItem *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QGraphicsEllipseItem *topLeftHandle;
    QGraphicsEllipseItem *bottomRightHandle;
    bool isDraggingTopLeftHandle;
    bool isDraggingBottomRightHandle;
    bool isDraggingRect;

    void updateHandlesPosition();

public:
    QPointF LT;//lefttop
    QPointF RB; //rightbottom
};

#endif // CUSTOMRECTANGLEITEM_H
