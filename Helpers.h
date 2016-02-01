#ifndef HELPERS_H
#define HELPERS_H

#include <qvector.h>

struct position{
    int x;          //column
    int y;          //row
};

struct itemValue{
    ///prosta srednia wazona
    int value;      //wartosc pola
    int numerator;  //licznik
    int divisor;    //dzielnik,mianownik

    ///wazona srednia kroczaca
    double dval;
    QVector<double> p; //kontener wartosci
    QVector<double> n;

    ///wlasny

};

#endif // HELPERS_H
