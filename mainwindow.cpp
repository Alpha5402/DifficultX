#include "MainWindow.h"
#include <QToolBar>
#include <QGraphicsSceneMouseEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <QLineEdit>

// 构造函数，初始化主窗口
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isAddingCircle(false), isAddingText(false)
{
    // 创建自定义图形场景和视图，并设置为中央部件
    scene = new CustomGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    setCentralWidget(view);

    // 创建按钮
    addCircleButton = new QPushButton("Add Circle", this);
    addTextButton = new QPushButton("Add Text", this);
    generateCodeButton = new QPushButton("Generate Code", this);

    // 创建工具栏并添加按钮
    QToolBar *toolBar = new QToolBar(this);
    toolBar->addWidget(addCircleButton);
    toolBar->addWidget(addTextButton);
    toolBar->addWidget(generateCodeButton);
    addToolBar(toolBar);

    // 连接按钮点击信号到槽函数
    connect(addCircleButton, &QPushButton::clicked, this, &MainWindow::onAddCircleButtonClicked);
    connect(addTextButton, &QPushButton::clicked, this, &MainWindow::onAddTextButtonClicked);
    connect(generateCodeButton, &QPushButton::clicked, this, &MainWindow::onGenerateCodeButtonClicked);
    connect(scene, &CustomGraphicsScene::sceneClicked, this, &MainWindow::onSceneClicked);

    // 设置窗口固定大小
    setFixedSize(800, 600);
}

// 析构函数
MainWindow::~MainWindow()
{
}

// 添加圆按钮点击事件处理
void MainWindow::onAddCircleButtonClicked()
{
    isAddingCircle = !isAddingCircle;  // 切换是否添加圆的状态
    isAddingText = false;  // 确保不在添加文字状态
}

// 添加文字按钮点击事件处理
void MainWindow::onAddTextButtonClicked()
{
    isAddingText = !isAddingText;  // 切换是否添加文字的状态
    isAddingCircle = false;  // 确保不在添加圆状态
}

// 生成代码按钮点击事件处理
void MainWindow::onGenerateCodeButtonClicked()
{
    // 弹出文件保存对话框，让用户选择保存文件的路径和名称
    QString filename = QFileDialog::getSaveFileName(this, "Save Code", "", "Text Files (*.txt)");
    if (!filename.isEmpty()) {
        EasyXCodeGenerator generator;
        generator.generateCode(filename);  // 调用代码生成器生成代码
    }
}

// 场景点击事件处理
void MainWindow::onSceneClicked(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->scenePos();  // 获取点击的位置

    if (isAddingCircle) {
        // 如果是添加圆的状态
        if (firstPoint.isNull()) {
            firstPoint = pos;  // 记录第一个点的位置
        } else {
            qreal radius = QLineF(firstPoint, pos).length();  // 计算圆的半径
            CustomCircleItem *circle = new CustomCircleItem(firstPoint, radius);  // 创建圆形项目
            scene->addItem(circle);  // 添加到场景中
            EasyXCodeGenerator generator;
            generator.addCircle(firstPoint);  // 将圆的信息添加到代码生成器
            firstPoint = QPointF();  // 重置为下一个圆做准备
            isAddingCircle = false;  // 结束添加圆的状态
        }
    } else if (isAddingText) {
        // 如果是添加文字的状态
        bool ok;
        QString text = QInputDialog::getText(this, "输入文字", "请输入文字:", QLineEdit::Normal, "", &ok);  // 弹出输入文字对话框
        if (ok && !text.isEmpty()) {
            TextItem *textItem = new TextItem(pos, text);  // 创建文字项目
            scene->addItem(textItem);  // 添加到场景中
            EasyXCodeGenerator generator;
            generator.addText(text, pos);  // 将文字的信息添加到代码生成器
            isAddingText = false;  // 结束添加文字的状态
        }
    }
}
