#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include "communal.h"
#include "drawingArea.h"
#include "customDrawingArea.h"
#include "customgraphicssene.h"
#include "easyxcodegenerator.h"

class ImageEditor : public QWidget {
    Q_OBJECT
public:
    DrawingArea *drawingArea;
    ImageEditor(QWidget *parent = nullptr);
    void openColorPickerWidget();
    QString ToHandle;
    QWidget * colorPicker;
private:
    bool isAddingCircle;
    bool isAddingText;
    EasyXCodeGenerator generator;
    QLineEdit *lineEdit_1;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;


signals:
    void sendData(const QString &data);
    void drawingFinished(const QString &message);

private slots:
    void onShapeSelected(const QString &shapeInfo);
    void onAddCircleButtonClicked();
    void receiveColorData(const QString &data);
    void onSendData();
    void handleCircleAdded(QPointF center,qreal r);
    void handleDrawingFinished(const QString& message);
    void updateLineEdit(const QString& message);
};

#endif // IMAGEEDITOR_H
