#include "MainWindow.h"
#include <QToolBar>
#include <QGraphicsSceneMouseEvent>
#include <QFileDialog>
#include <QInputDialog> // 添加这一行
#include <QLineEdit>    // 添加这一行

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), isAddingCircle(false), isAddingText(false)
{
    scene = new CustomGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    setCentralWidget(view);

    addCircleButton = new QPushButton("Add Circle", this);
    addTextButton = new QPushButton("Add Text", this);
    generateCodeButton = new QPushButton("Generate Code", this);

    QToolBar *toolBar = new QToolBar(this);
    toolBar->addWidget(addCircleButton);
    toolBar->addWidget(addTextButton);
    toolBar->addWidget(generateCodeButton);
    addToolBar(toolBar);

    connect(addCircleButton, &QPushButton::clicked, this, &MainWindow::onAddCircleButtonClicked);
    connect(addTextButton, &QPushButton::clicked, this, &MainWindow::onAddTextButtonClicked);
    connect(generateCodeButton, &QPushButton::clicked, this, &MainWindow::onGenerateCodeButtonClicked);
    connect(scene, &CustomGraphicsScene::sceneClicked, this, &MainWindow::onSceneClicked);

    setFixedSize(800, 600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onAddCircleButtonClicked()
{
    isAddingCircle = !isAddingCircle;
    isAddingText = false;
}

void MainWindow::onAddTextButtonClicked()
{
    isAddingText = !isAddingText;
    isAddingCircle = false;
}

void MainWindow::onGenerateCodeButtonClicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save Code", "", "C++ Files (*.cpp)");
    if (!filename.isEmpty()) {
        EasyXCodeGenerator generator;
        generator.generateCode(filename);
    }
}

void MainWindow::onSceneClicked(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->scenePos();

    if (isAddingCircle) {
        if (firstPoint.isNull()) {
            firstPoint = pos;
        } else {
            qreal radius = QLineF(firstPoint, pos).length();
            CustomCircleItem *circle = new CustomCircleItem(firstPoint, radius);
            scene->addItem(circle);
            EasyXCodeGenerator generator;
            generator.addCircle(firstPoint);
            firstPoint = QPointF();  // 重置为下一个圆做准备
            isAddingCircle = false;
        }
    } else if (isAddingText) {
        bool ok;
        QString text = QInputDialog::getText(this, "输入文字", "请输入文字:", QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
            TextItem *textItem = new TextItem(pos, text);
            scene->addItem(textItem);
            EasyXCodeGenerator generator;
            generator.addText(text, pos);
            isAddingText = false;
        }
    }
}
