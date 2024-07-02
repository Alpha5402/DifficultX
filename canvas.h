#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPushButton>
#include <QMouseEvent>
#include <vector>
#include "easyxcodegenerator.h"

struct TextInfo {
    QPoint position;  // 文字位置
    QString text;  // 文字内容
};

class Canvas : public QWidget {
    Q_OBJECT

public:
    explicit Canvas(QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<QPoint> circles;  // 圆的位置列表
    std::vector<TextInfo> texts;  // 文字信息列表
    EasyXCodeGenerator *codeGenerator;  // EasyX 代码生成器对象指针
    QPushButton *generateButton;  // 生成代码按钮
    QPushButton *drawCircleButton;  // 绘制圆按钮
    QPushButton *addTextButton;  // 添加文字按钮
    bool isDrawing;  // 是否正在绘制圆的标志
    bool isAddingText;  // 是否正在添加文字的标志
    QPoint currentPos;  // 当前鼠标位置

private slots:
    void generateCode();  // 生成EasyX代码的槽函数
    void startDrawing();  // 开始绘制圆的槽函数
    void startAddingText();  // 开始添加文字的槽函数
    void addText(const QString &text, const QPoint &position);  // 添加文字的方法
};

#endif // CANVAS_H
