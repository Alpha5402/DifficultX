#include "canvas.h"
#include "easyxcodegenerator.h"
#include"displaywindow.h"
#include <QMouseEvent>
#include <QPainter>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

Canvas::Canvas(QWidget *parent)
    : QGraphicsView(parent), codeGenerator(new EasyXCodeGenerator()), isDrawing(false), isAddingText(false)
{
    scene = new QGraphicsScene(this);
    setScene(scene);

    generateButton = new QPushButton("生成代码", this);
    drawCircleButton = new QPushButton("绘制圆", this);
    addTextButton = new QPushButton("添加文字", this);

    connect(generateButton, &QPushButton::clicked, this, &Canvas::generateCode);
    connect(drawCircleButton, &QPushButton::clicked, this, &Canvas::startDrawing);
    connect(addTextButton, &QPushButton::clicked, this, &Canvas::startAddingText);
}

void Canvas::mousePressEvent(QMouseEvent *event)
{
    if (isDrawing) {
        QGraphicsEllipseItem *circle = new QGraphicsEllipseItem(event->pos().x() - 25, event->pos().y() - 25, 50, 50);
        circles.push_back(circle);
        scene->addItem(circle);
    } else if (isAddingText) {
        bool ok;
        QString text = QInputDialog::getText(this, tr("输入文字"), tr("文字:"), QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
            TextInfo textInfo = {event->pos(), text};
            texts.push_back(textInfo);
            addText(text, event->pos());
        }
    }
}

void Canvas::mouseMoveEvent(QMouseEvent *event)
{
    // 根据需要实现移动事件
}

void Canvas::paintEvent(QPaintEvent *event)
{
    QGraphicsView::paintEvent(event);
    QPainter painter(viewport());

    for (const auto &text : texts) {
        painter.drawText(text.position, text.text);
    }
}

void Canvas::generateCode()
{
    codeGenerator->clear();  // 清空之前的代码

    for (const auto &circle : circles) {
        codeGenerator->addCircle(circle->scenePos());  // 添加圆的代码
    }

    for (const auto &textInfo : texts) {
        codeGenerator->addText(textInfo.text, textInfo.position);  // 添加文字的代码
    }

    QString filename = QFileDialog::getSaveFileName(this, "Save Code", "", "Text Files (*.txt)");
    if (!filename.isEmpty())
    {
        codeGenerator->getCode();

        DisplayWindow *displayWindow = new DisplayWindow(filename,this);  // 创建显示窗口对象
        displayWindow->show();  // 显示代码窗口
    }
}

void Canvas::startDrawing()
{
    isDrawing = true;
    isAddingText = false;
}

void Canvas::startAddingText()
{
    isDrawing = false;
    isAddingText = true;
}

void Canvas::addText(const QString &text, const QPoint &position)
{
    QGraphicsTextItem *textItem = new QGraphicsTextItem(text);
    textItem->setPos(position);
    scene->addItem(textItem);
}
