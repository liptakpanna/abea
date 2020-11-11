#include "mainwindow.h"
#include <QLabel>
#include <QInputDialog>
#include "graphmanager.h"
#include "graphwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {
    setFixedSize(800,850);
    setWindowTitle(tr("Budgeted influence maximization"));

    graphManager = new GraphManager();

    connect(graphManager, SIGNAL(printVertex(std::string, std::string)), this, SLOT(graphManager_PrintVertex(std::string, std::string)));

    connect(demoButton, SIGNAL(clicked()), this, SLOT(on_demoButton_clicked()));
    connect(algoButton, SIGNAL(clicked()), this, SLOT(on_algoButton_clicked()));

    mainLayout = new QHBoxLayout();
    mainLayout->addWidget(demoButton);
    mainLayout->addWidget(algoButton);

    setLayout(mainLayout);
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_demoButton_clicked(){
    mainLayout->removeWidget(demoButton);
    mainLayout->removeWidget(algoButton);

    QPushButton *backButton = new QPushButton(tr("Back"));
    QLabel *budgetLabel = new QLabel("Set budget: ");
    QInputDialog *budgetInput = new QInputDialog();
    budgetInput->setInputMode(QInputDialog::DoubleInput);
    budgetInput->setDoubleRange(0,100.0);
    QVBoxLayout *settingsLayout = new QVBoxLayout();
    settingsLayout->addWidget(budgetLabel);
    settingsLayout->addWidget(budgetInput);
    settingsLayout->addWidget(backButton);
    GraphWidget *widget = new GraphWidget(this, graphManager);
    QVBoxLayout *wholeLayout = new QVBoxLayout();
    wholeLayout->addLayout(settingsLayout);
    wholeLayout->addWidget(widget);
    mainLayout->addLayout(wholeLayout);
    graphManager->getDemoGraph();
}

void MainWindow::on_algoButton_clicked(){

}

void MainWindow::graphManager_PrintVertex(std::string label, std::string cost)
{
    std::string tmp = "\n "  + label + " : " + cost + "\n";
    demoLabel->setText( demoLabel->text() + QString::fromStdString(tmp) );
}

