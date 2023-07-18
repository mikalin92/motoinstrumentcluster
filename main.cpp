#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "motocycle.h"



QObject *rootObject;
QObject *recObject;


void insertProperty(char* what, QVariantList data ){
    rootObject->setProperty(what,data);
}


int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    rootObject = engine.rootObjects().first();
    recObject=rootObject->findChild<QObject*>("rec0");

    MotoCycle mt(rootObject);
    mt.move();


    return app.exec();
}


