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

signals:
    void printVertex(std::string label, std::string cost);
    void addVertex(std::string label, float cost);
    void addEdge(std::string source, std::string dest, float p);

private:
    Graph *g;
};

#endif // GRAPHMANAGER_H
