#include "locationservice.h"


#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDir>
#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>



LocationService::LocationService(QString path){
    try{
    //OPEN FILE
    QFile qf(path);
    if(!qf.exists()) {
        qDebug()<<"file did not exist";
        throw 101;
    }

    if(!qf.open(QIODevice::ReadOnly)) {
        qDebug()<<"file did not open";
        throw 102;
    }

    //READ FILE AS QJSON, READ COORDINATES FROM SUBARRAY
    QJsonDocument qjd=QJsonDocument::fromJson(qf.readAll());
    //root
    QJsonObject qjo =qjd.object();
    QJsonValue qjv=qjo.take("features")[0];
    //first object
    QJsonObject qjo1 =qjv.toObject();
    QJsonValue qjv1=qjo1.take("geometry");

    QJsonObject qjo2 =qjv1.toObject();
    QJsonValue qjv2=qjo2.take("coordinates");
    //extract coordinates
    QJsonArray qja=qjv2.toArray();

    //READ COORDINATES TO DOUBLE ARRAYS
    this->coordinate_iterator=0;
    this->coordamount=qja.size();
    this->e_coords=new double[coordamount];
    this->n_coords=new double[coordamount];

    int i=0;
    for (QJsonValue qiter : qja){

//        qDebug()<< qiter[0].toDouble();
//        qDebug()<< qiter[1].toDouble();
//        qDebug()<< "-----";
        e_coords[i]=qiter[0].toDouble();
        n_coords[i]=qiter[1].toDouble();
        i++;
    }

    //DEBUG
    //for (int j=0;j<coordamount;j++){
    //    qDebug()<<e_coords[j];
    //}
    //qDebug()<< qja.size();

    //BUILD HEADING ANGLES, POSITIVE RADS FROM NORTH
    //see atan2 coordinate order
    //for last, second last is used
    this->headings=new double[coordamount];
    for (int j=0;j<coordamount-1;j++){
        headings[j]=std::atan2(e_coords[j+1]-e_coords[j],n_coords[j+1]-n_coords[j]     );

        if(headings[j]<0){
            headings[j]+=2*3.14159265358979323846;
        }
        //qDebug()<<headings[j]*180/3.14159265358979323846<<"\n---";
    }
    headings[coordamount-1]=std::atan2(e_coords[coordamount-1]-e_coords[coordamount-2] , n_coords[coordamount-1]-n_coords[coordamount-2]   );
    if(headings[coordamount-1]<0){
        headings[coordamount-1]+=2*3.14159265358979323846;
    }
    //qDebug()<<headings[coordamount-1]*180/3.14159265358979323846<<"\n---";
    }catch(int errorcode){
    qDebug()<<"fileIO error. Does file exist? Is it compatible GEOJSON?";
    qDebug()<<"error n:o"<<errorcode;
                this->coordinate_iterator=0;
                this->coordamount=1;
                this->e_coords=new double[coordamount];
                this->n_coords=new double[coordamount];
                this->headings=new double[coordamount];
                e_coords[0]=27;
                n_coords[0]=61;
                headings[0]=0;
    }
}

void LocationService::nextLocation(double &e_cord,double &n_cord,double&m_head){

    e_cord=this->e_coords[this->coordinate_iterator];
    n_cord=this->n_coords[this->coordinate_iterator];
    m_head=this->headings[this->coordinate_iterator];
    this->coordinate_iterator++;
    if(this->coordinate_iterator>=this->coordamount){
        this->coordinate_iterator=0;
    }

}
