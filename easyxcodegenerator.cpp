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
    // CircleItem circle(c,r);//存个数据，没啥用
    // circles_gen.push_back(circle);
    // QString circle_code = QString("circle(%1,%2,%3);\n").arg(c.x()).arg(c.y()).arg(r);
    // test_is_code_right_generator << circle_code << Qt::endl; // test code
    // code.append(circle_code);
    // test_is_code_right_generator << "add circle." << code << Qt::endl; // test code
}

void EasyXCodeGenerator::addText(const QString &text, const QPointF &position)
{
    // QString text_code = QString("outtextxy(%1, %2,_T(\"%3\"));\n").arg(position.x()).arg(position.y()).arg(text);
    // test_is_code_right_generator << text_code << Qt::endl; // test code
    // code.append(text_code);
    // test_is_code_right_generator << "add text." << code << Qt::endl;
}
void EasyXCodeGenerator::addLine(QPointF p1,QPointF p2)//增加一个线段
{
    // LineItem line(p1,p2);//存个数据，没啥用
    // lines_gen.push_back(line);
    // QString line_code = QString("line(%1,%2,%3,%4);\n").arg(p1.x()).arg(p1.y()).arg(p2.x()).arg(p2.y());
    // test_is_code_right_generator << line_code << Qt::endl; // test code
    // code.append(line_code);
    // test_is_code_right_generator << "add line." << code << Qt::endl; // test code
}

void EasyXCodeGenerator::generateCode( const CustomGraphicsScene *scene)
{//点击genecode后遍历scene的所有item并添加到code在点击genecode后才能正常获取code

    int ini_R=0;
    int ini_G=0;
    int ini_B=0;
    QColor initextColor;//第一次调用时初始化

    QFont initial_font;
    initial_font.setFamily("000");
    initial_font.setPixelSize(-100);

    bool hasTextsyleChanged=false;

    code.clear();//清空缓存

    //预设信息
    code+="\tsetBKcolor(RGB(255,255,255));\n";
    code+="\tsetcolor(RGB(0,0,0));\n";
    code+="\tcleardevice()\n";
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
            //qDebug()<<circle->R<<circle->G<<circle->B<<endl;
            if(circle->R!=ini_R||circle->G!=ini_G||circle->B!=ini_B)
            {
                code+=QString("\tsetlinecolor(RGB(%1,%2,%3));\n").arg(circle->R).arg(circle->G).arg(circle->B);
                colorcode.push_back(QString("\tsetlinecolor(RGB(%1,%2,%3));\n").arg(circle->R).arg(circle->G).arg(circle->B));//如果有颜色，改画笔
                ini_R=circle->R;
                ini_G=circle->G;
                ini_B=circle->B;
            }
            //tempcode.push_back( QString("circle(%1,%2,%3);\n").arg(circle->center.x()).arg(circle->center.x()).arg(circle->radius));
            code+=QString("\tcircle(%1,%2,%3);\n").arg(circle->center.x()).arg(circle->center.x()).arg(circle->radius);
        }
        else if (CustomLineItem *line = dynamic_cast<CustomLineItem*>(item))
        {
            //qDebug()<<line->R<<"sdinjfni";

            if(line->R!=ini_R||line->G!=ini_G||line->B!=ini_B)
            {
                //qDebug()<<"222"<<line->G;
                code+=QString("\tsetlinecolor(RGB(%1,%2,%3));\n").arg(line->R).arg(line->G).arg(line->B);//如果有颜色，改画笔
                ini_R=line->R;
                ini_G=line->G;
                ini_B=line->B;
            }
            code+= QString("\tline(%1,%2,%3,%4);\n").arg(line->point1.x()).arg(line->point1.y()).arg(line->point2.x()).arg(line->point2.y());
            // qDebug()<< QString("line(%1,%2,%3,%4);\n").arg(line->point1.x()).arg(line->point1.y()).arg(line->point2.x()).arg(line->point2.y());
        }
        else if(CustomRectangleItem *rec=dynamic_cast<CustomRectangleItem*>(item))
        {
            // if(line->R!=ini_R||line->G!=ini_G||line->B!=ini_B)
            // {
            //     code+=QString("\tsetlinecolor(RGB(%1,%2,%3));\n").arg(rec->R).arg(rec->G).arg(rec->B);//如果有颜色，改画笔
            //     ini_R=rec->R;
            //     ini_G=rec->G;
            //     ini_B=rec->B;//无初始化与声明定义
            // }
            code+= QString("\trectangle(%1,%2,%3,%4);\n").arg(rec->LT.x()).arg(rec->LT.y()).arg(rec->RB.x()).arg(rec->RB.y());
        }
        else if(CustomTextItem *text=dynamic_cast<CustomTextItem*>(item))
        {
            if(text->font().family()!=initial_font.family())
            {
                code+=QString("\t_tcscpy_(f.lfFaceName, _T(\"%1\"));\n").arg(text->font().family());
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
    // std::vector<std::pair<QPointF, qreal>> circles = scene->getCircles();
    // for (const auto &circle : circles)
    // {
    //     addCircle(circle.first, circle.second); // 使用圆心坐标和半径生成代码
    // }
    // // for (const CircleItem &circle : circles) {
    // //     code+=QString("circle(%1,%2,%3);\n").arg(circle.center.x()).arg(circle.center.y() - 6).arg(circle.radius);
    // //      // 将圆的信息写入qstring
    // // }
    // QFile file(filename);
    // if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    // {
    //     QTextStream out(&file);

    //     test_is_code_right_generator << "code is: " << code << Qt::endl; // 将字符串输出到标准输出
    //     out << code;
    //     file.close();
    // }
}

QString EasyXCodeGenerator::getCode() const
{
    qDebug()<<"gened code: "<<code<<endl;
    return code;
}
