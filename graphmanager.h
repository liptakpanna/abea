#ifndef GRAPHMANAGER_H
#define GRAPHMANAGER_H

#include <QObject>
#include "src/Graph.hpp"

class GraphManager : public QObject
{
    Q_OBJECT
public:
    explicit GraphManager(QObject *parent = nullptr);
    void getDemoGraph();
    void doBudgetedMaxCover(float budget, int RRLength);
    void doImage(float budget, double threshold);
    void getPokecGraph(int edgeCount, bool first);
    int getVertexCount();
    int getEdgeCount();
    void getPokecResults(int edges, float budget, float threshold);
signals:
    void printVertex(std::string label, std::string cost);
    void addVertex(std::string label, float cost);
    void addEdge(std::string source, std::string dest, float p);
    void paintVertex(std::string label);
    void setNodesToDefault();
    void imageExpInf(double i);
    void imageTime(double t);
    void imageBrExpInf(double i);
    void imageBrTime(double t);

private:
    Graph *g;
};

#endif // GRAPHMANAGER_H
