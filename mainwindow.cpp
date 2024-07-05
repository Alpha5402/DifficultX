#include <math.h>
#include "mainwindow.h"
#include "customDrawingArea.h"
#include "imageEditor.h""

int screenWidth = GetSystemMetrics(SM_CXSCREEN);
int screenHeight = GetSystemMetrics(SM_CYSCREEN);

void MainWindow::onCursorPositionChanged(){
    qDebug() << "Hello World" << Qt::endl;
}

Operation extractVariables(const QString &input) {
    // 定义不同的正则表达式模式
    Operation ans;
    QRegularExpression circleRegex(R"((circle)\(([^)]*)\))");
    QRegularExpression lineRegex(R"((line)\(([^)]*)\))");
    QRegularExpression rectRegex(R"((rectangle)\(([^)]*)\))");
    QRegularExpression colorRegex(R"((setbkcolor|setlinecolor|setfillcolor)\(RGB\((\d+),\s*(\d+),\s*(\d+)\)\);)");

    // 尝试匹配不同的操作类型
    QRegularExpressionMatch circleMatch = circleRegex.match(input);
    QRegularExpressionMatch lineMatch = lineRegex.match(input);
    QRegularExpressionMatch rectMatch = rectRegex.match(input);
    QRegularExpressionMatch colorMatch = colorRegex.match(input);

    if (circleMatch.hasMatch()) {
        QString operation = circleMatch.captured(1); // 匹配到的操作名称
        QString parameters = circleMatch.captured(2); // 匹配到的参数列表

        // 分割参数列表
        QStringList parameterList = parameters.split(",", Qt::SkipEmptyParts);

        // 去除参数中的空格
        for (QString &param : parameterList) {
            param = param.trimmed();
        }

        if (parameterList.size() == 3) {
            qDebug() << "Operation:" << operation;
            qDebug() << "Parameters:" << parameterList;
            ans.type = operation;
            ans.parameter_1 = parameterList[0].toDouble();
            ans.parameter_2 = parameterList[1].toDouble();
            ans.parameter_3 = parameterList[2].toDouble();
        } else {
            qDebug() << "Invalid number of parameters for circle.";
        }

    } else if (lineMatch.hasMatch() || rectMatch.hasMatch()) {
        QString operation = lineMatch.captured(1); // 匹配到的操作名称
        QString parameters = lineMatch.captured(2); // 匹配到的参数列表

        // 分割参数列表
        QStringList parameterList = parameters.split(",", Qt::SkipEmptyParts);

        // 去除参数中的空格
        for (QString &param : parameterList) {
            param = param.trimmed();
        }

        if (parameterList.size() == 4) {
            qDebug() << "Operation:" << operation;
            qDebug() << "Parameters:" << parameterList;
            ans.type = operation;
            ans.parameter_1 = parameterList[0].toDouble();
            ans.parameter_2 = parameterList[1].toDouble();
            ans.parameter_3 = parameterList[2].toDouble();
            ans.parameter_4 = parameterList[3].toDouble();
        } else {
            qDebug() << "Invalid number of parameters for line.";
        }

    } else if (colorMatch.hasMatch()) {
        QString operation = colorMatch.captured(1); // 匹配到的操作名称
        QString r = colorMatch.captured(2); // 匹配到的 R 参数
        QString g = colorMatch.captured(3); // 匹配到的 G 参数
        QString b = colorMatch.captured(4); // 匹配到的 B 参数

        // 输出结果
        qDebug() << "Operation:" << operation;
        qDebug() << "R:" << r;
        qDebug() << "G:" << g;
        qDebug() << "B:" << b;
        ans.type = operation;
        ans.parameter_1 = r.toDouble();
        ans.parameter_2 = g.toDouble();
        ans.parameter_3 = b.toDouble();
    } else {
        qDebug() << "No match found.";
    }
}

