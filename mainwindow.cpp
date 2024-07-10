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
    QRegularExpression lineAndRectRegex(R"((line|rectangle)\(([^)]*)\))");
    QRegularExpression colorRegex(R"((setbkcolor|setlinecolor|setfillcolor)\(RGB\((\d+),\s*(\d+),\s*(\d+)\)\);)");
    QRegularExpression lineStyleRegex(R"(setlinestyle\(PS_SOLID,\s*([^)]*)\);)");

    // 尝试匹配不同的操作类型
    QRegularExpressionMatch circleMatch = circleRegex.match(input);
    QRegularExpressionMatch lineAndRectMatch = lineAndRectRegex.match(input);
    QRegularExpressionMatch colorMatch = colorRegex.match(input);
    QRegularExpressionMatch lineStyleMatch = lineStyleRegex.match(input);

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

    } else if (lineAndRectMatch.hasMatch()) {
        QString operation = lineAndRectMatch.captured(1); // 匹配到的操作名称
        QString parameters = lineAndRectMatch.captured(2); // 匹配到的参数列表

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
    } else if (lineStyleMatch.hasMatch()) {
        QString para_1 = lineStyleMatch.captured(1); // 匹配到的操作名称

        ans.type = "setlinestyle";
        ans.parameter_1 = para_1.toDouble();
    } else {
        qDebug() << "No match found.";
    }
    return ans;
}

void MainWindow::openImageEditor(){
    ImageEditor *imageditor = new ImageEditor();

    // 获取A窗口的大小并设置给B窗口
    imageditor->resize(this->size());

    // 获取A窗口的位置并设置给B窗口
    imageditor->move(this->pos());

    std::list<Operation> Handle = ReadCodes();
    qDebug() << "[INFO] Handle size " << Handle.size();

    for (auto element = Handle.begin(); element != Handle.end(); ++element) {
        qDebug() << "type: " << (*element).type << " para1: " << (*element).parameter_1 << " para2: " << (*element).parameter_2 << " para3: " << (*element).parameter_3;

        if ((*element).type == "circle") {
            qDebug() << "[INFO] Circle detected";
            imageditor->drawingArea->scene->DrawCircle((*element).parameter_1, (*element).parameter_2, (*element).parameter_3 * 2);
        } else if ((*element).type == "line") {
            qDebug() << "[INFO] Circle detected";
            imageditor->drawingArea->scene->DrawLine((*element).parameter_1, (*element).parameter_2, (*element).parameter_3, (*element).parameter_4);
        } else if ((*element).type == "rectangle") {
            qDebug() << "[INFO] Circle detected";
            imageditor->drawingArea->scene->DrawRect((*element).parameter_1, (*element).parameter_2, (*element).parameter_3, (*element).parameter_4);
        } else if ((*element).type == "setbkcolor") {
            qDebug() << "[INFO] Bkcolor detected";
            imageditor->drawingArea->scene->BackColor.setColor(QColor((*element).parameter_1, (*element).parameter_2, (*element).parameter_3));
        } else if ((*element).type == "setlinecolor") {
            qDebug() << "[INFO] Bkcolor detected";
            imageditor->drawingArea->scene->LineColor.setColor(QColor((*element).parameter_1, (*element).parameter_2, (*element).parameter_3));
        } else if ((*element).type == "setfillcolor") {
            qDebug() << "[INFO] Bkcolor detected";
            imageditor->drawingArea->scene->FillColor.setColor(QColor((*element).parameter_1, (*element).parameter_2, (*element).parameter_3));
        } else if ((*element).type == "setlinestyle") {
            qDebug() << "[INFO] Bkcolor detected";
            imageditor->drawingArea->scene->LineColor.setWidth((*element).parameter_1);
            imageditor->LineColor.setWidth((*element).parameter_1);
            //imageditor->drawingArea->scene->LineColor.setWidth((*element).parameter_1);
        }
    }

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
            Operation temp = extractVariables(lineText);
            ans.push_back(extractVariables(lineText));
            qDebug() << "[INFO] Parameter " << temp.type << " " << temp.parameter_1 << " " << temp.parameter_2 << " " << temp.parameter_3;
        }
    }
    return ans;
}

