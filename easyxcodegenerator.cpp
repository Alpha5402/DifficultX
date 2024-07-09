#include "easyxcodegenerator.h"
#include <QFile>
#include <QTextStream>

using Qt::endl;

QTextStream test_is_code_right_generator(stdout); // 将输出流绑定到标准输出,检查code是否成功添加代码

vector<CircleItem>circles_gen;//储存圆信息
vector<LineItem>lines_gen;//储存直线信息
vector<Operation> HandleList;


Operation::Operation(){
    type = "null";
    ptr = nullptr;
    parameter_1 = 0;
    parameter_2 = 0;
    parameter_3 = 0;
    parameter_4 = 0;
};


void EasyXCodeGenerator::clear()
{
    test_is_code_right_generator << "clear" << endl;
    code.clear();
}

void EasyXCodeGenerator::addCircle(QPointF c,qreal r)
{
    CircleItem circle(c,r);//存个数据，没啥用
    circles_gen.push_back(circle);
    QString circle_code = QString("circle(%1,%2,%3);\n").arg(c.x()).arg(c.y()).arg(r);
    test_is_code_right_generator << circle_code << Qt::endl; // test code
    code.append(circle_code);
    test_is_code_right_generator << "add circle." << code << Qt::endl; // test code
}

void EasyXCodeGenerator::addText(const QString &text, const QPointF &position)
{
    QString text_code = QString("outtextxy(%1, %2,_T(\"%3\"));\n").arg(position.x()).arg(position.y()).arg(text);
    test_is_code_right_generator << text_code << Qt::endl; // test code
    code.append(text_code);
    test_is_code_right_generator << "add text." << code << Qt::endl;
}
void EasyXCodeGenerator::addLine(QPointF p1,QPointF p2)//增加一个线段
{
    LineItem line(p1,p2);//存个数据，没啥用
    lines_gen.push_back(line);
    QString line_code = QString("line(%1,%2,%3,%4);\n").arg(p1.x()).arg(p1.y()).arg(p2.x()).arg(p2.y());
    test_is_code_right_generator << line_code << Qt::endl; // test code
    code.append(line_code);
    test_is_code_right_generator << "add line." << code << Qt::endl; // test code
}