void MainWindow::openImageEditor(){
    ImageEditor *imageditor = new ImageEditor();

    // 获取A窗口的大小并设置给B窗口
    imageditor->resize(this->size());

    // 获取A窗口的位置并设置给B窗口
    imageditor->move(this->pos());

    connect(imageditor, &ImageEditor::sendData, this, &MainWindow::receiveData);

    imageditor->show();
}

std::list<Operation> MainWindow::ReadCodes(){
    std::list<Operation> ans;
    QTextCursor cursor = textEdit->textCursor();
    QString searchString = "void solve(){";
    //cursor.movePosition(QTextCursor::Start);

    for(cursor.movePosition(QTextCursor::Start); cursor.position() != QTextCursor::End; cursor.movePosition(QTextCursor::NextBlock)){
        QString lineText = cursor.block().text();
        if (lineText == "void solve(){"){
            qDebug() << "Successfully found solve";
            break;
        }
    }

    for(cursor.movePosition(QTextCursor::NextBlock); cursor.position() != QTextCursor::End; cursor.movePosition(QTextCursor::NextBlock)){
        QString lineText = cursor.block().text();
        if (lineText == "}// End Solves"){
            qDebug() << "Has find all of them";
            break;
        } else {
            ans.push_back(extractVariables(lineText));
        }
    }

    return ans;
}

void MainWindow::DrawSomeThing(const QString& text){
    QTextCursor cursor = textEdit->textCursor();
    bool found = false;
    // 获取当前行号
    int currentLineNumber = cursor.blockNumber();
    // 移动到行尾部
    cursor.movePosition(QTextCursor::EndOfBlock);
    // 获取行文本
    QString lineText = cursor.block().text();

    if (lineText.lastIndexOf(';') != -1) {
        cursor.movePosition(QTextCursor::NextBlock);
        cursor.insertText("\t" + text + "\n");
    } else if (lineText.lastIndexOf('{') != -1){
        cursor.movePosition(QTextCursor::NextBlock);
        cursor.insertText("\t" + text + "\n");
    } else if (lineText.lastIndexOf('}') != -1){
        cursor.movePosition(QTextCursor::End);
        cursor.deletePreviousChar();
        cursor.insertText("\t" + text + "\n}");
    } else {
        if (lineText == "\t")
            cursor.insertText(text + "\n");
        else if (lineText.isEmpty())
            cursor.insertText("\t" + text + "\n");
        else
            cursor.insertText("\n" + text + "\t");
        cursor.movePosition(QTextCursor::NextBlock);
    }
}

