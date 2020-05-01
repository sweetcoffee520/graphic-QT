#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "QFile"
#include "QTextStream"
#include "QFileDialog"
#include "QIODevice"
#include "QPainter"
#include "QDebug"
#include "QScreen"
#include "QLabel"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setFixedSize(1650,900);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if(flag!='N')
    {
        //新建pix以储存图像
        QPixmap pix(2400,1500);
        pix.fill(Qt::white);
        int i;
        QPainter painter(&pix);
        painter.setRenderHint(QPainter::Antialiasing);
        //平移坐标原点到左下角处
        painter.setWindow(0,100,2400,1500);
        //painter.setViewport(0,-100,9*ui->scrollArea->width()/10,ui->scrollArea->height());
        painter.translate(pix.width()/10,pix.height()*9/10); //坐标系统平移变换
        //painter.scale(0.6*ui->scrollArea->width()/1600,-0.6*ui->scrollArea->height()/750); //Y轴向上翻转,翻转成正常平面直角坐标系
        painter.scale(1,-1);
        QPen pen(QColor(255,0,0),2);
        painter.setPen(pen);
        //获取偏离度最大和最小的值
        double max,min;
        switch (flag)
        {
        case 'X':
        {
            max=coor.at(0).x;
            min=coor.at(0).x;
            points=new QPointF[coor.length()];
            for(i=0;i<coor.length();i++)
            {
                if(max<coor.at(i).x)max=coor.at(i).x;
                if(min>coor.at(i).x)min=coor.at(i).x;
            }
            //painter.drawPolyline(points,coor.length());
            painter.save();
            painter.setPen(QPen(QColor(Qt::black),2));
            //画x轴，放大80倍显示，基于像素的坐标系统
            painter.drawLine(0,0,coor.length()*80+40,0);
            //画y轴，由于最大最小值相差很小，所以放啊100000倍显示（尝试过放大坐标系，会出现线条变粗等各种问题
            //所以放弃，不过应该可以使用变换逻辑坐标系实现，但是我不想改了。
            painter.drawLine(QPointF(0,0),QPointF(0,(max-min)*100000+40));
            //画x轴箭头
            painter.drawLine(coor.length()*80+20,10,coor.length()*80+40,0);
            painter.drawLine(coor.length()*80+20,-10,coor.length()*80+40,0);
            //画y轴箭头
            painter.drawLine(-10,(max-min)*100000+20,0,(max-min)*100000+40);
            painter.drawLine(10,(max-min)*100000+20,0,(max-min)*100000+40);
            //变换坐标系以画文字，否则会倒过来
            painter.scale(1,-1);
            //x轴标注及其内部格网
            for(i=0;i<coor.length();i++)
            {
                painter.drawText((i+1)*80-10,1,20,20,Qt::AlignCenter,QString::number(i+1));
                painter.drawLine((i+1)*80,0,(i+1)*80,-10);
            }
            //y轴标注及其内部格网
            for(i=0;i<5;i++)
            {
                painter.drawText(-101,-10-(max-min)*100000/4*i,100,20,Qt::AlignCenter,QString::number(min+(max-min)/4*i));
                painter.drawLine(0,-(max-min)*100000/4*i,coor.length()*80+40,-(max-min)*100000/4*i);
                //painter.drawText(-101,-(max-min)*100000-10,100,20,Qt::AlignCenter,QString::number(max));
            }
            //图形标签
            painter.drawText((coor.length()*80+40)/2-500,-(max-min)*100000-150,1000,30,Qt::AlignCenter,"X方向变形");
            //恢复画笔状态
            painter.restore();
            //画主要折线部分
            for(i=0;i<coor.length();i++)
            {
                points[i]=QPointF((i+1)*80,100000*(coor.at(i).x-min));
            }
            painter.drawPolyline(points,coor.length());
            delete points;
            painter.end();
            break;
        }
        case 'Y':
        {
            max=coor.at(0).y;
            min=coor.at(0).y;
            points=new QPointF[coor.length()];
            for(i=0;i<coor.length();i++)
            {
                if(max<coor.at(i).y)max=coor.at(i).y;
                if(min>coor.at(i).y)min=coor.at(i).y;
            }
            //储存笔的状态
            painter.save();
            painter.setPen(QPen(QColor(Qt::black),2));
            //画x轴
            painter.drawLine(0,0,coor.length()*80+40,0);
            //画y轴
            painter.drawLine(QPointF(0,0),QPointF(0,(max-min)*100000+40));
            //画x轴箭头
            painter.drawLine(coor.length()*80+20,10,coor.length()*80+40,0);
            painter.drawLine(coor.length()*80+20,-10,coor.length()*80+40,0);
            //画y轴箭头
            painter.drawLine(-10,(max-min)*100000+20,0,(max-min)*100000+40);
            painter.drawLine(10,(max-min)*100000+20,0,(max-min)*100000+40);
            //变换坐标系以画文字，否则会倒过来
            painter.scale(1,-1);
            //x轴标注及其内部格网
            for(i=0;i<coor.length();i++)
            {
                painter.drawText((i+1)*80-10,1,20,20,Qt::AlignCenter,QString::number(i+1));
                painter.drawLine((i+1)*80,0,(i+1)*80,-10);
            }
            //y轴标注及其内部格网
            for(i=0;i<5;i++)
            {
                painter.drawText(-101,-10-(max-min)*100000/4*i,100,20,Qt::AlignCenter,QString::number(min+(max-min)/4*i));
                painter.drawLine(0,-(max-min)*100000/4*i,coor.length()*80+40,-(max-min)*100000/4*i);
                //painter.drawText(-101,-(max-min)*100000-10,100,20,Qt::AlignCenter,QString::number(max));
            }
            painter.drawText((coor.length()*80+40)/2-500,-(max-min)*100000-150,1000,30,Qt::AlignCenter,"Y方向变形");
            painter.restore();
            for(i=0;i<coor.length();i++)
            {
                points[i]=QPointF((i+1)*80,100000*(coor.at(i).y-min));
            }
            painter.drawPolyline(points,coor.length());
            delete points;
            painter.end();
            //ui->scroolArea->setPixmap(pix);
            break;
        }
        case 'H':
        {
            max=coor.at(0).h;
            min=coor.at(0).h;
            points=new QPointF[coor.length()];
            for(i=0;i<coor.length();i++)
            {
                if(max<coor.at(i).h)max=coor.at(i).h;
                if(min>coor.at(i).h)min=coor.at(i).h;
            }
            //painter.drawPolyline(points,coor.length());
            painter.save();
            painter.setPen(QPen(QColor(Qt::black),2));
            //画x轴
            painter.drawLine(0,0,coor.length()*80+40,0);
            //画y轴
            painter.drawLine(QPointF(0,0),QPointF(0,(max-min)*100000+40));
            //画x轴箭头
            painter.drawLine(coor.length()*80+20,10,coor.length()*80+40,0);
            painter.drawLine(coor.length()*80+20,-10,coor.length()*80+40,0);
            //画y轴箭头
            painter.drawLine(-10,(max-min)*100000+20,0,(max-min)*100000+40);
            painter.drawLine(10,(max-min)*100000+20,0,(max-min)*100000+40);
            //变换坐标系以画文字，否则会倒过来
            painter.scale(1,-1);
            //x轴标注及其内部格网
            for(i=0;i<coor.length();i++)
            {
                painter.drawText((i+1)*80-10,1,20,20,Qt::AlignCenter,QString::number(i+1));
                painter.drawLine((i+1)*80,0,(i+1)*80,-10);
            }
            //y轴标注及其内部格网
            for(i=0;i<5;i++)
            {
                painter.drawText(-101,-10-(max-min)*100000/4*i,100,20,Qt::AlignCenter,QString::number(min+(max-min)/4*i));
                painter.drawLine(0,-(max-min)*100000/4*i,coor.length()*80+40,-(max-min)*100000/4*i);
                //painter.drawText(-101,-(max-min)*100000-10,100,20,Qt::AlignCenter,QString::number(max));
            }
            painter.drawText((coor.length()*80+40)/2-500,-(max-min)*100000-150,1000,30,Qt::AlignCenter,"H方向变形");
            painter.restore();
            for(i=0;i<coor.length();i++)
            {
                points[i]=QPointF((i+1)*80,100000*(coor.at(i).h-min));
            }
            painter.drawPolyline(points,coor.length());
            delete points;
            painter.end();
            break;
        }
        }
        if(saveflag==true)
        {
            pix.save(savefilename);
        }
        label->setPixmap(pix.scaled(ui->scrollArea->size(),Qt::KeepAspectRatio));
        ui->scrollArea->setWidget(label);
    }

}

