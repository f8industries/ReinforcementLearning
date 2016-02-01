#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    tab = ui->tableWidget;

    tab->verticalHeader()->hide();
    tab->horizontalHeader()->hide();

    m = 20;
    worldSize = 8;
    on_spinBox_valueChanged(ui->spinBox->value());

    on_spinBox_M_valueChanged(ui->spinBox_M->value());

    on_spinBox_2_valueChanged(ui->spinBox_2->value());

    updateLearningSteps();

    agent = new Agent(this, ui->tableWidget, learningSteps);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::updateLearningSteps()
{
    learningSteps = ui->spinBox->value() * ui->spinBox_2->value();
    ui->label_3->setText(QString::number(learningSteps));
}

void Dialog::on_spinBox_2_valueChanged(int arg1)
{
    m = arg1;
    updateLearningSteps();
}

///zmiana rozmiaru swiata
void Dialog::on_spinBox_valueChanged(int arg1)
{
    worldSize = arg1;
    qDebug()<< "world size changed to: " << worldSize;

    ui->tableWidget->setColumnCount(worldSize);
    ui->tableWidget->setRowCount(worldSize);

    ui->tableWidget->setAutoFillBackground(true);
    QSize tableSize, cellSize;

    tableSize = ui->tableWidget->size();
    cellSize.setHeight(tableSize.height() / worldSize -1);
    cellSize.setWidth(tableSize.width() / worldSize -1);


    qDebug()<< tableSize.width() << "tableSize " << tableSize.height();
    qDebug()<< cellSize.width() << "cellSize " << cellSize.height();

    for(int i=0; i < worldSize ;i++)
    {
        ui->tableWidget->setRowHeight(i,cellSize.height());
        ui->tableWidget->setColumnWidth(i,cellSize.width());
    }

    createItems();

    updateLearningSteps();
}

///generowanie scian na planszy
void Dialog::on_wallGenButton_clicked()
{
    int prob;
    int up,down,right,left;
    int ws = worldSize;
    prob = ui->horizontalSlider->value();
    deleteWalls();


    //qDebug()<< "prob value"<< prob << "qrand" << qrand() % 100;
    ///wall generation
    for(int y =0;y < worldSize; y++){
        for(int x =0;x < worldSize; x++){

            y-1<0 ? up=y :up=y-1;
            y+1>ws ? down=y : down=y+1;
            x+1>ws ? right=x : right=x+1;
            x-1<0 ? left=x :left=x-1;
            //qDebug()<<"up down right left"<< up<<down<<right<<left << "cords: " << x << y;

            if((qrand() % 100) < prob){
                if(tab->item(up,x)->backgroundColor() != Qt::gray &&
                   tab->item(y,left)->backgroundColor() != Qt::gray)
                tab->item(y,x)->setBackgroundColor(Qt::gray);
            };
        };
    };

    ///exits generation
    int egdeItems = 4*worldSize -4;
    int temp;
    QVector<int> exits;
    //qDebug()<< "egditems: "<<egdeItems<< "exits"<< M;

    for(int q=1; q <= M; q++)//////rozdanie wyjsc równomiernie wokoło klatki
    {
        temp = q * egdeItems/M;
        //qDebug()<<"temp"<<temp;
        exits.append(temp);
    };
    temp=0;

    for(int pg=0; pg < worldSize;pg++)///gorna krawedz w prawo//////przebieg po każdej z 4 krawędzi
    {
        temp++;
        if(temp == exits.at(0))
        {
            setExit(0,pg);
            exits.pop_front();
        }
    };
    for(int dp=1; dp < worldSize-1; dp++)
    {
        temp++;
        if(temp == exits.at(0))
        {
            setExit(dp,worldSize-1);
            exits.pop_front();
        }
    };
    for(int ld=9; ld > -1; ld--)
    {
        temp++;
        if(temp == exits.at(0))
        {
            setExit(worldSize-1,ld);
            exits.pop_front();
        }
    };
    for(int gl=8; gl > 0; gl--)
    {
        temp++;
        if(temp == exits.at(0))
        {
            setExit(gl,0);
            exits.pop_front();
        }
    };
}

/// wyczyszczenie swiata
/// 1. usuniecie sciana
/// 2. wyzerowanie textow // TODO
void Dialog::deleteWalls()
{
    for(int i =0;i < worldSize; i++){
        for(int j =0;j < worldSize; j++){
                ui->tableWidget->item(i,j)->setBackgroundColor(Qt::white);
                ui->tableWidget->item(i,j)->setText("");
        };
    };

    agent->reset();
}

void Dialog::setExit(int row, int column)
{
    tab->item(row,column)->setBackgroundColor(Qt::green);
    tab->item(row,column)->setText("100");
    tab->item(row,column)->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
}

///alokacja itemow w tabeli
void Dialog::createItems()
{
    for(int i =0;i < worldSize; i++){
        for(int j =0;j < worldSize; j++){

                ui->tableWidget->setItem(i, j, new QTableWidgetItem);
        };
    };
}

void Dialog::on_spinBox_M_valueChanged(int arg1)
{
    M = arg1;
}

///proces uczenia sie
void Dialog::on_learnButton_clicked()
{
    uint reps = ui->spinBox_repeats->value();
    qDebug ()<<"starting "<< reps << "learning runs";
    agent->start(learningSteps,reps);
}

///proces poruszania sie po "wyuczonym" swiecie
void Dialog::on_runButton_clicked()
{
    agent->startDemo();
    qDebug()<<"start demo";
}

/// wyczyszczenie swiata
void Dialog::on_refreshButton_clicked()
{
    deleteWalls();
}


