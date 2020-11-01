#include "Vertex.hpp"
#include "Edge.hpp"

#include <algorithm>
#include <iterator>

std::vector<std::vector<Vertex *>> Vertex::getCoveredRRset(std::vector<std::vector<Vertex *>> RRset) {
	std::vector<Vertex *> S = {this};
	std::vector<std::vector<Vertex *>> result;
	
	for(int i = 0; i < RRset.size(); i++) {
		std::vector <Vertex*> intersection;
		std::set_intersection(S.begin(),S.end(), RRset[i].begin(),RRset[i].end(), std::back_inserter(intersection));
		if(!intersection.empty()) result.push_back(RRset[i]);
	}
	return result;
}


