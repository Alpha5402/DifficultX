#include "MainWindow.h"
#include "DisplayWindow.h"
#include <QToolBar>
#include <QFileDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QTextStream>

QTextStream test_is_code_right(stdout);
using Qt::endl;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isAddingCircle(false), isAddingText(false) {
    scene = new CustomGraphicsScene(this);  // 创建自定义图形场景
    view = new QGraphicsView(scene, this);  // 创建图形视图，并将场景设置为中心部件
    setCentralWidget(view);  // 设置中心部件为图形视图

    addCircleButton = new QPushButton("Add Circle", this);  // 创建添加圆形按钮
    addTextButton = new QPushButton("Add Text", this);  // 创建添加文本按钮
    generateCodeButton = new QPushButton("Generate Code", this);  // 创建生成代码按钮

    QToolBar *toolBar = new QToolBar(this);  // 创建工具栏
    toolBar->addWidget(addCircleButton);  // 将添加圆形按钮添加到工具栏
    toolBar->addWidget(addTextButton);  // 将添加文本按钮添加到工具栏
    toolBar->addWidget(generateCodeButton);  // 将生成代码按钮添加到工具栏
    addToolBar(toolBar);  // 添加工具栏到主窗口

    connect(addCircleButton, &QPushButton::clicked, this, &MainWindow::onAddCircleButtonClicked);  // 连接添加圆形按钮点击事件
    connect(addTextButton, &QPushButton::clicked, this, &MainWindow::onAddTextButtonClicked);  // 连接添加文本按钮点击事件
    connect(generateCodeButton, &QPushButton::clicked, this, &MainWindow::onGenerateCodeButtonClicked);  // 连接生成代码按钮点击事件
    connect(scene, &CustomGraphicsScene::circleAdded, this, &MainWindow::handleCircleAdded);  // 连接图形场景的圆形添加信号
    connect(scene, &CustomGraphicsScene::textAdded, this, &MainWindow::handleTextAdded);  // 连接图形场景的文本添加信号

    setFixedSize(640, 480);  // 设置主窗口固定大小
}

MainWindow::~MainWindow() {}  // 析构函数为空实现

void MainWindow::onAddCircleButtonClicked() {
    isAddingCircle = !isAddingCircle;  // 切换添加圆形状态
    isAddingText = false;  // 确保添加文本状态为false
    scene->setAddingCircle(isAddingCircle);  // 通知场景更新添加圆形状态
    scene->setAddingText(false);  // 确保场景的添加文本状态为false
}

void MainWindow::onAddTextButtonClicked() {
    isAddingText = !isAddingText;  // 切换添加文本状态
    isAddingCircle = false;  // 确保添加圆形状态为false
    scene->setAddingCircle(false);  // 确保场景的添加圆形状态为false
    scene->setAddingText(isAddingText);  // 通知场景更新添加文本状态
}

void MainWindow::onGenerateCodeButtonClicked() {
    QString filename = QFileDialog::getSaveFileName(this, "Save Code", "", "Text Files (*.txt)");  // 获取保存文件名
    if (!filename.isEmpty()) {  // 如果文件名非空
        generator.generateCode(filename);  // 生成代码文件
        test_is_code_right << "ready to display" << endl;  // 输出调试信息
        DisplayWindow *displayWindow = new DisplayWindow(filename);  // 创建显示窗口
        displayWindow->show();  // 显示显示窗口
    }
}

void MainWindow::handleCircleAdded(const QPointF &center) {
    generator.addCircle(center);  // 将圆形添加到生成器中
}

void MainWindow::handleTextAdded(const QString &text, const QPointF &pos) {
    generator.addText(text, pos);  // 将文本添加到生成器中
}
