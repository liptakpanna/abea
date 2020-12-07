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
    void setDemoBudget(float _budget);
    void setPokecBudget(float _budget);
    void setPokecThresHold(float _threshold);
    void setDemoThreshold(double _t);


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
    QBarSet *imageExpInf, *imageBrExpInf, *imageStep, *imageBrStep;

    float demoBudget, pokecBudget, pokecThreshold;
    double demoThreshold;

    void setDemoPage();
    void setPokecPage();
    void showPokecImportDialog();
    void createCharts();
};
#endif // MAINWINDOW_H
