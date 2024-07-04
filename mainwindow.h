#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "communal.h"
#include "customDrawingArea.h"

struct Operation{
    QString type;
    double parameter_1;
    double parameter_2;
    double parameter_3;
    double parameter_4;
};

struct Line{
    QPoint Point1;
    QPoint Point2;
};


struct Circle{
    QPoint center;
    double Radius;
};

struct myRectangle{
    QPoint Point1;
    QPoint Point2;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    std::vector<Line> lineList;
    std::vector<Circle> circleList;
    std::vector<myRectangle> rectangleList;

private:
    QTextEdit *textEdit;
    std::list<Operation> ReadCodes();
private slots:
    void onCursorPositionChanged();
    void openImageEditor();
public slots:
    void DrawSomeThing(const QString& text);
    void receiveData(const QString &data);
};

#endif // MAINWINDOW_H
