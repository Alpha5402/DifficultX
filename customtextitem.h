#ifndef CUSTOMTEXTITEM_H
#define CUSTOMTEXTITEM_H

#include <QGraphicsTextItem>
#include <QFont>
#include<QInputDialog>
#include <QGraphicsSceneMouseEvent>

class CustomTextItem : public QGraphicsTextItem {
public:
    CustomTextItem(const QString &text, QGraphicsItem *parent = nullptr);

    void setFont(const QFont &font);
    void setText(const QString &text);
    void setMovable(bool movable);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool isMovable;
    QString textstyle;//没初始化
    int textsize;
    QPointF position;//左上位置
    // QString text;
};

#endif // CUSTOMTEXTITEM_H
