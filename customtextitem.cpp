// 包含CustomTextItem.h头文件
#include "CustomTextItem.h"

// CustomTextItem类的构造函数
// 参数：text - 要显示的文本内容
// 参数：parent - 父级QGraphicsItem对象
CustomTextItem::CustomTextItem(const QString &text, QGraphicsItem *parent)
    : QGraphicsTextItem(text, parent), // 调用基类的构造函数，初始化文本项
    isMovable(false) // 设置isMovable标志为false，表示文本项不可移动
{
    setFlag(QGraphicsItem::ItemIsSelectable, true); // 设置文本项可以选择
    setFlag(QGraphicsItem::ItemIsFocusable, true); // 设置文本项可以获取焦点
}

// 设置字体大小和样式
// 参数：font - 要设置的QFont对象
void CustomTextItem::setFont(const QFont &font)
{
    QGraphicsTextItem::setFont(font); // 调用基类的方法设置字体
    textstyle=font.styleName();

}

// 设置文本内容
// 参数：text - 要设置的文本字符串
void CustomTextItem::setText(const QString &text) {
    QGraphicsTextItem::setPlainText(text); // 调用基类的方法设置文本
}

// 设置文本项是否可移动
// 参数：movable - 控制文本项是否可移动的布尔值
void CustomTextItem::setMovable(bool movable) {
    isMovable = movable; // 更新isMovable标志
    setFlag(QGraphicsItem::ItemIsMovable, movable); // 根据movable的值设置是否可移动的标志
}

// 鼠标按下事件处理函数
// 参数：event - QGraphicsSceneMouseEvent对象，包含鼠标事件信息
void CustomTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if(isSelected())
    {
        isMovable=true;
    }
    QGraphicsTextItem::mousePressEvent(event); // 调用基类的方法处理鼠标按下事件
}

// 鼠标移动事件处理函数
void CustomTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (isMovable)
    { // 只有当文本项可移动时，才处理移动
        QPointF newPoint=event->scenePos();
        position=newPoint;
        setPos(position);
    }
    QGraphicsTextItem::mouseMoveEvent(event); // 调用基类的方法处理鼠标移动事件
}

// 鼠标释放事件处理函数
// 参数：event - QGraphicsSceneMouseEvent对象，包含鼠标事件信息
void CustomTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    isMovable=false;
    qDebug()<<position;
    QGraphicsTextItem::mouseReleaseEvent(event); // 调用基类的方法处理鼠标释放事件
}

void CustomTextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)//双击事件处理函数
{
    bool ok;
    QString currentText = toPlainText();
    QString text = QInputDialog::getText(nullptr, "编辑文字", "请输入新文字:", QLineEdit::Normal, currentText, &ok);
    if (ok && !text.isEmpty()) {
        setText(text);
    }
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
