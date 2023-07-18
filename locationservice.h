#ifndef LOCATIONSERVICE_H
#define LOCATIONSERVICE_H

#include <QObject>
#include <QString>
class LocationService :  QObject
{
    Q_OBJECT

private:
    double *n_coords ;
    double *e_coords ;
    double *headings;
    int coordamount;
    int coordinate_iterator;

public:
    LocationService(QString path);

    void nextLocation(double &e_cord,double &y_cord,double &m_head);

};

#endif // LOCATIONSERVICE_H
