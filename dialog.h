#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QDebug>


#include "agent.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void on_spinBox_2_valueChanged(int arg1);

    void on_spinBox_valueChanged(int arg1);

    void on_wallGenButton_clicked();

    void on_spinBox_M_valueChanged(int arg1);

    void on_learnButton_clicked();

    void on_runButton_clicked();

    void on_refreshButton_clicked();

private:
    Ui::Dialog *ui;

    int worldSize;
    int m;
    int learningSteps;
    int M;

    void updateLearningSteps();
    void deleteWalls();
    void createItems();
    void setExit(int row, int column);

    void cleanWorld();

    QTableWidget *tab;

    Agent *agent;
};

#endif // DIALOG_H
