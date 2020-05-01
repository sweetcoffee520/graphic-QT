#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class coordinate
{
public:
    coordinate():x(0),y(0),h(0){}
    double x;
    double y;
    double h;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QList<coordinate> coor;
    char flag='N';
    QPointF *points;
    QString savepath;
    QLabel *label=new QLabel();
    bool saveflag=false;
    QString savefilename;
protected:
    void paintEvent(QPaintEvent *);
private slots:
    void on_actionopen_triggered();

    void on_actionsave_triggered();

    void on_actionclose_triggered();

    void on_actionpaint_x_triggered();

    void on_actionpaint_y_triggered();

    void on_actionpaint_z_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
