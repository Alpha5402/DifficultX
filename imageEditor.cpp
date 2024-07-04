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

void ImageEditor::handleDrawingFinished(const QString& message) {
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

    drawingArea = new CustomDrawingArea(this);
    connect(drawingArea, &DrawingArea::shapeSelected, this, &ImageEditor::onShapeSelected);
    connect(drawingArea->scene, &CustomGraphicsScene::drawingFinished, this, &ImageEditor::handleDrawingFinished);
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
    connect(insertCircleButton, &QPushButton::clicked, this, &ImageEditor::onAddCircleButtonClicked);
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
    lineEdit_1->setText("Value 1");  // 设置默认值
    int buttonHeight = lineEdit_1->sizeHint().height();
    parameterLabel_1->setFixedHeight(buttonHeight);
    lineEdit_1->setValidator(validator);
    para_box_1->addWidget(parameterLabel_1, 1);
    para_box_1->addWidget(lineEdit_1, 1);

    parameterTitle->setFixedHeight(buttonHeight);

    QWidget *parameter_2 = new QWidget(this);
    QVBoxLayout *para_box_2 = new QVBoxLayout(parameter_2);
    QLabel *parameterLabel_2 = new QLabel("参数 2", this);
    lineEdit_2 = new QLineEdit(this);
    lineEdit_2->setText("Value 2");  // 设置默认值
    parameterLabel_2->setFixedHeight(buttonHeight);
    lineEdit_2->setValidator(validator);
    para_box_2->addWidget(parameterLabel_2, 1);
    para_box_2->addWidget(lineEdit_2, 1);

    QWidget *parameter_3 = new QWidget(this);
    QVBoxLayout *para_box_3 = new QVBoxLayout(parameter_3);
    QLabel *parameterLabel_3 = new QLabel("参数 3", this);
    lineEdit_3 = new QLineEdit(this);
    lineEdit_3->setText("Value 3");  // 设置默认值
    parameterLabel_3->setFixedHeight(buttonHeight);
    lineEdit_3->setValidator(validator);
    para_box_3->addWidget(parameterLabel_3, 1);
    para_box_3->addWidget(lineEdit_3, 1);

    QPushButton *closeButton = new QPushButton("关闭", this);
    layout->addWidget(parameter_1, 1);
    layout->addWidget(parameter_2, 1);
    layout->addWidget(parameter_3, 1);

    QLabel *ColorTitle = new QLabel("调色区", this);
    ColorTitle->setAlignment(Qt::AlignCenter);
    ColorTitle->setFixedHeight(3*buttonHeight);
    QPushButton *ColorButton = new QPushButton("选取颜色", this);
    colorPicker = new ColorPickerWidget();

    QObject::connect(ColorButton, &QPushButton::clicked, this, &ImageEditor::openColorPickerWidget);


    layout->addWidget(ColorTitle, 3);
    layout->addWidget(ColorButton, 1);

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
    //qDebug() << "[INFO] imageEditor received " << data << Qt::endl;
}

void ImageEditor::onSendData() {
    emit sendData(ToHandle); // 发射信号将数据传回A窗口
    //qDebug() << "[INFO] ImageEditor sended " << ToHandle << Qt::endl;
    this->close(); // 关闭B窗口
}
