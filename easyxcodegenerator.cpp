#include "easyxcodegenerator.h"
#include <QFile>
#include <QTextStream>

using Qt::endl;

QTextStream test_is_code_right_generator(stdout); // 将输出流绑定到标准输出,检查code是否成功添加代码

vector<CircleItem>circles_gen;//储存圆信息
vector<LineItem>lines_gen;//储存直线信息

void EasyXCodeGenerator::clear()
{
    test_is_code_right_generator << "clear" << endl;
    code.clear();
}

void EasyXCodeGenerator::addCircle(QPointF c,qreal r)
{
    CircleItem circle(c,r);//存个数据，没啥用
    circles_gen.push_back(circle);
    QString circle_code = QString("circle(%1,%2,%3);\n").arg(c.x()).arg(c.y()).arg(r);
    test_is_code_right_generator << circle_code << Qt::endl; // test code
    code.append(circle_code);
    test_is_code_right_generator << "add circle." << code << Qt::endl; // test code
}

void EasyXCodeGenerator::addText(const QString &text, const QPointF &position)
{
    QString text_code = QString("outtextxy(%1, %2,_T(\"%3\"));\n").arg(position.x()).arg(position.y()).arg(text);
    test_is_code_right_generator << text_code << Qt::endl; // test code
    code.append(text_code);
    test_is_code_right_generator << "add text." << code << Qt::endl;
}
void EasyXCodeGenerator::addLine(QPointF p1,QPointF p2)//增加一个线段
{
    LineItem line(p1,p2);//存个数据，没啥用
    lines_gen.push_back(line);
    QString line_code = QString("line(%1,%2,%3,%4);\n").arg(p1.x()).arg(p1.y()).arg(p2.x()).arg(p2.y());
    test_is_code_right_generator << line_code << Qt::endl; // test code
    code.append(line_code);
    test_is_code_right_generator << "add line." << code << Qt::endl; // test code
}

void EasyXCodeGenerator::generateCode(const QString &filename, const CustomGraphicsScene *scene)
{//点击genecode后遍历scene的所有item并添加到code在点击genecode后才能正常获取code
    code.clear();
    circles_gen.clear();
    lines_gen.clear();
    std::vector<std::pair<QPointF, qreal>> circles = scene->getCircles();
    for (const auto &circle : circles)
    {
        addCircle(circle.first, circle.second); // 使用圆心坐标和半径生成代码
    }
    std::vector<std::pair<QPointF, QPointF>> Lines = scene->getLines();
    for (const auto &line : Lines)
    {
        addLine(line.first, line.second); // 使用直线两点生成代码
    }

    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);

        test_is_code_right_generator << "code is: " << code << Qt::endl; // 将字符串输出到标准输出
        out << code;
        file.close();
    }
}

QString EasyXCodeGenerator::getCode() const
{
    return code;
}
