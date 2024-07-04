#ifndef CUSTOMDRAWINGAREA_H
#define CUSTOMDRAWINGAREA_H

#include "drawingArea.h"
#include "communal.h"

class CustomDrawingArea : public DrawingArea {
    Q_OBJECT

public:
    CustomDrawingArea(QWidget *parent = nullptr);
    void drawShapes(QPainter &painter);
    bool isPointOnShape(const QPoint &point) override;
protected:
    QString checkShapeSelection(const QPoint &point);
    bool isPointNearLine(const QPoint &point, const QPoint &lineStart, const QPoint &lineEnd);
};

#endif // CUSTOMDRAWINGAREA_H
