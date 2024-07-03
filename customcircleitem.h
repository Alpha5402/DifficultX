#ifndef CUSTOMCIRCLEITEM_H
#define CUSTOMCIRCLEITEM_H

#include <QGraphicsEllipseItem>

class CustomCircleItem : public QGraphicsEllipseItem {
public:
    CustomCircleItem(qreal x, qreal y, qreal diameter, QGraphicsItem *parent = nullptr);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPointF center; // 圆心
    qreal radius; // 半径
};

#endif // CUSTOMCIRCLEITEM_H
