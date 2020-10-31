#include "Edge.hpp"
#include "Vertex.hpp"

Edge::Edge(Vertex *_source, Vertex *_destination, float _propagationProb) {
	source = _source;
	destination = _destination;
	propagationProbability = _propagationProb;
	
	source->addOutgoingEdge(this);
	destination->addIncomingEdge(this);
}

Edge::~Edge()
{
}

