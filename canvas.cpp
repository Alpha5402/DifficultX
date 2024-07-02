#include "canvas.h"
#include <QPainter>
#include <QInputDialog>
#include <QVBoxLayout>

Canvas::Canvas(QWidget *parent)
    : QWidget(parent), codeGenerator(new EasyXCodeGenerator()), isDrawing(false), isAddingText(false) {
    // 创建按钮
    generateButton = new QPushButton("生成代码", this);
    drawCircleButton = new QPushButton("绘制圆", this);
    addTextButton = new QPushButton("添加文字", this);

    // 将按钮添加到布局中
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(drawCircleButton);
    layout->addWidget(addTextButton);
    layout->addWidget(generateButton);
    setLayout(layout);

    // 连接按钮的信号与槽函数
    connect(generateButton, &QPushButton::clicked, this, &Canvas::generateCode);
    connect(drawCircleButton, &QPushButton::clicked, this, &Canvas::startDrawing);
    connect(addTextButton, &QPushButton::clicked, this, &Canvas::startAddingText);
}

void Canvas::mousePressEvent(QMouseEvent *event) {
    if (isDrawing) {
        circles.push_back(event->pos());  // 将圆的位置添加到列表中
        isDrawing = false;
    } else if (isAddingText) {
        bool ok;
        QString text = QInputDialog::getText(this, "输入文字", "请输入文字:", QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
            addText(text, event->pos());  // 添加文字到指定位置
        }
        isAddingText = false;
    }
    update();  // 触发重绘事件
}

void Canvas::mouseMoveEvent(QMouseEvent *event) {
    currentPos = event->pos();  // 更新当前鼠标位置
    update();  // 触发重绘事件
}

void Canvas::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // 绘制所有的圆
    for (const auto &circle : circles) {
        painter.drawEllipse(circle, 20, 20);  // 假设圆的半径为20
    }
    // 绘制所有的文字
    for (const auto &textInfo : texts) {
        painter.drawText(textInfo.position, textInfo.text);
    }
}

void Canvas::generateCode() {
    codeGenerator->clear();  // 清空之前的代码
    for (const auto &circle : circles) {
        codeGenerator->addCircle(circle);  // 添加圆的代码
    }
    for (const auto &textInfo : texts) {
        codeGenerator->addText(textInfo.text, textInfo.position);  // 添加文字的代码
    }
    codeGenerator->generateCode();  // 生成最终的代码
}

void Canvas::startDrawing() {
    isDrawing = true;  // 设置绘制圆的标志
}

void Canvas::startAddingText() {
    isAddingText = true;  // 设置添加文字的标志
}

void Canvas::addText(const QString &text, const QPoint &position) {
    texts.push_back({position, text});  // 将文字信息添加到列表中
}