// 自定义语法高亮器类
class Highlighter : public QSyntaxHighlighter
{
public:
    Highlighter(QTextDocument *parent = nullptr) : QSyntaxHighlighter(parent)
    {
        // 定义关键字格式
        keywordFormat.setForeground(Qt::blue);
        keywordFormat.setFontWeight(QFont::Bold);

        // 定义关键字列表
        QStringList keywords;
        keywords << "\\balignas\\b" << "\\balignof\\b" << "\\band\\b" << "\\band_eq\\b" << "\\basm\\b"
                 << "\\bauto\\b" << "\\bbitand\\b" << "\\bbitor\\b" << "\\bbool\\b" << "\\bbreak\\b"
                 << "\\bcase\\b" << "\\bcatch\\b" << "\\bchar\\b" << "\\bchar8_t\\b" << "\\bchar16_t\\b"
                 << "\\bchar32_t\\b" << "\\bclass\\b" << "\\bcompl\\b" << "\\bconcept\\b" << "\\bconst\\b"
                 << "\\bconsteval\\b" << "\\bconstexpr\\b" << "\\bconstinit\\b" << "\\bconst_cast\\b" << "\\bcontinue\\b"
                 << "\\bco_await\\b" << "\\bco_return\\b" << "\\bco_yield\\b" << "\\bdecltype\\b" << "\\bdefault\\b"
                 << "\\bdelete\\b" << "\\bdo\\b" << "\\bdouble\\b" << "\\bdynamic_cast\\b" << "\\belse\\b"
                 << "\\benum\\b" << "\\bexplicit\\b" << "\\bexport\\b" << "\\bextern\\b" << "\\bfalse\\b"
                 << "\\bfloat\\b" << "\\bfor\\b" << "\\bfriend\\b" << "\\bgoto\\b" << "\\bif\\b"
                 << "\\binline\\b" << "\\bint\\b" << "\\blong\\b" << "\\bmutable\\b" << "\\bnamespace\\b"
                 << "\\bnew\\b" << "\\bnoexcept\\b" << "\\bnot\\b" << "\\bnot_eq\\b" << "\\bnullptr\\b"
                 << "\\boperator\\b" << "\\bor\\b" << "\\bor_eq\\b" << "\\bprivate\\b" << "\\bprotected\\b"
                 << "\\bpublic\\b" << "\\breinterpret_cast\\b" << "\\brequires\\b" << "\\breturn\\b" << "\\bshort\\b"
                 << "\\bsigned\\b" << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstatic_assert\\b" << "\\bstatic_cast\\b"
                 << "\\bstruct\\b" << "\\bswitch\\b" << "\\bsynchronized\\b" << "\\btemplate\\b" << "\\bthis\\b"
                 << "\\bthread_local\\b" << "\\bthrow\\b" << "\\btrue\\b" << "\\btry\\b" << "\\btypedef\\b"
                 << "\\btypeid\\b" << "\\btypename\\b" << "\\bunion\\b" << "\\bunsigned\\b" << "\\busing\\b"
                 << "\\bvirtual\\b" << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwchar_t\\b" << "\\bwhile\\b"
                 << "\\bxor\\b" << "\\bxor_eq\\b";

        // 生成正则表达式模式
        foreach (const QString &pattern, keywords) {
            HighlightingRule rule;
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywordFormat;
            highlightingRules.append(rule);
        }
    }

protected:
    void highlightBlock(const QString &text) override
    {
        // 遍历每个高亮规则并应用
        foreach (const HighlightingRule &rule, highlightingRules) {
            QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
            while (matchIterator.hasNext()) {
                QRegularExpressionMatch match = matchIterator.next();
                setFormat(match.capturedStart(), match.capturedLength(), rule.format);
            }
        }
    }

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
};

class PickPointsWidget : public QWidget{
public:
    explicit PickPointsWidget(QWidget *parent = nullptr) : QWidget(parent) {
        setFixedSize(400, 400); // 设置选择窗口大小
    }
    ~PickPointsWidget() {}
protected:
    int clickCount = 0;
    QPoint point1, point2;
    void mousePressEvent(QMouseEvent *event) override {
        if (clickCount == 0) {
            point1 = event->pos();
            clickCount++;
        } else if (clickCount == 1) {
            point2 = event->pos();
            qDebug() << "Point 1: " << point1;
            qDebug() << "Point 2: " << point2;
            clickCount = 0;
            close();
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (clickCount == 1) {
            point2 = event->pos();
            update(); // 更新界面以反映新的直线位置
        }
    }

    virtual void paintEvent(QPaintEvent *) override = 0;
    virtual void drawObjection() = 0;
    virtual void putCode() = 0;
};

class pickCircleWidget : public PickPointsWidget{
public:
    explicit pickCircleWidget(bool withSolid, bool withFill, QWidget *parent = nullptr) : PickPointsWidget(parent) {
        setFixedSize(screenWidth, screenHeight);
        setMouseTracking(true);
    }
    ~pickCircleWidget() {}
protected:
    void drawObjection() {};
    void putCode() {};
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setPen(Qt::blue); // 设置画笔颜色为蓝色
        double Radius = sqrt((point1.x()-point2.x()) * (point1.x()-point2.x()) + (point1.y()-point2.y()) * (point1.y()-point2.y()));
        if (clickCount == 1) {
            painter.drawEllipse(point1.x() - Radius, point1.y() - Radius, 2 * Radius, 2 * Radius); // 绘制从point1到当前鼠标位置的直线
        }
    }
};

