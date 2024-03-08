#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QList>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include <QNetworkReply>
#include <QPainter>
#include <QTimer>
#include "WeatherTool.h"
#include "weatherdata.h"


//http://t.weather.itboy.net/api/weather/city/

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    virtual bool eventFilter(QObject* watched, QEvent* event);
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QMenu* exitMenu;//退出菜单
    QAction* exitAct;//退出行为
    QPoint mPos;
    WeatherTool city2code;//城市代码
    //ui对象的列表
    QList<QLabel*> weekList;
    QList<QLabel*> dateList;
    QList<QLabel*> aqiList;
    QList<QLabel*> typeList;
    QList<QLabel*> iconList;
    QList<QLabel*> highList;
    QList<QLabel*> lowList;
    Today today;//今天天气的数据
    Forecast forecast[6];//包括今天，后六天的数据
    QNetworkAccessManager* manage;
    QString city;
    QString tempCity;
    QString url;
    static const QPoint sunLinePoints[2];//直线的两个点
    static const QRect sunTimeRect[2];//日出日落时间
    static const QRect sunRect[2];//半圆和文本
    QTimer* sunTimer;


protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent * event);
    void getWeatherInfo(QNetworkAccessManager* manage);
    void setLabelContent();
    void setAqiLabel(int aqi, QLabel* label);
    void setIconLabel(QString type, QLabel* label);
    void paintSunRiseSet();
    void paintCurve();

private:
    Ui::MainWindow *ui;

private slots:
    void slot_exitApp();
    void replyFinished(QNetworkReply *reply);
    void on_refreshBt_clicked();
    void on_searchBt_clicked();
};
#endif // MAINWINDOW_H
