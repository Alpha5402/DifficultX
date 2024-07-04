#ifndef CUSTOMCIRCLEITEM_H
#define CUSTOMCIRCLEITEM_H

#include "communal.h"
#include <QWheelEvent>

class CustomCircleItem : public QGraphicsEllipseItem {
    //Q_OBJECT
public:
    CustomCircleItem(qreal x, qreal y, qreal diameter, QGraphicsItem *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    //

public:
    QPointF center; // 圆心
    qreal radius; // 半径
    QGraphicsEllipseItem *centerPoint;
    QGraphicsEllipseItem *radiusPoint;
    QGraphicsLineItem *radiusLine;
    bool selected;

    // void updateCircle();
//signals:
    //void drawingFinished(const QString &msg);
};

#endif // CUSTOMCIRCLEITEM_H
