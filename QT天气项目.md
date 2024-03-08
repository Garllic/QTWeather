# 实现思路

1. UI：
   - 绘制基本界面，设计界面网格表格
   - 添加控件、添加资源图片
2. 网络：
   - 请求天气数据
   - 解析天气数据
3. UI：
   - 根据数据更新控件上的数据和资源图标
   - 根据数据绘制图案

# 用定时器实时更新

```c++
    ui->sunRiseSetLb->installEventFilter(this);//将this设置为过滤器对象，当ui->sunRiseSetLb触发事件时候，执行this->eventFilter(QObject* obj, QEvent* e)，即ui->sunRiseSetLb开始被this观察
    sunTimer = new QTimer(ui->sunRiseSetLb);
    connect(sunTimer, SIGNAL(timeout()), ui->sunRiseSetLb, SLOT(updata()));//updata中会触发paintEvent()
    sunTimer->start(1000);
```

```
    virtual bool eventFilter(QObject* watched, QEvent* event);
    
    bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == ui->sunRiseSetLb){
        if(event->type()==QEvent::Paint){
            paintSunRiseSet();
        }
    }
    return QWidget::eventFilter(watched, event);
}
```

1、Qt实现事件过滤器的步骤如下：
①、Qt调用
void QObject::installEventFilter (QObject* filterObj)
把filterObj对象安装(或注册)为事件过滤器，filterObj也称为过滤器对象。事件过滤器通常在构造函数中进行注册。
②、在上一步注册的filterObj对象，通过调用
bool QObject::eventFilter(QObject* obj, QEvent* e);
来接收拦截到的事件。也就是说拦截到的事件在filterObj对象中的eventFilter函数中处理。eventFilter的第一个参数obj指向的是事件本应传递到的目标对象。
③、使用QObject::removeEventFilter(QObject *obj)函数可以删除事件过滤器。

2、理解事件过滤器
观察者模式：其原理为，设有一目标对象S，它有多个观察该对象的对象G1，G2，G3，当S发生变化时，S的观察者会依情形改变自身。应用于Qt事件过滤器，则是，首先使用S的成员函数installEventFilter函数把G1，G2，G3设置为S的观察者，所有本应传递给S的事件E，则先传递给观察者G1，G2，G3，然后观察者调用其成员函数eventFilter对传递进来的事件进行处理，若eventFilter返回true表示事件处理完毕，返回false则返回给被观察者S进行处理。见图2-13。若同一对象安装了多个事件过滤器，则最后安装的过滤器首先被激活。

![img](https://img-blog.csdn.net/20181011170925908?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2h5b25naWxmbW1t/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



# 资源名带中文的问题

QT控制台
报错： No rule to make target ‘/??.png’, needed by ‘debug/qrc_Icon.cpp’. Stop.

原因： 资源文件中包含中文命名的图片文件。

解决办法：

方法1： 在资源文件中重命名含有中文名的图片文件，执行qmake，重新构建（注意对之前添加图片的路径及时修改）；

方法2： 将含有中文命名的图片文件从资源文件中移除，执行qmake，在重新添加有中文图片的文件，再重新编译就不会出错了。
注意如果添加含有中文命名的图片文件再执行qmake，重新构建会报错。
