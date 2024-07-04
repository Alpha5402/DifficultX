#include "customDrawingArea.h"

CustomDrawingArea::CustomDrawingArea(QWidget *parent) : DrawingArea(parent) {}

void CustomDrawingArea::drawShapes(QPainter &painter) {
    // 具体图形绘制代码，例如绘制线段
    painter.drawLine(QPoint(50, 50), QPoint(150, 150));
}

QString CustomDrawingArea::checkShapeSelection(const QPoint &point) {
    // 具体图形选中检测，例如检测线段
    if (isPointNearLine(point, QPoint(50, 50), QPoint(150, 150))) {
        return "Line from (50, 50) to (150, 150)";
    }
    return "";
}

bool CustomDrawingArea::isPointOnShape(const QPoint &point) {
    // 检查点是否在图形上，例如检测线段
    return isPointNearLine(point, QPoint(50, 50), QPoint(150, 150));
}

bool CustomDrawingArea::isPointNearLine(const QPoint &point, const QPoint &lineStart, const QPoint &lineEnd) {
    // 简单实现，实际中可能需要更复杂的判断
    const int threshold = 2;
    double dis = (lineEnd.x()-lineStart.x())*(point.y() - lineStart.y());
    dis -= (lineEnd.y()-lineStart.y())*(point.x() - lineStart.x());
    dis = abs(dis);
    dis /= std::hypot(lineEnd.x() - lineStart.x(), lineEnd.y() - lineStart.y());
    double distance = std::hypot(point.x() - lineStart.x(), point.y() - lineStart.y()) +
                      std::hypot(point.x() - lineEnd.x(), point.y() - lineEnd.y()) -
                      std::hypot(lineEnd.x() - lineStart.x(), lineEnd.y() - lineStart.y());
    //qDebug() << "[INFO] cursor" << point << " distance " << distance << Qt::endl;
    return distance < threshold;
}
