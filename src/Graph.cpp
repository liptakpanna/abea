#include "Graph.hpp"
#include <stdlib.h>
#include <ctime>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>
#include <fstream>
#include <math.h>
#include <chrono>

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
    int numberOfIterations = 0;
    std::vector<Vertex*> S ;
    std::set<Vertex*> V = this->getVertices();
    float costS = 0;

    auto startTime = std::chrono::high_resolution_clock::now();
   // std::cout <<"\n ========== STARTED BUDGETED MAX COVERAGE ========== " << std::endl;
    while(!V.empty()){
       // std::cout << "\tBUDGETED MAX COVERAGE ITERATION = " << ++numberOfIterations << std::endl;
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
    auto stopTime = std::chrono::high_resolution_clock::now();

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

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stopTime - startTime);
   // std::cout << "The budgeted max coverage algorithm was running for " << duration.count() << " seconds";
    //std::cout << ", with " << numberOfIterations << " iterations." << std::endl;

   // std::cout << "\n ========== ENDED BUDGETED MAX COVERAGE ========== "<< std::endl;
    if(maxCoverage > lambdaCover(S, RRset))
        return {s};
    else
        return S;
}

std::vector<Vertex *> Graph::budgetedMaxCoverage(float budget, int RRsetLength)
{
    return budgetedMaxCoverage(budget, getRandomRRSet(RRsetLength));
}

int Graph::getKmin(std::vector<Vertex *> sortedVertices, float budget)
{
    int kmin = 0;
    float currBudget = 0;
    std::vector<Vertex *>::reverse_iterator rit;
    for(rit = sortedVertices.rbegin(); rit!= sortedVertices.rend(); rit++) {
        Vertex *curr = *rit;
        if(currBudget + curr->getCost() <= budget) {
            currBudget += curr->getCost();
            kmin++;
        }
        else
            break;
    }
    return kmin;
}

int Graph::getKmax(std::vector<Vertex *> sortedVertices, float budget)
{
    int kmax = 0;
    float currBudget = 0;
    for(int i = 0; i < sortedVertices.size(); i++) {
        Vertex *v = sortedVertices[i];
        if(currBudget + v->getCost() <= budget) {
            currBudget += v->getCost();
            kmax++;
        }
        else
            break;
    }
    return kmax;
}


int Graph::initializeThetaMax(float nu, float threshold, float delta, float kMax, float n, float e) {
    //std::cout << "kmax = " << kMax << std::endl;
    double z = 1/exp(nu*(1-threshold));
    return (2 * n * pow( ( ( (1 - z) * sqrt( log(6 / delta) ) ) + sqrt( (1 - z) * ( ( kMax * log(n) ) + log(6 / delta) ) ) ), 2) ) / ( pow(e, 2) * kMax );
}

int Graph::initializeThetaZero(float nu, float threshold, float delta, float kMin, float n, float e) {
    //std::cout << "kmin = " << kMin << std::endl;
    double z = 1/exp(nu*(1-threshold));
    return ( 2 * n * pow( ( ( (1 - z) * sqrt( log(6 / delta) ) ) + sqrt( (1 - z) * ( ( kMin * log(n) ) + log(6 / delta) ) ) ), 2) ) / ( pow(e, 2) * n );
}

