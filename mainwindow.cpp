#include "mainwindow.h"
#include "ui_mainwindow.h"

#define SPAN_INDEX 3//温度曲线间隔指数
#define ORIGIN_SIZE 3//
#define TEMPERATURE_STARTING_COORDINATE 45

const QPoint MainWindow::sunLinePoints[2] = {
    QPoint(20, 75),
    QPoint(130, 75)
};//直线的两个点

const QRect MainWindow::sunTimeRect[2] = {
    QRect(0, 80, 50, 20),
    QRect(100, 80, 50, 20)
};//日出日落时间

const QRect MainWindow::sunRect[2] = {
    QRect(25, 25, 100, 100),//半圆
    QRect(50, 80, 50, 20)//文本
};

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , city2code()
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //无边框
    setWindowFlag(Qt::FramelessWindowHint);
    //固定size
    setFixedSize(this->width(), this->height());
    //关闭按钮
    exitMenu = new QMenu(this);
    exitAct = new QAction(this);
    exitMenu->addAction(exitAct);
    exitAct->setText("exit");
    exitAct->setIcon(QIcon(":/icons/close.png"));
    connect(exitAct, SIGNAL(triggered(bool)), this, SLOT(slot_exitApp()));
    //初始化ui_list对象
    this->weekList << ui->week0Lb << ui->week1Lb << ui->week2Lb << ui->week3Lb << ui->week4Lb << ui->week5Lb;
    this->dateList << ui->date0Lb << ui->date1Lb << ui->date2Lb << ui->date3Lb << ui->date4Lb << ui->date5Lb;
    this->aqiList << ui->quality0Lb << ui->quality1Lb << ui->quality2Lb << ui->quality3Lb << ui->quality4Lb << ui->quality5Lb;
    this->typeList << ui->typeLb_1 << ui->typeLb_2 << ui->typeLb_3 << ui->typeLb_4 << ui->typeLb_5 << ui->typeLb_6;
    this->iconList << ui->typeIco_1 << ui->typeIco_2 << ui->typeIco_3 << ui->typeIco_4 << ui->typeIco_5 << ui->typeIco_6;
    this->highList << ui->highLb_1 << ui->highLb_2 << ui->highLb_3 << ui->highLb_4 << ui->highLb_5 << ui->highLb_6;
    this->lowList << ui->lowLb_1 << ui->lowLb_2 << ui->lowLb_3 << ui->lowLb_4 << ui->lowLb_5 << ui->lowLb_6;
    //对weekList,dateList,cityLineEdit的初始化
    for(int i = 0 ; i< this->weekList.size();i++){
        this->weekList[i]->setStyleSheet("background-color:rgba(0, 255, 255, 100)");
        this->dateList[i]->setStyleSheet("background-color:rgba(0, 255, 255, 100)");
    }
    ui->cityLineEdit->setStyleSheet("QLineEdit{"
                                    "border: 1px solid grey;"
                                    "border-radius:4px;"
                                    "background-color:rgba(47,47,47,130);"
                                    "color:(255,255,255);}"
                                    "QLineEdit:hover{border-color:rgb(101,255,106)}");
    //网络请求
    this->city = "杭州";
    this->tempCity = "杭州";
    this->url = "http://t.weather.itboy.net/api/weather/city/";
    manage = new QNetworkAccessManager(this);
    connect(manage, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    getWeatherInfo(manage);

    ui->sunRiseSetLb->installEventFilter(this);
    ui->curveLb->installEventFilter(this);
    ui->cityLineEdit->installEventFilter(this);
    //用定时器触发
    sunTimer = new QTimer(ui->sunRiseSetLb);
    connect(sunTimer, SIGNAL(timeout()), ui->sunRiseSetLb, SLOT(update()));
    sunTimer->start(1000);
    //温度曲线
    ui->curveLb->update();
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->sunRiseSetLb){
        if(event->type()==QEvent::Paint){
            paintSunRiseSet();
        }
    }else if(watched == ui->curveLb){
        if(event->type()==QEvent::Paint){
            paintCurve();
        }
    }
    return QWidget::eventFilter(watched, event);
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    exitMenu->exec(QCursor::pos());
    event->accept();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    move(event->globalPos() - mPos);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    mPos = event->globalPos() - this->pos();
}

void MainWindow::getWeatherInfo(QNetworkAccessManager *manage)
{
    QString cityCode = this->city2code[this->city];
    if(cityCode == "000000000"){
        QMessageBox::information(this, "提示", "无该城市数据",QMessageBox::Ok);
        return ;
    }
    manage->get(QNetworkRequest(this->url+cityCode));
}

