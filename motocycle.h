#ifndef MOTOCYCLE_H
#define MOTOCYCLE_H
#include <QBasicTimer>
#include <QObject>
#include "locationservice.h"


class MotoCycle: QObject
{
    Q_OBJECT

private:
    double rpm;
    double kmph;
    int gear;
    QBasicTimer qti;
    QObject *drawObject;
    bool toBeAcc;
    double pos_n,pos_e,m_head;
    LocationService *ls;

public:
    MotoCycle(QObject *drawObject);
    double getRPM();
    double getKMPH();
    int getGear();
    void accelerate();
    void deccelerate();
    void move();
    void insertProperty(char* what, QVariantList data );

protected:
    void timerEvent(QTimerEvent *event) override;

};

#endif // MOTOCYCLE_H
