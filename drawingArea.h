
#ifndef DRAWINGAREA_H
#define DRAWINGAREA_H

#include "communal.h"
#include "customgraphicssene.h"

class DrawingArea : public QWidget {
    Q_OBJECT

public:
    DrawingArea(QPen color = QPen(RGB(255, 255, 255)), QWidget *parent = nullptr);
    virtual QString checkShapeSelection(const QPoint &point) {};
    virtual void drawShapes(QPainter &painter) {};
    virtual bool isPointOnShape(const QPoint &point) {};
    QGraphicsView *view;
    CustomGraphicsScene *scene;

signals:
    void shapeSelected(const QString &shapeInfo);
    //void drawingFinished(const QString &message);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // DRAWINGAREA_H
