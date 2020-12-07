#include "mainwindow.h"
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "graphmanager.h"
#include "graphwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {
    setFixedSize(800,850);
    setWindowTitle(tr("Budgeted influence maximization"));

    graphManager = new GraphManager();

    connect(demoButton, SIGNAL(clicked()), this, SLOT(on_demoButton_clicked()));
    connect(algoButton, SIGNAL(clicked()), this, SLOT(on_algoButton_clicked()));
    connect(backButton, SIGNAL(clicked()), this, SLOT(on_backButton_clicked()));

    demoPage = new QWidget();
    menu = new QWidget();
    runAlg = new QWidget();

    stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(menu);
    stackedLayout->addWidget(demoPage);
    stackedLayout->addWidget(runAlg);
    stackedLayout->setCurrentWidget(menu);

    mainLayout = new QVBoxLayout();
    mainLayout->addLayout(stackedLayout);
    setLayout(mainLayout);

    menuLayout = new QHBoxLayout();
    menuLayout->addWidget(demoButton);
    menuLayout->addWidget(algoButton);
    menu->setLayout(menuLayout);

    defaultFont.setPointSize(16);
    defaultFont.setBold(true);

    demoButton->setFixedSize(QSize(250, 150));
    demoButton->setFont(defaultFont);
    algoButton->setFixedSize(QSize(250, 150));
    algoButton->setFont(defaultFont);
    backButton->setFixedSize(QSize(100, 50));
    backButton->setFont(QFont("Balvaria",14));

    setDemoPage();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_demoButton_clicked(){
    stackedLayout->setCurrentWidget(demoPage);
    graphManager->getDemoGraph();
}

void MainWindow::on_algoButton_clicked(){

}

void MainWindow::on_backButton_clicked()
{
    stackedLayout->setCurrentWidget(menu);
}

void MainWindow::on_runDemo()
{
    graphManager->doImage(demoBudget, demoThreshold);
}

void MainWindow::setDemoBudget(double _budget)
{
    demoBudget = _budget;
}

void MainWindow::setDemoThreshold(double _t)
{
    demoThreshold = _t;
}

void MainWindow::setDemoPage()
{
    QHBoxLayout *spinboxLayout = new QHBoxLayout();
    QHBoxLayout *inputLayout = new QHBoxLayout();
    demoLayout = new QVBoxLayout();

    GraphWidget *widget = new GraphWidget(this, graphManager);
    QPushButton *runButton = new QPushButton(tr("Run IMAGE-BR"));
    runButton->setFixedSize(QSize(200, 50));
    runButton->setFont(QFont("Balvaria",14));
    connect(runButton, SIGNAL(clicked()), this, SLOT(on_runDemo()));

    QLabel *budgetLabel = new QLabel("Set budget: ");
    budgetLabel->setFont(QFont("Balvaria",14));
    QDoubleSpinBox *budgetInput = new QDoubleSpinBox();
    budgetInput->setRange(0,100.0);
    budgetInput->setSingleStep(10);
    budgetInput->setValue(50);
    setDemoBudget(budgetInput->value());
    budgetInput->setFixedHeight(30);
    budgetInput->setFont(QFont("Balvaria",14));
    connect(budgetInput, SIGNAL(valueChanged(double)), this, SLOT(setDemoBudget(double)));

    QLabel *thresholdLabel = new QLabel("Set threshold: ");
    thresholdLabel->setFont(QFont("Balvaria",14));
    QDoubleSpinBox *thresholdInput = new QDoubleSpinBox();
    thresholdInput->setRange(0,1.0);
    thresholdInput->setSingleStep(0.1);
    thresholdInput->setValue(0.05);
    setDemoThreshold(thresholdInput->value());
    thresholdInput->setFixedHeight(30);
    thresholdInput->setFont(QFont("Balvaria",14));
    connect(thresholdInput, SIGNAL(valueChanged(double)), this, SLOT(setDemoThreshold(double)));

    spinboxLayout->addWidget(budgetLabel);
    spinboxLayout->addWidget(budgetInput);
    spinboxLayout->setAlignment(Qt::AlignLeft);
    spinboxLayout->addSpacing(20);

    spinboxLayout->addWidget(thresholdLabel);
    spinboxLayout->addWidget(thresholdInput);
    spinboxLayout->setAlignment(Qt::AlignLeft);

    inputLayout->addWidget(backButton);
    inputLayout->addSpacing(10);
    inputLayout->addLayout(spinboxLayout);
    inputLayout->addWidget(runButton);
    demoLayout->addLayout(inputLayout);
    demoLayout->addWidget(widget);

    demoPage->setLayout(demoLayout);
}


