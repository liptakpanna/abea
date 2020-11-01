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
	Vertex *randomVertex = this->getRandomVertex();
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

int Graph::lambdaCover(Vertex* v, std::vector<std::vector<Vertex *>> RRset){
	std::vector<Vertex*> S = {v};
	return lambdaCover(S, RRset);
}

int Graph::lambdaCover(std::vector<Vertex*> S, std::vector<std::vector<Vertex *>> RRset) {
	if(S.empty()) return 0;
	
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

Vertex* Graph::getRandomVertex(){
	int randIndex = rand() % this->vertices.size();
	std::set<Vertex*>::iterator it = this->vertices.begin();
	std::advance(it, randIndex);
	return *it;
}

std::vector<Vertex*> Graph::budgetedMaxCoverage(float B, std::vector<std::vector<Vertex *>> RRset) {
	std::vector<Vertex*> S ;
	std::set<Vertex*> V = this->getVertices();
	float costS = 0;
	bool stop = false;
	
	while(!V.empty() && !stop){
		Vertex *u;
		float maxCoveragePerCost = 0;
		for(Vertex *v : V) {
			float currentCoveragePerCost = (float)lambdaCover(S, RRset, v)/v->getCost();
			std::cout << currentCoveragePerCost << std::endl;
			if(currentCoveragePerCost == 0) stop = true;
			if(currentCoveragePerCost > maxCoveragePerCost){
				maxCoveragePerCost = currentCoveragePerCost;
				u = v;
			}
		}
		if(costS + u->getCost() <= B) {
			S.push_back(u);
			costS += u->getCost();
			std::cout << costS << std::endl;
		}
		std::cout << V.size() <<std::endl;
		V.erase(u);
		std::cout << V.size() << std::endl;
	}
	
	int maxCoverage = 0, currentCoverage;
	Vertex *s;
	for(Vertex *v : this->getVertices()) {
		if(v->getCost() <= B) {
			currentCoverage = lambdaCover(v, RRset);
			if(currentCoverage > maxCoverage){
				s = v;
				maxCoverage = currentCoverage;
			}
		}
	}
	
	std::cout << s->getLabel() << std::endl;
	
	if(maxCoverage > lambdaCover(S, RRset))
		return {s};
	else
		return S;
}

Graph::~Graph()
{
}

