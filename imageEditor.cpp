#include "imageEditor.h"
#include "colorpickerwidget.h"

void ImageEditor::onShapeSelected(const QString &shapeInfo) {
    qDebug() << shapeInfo << Qt::endl;
}

void ImageEditor::openColorPickerWidget(){
    ColorPickerWidget *colorPicker = new ColorPickerWidget();
    // 获取A窗口的大小并设置给B窗口
    colorPicker->resize(this->size());

    // 获取A窗口的位置并设置给B窗口
    colorPicker->move(this->pos());
    connect(colorPicker, &ColorPickerWidget::sendColorData, this, &ImageEditor::receiveColorData);
    colorPicker->LineColor = QString("RGB(%1, %2, %3)").arg(drawingArea->scene->LineColor.color().red()).arg(drawingArea->scene->LineColor.color().green()).arg(drawingArea->scene->LineColor.color().blue());
    qDebug() << "[DEBUG] The colorPicker's LineColor has been set to " << colorPicker->LineColor;
    colorPicker->BackColor = QString("RGB(%1, %2, %3)").arg(drawingArea->scene->BackColor.color().red()).arg(drawingArea->scene->BackColor.color().green()).arg(drawingArea->scene->BackColor.color().blue());
    colorPicker->FillColor = QString("RGB(%1, %2, %3)").arg(drawingArea->scene->FillColor.color().red()).arg(drawingArea->scene->FillColor.color().green()).arg(drawingArea->scene->FillColor.color().blue());
        //QString("RGB(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue());
    colorPicker->show();
}

void ImageEditor::handleDrawingCircleFinished(const QString& message) {
    //qDebug() << "[INFO] ImageEditor has received the msg " << message;

    QStringList dataList = message.split(',');

    double x, y, Radius;

    if (dataList.size() == 3) {
        bool ok1, ok2, ok3;
        x = dataList[0].toDouble(&ok1);
        y = dataList[1].toDouble(&ok2);
        Radius = dataList[2].toDouble(&ok3);
    } else {
        qDebug() << "Incorrect format.";
    }

    lineEdit_1->setText(QString::number(x));
    lineEdit_2->setText(QString::number(y));
    lineEdit_3->setText(QString::number(Radius));
    //qDebug() << "[INFO] ImageEditor has received the msg " << x << "," << y << "," << Radius;
}

void ImageEditor::handleDrawingLineFinished(const QString& message) {
    //qDebug() << "[INFO] ImageEditor has received the msg " << message;

    QStringList dataList = message.split(',');

    double x1, y1, x2, y2;

    if (dataList.size() == 4) {
        bool ok1, ok2, ok3, ok4;
        x1 = dataList[0].toDouble(&ok1);
        y1 = dataList[1].toDouble(&ok2);
        x2 = dataList[2].toDouble(&ok3);
        y2 = dataList[3].toDouble(&ok4);
    } else {
        qDebug() << "Incorrect format.";
    }

    lineEdit_1->setText(QString::number(x1));
    lineEdit_2->setText(QString::number(y1));
    lineEdit_3->setText(QString::number(x2));
    lineEdit_4->setText(QString::number(y2));
    //qDebug() << "[INFO] ImageEditor has received the msg " << x << "," << y << "," << Radius;
}

void ImageEditor::handleDrawingRectFinished(const QString& message) {
    QStringList dataList = message.split(',');

    double x1, y1, x2, y2;

    if (dataList.size() == 4) {
        bool ok1, ok2, ok3, ok4;
        x1 = dataList[0].toDouble(&ok1);
        y1 = dataList[1].toDouble(&ok2);
        x2 = dataList[2].toDouble(&ok3);
        y2 = dataList[3].toDouble(&ok4);
    } else {
        qDebug() << "Incorrect format.";
    }

    lineEdit_1->setText(QString::number(x1));
    lineEdit_2->setText(QString::number(y1));
    lineEdit_3->setText(QString::number(x2));
    lineEdit_4->setText(QString::number(y2));
}

void ImageEditor::updateLineEdit(const QString& message) {
    //qDebug() << "[INFO] ImageEditor has received the msg " << message;
}

