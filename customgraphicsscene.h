#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include <QGraphicsScene>
#include<QPointF>
#include<vector>
#include <QGraphicsView>
#include <QGraphicsLineItem>

QT_BEGIN_NAMESPACE
class QGraphicsEllipseItem;
class QGraphicsLineItem;
QT_END_NAMESPACE

class CustomGraphicsScene : public QGraphicsScene {
    Q_OBJECT
public:
    explicit CustomGraphicsScene(QObject *parent = nullptr);

    // 状态控制函数
    void setAddingCircle(bool condition);
    void setAddingText(bool condition);
    void setAddingLine(bool condition);// 设置添加直线状态

    //遍历获取item信息函数
    std::vector<std::pair<QPointF, qreal>> getCircles()const;
    std::vector<std::pair<QPointF,QPointF>> getLines()const;//改了，新增获取line信息函数

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QGraphicsEllipseItem *circle;
    QGraphicsLineItem *line;//xian
    QGraphicsEllipseItem *centerPoint;
    QGraphicsEllipseItem *edgePoint;
    QGraphicsLineItem *tempLine;


    bool drawing;
    bool isAddingCircle;
    bool isAddingText;
    bool isAddingLine;//xian
    QPointF firstPoint;

signals:
    void circleAdded(QPointF center,qreal r);//用不上
    void textAdded(const QString &text, const QPointF &pos);//用不上
};

#endif // CUSTOMGRAPHICSSCENE_H
