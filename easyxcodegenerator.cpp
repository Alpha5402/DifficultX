
#include "easyxcodegenerator.h"
#include <QFile>
#include <QTextStream>

EasyXCodeGenerator::EasyXCodeGenerator() {
    clear();
}

void EasyXCodeGenerator::clear() {
    codeLines.clear();
    codeLines.push_back("#include <graphics.h>");
    codeLines.push_back("int main() {");
    codeLines.push_back("initgraph(640, 480);");
    codeLines.push_back("settextstyle(20, 0, \"Arial\");");  // 设置字体大小和样式
}

void EasyXCodeGenerator::addCircle(const QPoint &position) {
    codeLines.push_back(QString("circle(%1, %2, 20);").arg(position.x()).arg(position.y()));
}

void EasyXCodeGenerator::addText(const QString &text, const QPoint &position) {
    codeLines.push_back(QString("outtextxy(%1, %2, \"%3\");").arg(position.x()).arg(position.y()).arg(text.toStdString().c_str()));
}

void EasyXCodeGenerator::generateCode() {
    codeLines.push_back("getch();");
    codeLines.push_back("closegraph();");
    codeLines.push_back("return 0;");
    codeLines.push_back("}");

    QFile file("output.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const auto &line : codeLines) {
            out << line << "\n";
        }
        file.close();
    }
}
