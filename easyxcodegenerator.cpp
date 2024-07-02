#include "easyxcodegenerator.h"
#include <QFile>
#include <QTextStream>

void EasyXCodeGenerator::clear()
{
    code.clear();
}

void EasyXCodeGenerator::addCircle(const QPointF &position)
{
    code += QString("circle(%1, %2);\n").arg(position.x()).arg(position.y());
}

void EasyXCodeGenerator::addText(const QString &text, const QPointF &position)
{
    code += QString("outtextxy(%1, %2,_T(\"%3\"));\n").arg(position.x()).arg(position.y()).arg(text);
}

void EasyXCodeGenerator::generateCode(const QString &filename)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << code;
        file.close();
    }
}

QString EasyXCodeGenerator::getCode() const
{
    return code;
}
