#include <iostream>
#include "Vertex.hpp"
#include "Edge.hpp"
#include "Graph.hpp"

int main()
{
    Vertex v1 ("v1", 1);
	Vertex v2 ("v2", 3);
	Vertex v3 ("v3", 4);
	Vertex v4 ("v4", 2);
	
	Edge e1(&v1, &v2, 0.4);
	Edge e2(&v1, &v3, 0.2);
	Edge e3(&v4, &v3, 0.1);
	Edge e4(&v4, &v1, 0.3);
	
	Graph g;
	g.addEdge(&e1);
	g.addEdge(&e2);
	g.addEdge(&e3);
	g.addEdge(&e4);
	
	std::vector<Vertex*> S_ = {&v3,&v2};
	
	g.printVertices();
	std::vector<std::vector<Vertex *>> RRset = g.getRandomRRSet(10);
	for(int i = 0;i<10;i++) {
		std::cout << "RSET: " ;
		for(Vertex* v : RRset[i]) std::cout << v->getLabel();
		std::cout  << std::endl;
	}
	
	std::cout << g.lambdaCover(S_, RRset) << std::endl;
	
	std::vector<Vertex *> S = g.budgetedMaxCoverage(5, RRset);
	
	for(Vertex *v : S) std::cout << v->getLabel() << " ";
	std::cout << std::endl;
	
	return 0;
}
 
