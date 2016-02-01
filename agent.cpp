#include "agent.h"

Agent::Agent(QObject *parent, QTableWidget *tab, uint maxSteps) :
    QObject(parent)
{
    maxSteps_ = maxSteps;
    stepsLimit = maxSteps;
    stepNo = 0;
    tab_ = tab;

    timer.setParent(this);
    timer.setInterval(5);

    dtimer.setParent(this);
    dtimer.setInterval(500);

    connect(&timer, SIGNAL(timeout()), this, SLOT(nextStep()));
    connect(&dtimer, SIGNAL(timeout()), this, SLOT(nextDemoStep()));
}

void Agent::nextStep()
{
    qDebug()<< stepNo <<"step. Limit "<<maxSteps_ << "x="<<lPos.x << "y="<<lPos.y;
    tab_->item(lPos.y,lPos.x)->setBackgroundColor(Qt::white);//zeruje kolor
    maxSteps_--;
    stepNo++;

    //nastepna pozycja
    position nPos = nextMove(lPos);//

    updateValue(lPos, nPos);

    if(tab_->item(nPos.y,nPos.x)->backgroundColor() == Qt::green)
    {
        qDebug()<<"znalazl wyjscie";
        stop();
        return;
    }

    tab_->item(nPos.y,nPos.x)->setBackgroundColor(Qt::yellow);
    lPos = nPos;

    if(maxSteps_ > 1)
    {
        timer.start();
    }else{
        stop();
        qDebug()<<"limit krokow";
        return;
    }
}

position Agent::nextMove(position pos)
{
   position nextPos = Nav::randomMove(pos);

   while(1)///bedzie szukal az znajdzie nastepne dobry spot
   {
   if (tab_->item(nextPos.y,nextPos.x) != 0 && tab_->item(nextPos.y,nextPos.x)->backgroundColor() != Qt::gray)
    return nextPos;

   nextPos = Nav::randomMove(pos);
   }

}

void Agent::stop()
{
    timer.stop();
    tab_->item(lPos.y,lPos.x)->setBackgroundColor(Qt::white);//zeruje kolor

    repeats_--;
    if(repeats_>0)
        start(stepsLimit,repeats_);
}

void Agent::start(uint maxSteps, uint repeats)
{
    reset();
    stepNo = 0;

    maxSteps_ = maxSteps;
    repeats_ = repeats;

    QList<QTableWidgetItem*> selections = tab_->selectedItems();

    if (selections.count() == 0)
    {
        sPos = setStart();
        tab_->setCurrentCell(sPos.y, sPos.x,QItemSelectionModel::Select);
    }
    else{
        sPos.x = selections.at(0)->column();
        sPos.y = selections.at(0)->row();
    };
    qDebug()<< "agent start at position: " <<sPos.x<<sPos.y;

    lPos = sPos;
    tab_->item(lPos.y,lPos.x)->setBackgroundColor(Qt::blue);

    timer.start();
}

void Agent::reset()
{
    itemValue v;
    v.value=0;
    v.numerator=0;
    v.divisor=1; //wstepna wartosc to pierwsza wartosc

    v.dval = 0;
    v.p.clear();

    int rowCount = tab_->rowCount();

    for(int i = 0; i<rowCount;i++)
    {
        for(int j =0; j<rowCount;j++)
        {
            arr[i][j].divisor = 1;
            arr[i][j].numerator = 0;
            arr[i][j].p.clear();
            arr[i][j].value = 0;
            if(tab_->item(i,j)->text().toInt() > 0)
            {
                arr[i][j].value = tab_->item(i,j)->text().toInt();
                arr[i][j].dval = tab_->item(i,j)->text().toInt();/// i tak przepisuje tylko pelne setki
            };
        }
    }

}

