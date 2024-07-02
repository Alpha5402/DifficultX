#ifndef DISPLAYWINDOW_H
#define DISPLAYWINDOW_H

#include <QWidget>
#include <QString>
#include <QTextEdit>

class DisplayWindow : public QWidget {
    Q_OBJECT

public:
    explicit DisplayWindow(const QString &filename, QWidget *parent = nullptr);  // 构造函数声明

private:
    QTextEdit *textEdit;  // 文本编辑器指针
};

#endif // DISPLAYWINDOW_H
