#ifndef EDGE_HPP
#define EDGE_HPP

class Vertex;

class Edge
{
	Vertex *source;
	Vertex *destination;
	float propagationProbability;
public:
	Edge() : source(nullptr), destination(nullptr), propagationProbability(0) {}
	Edge(Vertex *_source, Vertex *_destination, float _propagationProb);
	
	float getPropagationProbability() {return propagationProbability;}
	Vertex* getSource() {return source;}
	Vertex* getDestination() {return destination;}
	
	~Edge();

};

#endif // EDGE_HPP