void Agent::updateValue(position lastPos, position newPos)
{
    itemValue it;// next item
    itemValue lit;//last item
    ///arr podreczna tablica wartosci
    ///tab_ tutaj wpisujemy wynik

    it = arr[newPos.y][newPos.x];// wartosc z nowego pola
    lit = arr[lastPos.y][lastPos.x]; // wartosc ze starego pola
    ///SMA simple moving average
//        int X = it.value;
//        //wartosc do starego pola
//        it = arr[lastPos.y][lastPos.x];
//        it.numerator += X;
//        it.divisor ++;
//        X = it.value = it.numerator / it.divisor;
//        tab_->item(lastPos.y,lastPos.x)->setText(QString::number(X));

    ///WMA  weighted moving average
        double X = arr[newPos.y][newPos.x].dval * 0.99;          //bierzemy wartosc z nowego pola
        double num = 0;// podreczny licznik i mianownik
        double div = 0;

        lit.p.append(X);
        lit.n.append(stepNo);
        int c = lit.p.count();    // ile elementow w kontenerach
        for(int i = 0; i < c; i++)
        {
            num = num + lit.p.at(i)/lit.n.at(i);        ///sumowanie licznika
            div = div + 1/lit.n.at(i);                  ///sumowanie mianownika
            qDebug()<< "               i:" << i << "num" << num << "div" << div;
        };


        lit.dval = X = num / div; // nowa wartosc
        qDebug()<<"                                                        X"<<X;

        arr[lastPos.y][lastPos.x] = lit; // item spowrotem do starego w tablicy
        tab_->item(lastPos.y,lastPos.x)->setText(QString::number(X));

}

position Agent::setStart()
{
    position p;
    ///srodek planszy
    p.x = tab_->rowCount()/2;
    p.y = tab_->columnCount()/2;

    for (int i= 0; i < 20; i++)
    {
        if(tab_->item(p.y,p.x)->backgroundColor() !=Qt::white) //sprawdza czy nie ma sciany
        {
            p = Nav::randomMove(p);
        }else
            return p;
    }
    return p;
}

void Agent::startDemo()
{
//    sPos
    tab_->item(sPos.y,sPos.x)->setBackgroundColor(Qt::blue);
    lPos = sPos;
    dtimer.start();
}

void Agent::nextDemoStep()
{
    ///tab_->item(lPos.y,lPos.x)->setBackgroundColor(Qt::white);//zeruje kolor///nie zerujemy koloru, zaznacza tam gdzie byl

    if(tab_->item(lPos.y,lPos.x)->text().toInt() == 100)// jezeli koniec
    {
        tab_->item(lPos.y,lPos.x)->setBackgroundColor(Qt::yellow);
        qDebug()<<"koniec ";
        dtimer.stop();
        return;
    }

     ///wybierz najlepsze miejsce dookoła
    position nextPos, temp;
    double bestValue = 0;

    temp.y = lPos.y - 1;
    temp.x = lPos.x;
    if(tab_->item(temp.y,temp.x) != 0 && tab_->item(temp.y,temp.x)->text().toDouble() > bestValue && tab_->item(temp.y,temp.x)->backgroundColor() != Qt::blue){
        bestValue = tab_->item(temp.y,temp.x)->text().toDouble();
        nextPos = temp;
    };
    temp.x = lPos.x + 1;
    temp.y = lPos.y;
    if(tab_->item(temp.y,temp.x) != 0 && tab_->item(temp.y,temp.x)->text().toDouble() > bestValue && tab_->item(temp.y,temp.x)->backgroundColor() != Qt::blue){
        bestValue = tab_->item(temp.y,temp.x)->text().toDouble();
        nextPos = temp;
    };
    temp.y = lPos.y + 1;
    temp.x = lPos.x;
    if(tab_->item(temp.y,temp.x) != 0 && tab_->item(temp.y,temp.x)->text().toDouble() > bestValue && tab_->item(temp.y,temp.x)->backgroundColor() != Qt::blue){
        bestValue = tab_->item(temp.y,temp.x)->text().toDouble();
        nextPos = temp;
    };
    temp.x = lPos.x - 1;
    temp.y = lPos.y;
    if(tab_->item(temp.y,temp.x) != 0 && tab_->item(temp.y,temp.x)->text().toDouble() > bestValue && tab_->item(temp.y,temp.x)->backgroundColor() != Qt::blue){
        bestValue = tab_->item(temp.y,temp.x)->text().toDouble();
        nextPos = temp;
    };

    if(bestValue == 0)
        nextPos = nextMove(lPos); // zrobi losowy krok jezeli same zera dookola


    if((lPos.x == nextPos.x) && (lPos.y == nextPos.y))
    {
        qDebug()<<"stoimy ";
        tab_->item(lPos.y,lPos.x)->setBackgroundColor(Qt::red);
        dtimer.stop();
    }else{
        qDebug()<<"best move ";
        lPos = nextPos;
        tab_->item(nextPos.y,nextPos.x)->setBackgroundColor(Qt::blue);
        dtimer.start();
    }


}