void MainWindow::setLabelContent()
{
    //用数据改变ui内容
    for (int i = 0 ; i < 6 ; i++){
        this->weekList[i]->setText(forecast[i].week);
        setAqiLabel(forecast[i].aqi.toInt(), this->aqiList[i]);
        this->dateList[i]->setText(forecast[i].date);
        this->highList[i]->setText(forecast[i].high.right(3));
        this->lowList[i]->setText(forecast[i].low.right(3));
        this->typeList[i]->setText(forecast[i].type);
        setIconLabel(forecast[i].type, this->iconList[i]);
    }
    this->weekList[0]->setText("今天");
    this->weekList[1]->setText("明天");
    this->weekList[2]->setText("后天");
    ui->cityNameLb->setText(today.city);
    ui->fengliLb->setText(today.fl);
    ui->fengxiangLb->setText(today.fx);
    ui->ganmaoBrowser->setText(today.cold);
    ui->shiduLb->setText(today.humidity);
    ui->noticeLb->setText(today.notice);
    ui->temLb->setText(today.temperature);
    ui->pmLb->setText(today.pm25);
    ui->dateLb->setText(today.date);
    ui->qualityLb->setText(today.quality);
    setIconLabel(today.type, ui->typeIcoLb);
    ui->typeIcoLb->setStyleSheet(ui->typeIcoLb->styleSheet()+"background-color: rgba(255, 255, 255, 0);");
}

void MainWindow::setAqiLabel(int aqi, QLabel* label)
{
    QString res;
    QString sheet;
    int catagory = aqi/50;
    switch (catagory) {
    case 0:
        res = "优质";
        sheet = "color: rgb(0, 130, 0)";
        break;
    case 1:
        res = "良好";
        sheet = "color: rgb(255, 255, 0)";
        break;
    case 2:
        res = "轻度污染";
        sheet = "color: rgb(255, 170, 0)";
        break;
    case 3:
        res = "中度污染";
        sheet = "color: rgb(255, 0, 0)";
        break;
    default:
        res = "重度污染";
        sheet = "color: rgb(170, 0, 0)";
        break;
    }
    label->setText(res);
    label->setStyleSheet(sheet);
}

void MainWindow::setIconLabel(QString type, QLabel *label)
{
    QString iconName = ":/weatherIcons/" + type + ".png";
    label->setStyleSheet("image:url(" + iconName + ");");
}

void MainWindow::paintSunRiseSet()
{
    QPainter painter(ui->sunRiseSetLb);
    painter.setRenderHint(QPainter::Antialiasing, true);
    //绘制直线
    painter.save();
    QPen pen = painter.pen();
    pen.setColor(Qt::yellow);
    pen.setWidthF(0.5);
    painter.setPen(pen);
    painter.drawLine(sunLinePoints[0], sunLinePoints[1]);
    painter.restore();
    //绘制文字
    painter.save();
    painter.setFont(QFont("Microsoft Yahei", 8, QFont::Normal));
    painter.setPen(Qt::white);
    if(today.sunrise!=""&&today.sunset!=""){
        painter.drawText(sunTimeRect[0], Qt::AlignHCenter, today.sunrise);
        painter.drawText(sunTimeRect[1], Qt::AlignHCenter, today.sunset);
    }
    QFont font = painter.font();
    font.setPixelSize(12);
    painter.setFont(font);
    painter.drawText(sunRect[1], Qt::AlignHCenter, "日出日落");
    painter.restore();
    //绘制圆弧
    painter.save();
    pen.setWidthF(0.5);
    pen.setColor(Qt::green);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.drawArc(sunRect[0], 0, 180 * 16); //qt中1度等于现实1/16度
    painter.restore();
    //绘制日落日出占比
    if(today.sunrise!=""&&today.sunset!=""){
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(255, 85, 0, 100));
        int curTime, riseTime, setTime;
        int startAngle, spanAngle;
        QString sunsetTime = today.date + " " + today.sunset;
        if(QDateTime::currentDateTime().time()>QDateTime::fromString(sunsetTime, "yyyy-MM-dd hh:mm").time()){
            startAngle = 0*16;
            spanAngle = 180*16;
        }else{
            curTime = QTime::currentTime().hour()*60+QTime::currentTime().minute();
            auto rise = today.sunrise.split(":");
            auto set = today.sunset.split(":");
            riseTime = rise[0].toInt()*60+rise[1].toInt();
            setTime = set[0].toInt()*60+set[1].toInt();
            startAngle = (double(setTime-curTime)/double(setTime-riseTime)) * 180*16;
            spanAngle = (double(curTime-riseTime)/double(setTime-riseTime))*180*16;
        }
        if (startAngle>=0&&spanAngle>0){
            painter.drawPie(sunRect[0], startAngle, spanAngle);
        }
    }
}

