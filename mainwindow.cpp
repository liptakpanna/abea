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
    graphManager->doBudgetedMaxCover(demoBudget, demoRRsetSize);
}

void MainWindow::setDemoBudget(double _budget)
{
    demoBudget = _budget;
}

void MainWindow::setDemoRRSetSize(int _size)
{
    demoRRsetSize = _size;
}

void MainWindow::setDemoPage()
{
    QHBoxLayout *spinboxLayout = new QHBoxLayout();
    QHBoxLayout *inputLayout = new QHBoxLayout();
    demoLayout = new QVBoxLayout();

    GraphWidget *widget = new GraphWidget(this, graphManager);
    QPushButton *runButton = new QPushButton(tr("Run demo"));
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

    QLabel *RRsetLabel = new QLabel("Set RR set size: ");
    RRsetLabel->setFont(QFont("Balvaria",14));
    QSpinBox *RRsetInput = new QSpinBox();
    RRsetInput->setRange(1,100);
    RRsetInput->setSingleStep(10);
    RRsetInput->setValue(20);
    setDemoRRSetSize(RRsetInput->value());
    RRsetInput->setFixedHeight(30);
    RRsetInput->setFont(QFont("Balvaria",14));
    connect(RRsetInput, SIGNAL(valueChanged(int)), this, SLOT(setDemoRRSetSize(int)));

    spinboxLayout->addWidget(budgetLabel);
    spinboxLayout->addWidget(budgetInput);
    spinboxLayout->setAlignment(Qt::AlignLeft);
    spinboxLayout->addSpacing(20);

    spinboxLayout->addWidget(RRsetLabel);
    spinboxLayout->addWidget(RRsetInput);
    spinboxLayout->setAlignment(Qt::AlignLeft);

    inputLayout->addWidget(backButton);
    inputLayout->addSpacing(10);
    inputLayout->addLayout(spinboxLayout);
    inputLayout->addWidget(runButton);
    demoLayout->addLayout(inputLayout);
    demoLayout->addWidget(widget);

    demoPage->setLayout(demoLayout);
}


