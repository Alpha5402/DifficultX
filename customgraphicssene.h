#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include "communal.h"
#include "customcircleitem.h"
#include"customlineitem.h"

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
    void setAddingLine(bool condition);

    std::vector<std::pair<QPointF, qreal>> getCircles()const;
    void updateData();
    CustomCircleItem *circle;
    CustomLineItem *line;
    QPen LineColor;
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;


private:

    QGraphicsEllipseItem *centerPoint;
    QGraphicsEllipseItem *edgePoint;
    QGraphicsLineItem *tempLine;
    //状态控制
    bool drawing;
    bool isAddingCircle;
    bool isAddingText;
    bool isAddingLine;

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
