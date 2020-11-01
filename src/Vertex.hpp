#ifndef Vertex_HPP
#define Vertex_HPP

#include <iostream>
#include <set>
#include <vector>

class Edge;

class Vertex
{
	std::string label;
	float cost;
	std::set<Edge*> incomingEdges;
	std::set<Edge*> outgoingEdges;
public:
	Vertex() : label(""), cost(0) {}
	Vertex(std::string _label, float _cost) : label(_label), cost(_cost) {}
	
	float getCost() {return cost;}
	std::string getLabel() {return label;}
	std::set<Edge*> getIncomingEdges() {return incomingEdges;}
	std::set<Edge*> getOutgoingEdges() {return outgoingEdges;}
	
	void addIncomingEdge(Edge *n) {incomingEdges.insert(n);}
	void addOutgoingEdge(Edge *n) {outgoingEdges.insert(n);}
	
	std::vector<std::vector<Vertex *>> getCoveredRRset(std::vector<std::vector<Vertex *>> RRset);
	
	bool operator() (const Vertex& lhs, const Vertex& rhs) const
	{return lhs.label>rhs.label;}

	~Vertex() {};
};

#endif // Vertex_HPP
