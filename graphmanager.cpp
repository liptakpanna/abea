#include "graphmanager.h"
#include <QTemporaryDir>

GraphManager::GraphManager(QObject *parent) : QObject(parent)
{

}

void GraphManager::getDemoGraph()
{
    QTemporaryDir tempDir;
    if (tempDir.isValid()) {
        const QString tempFile = tempDir.path() + "/demo_graph.txt";
          if (QFile::copy(":demo_graph.txt", tempFile)) {
              g = new Graph(tempFile.toStdString());
              for(Vertex *v : g->getVertices()) {
                  //emit(printVertex(v->getLabel(), std::to_string(v->getCost())));
                  emit(addVertex(v->getLabel(), v->getCost()));
              }
              for(Edge *e : g->getEdges())
                  emit(addEdge(e->getSource()->getLabel(), e->getDestination()->getLabel(),e->getPropagationProbability()));
          }
    }

    tempDir.remove();
}

void GraphManager::doBudgetedMaxCover(float budget, int RRLength)
{
    emit(setNodesToDefault());
    std::vector<Vertex*> result = g->budgetedMaxCoverage(budget, RRLength);
    for(Vertex *v : result) {
        emit(paintVertex(v->getLabel()));
    }
}

void GraphManager::getPokecGraph(int edgeCount, bool first)
{
    if(first)
        g = new Graph();

    QTemporaryDir tempDir;
    if (tempDir.isValid()) {
        const QString tempFile = tempDir.path() + QString::fromStdString("/pokec_"+std::to_string(edgeCount))+".txt";
          if (QFile::copy(QString::fromStdString(":pokec_"+std::to_string(edgeCount))+".txt", tempFile)) {
              g->loadPokec(tempFile.toStdString(), 10, 100);
          }
    }
    tempDir.remove();
}

int GraphManager::getVertexCount()
{
    return g->getVertices().size();
}

int GraphManager::getEdgeCount()
{
    return g->getEdges().size();
}

void GraphManager::getPokecResults(int edges ,float budget, float threshold)
{
    double sum1 = 0, sum2 = 0;
    for(int i = 1; i <= edges; i++) {
        getPokecGraph(i, i == 1);
        for(int j = 0; j < 5; j++) {
            Graph::stat sImage = g->getImageStat(budget, threshold);
            sum1 += sImage.expectedInf;
            sum2 += sImage.time;
        }
        std::cout << i << " hundred edges IMAGE influence: " << sum1/5.0 << " time : " << sum2/5.0 << std::endl;
        emit(imageExpInf(sum1/5.0));
        emit(imageTime(sum2/5.0));

        sum1 = 0;
        sum2 = 0;
        for(int j = 0; j < 5; j++) {
            Graph::stat sImageBR = g->getImageBRStat(budget, threshold);
            sum1 += sImageBR.expectedInf;
            sum2 += sImageBR.time;
        }
        std::cout << i << " hundred edges IMAGE-BR influence: " << sum1/5.0 << " time : " << sum2/5.0 << std::endl;
        emit(imageBrExpInf(sum1/5.0));
        emit(imageBrTime(sum2/5.0));
    }
}

void GraphManager::doImage(float budget, double threshold)
{
    emit(setNodesToDefault());
    std::vector<Vertex*> result = g->image(budget, threshold);
    for(Vertex *v : result) {
        emit(paintVertex(v->getLabel()));
    }
    std::cout << result.size() << std::endl;
}