void MainWindow::DrawSomeThing(const QString& text){
    QTextCursor cursor = textEdit->textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor = textEdit->document()->find("void solve(){", cursor);
    cursor.movePosition(QTextCursor::Down);
    cursor.movePosition(QTextCursor::StartOfLine);
    cursor.insertText(text + "\n");

    // if (lineText.lastIndexOf(';') != -1) {
    //     cursor.movePosition(QTextCursor::NextBlock);
    //     cursor.insertText("\t" + text + "\n");
    // } else if (lineText.lastIndexOf('{') != -1){
    //     cursor.movePosition(QTextCursor::NextBlock);
    //     cursor.insertText("\t" + text + "\n");
    // } else if (lineText.lastIndexOf('}') != -1){
    //     cursor.movePosition(QTextCursor::End);
    //     cursor.deletePreviousChar();
    //     cursor.insertText("\t" + text + "\n}");
    // } else {
    //     if (lineText == "\t")
    //         cursor.insertText(text + "\n");
    //     else if (lineText.isEmpty())
    //         cursor.insertText("\t" + text + "\n");
    //     else
    //         cursor.insertText("\n" + text + "\t");
    //     cursor.movePosition(QTextCursor::NextBlock);
    // }
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

void MainWindow::removeSolveFunction(QTextEdit *document) {
    QTextCursor cursor(textEdit->textCursor());

    // 从文档的开始位置开始查找
    cursor.movePosition(QTextCursor::Start);

    while (true) {
        cursor = textEdit->document()->find("void solve(){", cursor);
         if (cursor.isNull()) {
             break;
         }

        // 移动到匹配项的结尾，即函数体的起始大括号位置
        cursor.movePosition(QTextCursor::NextBlock);
        //cursor.insertText("begin");
        int startPos = cursor.position();


        // 找到匹配的结束大括号位置，并保留注释
        QTextCursor endCursor = textEdit->document()->find("}// End Solves", cursor);
        if (endCursor.isNull()) {
            break;
        }
        //endCursor.insertText("end");

        // 调整结束位置，保留 "} // End Solves"
        endCursor.movePosition(QTextCursor::StartOfLine);

        // 删除起始大括号与结束大括号之间的内容
        cursor.setPosition(startPos, QTextCursor::MoveAnchor);
        cursor.setPosition(endCursor.position(), QTextCursor::KeepAnchor);
        cursor.removeSelectedText();

        // 插入一个换行符，并将光标移动到空行处
        cursor.insertText("\n");
        //cursor.insertText("current");

        cursor.movePosition(QTextCursor::Up);
    }
}

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
                      "\n"
                      "}"
                      "// End Solves\n");

    // 创建布局
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // 文本框占据左三分之二
    textEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(textEdit, 2);

    QWidget *buttonContainer = new QWidget(this);
    QVBoxLayout *buttonLayout = new QVBoxLayout(buttonContainer);

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

    QPushButton *openEditor = new QPushButton("打开编辑器", buttonContainer);
    openEditor->setMinimumHeight(64);
    QObject::connect(openEditor, &QPushButton::clicked, this, &MainWindow::openImageEditor);

    QPushButton *Save = new QPushButton("保存", buttonContainer);
    Save->setMinimumHeight(64);
    connect(Save, &QPushButton::clicked, saveAction, &QAction::trigger);

    QPushButton *openFiles = new QPushButton("打开文件", buttonContainer);
    openFiles->setMinimumHeight(64);
    connect(openFiles, &QPushButton::clicked, openAction, &QAction::trigger);

    QPushButton *setStyle = new QPushButton("设置文本字体", buttonContainer);
    setStyle->setMinimumHeight(64);
    connect(setStyle, &QPushButton::clicked, setFontAction, &QAction::trigger);

    QPushButton *exits = new QPushButton("退出", buttonContainer);
    exits->setMinimumHeight(64);
    connect(exits, &QPushButton::clicked, exitAction, &QAction::trigger);

    QPushButton *compileButton = new QPushButton("使用本地 Windows 调试器编译并运行", buttonContainer);
    compileButton->setMinimumHeight(64);
    connect(compileButton, &QPushButton::clicked, this, &MainWindow::compileAndRunCode);

    buttonLayout->addWidget(openFiles);
    buttonLayout->addWidget(setStyle);
    buttonLayout->addWidget(openEditor);
    buttonLayout->addWidget(Save);
    buttonLayout->addWidget(compileButton);
    buttonLayout->addWidget(exits);
    //buttonLayout->addWidget(button6);
    //buttonLayout->addStretch();

    //buttonContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(buttonContainer, 1);

    //QWidget *TempContainer = new QWidget(this);
    //QVBoxLayout *TempLayout = new QVBoxLayout(TempContainer);
    //mainLayout->addWidget(TempContainer, 4);

    // 将操作添加到菜单中
    // fileMenu->addAction(openAction);
    // fileMenu->addAction(saveAction);
    // fileMenu->addAction(setFontAction);
    // fileMenu->addSeparator();
    // fileMenu->addAction(exitAction);

    // 应用语法高亮器
    Highlighter *highlighter = new Highlighter(textEdit->document());
}

