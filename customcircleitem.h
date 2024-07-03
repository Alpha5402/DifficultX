#ifndef CUSTOMCIRCLEITEM_H
#define CUSTOMCIRCLEITEM_H

#include <QGraphicsEllipseItem>
#include <QGraphicsItemGroup>

class CustomCircleItem : public QGraphicsEllipseItem {
public:
    CustomCircleItem(qreal x, qreal y, qreal diameter, QGraphicsItem *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

public:
    QPointF center; // 圆心
    qreal radius; // 半径
    QGraphicsEllipseItem *centerPoint;
    QGraphicsEllipseItem *radiusPoint;
    QGraphicsLineItem *radiusLine;
    bool selected;

    void updateCircle();

};

#endif // CUSTOMCIRCLEITEM_H
