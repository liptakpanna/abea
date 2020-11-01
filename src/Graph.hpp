#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "Vertex.hpp"
#include "Edge.hpp"
#include <vector>

class Graph
{
	std::set<Vertex *> vertices;
	std::set<Edge *> edges;
public:
	Graph();
	
	void addVertex(Vertex *v) {vertices.insert(v);}
	void addEdge(Edge *e);
	
	std::set<Vertex *> getVertices() {return vertices;}
	void printVertices();
	Vertex* getRandomVertex();
	
	std::vector<Vertex *> getRandomRSet();
	std::vector<std::vector<Vertex *>> getRandomRRSet(int length);
	
	int lambdaCover(Vertex* v, std::vector<std::vector<Vertex *>> RRset);
	int lambdaCover(std::vector<Vertex*> S, std::vector<std::vector<Vertex *>> RRset);
	int lambdaCover(std::vector<Vertex*> S, std::vector<std::vector<Vertex *>> RRset, Vertex* v);
	
	std::vector<Vertex*> budgetedMaxCoverage(float B, std::vector<std::vector<Vertex *>> RRset);
	
	~Graph();

};

#endif // GRAPH_HPP