class pickLineWidget : public PickPointsWidget{
public:
    explicit pickLineWidget(QWidget *parent = nullptr) : PickPointsWidget(parent) {
        setFixedSize(screenWidth, screenHeight);// 设置选择窗口大小
        setMouseTracking(true);
    }
    ~pickLineWidget() {}
protected:
    void drawObjection() {};
    void putCode() {};
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setPen(Qt::blue); // 设置画笔颜色为蓝色
        if (clickCount == 1) {
            painter.drawLine(point1, point2); // 绘制从point1到当前鼠标位置的直线
        }
    }
};

class pickRectangleWidget : public PickPointsWidget{
public:
    explicit pickRectangleWidget(bool withSolid, bool withFill, QWidget *parent = nullptr) : PickPointsWidget(parent) {
        setFixedSize(screenWidth, screenHeight);  // 设置选择窗口大小
        setMouseTracking(true);
    }
    ~pickRectangleWidget() {}
protected:
    void drawObjection() {};
    void putCode() {};
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setPen(Qt::blue); // 设置画笔颜色为蓝色
        if (clickCount == 1) {
            int width = abs(point2.x() - point1.x());
            int height = abs(point2.y() - point1.y());
            painter.drawRect(point1.x() <= point2.x() ? point1.x() : point2.x(),
                             point1.y() <= point2.y() ? point1.y() : point2.y(), width, height); // 绘制从point1到当前鼠标位置的直线
        }
    }
};




