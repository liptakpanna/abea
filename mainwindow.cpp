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

    //showPokecImportDialog();
    expectedInfluenceChart->setVisible(false);
    stepCountChart->setVisible(false);
    stackedLayout->setCurrentWidget(runAlg);
}

void MainWindow::showPokecImportDialog(){
    QDialog * d = new QDialog();
    QVBoxLayout * vbox = new QVBoxLayout();

    QLabel *edgeLabel = new QLabel("Set thousands edges to import ");
    edgeLabel->setFont(QFont("Balvaria",14));
    QSpinBox *edgeInput = new QSpinBox();
    edgeInput->setRange(1,10);
    edgeInput->setSingleStep(1);
    edgeInput->setValue(5);
    edgeInput->setFixedHeight(30);
    edgeInput->setFont(QFont("Balvaria",14));

   /*QLabel *costLabel = new QLabel("Set min/max edge cost");
    costLabel->setFont(QFont("Balvaria",14));
    QSpinBox *costMinInput = new QSpinBox();
    costMinInput->setRange(1,500);
    costMinInput->setSingleStep(1);
    costMinInput->setValue(10);
    costMinInput->setFixedHeight(30);
    costMinInput->setFont(QFont("Balvaria",14));
    QSpinBox *costMaxInput = new QSpinBox();
    costMaxInput->setRange(1,500);
    costMaxInput->setSingleStep(1);
    costMaxInput->setValue(100);
    costMaxInput->setFixedHeight(30);
    costMaxInput->setFont(QFont("Balvaria",14));*/

    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                        | QDialogButtonBox::Cancel);

    QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));

    vbox->addWidget(edgeLabel);
    vbox->addWidget(edgeInput);
    vbox->addWidget(buttonBox);
    /*vbox->addWidget(costLabel);
    vbox->addWidget(costMinInput);
    vbox->addWidget(costMaxInput);*/

    d->setLayout(vbox);

    int result = d->exec();
    if(result == QDialog::Accepted)
    {
        pokecManager->getPokecGraph(edgeInput->value());
    }
}

void MainWindow::on_backButton_clicked()
{
    stackedLayout->setCurrentWidget(menu);
}

void MainWindow::on_runDemo()
{
    graphManager->doBudgetedMaxCover(demoBudget, demoRRsetSize);
}

void MainWindow::on_runPokec()
{
    expectedInfluenceChart->setVisible(true);
    stepCountChart->setVisible(true);
   // pokecManager->getPokecResults(pokecBudget, pokecThreshold);
}

void MainWindow::setPokecThresHold(float _threshold)
{
    pokecThreshold = _threshold;
}

void MainWindow::setDemoBudget(float _budget)
{
    demoBudget = _budget;
}

void MainWindow::setPokecBudget(float _budget)
{
    pokecBudget = _budget;
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

    QPushButton *backButton = new QPushButton(tr("Back"));
    backButton->setFixedSize(QSize(100, 50));
    backButton->setFont(QFont("Balvaria",14));
    connect(backButton, SIGNAL(clicked()), this, SLOT(on_backButton_clicked()));

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

    QLabel *budgetLabel = new QLabel("Set budget: ");
    budgetLabel->setFont(QFont("Balvaria",14));
    QDoubleSpinBox *budgetInputP = new QDoubleSpinBox();
    budgetInputP->setRange(0,100.0);
    budgetInputP->setSingleStep(10);
    budgetInputP->setValue(50);
    setDemoBudget(budgetInputP->value());
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
    connect(budgetInputP, SIGNAL(valueChanged(double)), this, SLOT(setPokecThresHold(double)));

    QHBoxLayout *backInfoLayout = new QHBoxLayout();
    backInfoLayout->addWidget(backButton);
    backInfoLayout->addSpacing(20);
    backInfoLayout->addWidget(pokecInfo);
    backInfoLayout->addSpacing(200);
    backInfoLayout->addWidget(runButton);
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
    layout->addLayout(budgetLayout);
    layout->addLayout(thresholdLayout);
    //layout->addSpacing(650);

    createCharts();

    QChartView *chartView = new QChartView(expectedInfluenceChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QChartView *chartView2 = new QChartView(stepCountChart);
    chartView->setRenderHint(QPainter::Antialiasing);
    QVBoxLayout *chartLayout = new QVBoxLayout();
    chartLayout->addWidget(chartView);
    chartLayout->addWidget(chartView2);
    expectedInfluenceChart->setVisible(false);
    stepCountChart->setVisible(false);

    layout->addLayout(chartLayout);

    runAlg->setLayout(layout);
}


void MainWindow::createCharts()
{
    QStringList categories;
    categories << "1000" << "2000" << "3000" << "4000" << "5000" << "6000" << "7000" << "8000" << "9000" << "10000";
    QBarCategoryAxis *axis = new QBarCategoryAxis();
    axis->append(categories);
    QBarCategoryAxis *axis2 = new QBarCategoryAxis();
    axis2->append(categories);

    imageExpInf = new QBarSet("IMAGE");
    imageBrExpInf = new QBarSet("IMAGE-BR");

    *imageExpInf << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10;
    *imageBrExpInf << 10 << 9 << 8 << 7 << 6 << 5 << 4 << 3 << 2 << 1;

    QBarSeries *series = new QBarSeries();
    series->append(imageExpInf);
    series->append(imageBrExpInf);

    expectedInfluenceChart = new QChart();
    expectedInfluenceChart->addSeries(series);
    expectedInfluenceChart->setTitle("Expected Influence");
    expectedInfluenceChart->setAnimationOptions(QChart::SeriesAnimations);
    expectedInfluenceChart->createDefaultAxes();
    axis->setTitleText("Number of nodes");
    expectedInfluenceChart->setAxisX(axis, series);
    expectedInfluenceChart->legend()->setVisible(true);
    expectedInfluenceChart->legend()->setAlignment(Qt::AlignBottom);

    imageStep = new QBarSet("IMAGE");
    imageBrStep = new QBarSet("IMAGE-BR");

    *imageStep << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8 << 9 << 10;
    *imageBrStep << 10 << 9 << 8 << 7 << 6 << 5 << 4 << 3 << 2 << 1;

    QBarSeries *series2 = new QBarSeries();
    series2->append(imageStep);
    series2->append(imageBrStep);

    stepCountChart = new QChart();
    stepCountChart->addSeries(series2);
    stepCountChart->setTitle("Step count");
    stepCountChart->setAnimationOptions(QChart::SeriesAnimations);
    stepCountChart->createDefaultAxes();
    axis2->setTitleText("Number of nodes");
    stepCountChart->setAxisX(axis2, series2);
    stepCountChart->legend()->setVisible(true);
    stepCountChart->legend()->setAlignment(Qt::AlignBottom);

}


