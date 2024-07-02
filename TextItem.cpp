#include "TextItem.h"

TextItem::TextItem(const QPointF &position, const QString &text)
    : QGraphicsTextItem(text)
{
    setPos(position);
    setTextInteractionFlags(Qt::TextEditorInteraction);
}

void TextItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}
