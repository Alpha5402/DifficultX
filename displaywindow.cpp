#include "displaywindow.h"
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>

DisplayWindow::DisplayWindow(const QString &filename, QWidget *parent)
    : QWidget(parent), textEdit(new QTextEdit(this)) {
    QVBoxLayout *layout = new QVBoxLayout(this);  // 创建垂直布局管理器
    layout->addWidget(textEdit);  // 将文本编辑器添加到布局中

    QFile file(filename);  // 创建文件对象
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {  // 打开文件，只读文本模式
        QTextStream in(&file);  // 创建文本流对象
        textEdit->setText(in.readAll());  // 在文本编辑器中显示文件内容
        file.close();  // 关闭文件
    }

    setLayout(layout);  // 设置窗口布局
}
