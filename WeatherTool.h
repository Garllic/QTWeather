#ifndef WEATHERTOOL_H
#define WEATHERTOOL_H

#include <map>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <QJsonArray>
#include <QCoreApplication>
#include <QFile>

class WeatherTool{
public:
    std::map<QString, QString> cityToCode;
public:
    WeatherTool(){
        QString fileName = QCoreApplication::applicationDirPath();
        fileName += "/Citys.json";
        QFile file(fileName);
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        QByteArray json = file.readAll();
        file.close();
        QJsonParseError err;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(json, &err);
        QJsonArray provinceArr = jsonDoc.object().value("citycode").toArray();
        for(int i = 0 ; i < provinceArr.size();i++){
            QJsonArray cityArr = provinceArr.at(i).toObject().value("city").toArray();
            for(int j = 0 ; j < cityArr.size();j++){
                QString cityName, cityCode;
                cityName = cityArr.at(j).toObject().find("cityname").value().toString();
                cityCode = cityArr.at(j).toObject().find("code").value().toString();
                if(cityCode.size()){
                                    cityToCode.insert(std::pair<QString, QString>(cityName, cityCode));
                }
            }
        }
    }

    QString operator[](const QString &cityname){
        if(cityToCode.count(cityname)){
            return cityToCode.at(cityname);
        }
        QString tname = cityname;
        tname.remove(tname.size()-1, 1);
        if(cityToCode.count(tname)){
            return cityToCode.at(tname);
        }
        return "000000000";
    }
};

#endif // WEATHERTOOL_H
