#include "colorpickerwidget.h"

ColorPickerWidget::ColorPickerWidget(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    resize(640, 160);

    QPushButton *button = new QPushButton("选择颜色", this);
    QPushButton *setbkcolor = new QPushButton("设置为背景颜色", this);
    connect(setbkcolor, &QPushButton::clicked, this, &ColorPickerWidget::SetBackColor);
    QPushButton *setlinecolor = new QPushButton("设置为线条颜色", this);
    connect(setlinecolor, &QPushButton::clicked, this, &ColorPickerWidget::SetLineColor);
    QPushButton *setfillcolor = new QPushButton("设置为填充颜色", this);
    connect(setfillcolor, &QPushButton::clicked, this, &ColorPickerWidget::SetFillColor);
    QPushButton *settextcolor = new QPushButton("设置为字体颜色", this);
    connect(settextcolor, &QPushButton::clicked, this, &ColorPickerWidget::SetTextColor);
    QWidget *SetColor = new QWidget(this);
    QHBoxLayout *ButtonSet = new QHBoxLayout(SetColor);
    ButtonSet->addWidget(setbkcolor, 1);
    ButtonSet->addWidget(setlinecolor, 1);
    ButtonSet->addWidget(setfillcolor, 1);
    ButtonSet->addWidget(settextcolor, 1);

    QPushButton *getbkcolor = new QPushButton("获取背景颜色", this);
    QPushButton *getlinecolor = new QPushButton("获取线条颜色", this);
    QPushButton *getfillcolor = new QPushButton("获取填充颜色", this);
    QPushButton *gettextcolor = new QPushButton("获取字体颜色", this);
    QPushButton *confirm = new QPushButton("确定", this);
    connect(confirm, &QPushButton::clicked, this, &ColorPickerWidget::onSendColorData);
    QWidget *GetColor = new QWidget(this);
    QHBoxLayout *ButtonGet = new QHBoxLayout(GetColor);
    ButtonGet->addWidget(getbkcolor, 1);
    ButtonGet->addWidget(getlinecolor, 1);
    ButtonGet->addWidget(getfillcolor, 1);
    ButtonGet->addWidget(gettextcolor, 1);

    layout->addWidget(button);
    layout->addWidget(GetColor);

    colorLabel = new QLabel("RGB: ", this);
    layout->addWidget(colorLabel);

    layout->addWidget(SetColor);
    layout->addWidget(confirm);

    connect(button, &QPushButton::clicked, this, &ColorPickerWidget::chooseColor);
}

void ColorPickerWidget::chooseColor() {
    QColorDialog colorDialog(this);
    colorDialog.setOption(QColorDialog::DontUseNativeDialog, true); // 使用Qt对话框
    colorDialog.resize(800, 600); // 设置调色板对话框大小
    QColor color = colorDialog.getColor(Qt::white, this, "选择一个颜色");
    if (color.isValid()) {
        QString colorInfo = QString("RGB(%1, %2, %3)")
                                .arg(color.red())
                                .arg(color.green())
                                .arg(color.blue());
        colorLabel->setText(colorInfo);
        CurrentColor = colorInfo;
    }
}

void SendColor(QString color, QString type){
    color += type;
}

void ColorPickerWidget::onSendColorData() {
    QString data = "setbkcolor(" + BackColor +
                           ");\n\tsetlinecolor(" + LineColor +
                           ");\n\tsetfillcolor(" +  FillColor + ");";
                        // .arg(BackColor)
                        // .arg(LineColor)
                        // .arg(FillColor);
    qDebug() << "[INFO] colorPickerWidget sending" << Qt::endl;
    emit sendColorData(data); // 发射信号将数据传回A窗口
    qDebug() << "[INFO] colorPickerWidget sended " << data << Qt::endl;
    this->close(); // 关闭B窗口
}

void ColorPickerWidget::SetLineColor(){
    LineColor = CurrentColor;
}
void ColorPickerWidget::SetFillColor(){
    FillColor = CurrentColor;
}
void ColorPickerWidget::SetBackColor(){
    BackColor = CurrentColor;
}
void ColorPickerWidget::SetTextColor(){
    TextColor = CurrentColor;
}
