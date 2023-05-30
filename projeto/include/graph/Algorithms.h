//
// Created by NASA on 25/05/2023.
//

#ifndef DA_PROJECT2_ALGORITHMS_H
#define DA_PROJECT2_ALGORITHMS_H


#include "Graph.h"
#include "VertexEdge.h"


using namespace std;


class Algorithms {
public:


    Algorithms(Graph graph);

    void setGraph(Graph graph);

    void backtracking(std::vector<int>& path, std::vector<bool>& visited, double &min_cost, double cost_so_far);

private:

    vector<int> btLoop();
    void setAllNonVisited();

    Graph graph;
};


#endif //DA_PROJECT2_ALGORITHMS_H
