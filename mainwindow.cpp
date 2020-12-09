#include "mainwindow.h"
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QtCharts>
#include <QBarSet>
#include <QBarSeries>
#include <QChart>
#include <QBarCategoryAxis>
#include "graphmanager.h"
#include "graphwidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent) {
    setFixedSize(800,850);
    setWindowTitle(tr("Budgeted influence maximization"));

    graphManager = new GraphManager();
    pokecManager = new GraphManager();

    connect(demoButton, SIGNAL(clicked()), this, SLOT(on_demoButton_clicked()));
    connect(pokecButton, SIGNAL(clicked()), this, SLOT(on_pokecButton_clicked()));
    connect(pokecManager, SIGNAL(imageExpInf(double)), this, SLOT(graphManager_getImageExpectedInfluence(double)));
    connect(pokecManager, SIGNAL(imageTime(double)), this, SLOT(graphManager_getImageTime(double)));
    connect(pokecManager, SIGNAL(imageBrExpInf(double)), this, SLOT(graphManager_getImageBrExpectedInfluence(double)));
    connect(pokecManager, SIGNAL(imageBrTime(double)), this, SLOT(graphManager_getImageBrTime(double)));

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
    menuLayout->addWidget(pokecButton);
    menu->setLayout(menuLayout);

    defaultFont.setPointSize(16);
    defaultFont.setBold(true);

    demoButton->setFixedSize(QSize(250, 150));
    demoButton->setFont(defaultFont);
    pokecButton->setFixedSize(QSize(250, 150));
    pokecButton->setFont(defaultFont);


    imageTime = new QBarSet("IMAGE");
    imageBrTime = new QBarSet("IMAGE-BR");
    imageExpInf = new QBarSet("IMAGE");
    imageBrExpInf = new QBarSet("IMAGE-BR");

    *imageTime << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10;
    *imageBrTime << 10 << 9 << 8 << 7 << 6 << 5 << 4 << 3 << 2 << 1;
    *imageExpInf << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10;
    *imageBrExpInf << 10 << 9 << 8 << 7 << 6 << 5 << 4 << 3 << 2 << 1;

    setDemoPage();
    setPokecPage();
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_demoButton_clicked(){
    stackedLayout->setCurrentWidget(demoPage);
}

void MainWindow::on_pokecButton_clicked(){

    expectedInfluenceChart->setVisible(false);
    stepCountChart->setVisible(false);
    stackedLayout->setCurrentWidget(runAlg);
}

void MainWindow::on_backButton_clicked()
{
    stackedLayout->setCurrentWidget(menu);
}

void MainWindow::on_runDemo()
{
    graphManager->doImage(demoBudget, demoThreshold);
}

void MainWindow::on_runPokec()
{
    imageExpInf->remove(0,imageExpInf->count());
    imageTime->remove(0,imageTime->count());
    imageBrExpInf->remove(0,imageBrExpInf->count());
    imageBrTime->remove(0,imageBrTime->count());

    pokecManager->getPokecResults(pokecEdges,(float)pokecBudget, (float)pokecThreshold);
    createCharts();
    QChart *toDelete = chartViewInf->chart();
    chartViewInf->setChart(expectedInfluenceChart);
    chartViewInf->setRenderHint(QPainter::Antialiasing);
    delete toDelete;

    toDelete = chartViewTime->chart();
    chartViewTime->setChart(stepCountChart);
    chartViewTime->setRenderHint(QPainter::Antialiasing);
    delete toDelete;

    expectedInfluenceChart->setVisible(true);
    stepCountChart->setVisible(true);
}

void MainWindow::setPokecThresHold(double _threshold)
{
    pokecThreshold = _threshold;
}

void MainWindow::setDemoBudget(double _budget)
{
    demoBudget = _budget;
}

void MainWindow::setPokecBudget(double _budget)
{
    pokecBudget = _budget;
}

void MainWindow::setDemoThreshold(double _t)
{
    demoThreshold = _t;
}

void MainWindow::setPokecEdges(int e)
{
    pokecEdges = e;
}

void MainWindow::graphManager_getImageExpectedInfluence(double inf)
{
    imageExpInf->append(inf);
}

void MainWindow::graphManager_getImageTime(double t)
{
    imageTime->append(t);
}

void MainWindow::graphManager_getImageBrExpectedInfluence(double inf)
{
    imageBrExpInf->append(inf);
}

void MainWindow::graphManager_getImageBrTime(double t)
{
    imageBrTime->append(t);
}

void MainWindow::setDemoPage()
{
    QHBoxLayout *spinboxLayout = new QHBoxLayout();
    QHBoxLayout *inputLayout = new QHBoxLayout();
    demoLayout = new QVBoxLayout();

    QPushButton *backButton = new QPushButton(tr("Back"));
    backButton->setFixedSize(QSize(100, 50));
    backButton->setFont(QFont("Balvaria",14));
    connect(backButton, SIGNAL(clicked()), this, SLOT(on_backButton_clicked()));

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

    graphManager->getDemoGraph();
}