void ImageEditor::onAddCircleButtonClicked() {
    //qDebug() << "[INFO] Adding circle";
    isAddingCircle = true;  // 切换添加圆形状态
    isAddingText = false;  // 确保添加文本状态为false
    isAddingRectangle=false;
    isAddingLine = false;
    isAddingRectangle=false;
    drawingArea->scene->setAddingCircle(isAddingCircle);  // 通知场景更新添加圆形状态
    //drawingArea->scene->setAddingText(false);  // 确保场景的添加文本状态为false
    //qDebug() << "[INFO] Success";
}

void ImageEditor::onAddFillCircleButtonClicked() {
    isAddingCircle = true;  // 切换添加圆形状态
    isAddingText = false;  // 确保添加文本状态为false
    isAddingRectangle=false;
    isAddingLine = false;
    isAddingRectangle=false;
    drawingArea->scene->usingFill = true;
    drawingArea->scene->FillColor.setColor(FillCOLOR);
    qDebug() << "[INFO] ImageEditor send color" << drawingArea->scene->FillColor.color();
    drawingArea->scene->setAddingCircle(isAddingCircle);  // 通知场景更新添加圆形状态
    //drawingArea->scene->setAddingText(false);  // 确保场景的添加文本状态为false
}

void ImageEditor::onAddFillRectangleButtonClicked() {
    isAddingCircle = false; // 切换添加圆形状态
    isAddingText = false;  // 确保添加文本状态为false
    isAddingRectangle=true;
    isAddingLine = false;
    drawingArea->scene->usingFill = true;
    drawingArea->scene->FillColor.setColor(FillCOLOR);
    qDebug() << "[INFO] ImageEditor send color" << drawingArea->scene->FillColor.color();
    drawingArea->scene->setAddingRectangle(isAddingRectangle);  // 通知场景更新添加圆形状态
}

void ImageEditor::onAddLineButtonClicked()
{
    isAddingLine = !isAddingLine;  // 切换是否添加直线的状态
    isAddingCircle = false;  // 确保不在添加圆状态
    isAddingText = false;  // 确保不在添加文字状态
    isAddingRectangle=false;
    drawingArea->scene->setAddingLine(isAddingLine);  // 通知场景开始/停止添加直线
}

//文字按钮响应函数
void ImageEditor::onAddTextButtonClicked()
{
    isAddingText=!isAddingText;
    isAddingRectangle=false;//切换添加矩形状态
    isAddingLine = false;  // 确保不在添加直线的状态
    isAddingCircle = false;  // 确保不在添加圆状态
    //
    drawingArea->scene->setAddingText(isAddingText);  // 通知场景开始/停止添加矩形
}

//矩形按钮响应函数
void ImageEditor::onAddRectangleButtonClicked()
{
    isAddingRectangle=!isAddingRectangle;//切换添加矩形状态
    isAddingLine = false;  // 确保不在添加直线的状态
    isAddingCircle = false;  // 确保不在添加圆状态
    isAddingText = false;  // 确保不在添加文字状态
    drawingArea->scene->setAddingRectangle(isAddingRectangle);  // 通知场景开始/停止添加矩形
}

//点击生成代码触发
void ImageEditor::onGenerateCodeButtonClicked()
{
    // QString filename = QFileDialog::getSaveFileName(this, "Save Code", "", "Text Files (*.txt)");  // 获取保存文件名
    // if (!filename.isEmpty()) {  // 如果文件名非空
    generator.generateCode(drawingArea->scene);  // 生成代码文件
    // test_is_code_right << "ready to display" << endl;  // 输出调试信息
    // DisplayWindow *displayWindow = new DisplayWindow(filename);  // 创建显示窗口
    // displayWindow->show();  // 显示显示窗口
    // }
}
void ImageEditor::handleCircleAdded(QPointF center,qreal r) {
    generator.addCircle(center,r);  // 将圆形添加到生成器中
}

