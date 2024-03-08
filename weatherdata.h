#ifndef WEATHERDATA_H
#define WEATHERDATA_H

#include <QObject>
#include <QWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonArray>
#include <QDate>

class Today{
public:
    QString date;
    QString temperature;
    QString city;
    QString humidity;
    QString pm25;
    QString quality;
    QString cold;
    QString fx;
    QString fl;
    QString type;
    QString sunrise;
    QString sunset;
    QString notice;
public:
    Today();
    Today& operator=(const QJsonObject& obj);
};

class Forecast{
public:
    QString date;
    QString week;
    QString high;
    QString low;
    QString aqi;
    QString type;
public:
    Forecast();
    Forecast& operator=(const QJsonObject& obj);
};

#endif // WEATHERDATA_H
