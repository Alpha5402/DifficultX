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

private:

    CustomGraphicsScene *scene;
    QGraphicsView *view;
    QPushButton *addCircleButton;
    QPushButton *addTextButton;
    QPushButton *generateCodeButton;
    EasyXCodeGenerator generator;
    bool isAddingCircle;
    bool isAddingText;
};

#endif // MAINWINDOW_H
