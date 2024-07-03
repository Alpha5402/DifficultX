#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QPushButton>
#include "CustomGraphicsScene.h"
#include "CustomCircleItem.h"
#include "TextItem.h"
#include "EasyXCodeGenerator.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddCircleButtonClicked();
    void onAddTextButtonClicked();
    void onGenerateCodeButtonClicked();
    void onSceneClicked(QGraphicsSceneMouseEvent *event);

private:
    CustomGraphicsScene *scene;
    QGraphicsView *view;
    QPushButton *addCircleButton;
    QPushButton *addTextButton;
    QPushButton *generateCodeButton;
    bool isAddingCircle;
    bool isAddingText;
    QPointF firstPoint;
    std::vector<QPointF> circleCenters;
    std::vector<EasyXCodeGenerator::TextInfo> texts;
    EasyXCodeGenerator generator;  // 将生成器对象放在这里，确保它的状态在整个MainWindow类中保持一致
};

#endif // MAINWINDOW_H