void MainWindow::on_actionopen_triggered()
{
    int i;
    int num;
    QByteArray arr;
    QStringList data_list;
    coordinate temp;
    QString filepath=QFileDialog::getOpenFileName(this,QStringLiteral("打开文件"),"","*.txt");
    QFile infile(filepath);
    if(!infile.open(QIODevice::ReadOnly|QIODevice::Text)){QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("文件未打开"));
        return;
    }
    num=infile.readLine().toInt();
    while(!infile.atEnd())
    {
        for(i=0;i<num;i++)
        {
            arr=infile.readLine();
            data_list=QString(arr).split(QRegExp("\\s+|,+"));
            temp.x=data_list.at(0).toDouble();
            temp.y=data_list.at(1).toDouble();
            temp.h=data_list.at(2).toDouble();
            coor.append(temp);
        }
    }
    infile.close();
    QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("读取成功"));
}


void MainWindow::on_actionsave_triggered()
{
    savefilename=QFileDialog::getSaveFileName(this,QStringLiteral("保存文件"),"","*.jpg");
    QFile outfile(savefilename);
    if(!outfile.open(QIODevice::WriteOnly)){QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("文件未打开"));
    return;
    }
    saveflag=true;
    //QScreen *screen = QGuiApplication::primaryScreen();
    //screen->grabWindow(ui->scrollArea->winId()).save(filepath);
    repaint();
    saveflag=false;

}

void MainWindow::on_actionclose_triggered()
{
    this->close();
}

void MainWindow::on_actionpaint_x_triggered()
{
    if(coor.length()!=0)
    {
        flag='X';
        update();
    }
    else
    {
        QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("请先选择文件"));
    }
}

void MainWindow::on_actionpaint_y_triggered()
{
    if(coor.length()!=0)
    {
        flag='Y';
        update();
    }
    else
    {
        QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("请先选择文件"));
    }
}

void MainWindow::on_actionpaint_z_triggered()
{
    if(coor.length()!=0)
    {
        flag='H';
        update();
    }
    else
    {
        QMessageBox::information(this,QStringLiteral("提示"),QStringLiteral("请先选择文件"));
    }
}
