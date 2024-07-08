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
    bool isAddingLine;
    bool isAddingRectangle;

    EasyXCodeGenerator generator;
    QLineEdit *lineEdit_1;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QPen LineColor;
    QPen FillColor;
    QFont textFont;
    QPen BackColor;
    QLabel *styleLabel;
    QLabel *fontLabel;
    QComboBox *fontComboBox;


signals:
    void sendData(const QString &data);
    void drawingFinished(const QString &message);
    void Para1Changed(double newValue);
    void Para2Changed(double newValue);
    void Para3Changed(double newValue);
    void Para4Changed(double newValue);

private slots:
    void onShapeSelected(const QString &shapeInfo);

    void onAddCircleButtonClicked();
    void onAddLineButtonClicked();
    void onGenerateCodeButtonClicked();
    void onAddRectangleButtonClicked();
    void onAddTextButtonClicked();

    void receiveColorData(const QString &data);
    void onSendData();
    void handleCircleAdded(QPointF center,qreal r);
    void handleDrawingCircleFinished(const QString& message);
    void handleDrawingLineFinished(const QString& message);

    void updateLineEdit(const QString& message);
    void handlePara1EditingFinished();
    void handlePara2EditingFinished();
    void handlePara3EditingFinished();
    void handlePara4EditingFinished();

    void onSliderValueChanged(int value);
    void onFontChanged(const QString &fontName);
    void onFontSizeChanged(const QString &fontSize);
};

#endif // IMAGEEDITOR_H
