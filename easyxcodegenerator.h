#ifndef EASYXCODEGENERATOR_H
#define EASYXCODEGENERATOR_H

#include <QString>
#include <QPointF>
#include <vector>
#include <QTextStream>
#include "communal.h"
#include "customgraphicssene.h"
#include "customcircleitem.h"


using std::vector;

struct Operation{
    Operation();
    QString type;
    QString stringPara;
    QGraphicsItem * ptr;
    double parameter_1;
    double parameter_2;
    double parameter_3;
    double parameter_4;
    bool usingFill;
};

class CircleItem
{
public:
    QPointF center; // 圆心
    qreal radius; // 半径
    CircleItem(QPointF c,qreal r):center(c),radius(r){}
};

class LineItem
{
public:
    QPointF p1;
    QPointF p2;
    LineItem(QPointF p11,QPointF p22):p1(p11),p2(p22){}
};


class EasyXCodeGenerator
{
public:
    struct TextInfo {
        QString text;
        QPoint position;
    };
    void clear();
    void addCircle(QPointF c,qreal r);
    void addText(const QString &text, const QPointF &position);
    void addLine(QPointF p1,QPointF p2);
    //void generateCode(const QString &filename,const CustomGraphicsScene *scene);
    void generateCode(const CustomGraphicsScene *scene);
    QString getCode() const;  // 新增的方法

private:
    QString code;  // 将code定义为类的私有成员变量
};

#endif // EASYXCODEGENERATOR_H