// 逆天 Layout 毁灭吧
ImageEditor::ImageEditor(QWidget *parent) : QWidget(parent) {
    isAddingLine = false;
    isAddingCircle = false;
    isAddingText = false;
    isAddingRectangle = false;
    usingFill = false;
    setWindowTitle("图形编辑器");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QWidget *Buttons = new QWidget(this);
    QVBoxLayout *buttonsLayout = new QVBoxLayout(Buttons);
    QWidget *EditorSet = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(EditorSet);
    // 设置子窗口的布局和控件

    drawingArea = new CustomDrawingArea(LineColor, this);
    drawingArea->scene->setSceneRect(0, 0, 640, 480);
    //qDebug() << "[INFO] DrawingArea: Set Pen to " << LineColor;


    connect(drawingArea, &DrawingArea::shapeSelected, this, &ImageEditor::onShapeSelected);
    connect(drawingArea->scene, &CustomGraphicsScene::drawingCircleFinished, this, &ImageEditor::handleDrawingCircleFinished);
    connect(drawingArea->scene, &CustomGraphicsScene::drawingLineFinished, this, &ImageEditor::handleDrawingLineFinished);
    connect(drawingArea->scene, &CustomGraphicsScene::drawingRectFinished, this, &ImageEditor::handleDrawingRectFinished);

    mainLayout->addWidget(drawingArea, 4);
    //setCentralWidget(view);  // 设置中心部件为图形视图

    QWidget *parameterArea = new QWidget(this);
    QVBoxLayout *parameterAreaLayout = new QVBoxLayout(parameterArea);

        QLabel *parameterTitle = new QLabel("参数区", this);
        parameterTitle->setAlignment(Qt::AlignCenter);
        parameterTitle->setMinimumSize(QSize(4, 4));
        layout->addWidget(parameterTitle, 4);
        parameterAreaLayout->addWidget(parameterTitle, 1);

        QDoubleValidator *validator = new QDoubleValidator(this);
        validator->setNotation(QDoubleValidator::StandardNotation);
        // 设置输入验证器，只允许输入整数或浮点数

        QWidget *para_line_1 = new QWidget(this);
        QHBoxLayout *para_line_box_1 = new QHBoxLayout(para_line_1);
        QWidget *parameter_1 = new QWidget(this);
        QVBoxLayout *para_box_1 = new QVBoxLayout(parameter_1);
        QLabel *parameterLabel_1 = new QLabel("参数 1", this);
        lineEdit_1 = new QLineEdit(this);
        lineEdit_1->setPlaceholderText("Value 1");  // 设置默认值
        int buttonHeight = lineEdit_1->sizeHint().height();
        parameterLabel_1->setFixedHeight(buttonHeight);
        lineEdit_1->setValidator(validator);
        para_box_1->addWidget(parameterLabel_1, 1);
        para_box_1->addWidget(lineEdit_1, 1);
        connect(lineEdit_1, &QLineEdit::editingFinished, this, &ImageEditor::handlePara1EditingFinished);
        QObject::connect(this, &ImageEditor::Para1Changed, drawingArea->scene, &CustomGraphicsScene::ReceivePara1ValueChanged);
        para_line_box_1->addWidget(parameter_1, 1);

        parameterTitle->setFixedHeight(buttonHeight);

        QWidget *parameter_2 = new QWidget(this);
        QVBoxLayout *para_box_2 = new QVBoxLayout(parameter_2);
        QLabel *parameterLabel_2 = new QLabel("参数 2", this);
        lineEdit_2 = new QLineEdit(this);
        lineEdit_2->setPlaceholderText("Value 2");  // 设置默认值
        parameterLabel_2->setFixedHeight(buttonHeight);
        lineEdit_2->setValidator(validator);
        para_box_2->addWidget(parameterLabel_2, 1);
        para_box_2->addWidget(lineEdit_2, 1);
        connect(lineEdit_2, &QLineEdit::editingFinished, this, &ImageEditor::handlePara2EditingFinished);
        QObject::connect(this, &ImageEditor::Para2Changed, drawingArea->scene, &CustomGraphicsScene::ReceivePara2ValueChanged);
        para_line_box_1->addWidget(parameter_2, 1);

        QWidget *para_line_2 = new QWidget(this);
        QHBoxLayout *para_line_box_2 = new QHBoxLayout(para_line_2);
        QWidget *parameter_3 = new QWidget(this);
        QVBoxLayout *para_box_3 = new QVBoxLayout(parameter_3);
        QLabel *parameterLabel_3 = new QLabel("参数 3", this);
        lineEdit_3 = new QLineEdit(this);
        lineEdit_3->setPlaceholderText("Value 3");  // 设置默认值
        parameterLabel_3->setFixedHeight(buttonHeight);
        lineEdit_3->setValidator(validator);
        para_box_3->addWidget(parameterLabel_3, 1);
        para_box_3->addWidget(lineEdit_3, 1);
        connect(lineEdit_3, &QLineEdit::editingFinished, this, &ImageEditor::handlePara3EditingFinished);
        QObject::connect(this, &ImageEditor::Para3Changed, drawingArea->scene, &CustomGraphicsScene::ReceivePara3ValueChanged);
        para_line_box_2->addWidget(parameter_3, 1);

        QWidget *parameter_4 = new QWidget(this);
        QVBoxLayout *para_box_4 = new QVBoxLayout(parameter_4);
        QLabel *parameterLabel_4 = new QLabel("参数 4", this);
        lineEdit_4 = new QLineEdit(this);
        lineEdit_4->setPlaceholderText("Value 4");  // 设置默认值
        parameterLabel_4->setFixedHeight(buttonHeight);
        lineEdit_4->setValidator(validator);
        para_box_4->addWidget(parameterLabel_4, 1);
        para_box_4->addWidget(lineEdit_4, 1);
        connect(lineEdit_4, &QLineEdit::editingFinished, this, &ImageEditor::handlePara4EditingFinished);
        QObject::connect(this, &ImageEditor::Para4Changed, drawingArea->scene, &CustomGraphicsScene::ReceivePara4ValueChanged);
        para_line_box_2->addWidget(parameter_4, 1);

        parameterAreaLayout->addWidget(para_line_1, 1);
        parameterAreaLayout->addWidget(para_line_2, 1);
        layout->addWidget(parameterArea, 1);

    QWidget *insertButton[3];
    for(int i = 0; i < 3; i++)
        insertButton[i] = new QWidget(this);
    QHBoxLayout *insertButtonLayout[3];
    for(int i = 0; i < 3; i++)
        insertButtonLayout[i] = new QHBoxLayout(insertButton[i]);

    QPushButton *insertLineButton = new QPushButton("插入 直线", this);
    insertButtonLayout[0]->addWidget(insertLineButton);
    QPushButton *insertTextButton = new QPushButton("插入 文本", this);
    insertButtonLayout[0]->addWidget(insertTextButton);
    QPushButton *insertRectButton = new QPushButton("插入 矩形", this);
    insertButtonLayout[1]->addWidget(insertRectButton);
    QPushButton *insertCircleButton = new QPushButton("插入 圆", this);
    insertButtonLayout[1]->addWidget(insertCircleButton);
    QPushButton *insertFillRectButton = new QPushButton("插入 填充矩形", this);
    insertButtonLayout[2]->addWidget(insertFillRectButton);
    QPushButton *insertFillCircleButton = new QPushButton("插入 填充圆", this);
    insertButtonLayout[2]->addWidget(insertFillCircleButton);

    //链接按钮槽和信号
    connect(insertCircleButton, &QPushButton::clicked, this, &ImageEditor::onAddCircleButtonClicked);
    connect(insertFillCircleButton, &QPushButton::clicked, this, &ImageEditor::onAddFillCircleButtonClicked);
    connect(insertLineButton, &QPushButton::clicked, this, &ImageEditor::onAddLineButtonClicked);
    connect(insertRectButton, &QPushButton::clicked, this, &ImageEditor::onAddRectangleButtonClicked);
    connect(insertFillRectButton, &QPushButton::clicked, this, &ImageEditor::onAddFillRectangleButtonClicked);
    connect(insertTextButton, &QPushButton::clicked, this, &ImageEditor::onAddTextButtonClicked);

    connect(drawingArea->scene, &CustomGraphicsScene::circleAdded, this, &ImageEditor::handleCircleAdded);

    QPushButton *closeButton = new QPushButton("关闭", this);


    QLabel *ColorTitle = new QLabel("调色区", this);
    ColorTitle->setAlignment(Qt::AlignCenter);
    ColorTitle->setFixedHeight(2 * buttonHeight);
    QPushButton *ColorButton = new QPushButton("选取颜色", this);
    colorPicker = new ColorPickerWidget();

    QObject::connect(ColorButton, &QPushButton::clicked, this, &ImageEditor::openColorPickerWidget);

    layout->addWidget(ColorTitle, 1);
    layout->addWidget(ColorButton, 1);

    QLabel *StyleTitle = new QLabel("样式区", this);
    StyleTitle->setAlignment(Qt::AlignCenter);
    StyleTitle->setFixedHeight(2 * buttonHeight);
    fontComboBox = new QComboBox(this);
    QFontDatabase fontDatabase;
    fontComboBox->addItems(fontDatabase.families());

    QWidget * brushBox = new QWidget(this);
    QVBoxLayout * brushBoxLayout = new QVBoxLayout(brushBox);
    QSlider* brushSlider(new QSlider(Qt::Horizontal, this));
    styleLabel = new QLabel("设置笔刷大小: 4", this);
    brushSlider->setMinimum(1);
    brushSlider->setMaximum(16);
    brushSlider->setValue(4);
    brushSlider->setTickPosition(QSlider::TicksBelow);
    brushSlider->setTickInterval(1);
    brushBoxLayout->addWidget(styleLabel, 1);
    brushBoxLayout->addWidget(brushSlider, 1);
    LineColor.setWidth(4);
    drawingArea->scene->LineColor.setWidth(4);

    fontSizeComboBox = new QComboBox(this);
    for (int i = 8; i <= 48; i += 2) {
        fontSizeComboBox->addItem(QString::number(i));
    }
    fontSizeComboBox->setCurrentText("12");

    //setCentralWidget(centralWidget);

    fontLabel = new QLabel("设置文本字体: 微软雅黑", this);
    QString defaultFont = "微软雅黑"; // 预设的字体名称
    int defaultIndex = fontComboBox->findText(defaultFont);
    if (defaultIndex != -1) {
        fontComboBox->setCurrentIndex(defaultIndex);
        QFont font(defaultFont);
        fontLabel->setFont(font); // 设置 label 的初始字体
    }
    fontLabel->setFont(QFont("微软雅黑"));

    QWidget * FontBox = new QWidget(this);
    QVBoxLayout * fontBoxLayout = new QVBoxLayout(FontBox);
    QWidget * FontComboBox = new QWidget(this);
    QHBoxLayout * fontComboBoxLayout = new QHBoxLayout(FontComboBox);
    fontComboBox->setMinimumSize(QSize(16, 16));
    fontComboBoxLayout->addWidget(fontComboBox, 3);
    fontComboBoxLayout->addWidget(fontSizeComboBox, 1);
    fontBoxLayout->addWidget(fontLabel, 1);
    fontBoxLayout->addWidget(FontComboBox, 1);

    connect(brushSlider, &QSlider::valueChanged, this, &ImageEditor::onSliderValueChanged);
    connect(fontComboBox, &QComboBox::currentTextChanged, this, &ImageEditor::onFontChanged);
    connect(fontSizeComboBox, &QComboBox::currentTextChanged, this, &ImageEditor::onFontSizeChanged);

    layout->addWidget(StyleTitle, 1);
    layout->addWidget(brushBox, 1);
    layout->addWidget(FontBox, 1);

    QLabel *ButtonsTitle = new QLabel("插入区", this);
    ButtonsTitle->setFixedHeight(buttonHeight);
    ButtonsTitle->setAlignment(Qt::AlignCenter);
    buttonsLayout->addWidget(ButtonsTitle, 4);

    buttonsLayout->addWidget(insertButton[0], 1);
    buttonsLayout->addWidget(insertButton[1], 1);
    buttonsLayout->addWidget(insertButton[2], 1);
    //buttonsLayout->addWidget(insertTextButton, 1);
    layout->addWidget(Buttons, 6);

    layout->addWidget(closeButton, 3);
    EditorSet->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(EditorSet, 1);

    QPainter painter(this);
    painter.setPen(Qt::blue);
    drawingArea->drawShapes(painter);

    connect(closeButton, &QPushButton::clicked, this, &ImageEditor::onGenerateCodeButtonClicked);
    connect(closeButton, &QPushButton::clicked, this, &ImageEditor::onSendData);
}

