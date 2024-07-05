#include "drawingArea.h"
#include "imageEditor.h"

DrawingArea::DrawingArea(QPen color, QWidget *parent) : QWidget(parent) {
    setMouseTracking(true);
    // scene = new CustomGraphicsScene(this);  // 创建自定义图形场景
    // view = new QGraphicsView(scene);  // 创建图形视图，并将场景设置为中心部件

    QVBoxLayout *layout = new QVBoxLayout(this);
    setFixedSize(720, 540);
    //resize(640, 480);
    scene = new CustomGraphicsScene(this, color);  // 创建自定义图形场景
    view = new QGraphicsView(scene);  // 创建图形视图，并将场景设置为中心部件
    scene->setSceneRect(0, 0, 640, 480);
    // view->resize(640, 480);
    view->centerOn(scene->sceneRect().center());

    layout->addWidget(view);
    setLayout(layout);
}

void DrawingArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    drawShapes(painter); // 调用虚函数绘制图形
}

void DrawingArea::mousePressEvent(QMouseEvent *event) {
    QPoint clickedPoint = event->pos();
    QString shapeInfo = checkShapeSelection(clickedPoint);
    if (!shapeInfo.isEmpty()) {
        emit shapeSelected(shapeInfo);
    }
}

void DrawingArea::mouseMoveEvent(QMouseEvent *event) {
    QPoint movePoint = event->pos();
    if (isPointOnShape(movePoint)) {
        setCursor(Qt::PointingHandCursor); // 当鼠标在图形上时，改变指针样式
    } else {
        setCursor(Qt::ArrowCursor); // 当鼠标不在图形上时，恢复指针样式
    }
}
