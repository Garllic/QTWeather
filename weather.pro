QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    weatherdata.cpp

HEADERS += \
    WeatherTool.h \
    mainwindow.h \
    weatherdata.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    weather_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

DISTFILES += \
    icons/close.png \
    icons/leaf.png \
    icons/location.png \
    icons/refresh.png \
    icons/search.png \
    images/weaUI.jpg \
    weatherIcons/baoxue.png \
    weatherIcons/baoyu-dabaoyu.png \
    weatherIcons/dabaoyu-tedabaoyu.png \
    weatherIcons/dabaoyu.png \
    weatherIcons/daxue-baoxue.png \
    weatherIcons/daxue.png \
    weatherIcons/dayu-baoyu.png \
    weatherIcons/dongyu.png \
    weatherIcons/fuchen.png \
    weatherIcons/leizhenyubanyoubingbao.png \
    weatherIcons/qiangshachenbao.png \
    weatherIcons/shachenbao.png \
    weatherIcons/wu.png \
    weatherIcons/xiaoxue-zhongxue.png \
    weatherIcons/xiaoxue.png \
    weatherIcons/xiaoyu-zhongyu.png \
    weatherIcons/yangsha.png \
    weatherIcons/yewu.png \
    weatherIcons/yezhenxue.png \
    weatherIcons/yezhenyu.png \
    weatherIcons/yujiaxue.png \
    weatherIcons/zhenxue.png \
    weatherIcons/zhongxue-daxue.png \
    weatherIcons/zhongxue.png \
    weatherIcons/zhongyu-dayu.png \
    weatherIcons/中雨.png \
    weatherIcons/多云.png \
    weatherIcons/夜多云.png \
    weatherIcons/夜晴.png \
    weatherIcons/大雨.png \
    weatherIcons/小雨.png \
    weatherIcons/晴.png \
    weatherIcons/暴雨.png \
    weatherIcons/特大暴雨.png \
    weatherIcons/阴.png \
    weatherIcons/阵雨.png \
    weatherIcons/雷阵雨.png \
    weatherIcons/雾霾.png
