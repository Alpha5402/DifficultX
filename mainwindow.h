#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QPushButton>
#include "CustomGraphicsScene.h"
#include "easyxcodegenerator.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddCircleButtonClicked();
    void onAddTextButtonClicked();
    void onGenerateCodeButtonClicked();
    void handleCircleAdded(QPointF center,qreal r);
    void handleTextAdded(const QString &text, const QPointF &pos);
    void onAddLineButtonClicked();  // 新增：添加直线按钮点击事件处理

private:

    CustomGraphicsScene *scene;
    QGraphicsView *view;
    QPushButton *addCircleButton;
    QPushButton *addTextButton;
    QPushButton *generateCodeButton;
    QPushButton *addLineButton;
    EasyXCodeGenerator generator;

    //点击状态控制
    bool isAddingCircle;
    bool isAddingText;
    bool isAddingLine;

};

#endif // MAINWINDOW_H
