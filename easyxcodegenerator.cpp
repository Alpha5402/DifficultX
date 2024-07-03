#include "easyxcodegenerator.h"
#include <QFile>
#include <QTextStream>

using Qt::endl;

QTextStream test_is_code_right_generator(stdout); // 将输出流绑定到标准输出,检查code是否成功添加代码

vector<CircleItem>circles;//储存圆信息

void EasyXCodeGenerator::clear()
{
    test_is_code_right_generator << "clear" << endl;
    code.clear();
}

void EasyXCodeGenerator::addCircle(QPointF c,qreal r)
{
    CircleItem circle(c,r);
    circles.push_back(circle);
    // QString circle_code = QString("circle(%1, %2，%3);\n").arg(position.x()).arg(position.y() - 6.arg());
    // test_is_code_right_generator << circle_code << Qt::endl; // test code
    // code.append(circle_code);
    test_is_code_right_generator << "add circle." << code << Qt::endl; // test code
}

void EasyXCodeGenerator::addText(const QString &text, const QPointF &position)
{
    QString text_code = QString("outtextxy(%1, %2,_T(\"%3\"));\n").arg(position.x()).arg(position.y()).arg(text);
    test_is_code_right_generator << text_code << Qt::endl; // test code
    code.append(text_code);
    test_is_code_right_generator << "add text." << code << Qt::endl;
}

void EasyXCodeGenerator::generateCode(const QString &filename)
{
    // code.clear();
    for (const CircleItem &circle : circles) {
        code+=QString("circle(%1,%2,%3);\n").arg(circle.center.x()).arg(circle.center.y() - 6).arg(circle.radius);
         // 将圆的信息写入文件
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
