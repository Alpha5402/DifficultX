#include "MainWindow.h"
#include"displaywindow.h"
#include <QToolBar>
#include <QGraphicsSceneMouseEvent>
#include <QFileDialog>
#include <QInputDialog>
#include <QLineEdit>

QTextStream test_is_code_right(stdout);
using Qt::endl;

// 构造函数，初始化主窗口
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
    //connect(scene, &CustomGraphicsScene::sceneClicked, this, &MainWindow::onSceneClicked);

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
    QString filename = QFileDialog::getSaveFileName(this, "Save Code", "", "Text Files (*.txt)");
    if (!filename.isEmpty()) {
        generator.generateCode(filename);

        // 创建和显示代码窗口
        test_is_code_right<<"ready to display"<<endl;
        DisplayWindow *displayWindow = new DisplayWindow(filename);
        //displayWindow->setText(generator.getCode());
        displayWindow->show();
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
            CustomCircleItem *circle = new CustomCircleItem(firstPoint.x(), firstPoint.y(), radius * 2);
            scene->addItem(circle);
            generator.addCircle(firstPoint);
            firstPoint = QPointF();
            isAddingCircle = false;
        }
    } else if (isAddingText) {
        bool ok;
        QString text = QInputDialog::getText(this, "输入文字", "请输入文字:", QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
            TextItem *textItem = new TextItem(pos, text);
            scene->addItem(textItem);
            generator.addText(text, pos);
            isAddingText = false;
        }
    }
}