void MainWindow::setPokecPage()
{
    QPushButton *backButton = new QPushButton(tr("Back"));
    backButton->setFixedSize(QSize(100, 50));
    backButton->setFont(QFont("Balvaria",14));
    connect(backButton, SIGNAL(clicked()), this, SLOT(on_backButton_clicked()));

    QPushButton *runButton = new QPushButton(tr("Run"));
    runButton->setFixedSize(QSize(100, 50));
    runButton->setFont(QFont("Balvaria",14));
    connect(runButton, SIGNAL(clicked()), this, SLOT(on_runPokec()));

    pokecInfo = new QLabel();
    pokecInfo->setFont(QFont("Balvaria",14));
    pokecInfo->setText(QString::fromStdString("Vertex costs between 10 and 100"));

    QLabel *countLabel = new QLabel("Set number of edges (hundred): ");
    countLabel->setFont(QFont("Balvaria",14));
    QSpinBox *countInput = new QSpinBox();
    countInput->setRange(1,10);
    countInput->setSingleStep(1);
    countInput->setValue(10);
    setPokecBudget(countInput->value());
    countInput->setFixedHeight(30);
    countInput->setFont(QFont("Balvaria",14));
    connect(countInput, SIGNAL(valueChanged(int)), this, SLOT(setPokecEdges(int)));

    QLabel *budgetLabel = new QLabel("Set budget: ");
    budgetLabel->setFont(QFont("Balvaria",14));
    QDoubleSpinBox *budgetInputP = new QDoubleSpinBox();
    budgetInputP->setRange(0,10000.0);
    budgetInputP->setSingleStep(10);
    budgetInputP->setValue(50.0);
    setPokecBudget(budgetInputP->value());
    budgetInputP->setFixedHeight(30);
    budgetInputP->setFont(QFont("Balvaria",14));
    connect(budgetInputP, SIGNAL(valueChanged(double)), this, SLOT(setPokecBudget(double)));

    QLabel *thresholdLabel = new QLabel("Set threshold: ");
    thresholdLabel->setFont(QFont("Balvaria",14));
    QDoubleSpinBox *thresholdInput = new QDoubleSpinBox();
    thresholdInput->setRange(0,1.0);
    thresholdInput->setSingleStep(0.1);
    thresholdInput->setValue(0.05);
    setPokecThresHold(thresholdInput->value());
    thresholdInput->setFixedHeight(30);
    thresholdInput->setFont(QFont("Balvaria",14));
    connect(thresholdInput, SIGNAL(valueChanged(double)), this, SLOT(setPokecThresHold(double)));

    QHBoxLayout *backInfoLayout = new QHBoxLayout();
    backInfoLayout->addWidget(backButton);
    backInfoLayout->addSpacing(20);
    backInfoLayout->addWidget(pokecInfo);
    backInfoLayout->addSpacing(200);
    backInfoLayout->addWidget(runButton);
    QHBoxLayout *edgeCountLayout = new QHBoxLayout();
    edgeCountLayout->addSpacing(100);
    edgeCountLayout->addWidget(countLabel);
    edgeCountLayout->addWidget(countInput);
    edgeCountLayout->addSpacing(150);
    QHBoxLayout *budgetLayout = new QHBoxLayout();
    budgetLayout->addSpacing(100);
    budgetLayout->addWidget(budgetLabel);
    budgetLayout->addWidget(budgetInputP);
    budgetLayout->addSpacing(150);
    QHBoxLayout *thresholdLayout = new QHBoxLayout();
    thresholdLayout->addSpacing(100);
    thresholdLayout->addWidget(thresholdLabel);
    thresholdLayout->addWidget(thresholdInput);
    thresholdLayout->addSpacing(150);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(backInfoLayout);
    layout->addLayout(edgeCountLayout);
    layout->addLayout(budgetLayout);
    layout->addLayout(thresholdLayout);
    //layout->addSpacing(650);

    createCharts();

    chartViewInf = new QChartView(expectedInfluenceChart);
    chartViewInf->setRenderHint(QPainter::Antialiasing);
    chartViewTime = new QChartView(stepCountChart);
    chartViewTime->setRenderHint(QPainter::Antialiasing);
    QVBoxLayout *chartLayout = new QVBoxLayout();
    chartLayout->addWidget(chartViewInf);
    chartLayout->addWidget(chartViewTime);
    expectedInfluenceChart->setVisible(false);
    stepCountChart->setVisible(false);

    layout->addLayout(chartLayout);

    runAlg->setLayout(layout);
}


void MainWindow::createCharts()
{
    QStringList categories;
    for(int i = 0; i < pokecEdges; i++) {
        int x = (i+1)*100;
        //categories.append(QString::number(x));
    }
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    QBarCategoryAxis *axis2 = new QBarCategoryAxis();
    axis2->append(categories);

    QBarSeries *series = new QBarSeries();
    series->append(imageExpInf);
    series->append(imageBrExpInf);

    expectedInfluenceChart = new QChart();
    expectedInfluenceChart->addSeries(series);
    expectedInfluenceChart->setTitle("Expected Influence");
    expectedInfluenceChart->setAnimationOptions(QChart::SeriesAnimations);
    expectedInfluenceChart->createDefaultAxes();
    axis->setTitleText("Number of edges");
    expectedInfluenceChart->setAxisX(axis, series);
    expectedInfluenceChart->legend()->setVisible(true);
    expectedInfluenceChart->legend()->setAlignment(Qt::AlignBottom);

    QBarSeries *series2 = new QBarSeries();
    series2->append(imageTime);
    series2->append(imageBrTime);

    stepCountChart = new QChart();
    stepCountChart->addSeries(series2);
    stepCountChart->setTitle("Execution time");
    stepCountChart->setAnimationOptions(QChart::SeriesAnimations);
    stepCountChart->createDefaultAxes();
    axis2->setTitleText("Number of edges");
    stepCountChart->setAxisX(axis2, series2);
    stepCountChart->legend()->setVisible(true);
    stepCountChart->legend()->setAlignment(Qt::AlignBottom);
}