void MainWindow::paintCurve()
{
    QPainter painter(ui->curveLb);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int tempTotal = 0;
    int high[6] = {};
    int low[6] = {};
    //计算平均值
    QString h, l;
    for (int i = 0; i < 6 ; i++){
        h = forecast[i].high.right(3);
        h = h.left(h.length()-1);
        high[i] = (int)h.toDouble();
        tempTotal += high[i];

        l = forecast[i].low.right(3);
        l = l.left(l.length()-1);
        low[i] = (int)l.toDouble();
    }
    int aveTemp = (int)(tempTotal/6);
    //算出温度对应坐标
    int pointX[6] = {35, 103, 172, 241, 310, 379};
    int pointHY[6] = {0};
    int pointLY[6] = {0};
    for(int i = 0 ; i < 6 ;i++){
        pointHY[i] = TEMPERATURE_STARTING_COORDINATE - ((high[i] - aveTemp) * SPAN_INDEX);
        pointLY[i] = TEMPERATURE_STARTING_COORDINATE + ((aveTemp - low[i]) * SPAN_INDEX);
    }
    //绘制
    QPen pen = painter.pen();
    pen.setWidth(1);
        //高温
    painter.save();
    pen.setColor(QColor(255, 170, 0));
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.setBrush(QColor(255, 170, 0));
    painter.drawEllipse(QPoint(pointX[0], pointHY[0]), ORIGIN_SIZE, ORIGIN_SIZE);
    painter.drawEllipse(QPoint(pointX[1], pointHY[1]), ORIGIN_SIZE, ORIGIN_SIZE);
    painter.drawLine(pointX[0],pointHY[0],pointX[1],pointHY[1]);
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    painter.setPen(pen);

    for(int i = 1 ; i< 5 ;i++){
        painter.drawEllipse(QPoint(pointX[i+1], pointHY[i+1]), ORIGIN_SIZE, ORIGIN_SIZE);
        painter.drawLine(pointX[i],pointHY[i],pointX[i+1],pointHY[i+1]);
    }

    painter.restore();
        //低温
    painter.save();
    pen.setColor(QColor(0, 255, 255));
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.setBrush(QColor(0, 255, 255));
    painter.drawEllipse(QPoint(pointX[0], pointLY[0]), ORIGIN_SIZE, ORIGIN_SIZE);
    painter.drawEllipse(QPoint(pointX[1], pointLY[1]), ORIGIN_SIZE, ORIGIN_SIZE);
    painter.drawLine(pointX[0],pointLY[0],pointX[1],pointLY[1]);//第一段用虚线
    pen.setStyle(Qt::SolidLine);
    pen.setWidth(1);
    painter.setPen(pen);

    for(int i = 1 ; i< 5 ;i++){
        painter.drawEllipse(QPoint(pointX[i+1], pointLY[i+1]), ORIGIN_SIZE, ORIGIN_SIZE);
        painter.drawLine(pointX[i],pointLY[i],pointX[i+1],pointLY[i+1]);
    }
}

void MainWindow::slot_exitApp()
{
    qApp->exit(0);
}

void MainWindow::replyFinished(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(reply->error()!=QNetworkReply::NoError||statusCode != 200){
        QMessageBox::information(this, "提示", "无网络连接",QMessageBox::Ok);
        return ;
    }
    QByteArray json = reply->readAll();
    QJsonParseError err;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(json, &err);
    if(err.error != QJsonParseError::NoError){
        return ;
    }
    QString message = jsonDoc.object().value("message").toString();
    if (!message.contains("success")){
        QMessageBox::information(this, "提示", "城市错误",QMessageBox::Ok);
        this->city = this->tempCity;
        return ;
    }
    this->today = jsonDoc.object();
    QJsonArray forecastArr = jsonDoc.object().value("data").toObject().value("forecast").toArray();
    for(int i = 0 ; i < 6; i++){
        this->forecast[i] = forecastArr[i].toObject();
    }
    setLabelContent();
    ui->curveLb->update();
}


void MainWindow::on_refreshBt_clicked()
{
    getWeatherInfo(manage);
    ui->curveLb->update();
}


void MainWindow::on_searchBt_clicked()
{
    tempCity = city;
    city = ui->cityLineEdit->text();
    getWeatherInfo(manage);
    ui->curveLb->update();
}