void EasyXCodeGenerator::generateCode( const CustomGraphicsScene *scene)
{//点击genecode后遍历scene的所有item并添加到code在点击genecode后才能正常获取code

    QColor iniColor;
    QColor initextColor;//第一次调用时初始化
    QColor iniFillcolor;

    QFont initial_font;
    initial_font.setFamily("000");
    initial_font.setPixelSize(-100);

    bool hasTextsyleChanged=false;

    bool hasFrame;
    bool hasFilling;

    code.clear();//清空缓存

    //预设信息
    code+="\tinitgraph(640, 480);\n";
    code+="\tsetbkcolor(RGB(255,255,255));\n";
    code+="\tsetcolor(RGB(0,0,0));\n";
    code+="\tcleardevice();\n";
    code+="\tLOGFONT f;\n";
    code+="\tgettextstyle(&f);\n";


    vector<QGraphicsItem *>Item;
    vector<QString>tempcode;
    vector<QString>colorcode;
    vector<QString>colorPos;

    //大一统
    for (QGraphicsItem *item : scene->items())
    {
        Item.push_back(item);
    }

    for (int i=Item.size()-1;i>=0;i--)
    {
        QGraphicsItem *item=Item[i];
        if (CustomCircleItem *circle = dynamic_cast<CustomCircleItem*>(item))
        {
            QBrush brush = circle->brush();
            QPen pen=circle->pen();
            if (pen.style()!=Qt::NoPen)
            {
                QColor currentColor=circle->pen().color();
                // 无填充

                //qDebug()<<circle->R<<circle->G<<circle->B<<endl;
                if(currentColor!=iniColor)
                {
                    code+=QString("\tsetlinecolor(RGB(%1,%2,%3));\n").arg(circle->R).arg(circle->G).arg(circle->B);
                    colorcode.push_back(QString("\tsetlinecolor(RGB(%1,%2,%3));\n")
                                            .arg(circle->R).arg(circle->G).arg(circle->B));//如果有颜色，改画笔

                    iniColor=currentColor;
                }
                //tempcode.push_back( QString("circle(%1,%2,%3);\n").arg(circle->center.x()).arg(circle->center.x()).arg(circle->radius));

                hasFrame=true;
            }
            if (brush.style() != Qt::NoBrush) {
                // 有填充，获取填充颜色
                QColor currentFillColor = brush.color();

                // 检查颜色是否有变化
                if (iniFillcolor.isValid() && currentFillColor != iniFillcolor) {
                    // 如果填充颜色与iniFillColor不同，更新code
                    code += QString("\tsetfillcolor(RGB(%1,%2,%3);\n")
                                .arg(currentFillColor.red())
                                .arg(currentFillColor.green())
                                .arg(currentFillColor.blue());
                } else if (!iniFillcolor.isValid()) {
                    // 如果这是第一次发现有填充颜色的圆，初始化iniFillColor
                    iniFillcolor = currentFillColor;
                    // 添加初始化代码到code字符串
                    code += QString("\tsetfillcolor(RGB(%1,%2,%3);\n")
                                .arg(currentFillColor.red())
                                .arg(currentFillColor.green())
                                .arg(currentFillColor.blue());
                }
                hasFilling=true;

            }
            if(hasFilling&&hasFrame)
            {
                code+=QString("\tfillcircle(%1,%2,%3);\n")
                            .arg(circle->center.x())
                            .arg(circle->center.y())
                            .arg(circle->radius);
                hasFilling=false;
                hasFrame=false;
            }
            else if(hasFilling)
            {
                code+=QString("\tsolidcircle(%1,%2,%3);\n")
                            .arg(circle->center.x())
                            .arg(circle->center.y())
                            .arg(circle->radius);
                hasFilling=false;
            }
            else if(hasFrame)
            {
                 code+=QString("\tcircle(%1,%2,%3);\n")
                            .arg(circle->center.x())
                            .arg(circle->center.x())
                            .arg(circle->radius);
                hasFrame=false;
            }


        }
        else if (CustomLineItem *line = dynamic_cast<CustomLineItem*>(item))
        {
            //qDebug()<<line->R<<"sdinjfni";
            QColor currentColor;

            if(iniColor!=currentColor)
            {
                //qDebug()<<"222"<<line->G;
                code+=QString("\tsetlinecolor(RGB(%1,%2,%3));\n").arg(line->R).arg(line->G).arg(line->B);//如果有颜色，改画笔
                iniColor=currentColor;
            }
            code+= QString("\tline(%1,%2,%3,%4);\n").arg(line->point1.x()).arg(line->point1.y()).arg(line->point2.x()).arg(line->point2.y());
            // qDebug()<< QString("line(%1,%2,%3,%4);\n").arg(line->point1.x()).arg(line->point1.y()).arg(line->point2.x()).arg(line->point2.y());
        }
        else if(CustomRectangleItem *rec=dynamic_cast<CustomRectangleItem*>(item))
        {
            QBrush brush = rec->brush();

            if (brush.style() == Qt::NoBrush)
            {
                QColor currentColor=rec->pen().color();
                // 无填充
                if(currentColor!=iniColor)
                {
                    qDebug()<<iniColor<<currentColor;
                    code += QString("\tsetlinecolor(RGB(%1,%2,%3));\n")
                                .arg(currentColor.red())
                                .arg(currentColor.green())
                                .arg(currentColor.blue());
                    iniColor=currentColor;
                }
                hasFrame=true;
            }
            else
            {
                // 有填充，获取填充颜色
                QColor currentFillColor = brush.color();

                // 检查颜色是否有变化
                if (iniFillcolor.isValid() && currentFillColor != iniFillcolor)
                {
                    // 如果填充颜色与iniFillColor不同，更新code
                    code += QString("\tsetfillcolor(RGB(%1,%2,%3));\n")
                                .arg(currentFillColor.red()).arg(currentFillColor.green()).arg(currentFillColor.blue());
                    iniFillcolor = currentFillColor; // 更新初始填充颜色
                }
                else if (!iniFillcolor.isValid())
                {
                    // 如果这是第一次发现有填充颜色的矩形，初始化iniFillColor
                    iniFillcolor = currentFillColor;
                    // 添加初始化代码到code字符串
                    code += QString("\tsetfillcolor(RGB(%1,%2,%3));\n")
                                .arg(currentFillColor.red()).arg(currentFillColor.green()).arg(currentFillColor.blue());
                }
                hasFilling=true;

            }
            if(hasFilling&&hasFrame)
            {
                code+=QString("\tfillcircle(%1,%2,%3);\n")
                            .arg(circle->center.x())
                            .arg(circle->center.y())
                            .arg(circle->radius);
                hasFilling=false;
                hasFrame=false;
            }
            else if(hasFilling)
            {
                code+=QString("\tsolidcircle(%1,%2,%3);\n")
                            .arg(circle->center.x())
                            .arg(circle->center.y())
                            .arg(circle->radius);
                hasFilling=false;
            }
            else if(hasFrame)
            {
                code+=QString("\tcircle(%1,%2,%3);\n")
                            .arg(circle->center.x())
                            .arg(circle->center.x())
                            .arg(circle->radius);
                hasFrame=false;
            }
        }
        else if(CustomTextItem *text=dynamic_cast<CustomTextItem*>(item))
        {
            if(text->font().family()!=initial_font.family())
            {
                code+=QString("\t_tcscpy_s(f.lfFaceName, _T(\"%1\"));\n").arg(text->font().family());
                initial_font.setFamily(text->font().family());
                hasTextsyleChanged=true;
            }
            if(text->font().pixelSize()!=initial_font.pixelSize())
            {
                code+=QString("\tf.lfHeight = %1;\n").arg(-text->font().pixelSize());
                hasTextsyleChanged=true;
            }

            if(hasTextsyleChanged)
            {
                code+="\tsettextstyle(&f);\n";
                hasTextsyleChanged=false;//重置更改布尔值
            }

            if(!(initextColor.isValid()))
            {
                initextColor=text->defaultTextColor();//初始化，颜色没有初始化是无效值
                code+=QString("\tsettextcolor(RGB(%1,%2,%3));\n")
                            .arg(text->defaultTextColor().red()).arg(text->defaultTextColor().green()).arg(text->defaultTextColor().blue());
            }
            else if(initextColor!=text->defaultTextColor())
            {
                initextColor=text->defaultTextColor();//重置当前颜色
                code+=QString("\tsettextcolor(RGB(%1,%2,%3));\n")
                            .arg(text->defaultTextColor().red()).arg(text->defaultTextColor().green()).arg(text->defaultTextColor().blue());
            }

            code+=QString("\touttextxy(%1, %2, _T(\"%3\"));\n").arg(text->position.x()).arg(text->position.y()).arg(text->toPlainText());

        }
    }
    qDebug()<<code;

}

QString EasyXCodeGenerator::getCode() const
{
    qDebug()<<"gened code: "<<code<<endl;
    return code;
}