std::vector<Vertex*> Graph::image(float budget, float delta){
    std::vector<Vertex*> s1;

    std::cout << " ====== STARTED IMAGE ====== " << std::endl;

    std::vector<Vertex *> sortedVertices(vertices.begin(), vertices.end());
    std::sort(sortedVertices.begin(), sortedVertices.end(),[](const Vertex* lhs, const Vertex* rhs) -> bool {
            return lhs->getCost() < rhs->getCost();});

    int thetaZero = initializeThetaZero(nu, delta, failureProb, getKmin(sortedVertices, budget), this->getVertices().size(), approxError);
    std::cout << "THETA ZERO = " << thetaZero << std::endl;
    int thetaMax = initializeThetaMax(nu, delta,failureProb, getKmax(sortedVertices,budget), this->getVertices().size(), approxError);
    std::cout << "THETA MAX = " << thetaMax << std::endl;

    int theta = thetaZero;
    float iMax = log2((float)thetaMax / (float)thetaZero);

    auto imageStartTime = std::chrono::high_resolution_clock::now();
    while(theta <= thetaMax) {
        std::vector<std::vector<Vertex *>> R1 = this -> getRandomRRSet(theta);
        std::vector<std::vector<Vertex *>> R2 = this -> getRandomRRSet(theta);

        s1 = this->budgetedThresholdGreedy(budget, delta, R1);
        //s1 = this->budgetedMaxCoverage(budget, R2);

        int probability = this -> lambdaCover(s1, R2);

        float desired = (1 - ((2 * delta) / (3 * iMax)));
        if (probability >= desired) {
            auto imageStopTime = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::seconds>(imageStopTime - imageStartTime);
            std::cout << "\nThe image framework was running for " << duration.count() << " seconds." << std::endl;

            std::cout << " ====== ENDED IMAGE ====== " << std::endl;

            return s1;
        }

        theta = 2 * theta;
    }
    auto imageStopTime = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(imageStopTime - imageStartTime);
    std::cout << "\nThe image framework was running for " << duration.count() << " seconds." << std::endl;

    std::cout << " ====== ENDED IMAGE ====== " << std::endl;
    return s1;
}


Graph::stat Graph::getImageBRStat(float budget, float delta)
{
    std::vector<Vertex*> s1;

    std::vector<Vertex *> sortedVertices(vertices.begin(), vertices.end());
    std::sort(sortedVertices.begin(), sortedVertices.end(),[](const Vertex* lhs, const Vertex* rhs) -> bool {
            return lhs->getCost() < rhs->getCost();});

    int thetaZero = initializeThetaZero(nu, delta, failureProb, getKmin(sortedVertices, budget), this->getVertices().size(), approxError);
    //std::cout << "THETA ZERO = " << thetaZero << std::endl;
    int thetaMax = initializeThetaMax(nu, delta,failureProb, getKmax(sortedVertices,budget), this->getVertices().size(), approxError);
    //std::cout << "THETA MAX = " << thetaMax << std::endl;

    float theta = thetaZero;
    float iMax = log2(thetaMax / thetaZero);

    auto imageStartTime = std::chrono::high_resolution_clock::now();
    while(theta <= thetaMax) {
        std::vector<std::vector<Vertex *>> R1 = this -> getRandomRRSet(theta);
        std::vector<std::vector<Vertex *>> R2 = this -> getRandomRRSet(theta);

        s1 = this->budgetedThresholdGreedy(budget, delta, R1);

        int probability = this -> lambdaCover(s1, R2);

        float desired = (1 - ((2 * delta) / (3 * iMax)));
        if (probability >= desired) {
            auto imageStopTime = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::seconds>(imageStopTime - imageStartTime);
            return stat{(float)duration.count(),getExpectedInfluence(s1, (int)theta)};
        }

        theta = 2 * theta;
        //std::cout << "THETA: " << theta << std::endl;
    }
    auto imageStopTime = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(imageStopTime - imageStartTime);
    return stat{(float)duration.count(),getExpectedInfluence(s1, (int)theta/2)};
}

Graph::stat Graph::getImageStat(float budget, float delta)
{
    std::vector<Vertex*> s1;

    std::vector<Vertex *> sortedVertices(vertices.begin(), vertices.end());
    std::sort(sortedVertices.begin(), sortedVertices.end(),[](const Vertex* lhs, const Vertex* rhs) -> bool {
            return lhs->getCost() < rhs->getCost();});

    int thetaZero = initializeThetaZero(nu, delta, failureProb, getKmin(sortedVertices, budget), this->getVertices().size(), approxError);
   // std::cout << "THETA ZERO = " << thetaZero << std::endl;
    int thetaMax = initializeThetaMax(nu, delta,failureProb, getKmax(sortedVertices,budget), this->getVertices().size(), approxError);
   // std::cout << "THETA MAX = " << thetaMax << std::endl;

    float theta = thetaZero;
    float iMax = log2(thetaMax / thetaZero);

    auto imageStartTime = std::chrono::high_resolution_clock::now();
    while(theta <= thetaMax) {
        std::vector<std::vector<Vertex *>> R1 = this -> getRandomRRSet(theta);
        std::vector<std::vector<Vertex *>> R2 = this -> getRandomRRSet(theta);

        s1 = this->budgetedMaxCoverage(budget, R1);

        int probability = this -> lambdaCover(s1, R2);

        float desired = (1 - ((2 * delta) / (3 * iMax)));
        if (probability >= desired) {
            auto imageStopTime = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::seconds>(imageStopTime - imageStartTime);
            return stat{(float)duration.count(),getExpectedInfluence(s1, (int)theta)};
        }

        theta = 2 * theta;
        //std::cout << "THETA: " << theta << std::endl;
    }
    auto imageStopTime = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(imageStopTime - imageStartTime);
    return stat{(float)duration.count(),getExpectedInfluence(s1, (int)theta/2)};
}

