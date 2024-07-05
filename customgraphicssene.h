#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include "communal.h"
#include "customcircleitem.h"

QT_BEGIN_NAMESPACE
class QGraphicsEllipseItem;
class QGraphicsLineItem;
QT_END_NAMESPACE

class CustomGraphicsScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit CustomGraphicsScene(QObject *parent = nullptr, QPen color = QPen(RGB(255, 255, 255)));
    void setAddingCircle(bool condition);
    void setAddingText(bool condition);
    std::vector<std::pair<QPointF, qreal>> getCircles()const;
    void updateData();
    CustomCircleItem *circle;
    QPen LineColor;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;


private:

    QGraphicsEllipseItem *centerPoint;
    QGraphicsEllipseItem *edgePoint;
    QGraphicsLineItem *tempLine;
    bool drawing;
    bool isAddingCircle;
    bool isAddingText;
    QPointF firstPoint;
    double Radius;


signals:
    void circleAdded(QPointF center,qreal r);
    void textAdded(const QString &text, const QPointF &pos);
    void drawingFinished(const QString &msg);

public slots:
    void ReceivePara1ValueChanged(double value);
    void ReceivePara2ValueChanged(double value);
    void ReceivePara3ValueChanged(double value);
};

#endif // CUSTOMGRAPHICSSCENE_H