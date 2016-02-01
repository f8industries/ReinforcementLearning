#ifndef AGENT_H
#define AGENT_H

#include <QObject>
#include <QTimer>
#include <QTableWidget>
#include <QDebug>
#include <QVector>
#include <array>


#include "nav.h"
#include "Helpers.h"

class Agent : public QObject
{
    Q_OBJECT
public:
    Agent(QObject *parent, QTableWidget *tab, uint maxSteps);

signals:

public slots:
    void start(uint maxSteps, uint repeats);
    void startDemo();
    void reset();
    void stop();

private slots:
    void nextStep();
    void nextDemoStep();

private:
    QTimer timer;
    QTimer dtimer;//demo timer
    uint maxSteps_;
    uint stepsLimit;
    int stepNo;
    QTableWidget *tab_;
    uint repeats_;

    position sPos;//start
    position lPos;//last position
    position aPos;//actual

    QTableWidgetItem *prevItem;

    position setStart();
    position nextMove(position pos);
    void updateValue(position lastPos, position newPos);
    //std::array<std::array<itemValue,100>,100> arr;
    itemValue arr[100][100];

};

#endif // AGENT_H
