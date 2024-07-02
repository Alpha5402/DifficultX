#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), canvas(new Canvas(this)) {
    setCentralWidget(canvas);  // 将画布设置为主窗口的中心部件
}

MainWindow::~MainWindow() {
    delete canvas;  // 删除画布对象
}
