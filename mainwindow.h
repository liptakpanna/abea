#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
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
    void on_algoButton_clicked();
    void on_backButton_clicked();
    void on_runDemo();
    void setDemoBudget(double _budget);
    void setDemoThreshold(double _t);

private:
    GraphManager *graphManager;

    QPushButton *demoButton = new QPushButton(tr("Demo"));
    QPushButton *algoButton = new QPushButton(tr("Run algorithm \n with \n custom parameters"));
    QPushButton *backButton = new QPushButton(tr("Back"));

    QHBoxLayout *menuLayout;
    QVBoxLayout *demoLayout, *mainLayout;
    QFont defaultFont;

    QWidget *demoPage, *menu, *runAlg;
    QStackedLayout *stackedLayout;

    float demoBudget;
    double demoThreshold;

    void setDemoPage();
};
#endif // MAINWINDOW_H
