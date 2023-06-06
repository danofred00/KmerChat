
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    QQmlApplicationEngine engine;

    const QUrl url(u"qrc:/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreationFailed, &a, [&]() {
        qDebug() << "Unable to load Qml File : " << url;
        QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);

    engine.load(url);

    return a.exec();
}
