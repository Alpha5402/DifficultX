#ifndef TEXTITEM_H
#define TEXTITEM_H

#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>

// 自定义文字项类，继承自 QGraphicsTextItem
class TextItem : public QGraphicsTextItem
{
public:
    TextItem(const QPointF &position, const QString &text);

protected:
    // 重写鼠标双击事件处理方法
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // TEXTITEM_H
