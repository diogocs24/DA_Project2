//
// Created by NASA on 25/05/2023.
//

#ifndef DA_PROJECT2_ALGORITHMS_H
#define DA_PROJECT2_ALGORITHMS_H


#include "Graph.h"
#include "VertexEdge.h"
#include <xmath.h>
#include "MutablePriorityQueue.h"


using namespace std;


class Algorithms {
public:


    explicit Algorithms(Graph graph);

    void setGraph(Graph graph_);

    void setAllNonVisited();

    double backtracking(Vertex *curr_vertex, std::vector<bool> &visited, int count, double weight, double min_weight, Vertex *start_vertex, std::vector<int> &min_path, std::vector<int> &curr_path);

    double primMST(Graph graph);

private:

    vector<int> btLoop();

    Graph graph;
};


#endif //DA_PROJECT2_ALGORITHMS_H
