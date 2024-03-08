#include "weatherdata.h"

Today::Today()
{
    date = "无数据";
    temperature = "无数据";
    city = "无数据";
    humidity = "无数据";
    pm25 = "无数据";
    quality = "无数据";
    cold = "无数据";
    fx = "无数据";
    fl = "无数据";
    type = "无数据";
    sunrise = "00:00";
    sunset = "00:00";
    notice = "无数据";
}

Today &Today::operator=(const QJsonObject &obj)
{
    this->date = obj.value("date").toString();
    this->date = QDate::fromString(this->date,"yyyyMMd").toString("yyyy-MM-dd");
    this->city = obj.value("cityInfo").toObject().value("city").toString();

    QJsonObject todayInfo = obj.value("data").toObject();
    this->temperature = todayInfo.value("wendu").toString();
    this->humidity = todayInfo.value("shidu").toString();
    this->pm25 = QString::number(todayInfo.value("pm25").toDouble());
    this->quality = todayInfo.value("quality").toString();
    this->cold = todayInfo.value("ganmao").toString();

    QJsonObject todayFc = obj.value("data").toObject().value("forecast").toArray().at(0).toObject();
    this->fx = todayFc.value("fx").toString();
    this->fl = todayFc.value("fl").toString();
    this->type = todayFc.value("type").toString();
    this->sunrise = todayFc.value("sunrise").toString();
    this->sunset = todayFc.value("sunset").toString();
    this->notice = todayFc.value("notice").toString();

    return *this;
}

Forecast::Forecast()
{
    date = "无数据";
    week = "无数据";
    high = "无数据";
    low = "无数据";
    aqi = "无数据";
    type = "无数据";
}

Forecast &Forecast::operator=(const QJsonObject &obj)
{
    this->date = obj.value("date").toString();
    this->week = obj.value("week").toString();
    this->high = obj.value("high").toString();
    this->low = obj.value("low").toString();
    this->aqi = QString::number(obj.value("aqi").toDouble());
    this->type = obj.value("type").toString();

    return *this;
}
