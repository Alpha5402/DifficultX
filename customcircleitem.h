#ifndef CUSTOMCIRCLEITEM_H
#define CUSTOMCIRCLEITEM_H

#include "communal.h"
#include <QWheelEvent>

class CustomCircleItem : public QGraphicsEllipseItem {
    //Q_OBJECT
public:
    CustomCircleItem(qreal x, qreal y, qreal diameter, QGraphicsItem *parent = nullptr);
    //void changeByLineedit(QPointF c,double r);
    void changeByLineedit(double cx,double cy,double r);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void wheelEvent(QGraphicsSceneWheelEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override {
        setCursor(Qt::PointingHandCursor); // 鼠标悬停时变为手型光标
        QGraphicsItem::hoverEnterEvent(event);
    }

    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override {
        unsetCursor(); // 鼠标离开时恢复默认光标
        QGraphicsItem::hoverLeaveEvent(event);
    }

public:
    QPointF center; // 圆心
    qreal radius; // 半径
    QGraphicsEllipseItem *centerPoint;
    QGraphicsEllipseItem *radiusPoint;
    QGraphicsLineItem *radiusLine;
    bool selected;
    int R;
    int G;
    int B;

    // void updateCircle();
//signals:
    //void drawingFinished(const QString &msg);
};

#endif // CUSTOMCIRCLEITEM_H
