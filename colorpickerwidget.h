#ifndef COLORPICKERWIDGET_H
#define COLORPICKERWIDGET_H

#include "communal.h"

class ColorPickerWidget : public QWidget {
    Q_OBJECT

public:
    ColorPickerWidget(QWidget *parent = nullptr);
    QString LineColor;
    QString FillColor;
    QString BackColor;
    QString TextColor;

signals:
    void sendColorData(const QString &data);

private slots:
    void onSendColorData();
    void chooseColor();

private:
    void SetLineColor();
    void SetFillColor();
    void SetBackColor();
    void SetTextColor();
    void getLineColor();
    void getFillColor();
    void getBackColor();
    void getTextColor();
    QLabel *colorLabel;

    QString CurrentColor;
};
#endif // COLORPICKERWIDGET_H