void ImageEditor::receiveColorData(const QString &data) {
    ToHandle += data;

    QRegularExpression regex("RGB\\((\\d+),\\s*(\\d+),\\s*(\\d+)\\)");
    QRegularExpressionMatchIterator i = regex.globalMatch(data);

    QList<QList<int>> colors;

    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        if (match.hasMatch()) {
            QList<int> color;
            color.append(match.captured(1).toInt());
            color.append(match.captured(2).toInt());
            color.append(match.captured(3).toInt());
            colors.append(color);
        } else {
            QList<int> color;
            color.append(255);
            color.append(255);
            color.append(255);
            colors.append(color);
        }
    }

    if (!colors.isEmpty()) {
        QList<int> backColor = colors.at(0);
        BackColor = QPen(QColor(backColor.at(0), backColor.at(1), backColor.at(2)));

        if (colors.size() > 1) {
            QList<int> lineColor = colors.at(1);
            //LineColor = QPen(QColor(lineColor.at(0), lineColor.at(1), lineColor.at(2)));
            LineColor.setColor(QColor(lineColor.at(0), lineColor.at(1), lineColor.at(2)));
            drawingArea->scene->LineColor = LineColor;
        }

        if (colors.size() > 2) {
            QList<int> fillColor = colors.at(2);
            FillColor.setColor(QColor(fillColor.at(0), fillColor.at(1), fillColor.at(2)));
            FillCOLOR = QColor(fillColor.at(0), fillColor.at(1), fillColor.at(2));
        }
    }
}

