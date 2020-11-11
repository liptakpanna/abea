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
              Graph *g = new Graph(tempFile.toStdString());
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
