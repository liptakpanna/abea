#include "Graph.hpp"
#include <stdlib.h> 
#include <ctime>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>
#include <fstream>

Graph::Graph()
{
	srand((unsigned int)time(NULL));
}

Graph::Graph(std::string source_file) {
    srand((unsigned int)time(NULL));

    std::string line;
    bool isVertex = true;
    std::ifstream file(source_file);
    if(file.is_open()) {
        getline(file,line); //First line is #Vertex
        while(getline(file,line)){
            std::istringstream stream (line);
            if(line.find("# Edge") != std::string::npos) {
                isVertex = false;
                continue;
            }
            if(isVertex) {
                std::string label, cost;
                stream >> label >> cost;
                Vertex *v = new Vertex(label,std::stof(cost));
                this->addVertex(v);
            } else {
                std::string source, dest, prop;
                stream >> source >> dest >> prop;
                Vertex *s = this->findVertexByLabel(source);
                Vertex *d = this->findVertexByLabel(dest);
                if(s == nullptr || d == nullptr) {
                    std::cout << "Error vertex not defined" <<std::endl;
                }
                else {
                    Edge* e = new Edge(s,d,std::stof(prop));
                    this->addEdge(e);
                }
            }
        }
        file.close();
    }
    else std::cout << "Unable to open file";
}

void Graph::loadPokec(std::string source_file, int minCost, int maxCost)
{
    srand((unsigned int)time(NULL));

    std::string line;
    std::ifstream file(source_file);
    if(file.is_open()) {
        while(getline(file,line)){
            std::istringstream stream (line);
            std::string source, dest;
            stream >> source >> dest;
            Vertex *s = findVertexByLabel("v"+source);
            Vertex *d = findVertexByLabel("v"+dest);
            if(s == nullptr){
                float sCost = rand()%(maxCost-minCost + 1) + minCost;
                s = new Vertex("v"+source, sCost);
                this->addVertex(s);
            }
            if(d == nullptr){
                float dCost = rand()%(maxCost-minCost + 1) + minCost;
                d = new Vertex("v"+dest, dCost);
                this->addVertex(d);
            }
            float prop = ((double) rand() / (RAND_MAX)); //(0,1)
            if( prop < 0 || prop > 1) std::cout << "Bad propagation probability" << std::endl;
            Edge* e = new Edge(s,d,prop);
            this->addEdge(e);
        }
        file.close();
    }
    else std::cout << "Unable to open file";
}

Vertex* Graph::findVertexByLabel(std::string label){
    for(Vertex* v: this->getVertices()){
        if(v->getLabel() == label) return v;
    }
    return nullptr;
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
	return v->getCoveredRRset(RRset).size();
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
    int randIndex = rand() % vertices.size();
    std::set<Vertex*>::iterator it = vertices.begin();
	std::advance(it, randIndex);
	return *it;
}

std::vector<Vertex*> Graph::budgetedMaxCoverage(float budget, std::vector<std::vector<Vertex *>> RRset) {
    std::vector<Vertex*> S ;
	std::set<Vertex*> V = this->getVertices();
	float costS = 0;
	
	while(!V.empty()){
		Vertex *u = nullptr;
		float maxCoveragePerCost = 0;
		for(Vertex *v : V) {
			float currentCoveragePerCost = (float)lambdaCover(S, RRset, v)/v->getCost();
			//std::cout << "Current cov: " << currentCoveragePerCost << std::endl;
			if(currentCoveragePerCost >= maxCoveragePerCost){
				maxCoveragePerCost = currentCoveragePerCost;
				u = v;
			}
		}
		
		//std::cout << "u: " << u->getLabel() << std::endl;
		
        if(costS + u->getCost() <= budget) {
			S.push_back(u);
			costS += u->getCost();
			//std::cout<< "Cost S: " << costS << std::endl;
		}
		if(u != nullptr)
			V.erase(V.find(u));
	}
	
	int maxCoverage = 0, currentCoverage;
	Vertex *s;
	for(Vertex *v : this->getVertices()) {
        if(v->getCost() <= budget) {
			currentCoverage = lambdaCover(v, RRset);
			if(currentCoverage > maxCoverage){
				s = v;
				maxCoverage = currentCoverage;
			}
		}
	}
	
	if(maxCoverage > lambdaCover(S, RRset))
		return {s};
	else
        return S;
}

std::vector<Vertex *> Graph::budgetedMaxCoverage(float budget, int RRsetLength)
{
    return budgetedMaxCoverage(budget, getRandomRRSet(RRsetLength));
}

Graph::~Graph()
{
}