void ImageEditor::onSendData() {
    ToHandle = generator.getCode();
    emit sendData(ToHandle); // 发射信号将数据传回A窗口
    this->close(); // 关闭B窗口
}

void ImageEditor::handlePara1EditingFinished()
{
    bool ok;
    double value = lineEdit_1->text().toDouble(&ok);

    if (ok)
        emit Para1Changed(value);
    //LineColor.color()
}

void ImageEditor::handlePara2EditingFinished()
{
    bool ok;
    double value = lineEdit_2->text().toDouble(&ok);

    if (ok)
        emit Para2Changed(value);
}

void ImageEditor::handlePara3EditingFinished()
{
    bool ok;
    double value = lineEdit_3->text().toDouble(&ok);

    if (ok)
        emit Para3Changed(value);
}

void ImageEditor::handlePara4EditingFinished()
{
    bool ok;
    double value = lineEdit_4->text().toDouble(&ok);

    if (ok)
        emit Para4Changed(value);
}

void ImageEditor::onSliderValueChanged(int value)
{
    styleLabel->setText(QString("设置笔刷大小: %1").arg(value));
    LineColor.setWidth(value);
    drawingArea->scene->LineColor.setWidth(value);
}

void ImageEditor::onFontChanged(const QString &fontName)
{
    QFont font(fontName);
    fontLabel->setText(QString("设置文本字体: %1").arg(fontName));
    font.setPixelSize(fontSizeComboBox->currentText().toInt());
    fontLabel->setFont(font);
    textFont = font;
    drawingArea->scene->text->setFont(textFont);
    //label->setFont(font);
}

void ImageEditor::onFontSizeChanged(const QString &fontSize)
{
    QFont font(fontComboBox->currentText(), fontSize.toInt());
    //textFont.setPixelSize(fontSize.toInt());
    textFont = font;
    drawingArea->scene->text->setFont(textFont);
}