MainWindow::~MainWindow()
{
}

void MainWindow::receiveData(const QString &data) {
    removeSolveFunction(textEdit);
    qDebug() << "[INFO] MainWindow received " << data << Qt::endl;
    DrawSomeThing(data);
}

QString MainWindow::createTempFilePath(const QString &fileName) {
    QString docsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString tempPath = docsPath + "/DifficultX";
    QDir dir(tempPath);
    if (!dir.exists()) {
        if (!dir.mkpath(tempPath)) {
            QMessageBox::critical(this, "Error", "Failed to create temporary directory.");
            return QString();
        }
    }
    return tempPath + "/" + fileName;
}

void MainWindow::compileAndRunCode() {
    QString code = textEdit->toPlainText();
    //qDebug() << "[DEBUG] The code " << code;
    QString filePath = createTempFilePath("temp_code.cpp");

    if (filePath.isEmpty()) {
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "Failed to open temporary file for writing.");
        return;
    }

    QTextStream out(&file);
    out << code;
    file.close();


    QProcess process;
    QStringList args;

    // QString vsDevCmdPath = "\"C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/Tools/VsDevCmd.bat\"";

    // // 构建完整的cl命令行，注意使用双引号包围含有空格的路径
    // QString clCommand = "cl temp_code.cpp /Fe:tempCode.exe /link /LIBPATH:\"C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/VS/lib/x64\" /LIBPATH:\"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64\" /LIBPATH:\"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/ucrt/x64\" /LIBPATH:\"C:/EasyX/lib\" EasyXa.lib user32.lib gdi32.lib kernel32.lib shell32.lib /SUBSYSTEM:CONSOLE";

    // // 准备命令行参数
    // args << "/C" << vsDevCmdPath << "&&" << "cd C:/Users/Alpha.LAPTOP-O2RBBHUH/Documents/DifficultX &&" << clCommand;

    //args << "ping www.baidu.com";
    QString cmd1 = "cd C:/Users/Alpha.LAPTOP-O2RBBHUH/Documents/DifficultX\r\n";
    QByteArray qbarr1 = cmd1.toLatin1();
    char *ch1 = qbarr1.data();
    qint64 len1 = cmd1.length();
    QString cmd2 = "\"C:/Program Files/Microsoft Visual Studio/2022/Community/Common7/Tools/VsDevCmd.bat\" -arch=amd64 && cl temp_code.cpp /Fe:tempCode.exe /link /LIBPATH:\"C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/VS/lib/x64\" /LIBPATH:\"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/um/x64\" /LIBPATH:\"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.22621.0/ucrt/x64\" /LIBPATH:\"C:/EasyX/lib\" EasyXa.lib user32.lib gdi32.lib kernel32.lib shell32.lib /SUBSYSTEM:CONSOLE\r\n";
    QByteArray qbarr2 = cmd2.toLatin1();
    char *ch2 = qbarr2.data();
    qint64 len2 = cmd2.length();
    QString cmd3 = "taskkill /IM tempCode.exe /F\r\n";
    QByteArray qbarr3 = cmd3.toLatin1();
    char *ch3 = qbarr3.data();
    qint64 len3 = cmd3.length();
    process.start("cmd.exe");
    process.write(ch3, len3);
    process.write(ch1, len1);
    process.write(ch2, len2);

    process.closeWriteChannel();

    if (!process.waitForFinished(-1)) {
        QMessageBox::critical(this, "Error 0", "Failed to compile and run the code.");
        return;
    }

     QByteArray errorOutput = process.readAll();
     QString errorString = QString::fromLocal8Bit(errorOutput);
     qDebug().noquote() << errorString;

    //qDebug() << process.readAll();
    //QByteArray errorOutput = process.readAllStandardError();
    //QString errorString = QString::fromLocal8Bit(errorOutput);


    if (!process.error())
        qDebug() << "[ERROR] return " << process.error();

    if (process.exitStatus() != QProcess::NormalExit || process.exitCode() != 0) {

        QByteArray errorOutput = process.readAllStandardError();
        QString errorString = QString::fromLocal8Bit(errorOutput);
        QMessageBox::critical(this, "Error 1", errorString);
        return;
    }

    QString output = process.readAll();
    qDebug() << process.readAllStandardError();

    QString exePath = QFileInfo(filePath).absolutePath() + "/tempCode.exe";
    if (!QProcess::startDetached(exePath)) {
        QMessageBox::critical(this, "Error 2", "Failed to run the executable: " + exePath);
    }
    //QProcess::startDetached(exePath);
}
