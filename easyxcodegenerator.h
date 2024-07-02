#ifndef EASYXCODEGENERATOR_H
#define EASYXCODEGENERATOR_H

#include <QString>
#include <vector>
#include <QPoint>

class EasyXCodeGenerator {
public:
    EasyXCodeGenerator();
    void clear();
    void addCircle(const QPoint &position);
    void addText(const QString &text, const QPoint &position);
    void generateCode();

private:
    std::vector<QString> codeLines;
};

#endif // EASYXCODEGENERATOR_H
