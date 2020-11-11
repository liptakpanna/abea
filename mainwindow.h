#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include "graphmanager.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_demoButton_clicked();
    void on_algoButton_clicked();
    void graphManager_PrintVertex(std::string label, std::string cost);

private:
    GraphManager *graphManager;

    QPushButton *demoButton = new QPushButton(tr("Demo"));
    QPushButton *algoButton = new QPushButton(tr("Run algorithm with custom parameters"));
    QLabel *demoLabel;

    QHBoxLayout* mainLayout;
};
#endif // MAINWINDOW_H