// 主窗口类
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), textEdit(new QTextEdit(this))
{
    // 创建中央小部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // 设置 Tab 的宽度为 80 像素
    QFont font ("Consolas", 12);
    QTextOption textOption = textEdit->document()->defaultTextOption();
    textOption.setTabStopDistance(32); // 以像素为单位
    textEdit->setFont(font);
    textEdit->document()->setDefaultTextOption(textOption);
    textEdit->setText("// Libraries\n"
                      "#include <graphics.h> \n"
                      "#include <conio.h>\n"
                      "// End Libraries\n"
                      "\n"
                      "// Declaration\n"
                      "void solve();"
                      "// End Declaration\n"
                      "\n"
                      "// Main\n"
                      "int main(){\n"
                      "\tinitgraph(800, 600);\n"
                      "\n"
                      "\tsolve();\n"
                      "\n"
                      "\t_getch();\n"
                      "\tclosegraph();\n"
                      "\treturn 0;\n"
                      "}"
                      "// End Main\n"
                      "\n"
                      "// Solves\n"
                      "void solve(){\n"
                      "\t"
                      "\n}"
                      "// End Solves\n");

    // 创建布局
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // 文本框占据左三分之二
    textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(textEdit, 2);

    // 创建右侧按钮布局
    QWidget *buttonContainer1 = new QWidget(this);
    QVBoxLayout *buttonLayout1 = new QVBoxLayout(buttonContainer1);

    QPushButton *button1 = new QPushButton("插入 圆", buttonContainer1);
    connect(button1, &QPushButton::clicked, this, &MainWindow::ReadCodes);
    QPushButton *button2 = new QPushButton("绘制 直线", buttonContainer1);

    //QPushButton *openPickerButton = new QPushButton("Pick Points", this);
    connect(button2, &QPushButton::clicked, this, [this]() {
        PickPointsWidget *picker = new pickLineWidget();
        picker->setAttribute(Qt::WA_DeleteOnClose); // 确保窗口关闭时释放资源
        picker->show();
    });
    QPushButton *button3 = new QPushButton("绘制 矩形", buttonContainer1);
    connect(button3, &QPushButton::clicked, this, [this]() {
        PickPointsWidget *picker = new pickRectangleWidget(true, true);
        picker->setAttribute(Qt::WA_DeleteOnClose); // 确保窗口关闭时释放资源
        picker->show();
    });
    buttonLayout1->addWidget(button1);
    buttonLayout1->addWidget(button2);
    buttonLayout1->addWidget(button3);
    buttonLayout1->addStretch();

    // 右边三分之一添加按钮布局
    buttonContainer1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(buttonContainer1, 1);

    QWidget *buttonContainer2 = new QWidget(this);
    QVBoxLayout *buttonLayout2 = new QVBoxLayout(buttonContainer2);

    QPushButton *button4 = new QPushButton("绘制 圆", buttonContainer2);
    connect(button4, &QPushButton::clicked, this, [this]() {
        PickPointsWidget *picker = new pickCircleWidget(true, true);
        picker->setAttribute(Qt::WA_DeleteOnClose); // 确保窗口关闭时释放资源
        picker->show();
    });
    QPushButton *button5 = new QPushButton("打开编辑器", buttonContainer2);
    //ImageEditor *imageEditor = new ImageEditor();

    // 将按钮和子窗口添加到主窗口的布局中
    //mainLayout->addWidget(button5);
    //mainLayout->addWidget(imageEditor);

    // 默认隐藏子窗口
    // imageEditor->resize(size());
    // imageEditor->move(pos());
    // imageEditor->hide();

    // 连接按钮的 clicked 信号到子窗口的 show 槽
    QObject::connect(button5, &QPushButton::clicked, this, &MainWindow::openImageEditor);
    QPushButton *button6 = new QPushButton("按钮6", buttonContainer2);

    buttonLayout2->addWidget(button4);
    buttonLayout2->addWidget(button5);
    buttonLayout2->addWidget(button6);
    buttonLayout2->addStretch();

    buttonContainer2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(buttonContainer2, 1);

    //QWidget *TempContainer = new QWidget(this);
    //QVBoxLayout *TempLayout = new QVBoxLayout(TempContainer);
    //mainLayout->addWidget(TempContainer, 4);

    // 创建菜单栏
    QMenu *fileMenu = menuBar()->addMenu(tr("文件"));

    // 创建打开文件操作
    QAction *openAction = new QAction(tr("打开"), this);
    openAction->setShortcuts(QKeySequence::Open);
    connect(openAction, &QAction::triggered, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"), "", tr("Text Files (*.cpp);;All Files (*)"));
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                textEdit->setPlainText(in.readAll());
            } else {
                QMessageBox::warning(this, tr("错误"), tr("无法打开文件"));
            }
        }
    });



    // 创建保存文件操作
    QAction *saveAction = new QAction(tr("保存"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    connect(saveAction, &QAction::triggered, this, [=]() {
        QString fileName = QFileDialog::getSaveFileName(this, tr("保存文件"), "", tr("Text Files (*.txt);;All Files (*)"));
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << textEdit->toPlainText();
            } else {
                QMessageBox::warning(this, tr("错误"), tr("无法保存文件"));
            }
        }
    });

    // 创建设置字体操作
    QAction *setFontAction = new QAction(tr("设置字体"), this);
    connect(setFontAction, &QAction::triggered, this, [=]() {
        bool ok;
        QFont font = QFontDialog::getFont(&ok, textEdit->font(), this);
        if (ok) {
            textEdit->setFont(font);
        }
    });

    // 创建退出操作
    QAction *exitAction = new QAction(tr("退出"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    connect(exitAction, &QAction::triggered, this, &QWidget::close);

    // 将操作添加到菜单中
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(setFontAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    // 应用语法高亮器
    Highlighter *highlighter = new Highlighter(textEdit->document());
}

MainWindow::~MainWindow()
{
}

void MainWindow::receiveData(const QString &data) {
    qDebug() << "[INFO] MainWindow received " << data << Qt::endl;
    DrawSomeThing(data);
}
