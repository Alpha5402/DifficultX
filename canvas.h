// #ifndef CANVAS_H
// #define CANVAS_H

// #include <QGraphicsView>
// #include <QGraphicsEllipseItem>
// #include <QPushButton>
// #include <vector>
// #include <QPointF>
// #include <QInputDialog>

// class EasyXCodeGenerator;

// class Canvas : public QGraphicsView
// {
//     Q_OBJECT
// public:
//     Canvas(QWidget *parent = nullptr);
//     void generateCode();

// protected:
//     void mousePressEvent(QMouseEvent *event) override;
//     void mouseMoveEvent(QMouseEvent *event) override;
//     void paintEvent(QPaintEvent *event) override;

// private:
//     void startDrawing();
//     void startAddingText();
//     void addText(const QString &text, const QPoint &position);

//     QGraphicsScene *scene;
//     std::vector<QGraphicsEllipseItem*> circles;
//     struct TextInfo {
//         QPointF position;
//         QString text;
//     };
//     std::vector<TextInfo> texts;
//     EasyXCodeGenerator *codeGenerator;

//     QPushButton *generateButton;
//     QPushButton *drawCircleButton;
//     QPushButton *addTextButton;

//     bool isDrawing;
//     bool isAddingText;
// };

// #endif // CANVAS_H
