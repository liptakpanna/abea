#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Vertex.hpp"
#include "Edge.hpp"
#include <vector>
#include <string>

class Graph
{
	std::set<Vertex *> vertices;
	std::set<Edge *> edges;
public:
	Graph();
    Graph(std::string source_file);

    void addVertex(Vertex *v) {vertices.insert(v);}
	void addEdge(Edge *e);
	
    std::set<Vertex *> getVertices() {return vertices;}
    std::set<Edge *> getEdges() {return edges;}
    Vertex* findVertexByLabel(std::string label);
	void printVertices();
	Vertex* getRandomVertex();
	
	std::vector<Vertex *> getRandomRSet();
	std::vector<std::vector<Vertex *>> getRandomRRSet(int length);
	
	int lambdaCover(Vertex* v, std::vector<std::vector<Vertex *>> RRset);
	int lambdaCover(std::vector<Vertex*> S, std::vector<std::vector<Vertex *>> RRset);
	int lambdaCover(std::vector<Vertex*> S, std::vector<std::vector<Vertex *>> RRset, Vertex* v);
	
    std::vector<Vertex*> budgetedMaxCoverage(float budget, std::vector<std::vector<Vertex *>> RRset);
    std::vector<Vertex*> budgetedMaxCoverage(float budget, int RRsetLength);

    std::vector<Vertex*> image(float budget, float delta);
    std::vector<Vertex*> budgetedThresholdGreedy(float budget, float threshold, std::vector<std::vector<Vertex *>> RRset);
	
    int initializeThetaZero(float nu, float threshold, float delta, float kMin, float n, float e);
    int initializeThetaMax(float nu, float threshold, float delta, float kMax, float n, float e);

    int getKmin(std::vector<Vertex*> sortedVertices, float budget);
    int getKmax(std::vector<Vertex*> sortedVertices, float budget);

    struct stat{float time; double expectedInf; };

    stat getImageBRStat(float budget, float delta);
    stat getImageStat(float budget, float delta);

    ~Graph();

private:
    double getExpectedInfluence(std::vector<Vertex *> S, int theta);

    float nu = 0.7;
    float failureProb = 0.1;
    float approxError = 0.2;
};

#endif // GRAPH_HPP
