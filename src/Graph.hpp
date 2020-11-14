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
	
	~Graph();

};

#endif // GRAPH_HPP
