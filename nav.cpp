#include "nav.h"

/// losowy wybor jednego z 4 kierunkow
position Nav::randomMove(position p)
{
    int dir = qrand() % 4;
    switch (dir) {
    case 0:
        p.x --;//lewo
        qDebug()<<"left";
        break;
    case 1:
        p.y --;//gora
        qDebug()<<"up";
        break;
    case 2:
        p.x ++;//prawo
        qDebug()<<"right";
        break;
    case 3:
        p.y ++;//dol
        qDebug()<<"down";
        break;
    default:
        break;
    };
    return p;
}