std::vector<Vertex*> Graph::budgetedThresholdGreedy(float budget, float threshold, std::vector<std::vector<Vertex *>> RRset) {
    //std::cout << "\n ========== STARTED BUDGETED TRESHOLD GREEDY ========== " << std::endl;
    if(threshold == 0) {
        threshold = 0.7;
    }

    int numberOfIterations = 0;
    std::vector<Vertex*> S;
    float dMax= 0;
    float dMin = 0;
    Vertex *maxCostVertex;
    float maxCost = 0;
    Vertex *minCostVertex = *(this->getVertices().begin());
    float minCost = minCostVertex->getCost();
    Vertex *s;
    float maxCoverage;
    float currentCost;
    float currentRatio;
    float currentCoverage;

    for(Vertex *v : this->getVertices()) {
        if (v->getCost() <= budget) {
            currentCoverage = lambdaCover(v, RRset);
            currentCost = v->getCost();
            currentRatio = currentCoverage / currentCost;

            if (currentRatio > dMax) {
                maxCostVertex = v;
                dMax = currentRatio;
            }

            if (currentCost > maxCost) {
                maxCost = currentCost;
                dMin = 1 / maxCost;
            }

            if(currentCost < minCost){
                minCostVertex = v;
                minCost = currentCost;
            }

            if(currentCoverage > maxCoverage) {
                maxCoverage = currentCoverage;
                s = v;
            }
        }
    }

    auto startTime = std::chrono::high_resolution_clock::now();
    float costOfSeedSet = 0;
    for(float w = dMax; w >= dMin * (1 - threshold); w = w * (1 - threshold)) {
        for(Vertex *v : this->getVertices()) {
           // std::cout << "\tBUDGETED TRESHOLD GREEDY ITERATION = " << ++numberOfIterations << std::endl;

            float currentCoveragePerCost = (float)lambdaCover(S, RRset, v) / v->getCost();

            //std::cout << "\tCOST OF SEEDSET = " << costOfSeedSet << std::endl;
           // std::cout << "\tCOST OF VERTEX = " << v->getCost() << std::endl;

          //  std::cout << "\tCURRENT COVERAGE PER COST = " << currentCoveragePerCost << std::endl;
          //  std::cout << "\tCURRENT W = " << w << std::endl;
            if ((costOfSeedSet + v->getCost()) <= budget && currentCoveragePerCost >= w) {
                S.push_back(v);
                costOfSeedSet += v->getCost();
            }

            if (costOfSeedSet + minCost > budget) {
                break;
            }

        }
    }
    auto stopTime = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stopTime - startTime);
    //std::cout << "The budgeted threshold greedy algorithm was running for " << duration.count() << " seconds";
   // std::cout << ", with " << numberOfIterations << " iterations." << std::endl;

     //std::cout << "\n ========== ENDED BUDGETED TRESHOLD GREEDY ========== " << std::endl;
    if(maxCoverage > lambdaCover(S, RRset))
        return {s};
    else
        return S;
}

double Graph::getExpectedInfluence(std::vector<Vertex *> S, int theta)
{
    std::vector<std::vector<Vertex *>> RR = this -> getRandomRRSet(theta);
    int count = lambdaCover(S,RR);
    return (double)vertices.size()/(double)theta*(double)count;
}

Graph::~Graph()
{
}

