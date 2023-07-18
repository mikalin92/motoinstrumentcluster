#include "motocycle.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

MotoCycle::MotoCycle(QObject *drawObject)
{
    this->kmph=0;
    this->rpm=1000;
    this->gear=1;
    this->qti.start(30,this);
    this->drawObject=drawObject;
    this->toBeAcc=true;
    this->ls=new LocationService(QString::fromStdString(":/Kuopiopath2.geojson"));
}

int MotoCycle::getGear(){
    return this->gear;
}

double MotoCycle::getKMPH(){
    return this->kmph;
}

double MotoCycle::getRPM(){
    return this->rpm;
}

void MotoCycle::accelerate(){

    if(this->kmph<=180){
        this->kmph+=1;
        this->rpm+=20;
        if(rpm>2000){
            this->gear+=1;
            this->rpm=1000;
            this->move();
        }
    }else{
        this->toBeAcc=false;
    }
    QVariantList list({kmph,rpm,gear});
    this->insertProperty("motodata",list);

}

void MotoCycle::deccelerate(){

    if(this->kmph>=20){
        this->kmph-=1;
        this->rpm-=20;
        if(rpm<1000){
            this->gear-=1;
            this->rpm=2000;
            this->move();
        }
    }else{
        this->toBeAcc=true;
    }
    QVariantList list({kmph,rpm,gear});
    this->insertProperty("motodata",list);

}

void MotoCycle::move(){
    this->ls->nextLocation(pos_e,pos_n,m_head);
    QVariantList list({pos_e,pos_n,m_head});
    this->insertProperty("ccoords",list);
}


void MotoCycle::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == qti.timerId()) {
        //qDebug("timertest ab123");
        if(toBeAcc){
            accelerate();}
        else{
            deccelerate();
        }
    }
}


void MotoCycle::insertProperty(char* what, QVariantList data ){
    drawObject->setProperty(what,data);
}

