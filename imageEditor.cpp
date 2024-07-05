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

void ImageEditor::updateLineEdit(const QString& message) {
    //qDebug() << "[INFO] ImageEditor has received the msg " << message;
}

void ImageEditor::onAddCircleButtonClicked() {
    //qDebug() << "[INFO] Adding circle";
    isAddingCircle = !isAddingCircle;  // 切换添加圆形状态
    isAddingText = false;  // 确保添加文本状态为false
    drawingArea->scene->setAddingCircle(isAddingCircle);  // 通知场景更新添加圆形状态
    drawingArea->scene->setAddingText(false);  // 确保场景的添加文本状态为false
    //qDebug() << "[INFO] Success";
}

void ImageEditor::onAddLineButtonClicked()
{
    isAddingLine = !isAddingLine;  // 切换是否添加直线的状态
    isAddingCircle = false;  // 确保不在添加圆状态
    isAddingText = false;  // 确保不在添加文字状态
    drawingArea->scene->setAddingLine(isAddingLine);  // 通知场景开始/停止添加直线
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
    setWindowTitle("图形编辑器");

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QWidget *Buttons = new QWidget(this);
    QVBoxLayout *buttonsLayout = new QVBoxLayout(Buttons);
    QWidget *EditorSet = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(EditorSet);
    // 设置子窗口的布局和控件
    //QVBoxLayout *layout = new QVBoxLayout(this);
    //QLabel *label = new QLabel("这是一个嵌入的子窗口", this);

    drawingArea = new CustomDrawingArea(LineColor, this);
    drawingArea->scene->setSceneRect(0, 0, 640, 480);
    qDebug() << "[INFO] DrawingArea: Set Pen to " << LineColor;


    connect(drawingArea, &DrawingArea::shapeSelected, this, &ImageEditor::onShapeSelected);
    connect(drawingArea->scene, &CustomGraphicsScene::drawingCircleFinished, this, &ImageEditor::handleDrawingCircleFinished);
    connect(drawingArea->scene, &CustomGraphicsScene::drawingLineFinished, this, &ImageEditor::handleDrawingLineFinished);


    //connect(drawingArea->scene->circle, &CustomCircleItem::drawingFinished, this, &ImageEditor::handleDrawingFinished);
    //connect(drawingArea->scene->circle, &CustomGraphicsScene::drawingFinished, this, &ImageEditor::handleDrawingFinished);
    //connect(drawingArea->scene, &DrawingArea::drawingFinished, this, &ImageEditor::updateLineEdit);
    mainLayout->addWidget(drawingArea, 4);
    //setCentralWidget(view);  // 设置中心部件为图形视图

    QLabel *parameterTitle = new QLabel("参数区", this);
    parameterTitle->setAlignment(Qt::AlignCenter);
    layout->addWidget(parameterTitle, 4);


    QPushButton *insertLineButton = new QPushButton("插入 直线", this);
    QPushButton *insertRectButton = new QPushButton("插入 矩形", this);
    QPushButton *insertCircleButton = new QPushButton("插入 圆", this);
    //链接按钮槽和信号
    connect(insertCircleButton, &QPushButton::clicked, this, &ImageEditor::onAddCircleButtonClicked);
    connect(insertLineButton, &QPushButton::clicked, this, &ImageEditor::onAddLineButtonClicked);
    connect(insertRectButton, &QPushButton::clicked, this, &ImageEditor::onGenerateCodeButtonClicked);//临时链接

    connect(drawingArea->scene, &CustomGraphicsScene::circleAdded, this, &ImageEditor::handleCircleAdded);



    QPushButton *insertEllipticButton = new QPushButton("插入 椭圆", this);
    QPushButton *insertTextButton = new QPushButton("插入 文本", this);

    QDoubleValidator *validator = new QDoubleValidator(this);
    validator->setNotation(QDoubleValidator::StandardNotation);
    // 设置输入验证器，只允许输入整数或浮点数

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

    QPushButton *closeButton = new QPushButton("关闭", this);
    layout->addWidget(parameter_1, 1);
    layout->addWidget(parameter_2, 1);
    layout->addWidget(parameter_3, 1);
    layout->addWidget(parameter_4, 1);

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
    QPushButton *FontButton = new QPushButton("设置字体", this);
    QPushButton *BrushButton = new QPushButton("设置笔刷", this);

    layout->addWidget(StyleTitle, 1);
    layout->addWidget(FontButton, 1);
    layout->addWidget(BrushButton, 1);

    QLabel *ButtonsTitle = new QLabel("插入区", this);
    ButtonsTitle->setFixedHeight(buttonHeight);
    ButtonsTitle->setAlignment(Qt::AlignCenter);
    buttonsLayout->addWidget(ButtonsTitle, 4);

    buttonsLayout->addWidget(insertLineButton, 1);
    buttonsLayout->addWidget(insertRectButton, 1);
    buttonsLayout->addWidget(insertCircleButton, 1);
    buttonsLayout->addWidget(insertEllipticButton, 1);
    buttonsLayout->addWidget(insertTextButton, 1);
    layout->addWidget(Buttons, 6);

    layout->addWidget(closeButton, 3);
    EditorSet->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(EditorSet, 1);

    QPainter painter(this);
    painter.setPen(Qt::blue);
    drawingArea->drawShapes(painter);

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
            LineColor = QPen(QColor(lineColor.at(0), lineColor.at(1), lineColor.at(2)));
            drawingArea->scene->LineColor = LineColor;
        }

        if (colors.size() > 2) {
            QList<int> fillColor = colors.at(2);
            FillColor = QPen(QColor(fillColor.at(0), fillColor.at(1), fillColor.at(2)));
        }
    }
}

void ImageEditor::onSendData() {
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

