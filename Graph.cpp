#include "Graph.hpp"
#include <stdlib.h> 
#include <ctime>
#include <algorithm>
#include <iterator>

Graph::Graph()
{
	srand((unsigned int)time(NULL));
}

void Graph::addEdge(Edge *e){
	Vertex *source = e->getSource();
	Vertex *dest = e->getDestination();
	if (vertices.find(source) == vertices.end()) this->addVertex(source);
	if (vertices.find(dest) == vertices.end()) this->addVertex(dest);
	
	this->edges.insert(e);
}

void Graph::printVertices(){
	std::cout << "Vertices: ";
	for(Vertex *vertex : this->vertices ){
		std::cout << vertex->getLabel() << ",";
	}
	std::cout << std::endl;
}

std::vector<Vertex *> Graph::getRandomRSet(){
	int randIndex = rand() % this->vertices.size();
	//std::cout << randIndex << std::endl;
	std::set<Vertex*>::iterator it = this->vertices.begin();
	std::advance(it, randIndex);
	Vertex *randomVertex = *it;
	//std::cout << randomVertex->getLabel() << std::endl;
	std::vector<Vertex*> Rset = {randomVertex};
	
	for(int i = 0; i < Rset.size(); i++) {
		Vertex* vertex = Rset[i];
		//std::cout << vertex->getLabel() << std::endl;
		for(Edge *edge : vertex->getIncomingEdges()) {
			float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX); //between [0,1]
			//std::cout << "randfloat:" <<r << " prop: " << edge->getPropagationProbability() << std::endl;
			if (r < edge->getPropagationProbability()) Rset.push_back(edge->getSource());
		}
	}
	
	return Rset;
}

std::vector<std::vector<Vertex *>> Graph::getRandomRRSet(int length){
	std::vector<std::vector<Vertex *>> RRset;
	for(int i = 0; i < length; i++){
		RRset.push_back(this->getRandomRSet());
	}
	return RRset;
}

int Graph::lambdaCover(std::vector<Vertex*> S, std::vector<std::vector<Vertex *>> RRset) {
	int result = 0;
	for(int i = 0; i < RRset.size(); i++) {
		std::vector <Vertex*> intersection;
		std::set_intersection(S.begin(),S.end(), RRset[i].begin(),RRset[i].end(), std::back_inserter(intersection));
		if(!intersection.empty()) result++;
	}
	return result;
}

int Graph::lambdaCover(std::vector<Vertex*> S, std::vector<std::vector<Vertex *>> RRset, Vertex* v) {
	std::vector<Vertex*> unio = S;
	unio.push_back(v);
	return this->lambdaCover(unio, RRset) - this->lambdaCover(S, RRset);
}

Graph::~Graph()
{
}

