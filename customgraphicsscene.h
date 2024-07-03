#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QGraphicsEllipseItem;
class QGraphicsLineItem;
QT_END_NAMESPACE

class CustomGraphicsScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit CustomGraphicsScene(QObject *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QGraphicsEllipseItem *circle;
    QGraphicsEllipseItem *centerPoint;
    QGraphicsEllipseItem *edgePoint;
    QGraphicsLineItem *tempLine;
    bool drawing;
};

#endif // CUSTOMGRAPHICSSCENE_H
