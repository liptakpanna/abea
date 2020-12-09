#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QtCharts>
#include "graphmanager.h"
#include <QStackedLayout>

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_demoButton_clicked();
    void on_pokecButton_clicked();
    void on_backButton_clicked();
    void on_runDemo();
    void on_runPokec();
    void setDemoBudget(double _budget);
    void setPokecBudget(double _budget);
    void setPokecThresHold(double _threshold);
    void setDemoThreshold(double _t);
    void setPokecEdges(int e);
    void graphManager_getImageExpectedInfluence(double inf);
    void graphManager_getImageTime(double t);
    void graphManager_getImageBrExpectedInfluence(double inf);
    void graphManager_getImageBrTime(double t);


private:
    GraphManager *graphManager, *pokecManager;

    QPushButton *demoButton = new QPushButton(tr("Demo"));
    QPushButton *pokecButton = new QPushButton(tr("Run algorithm \n on Pokec"));

    QHBoxLayout *menuLayout;
    QVBoxLayout *demoLayout, *mainLayout;
    QFont defaultFont;

    QWidget *demoPage, *menu, *runAlg;
    QStackedLayout *stackedLayout;
    QLabel *pokecInfo;

    QChart *stepCountChart, *expectedInfluenceChart;
    QBarSet *imageExpInf, *imageBrExpInf, *imageTime, *imageBrTime;
    QChartView *chartViewInf, *chartViewTime;

    double demoBudget, pokecBudget, pokecThreshold,demoThreshold;
    int pokecEdges;

    void setDemoPage();
    void setPokecPage();
    void createCharts();
};
#endif // MAINWINDOW_H
