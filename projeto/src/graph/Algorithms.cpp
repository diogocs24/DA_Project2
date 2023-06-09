#include <utility>
#include <queue>
#include <stack>
#include <unordered_set>

#include "../../include/graph/Algorithms.h"

Algorithms::Algorithms() = default;
Algorithms::Algorithms(Graph graph) : graph(std::move(graph)) {}



void Algorithms::setGraph(Graph graph_) {
    this->graph = std::move(graph_);
}


void Algorithms::setAllNonVisited() {
    for(auto v : graph.getVertexSet()){
        v->setVisited(false);
    }
}

double Algorithms::backtracking(Vertex* curr_vertex, std::vector<bool>& visited, int count, double weight, double min_weight, Vertex* start_vertex, std::vector<int>& min_path, std::vector<int>& curr_path) {
    visited[curr_vertex->getId()] = true;
    curr_path.push_back(curr_vertex->getId());
    count++;

    if (count == visited.size()) {
        for (Edge* edge : curr_vertex->getAdj()) {
            if (edge->getDest() == start_vertex) {
                double total_weight = weight + edge->getWeight();
                if (total_weight < min_weight) {
                    min_weight = total_weight;
                    min_path = curr_path;
                }
                break;
            }
        }
    } else {
        for (Edge* edge : curr_vertex->getAdj()) {
            Vertex* adj_vertex = edge->getDest();
            if (!visited[adj_vertex->getId()]) {
                double new_weight = weight + edge->getWeight();
                if (new_weight < min_weight) {
                    min_weight = backtracking(adj_vertex, visited, count, new_weight, min_weight, start_vertex, min_path, curr_path);
                }
            }
        }
    }

    visited[curr_vertex->getId()] = false;
    curr_path.pop_back();
    count--;


    return min_weight;
}

vector<Vertex *> Algorithms::clusterBasedAlgorithm(int numClusters) {
    vector<Vertex *> path;

    vector<vector<Vertex*>> clusters(numClusters);
    vector<vector<Vertex*>> clusterPaths(numClusters);

    vector<Vertex*> auxVec(graph.getVertexSet());

    int n = auxVec.size();

    sort(auxVec.begin(),auxVec.end(),[](Vertex* a, Vertex* b) ->bool
    {return a->getLongitude()>b->getLongitude();});

    for(auto i = 0; i < numClusters ;i = i+2){
        vector<Vertex*> auxLat(auxVec.begin() + n/numClusters*i , auxVec.begin() + n/numClusters*(i+1));
        sort(auxLat.begin(),auxLat.end(),[](Vertex* a, Vertex* b) ->bool
        {return a->getLatitude()>b->getLatitude();});

        clusters[i].insert(clusters[i].begin(), auxLat.begin(),auxLat.begin() + n/numClusters/2);
        clusters[i+1].insert(clusters[i+1].begin(),auxLat.begin()+n/numClusters/2,auxLat.begin() + n/numClusters);
    }


    int count = 0;
    int clusterSize = clusters[0].size();
    for(auto &cluster : clusters){
        clusterPaths[count].push_back(cluster[0]);
        for(int i = 0; i<clusterSize-1;i++){
            Vertex * nextVertex = findNearestVertex(clusterPaths[count][i],cluster);
            clusterPaths[count].push_back(nextVertex);
            cluster.erase(find(cluster.begin(),cluster.end(),nextVertex));
        }
        count++;
    }

    for(auto cluster : clusterPaths){
        path.insert(path.end(),cluster.begin(),cluster.end());
    }

    return path;
}

Graph Algorithms::primMSTgraph() {
    // Cria um novo grafo para a MST
    Graph mstGraph;

    // Verifica se o grafo original é vazio
    if (graph.isEmpty()) {
        return mstGraph; // Retorna um grafo vazio
    }

    // Mapeia os vértices do grafo original para os vértices da MST
    unordered_map<int, Vertex*> vertexMap;

    // Inicializa os vértices do grafo original e adiciona ao mstGraph
    for (auto& v : graph.getVertexSet()) {
        Vertex* newVertex = new Vertex(v->getId());
        mstGraph.addVertex(newVertex);
        vertexMap[v->getId()] = newVertex;

        v->setDist(std::numeric_limits<double>::max());
        v->setPath(nullptr);
        v->setVisited(false);
    }

    // Seleciona o primeiro vértice como raiz
    Vertex* s = graph.getVertexSet().front();
    s->setDist(0);

    // Fila de prioridade para os vértices a serem processados
    MutablePriorityQueue<Vertex> q;
    q.insert(s);

    while (!q.empty()) {
        auto v = q.extractMin();
        v->setVisited(true);

        if (v->getPath() != nullptr) {
            // Obtém os vértices de origem e destino da aresta
            int origId = v->getPath()->getOrig()->getId();
            int destId = v->getPath()->getDest()->getId();

            // Adiciona os vértices correspondentes ao mstGraph
            mstGraph.addVertex(vertexMap[origId]);
            mstGraph.addVertex(vertexMap[destId]);

            // Adiciona a aresta ao grafo MST
            mstGraph.addEdge(origId, destId, v->getPath()->getWeight());
        }

        for (auto& e : v->getAdj()) {
            Vertex* w = e->getDest();
            if (!w->isVisited()) {
                auto oldDist = w->getDist();
                if (e->getWeight() < oldDist) {
                    w->setDist(e->getWeight());
                    w->setPath(e);
                    if (oldDist == std::numeric_limits<double>::max()) {
                        q.insert(w);
                    } else {
                        q.decreaseKey(w);
                    }
                }
            }
        }
    }

    return mstGraph;
}

vector<Vertex*> Algorithms::christofidesTSP() {
    // 1. Calcula a MST usando o algoritmo de Prim
    Graph mst = primMSTgraph();

    // 2. Encontra os vértices com grau ímpar na MST
    vector<Vertex*> oddVertices;

    for (auto v : mst.getVertexSet()) {
        if (v->getAdj().size() % 2 != 0) {
            oddVertices.push_back(v);
        }
    }

    // 3. Constrói um subgrafo apenas com os vértices de grau ímpar
    Graph oddGraph;

    // Adicionar os vértices ímpares ao oddGraph
    for (auto v : oddVertices) {
        oddGraph.addVertex(v->getId());
    }


    // Adicionar as arestas aos vértices ímpares no oddGraph
    for (auto v : oddVertices) {
        for (auto e : v->getAdj()) {
            oddGraph.addEdge(e->getOrig()->getId(), e->getDest()->getId(), e->getWeight());
        }
    }

    // Calcular o emparelhamento perfeito mínimo
    vector<Edge*> minimumMatching = oddGraph.calculateMinimumMatching();

    // Adicionar as arestas do emparelhamento perfeito mínimo à árvore geradora mínima (AGM)
    for (auto edge : minimumMatching) {
        mst.addEdge(edge->getOrig()->getId(), edge->getDest()->getId(), edge->getWeight());
    }
    // 4. Encontra um caminho Euleriano na MST
    vector<Vertex*> mstPath = mst.findEulerianPath();

    vector<Vertex*> hamiltonianCycle;

    unordered_set<int> visited;
    for (auto vertex : mstPath) {
        if (visited.count(vertex->getId()) == 0) {
            hamiltonianCycle.push_back(vertex);
            visited.insert(vertex->getId());
        }
    }
    hamiltonianCycle.push_back(mstPath[0]);

    return hamiltonianCycle;
}


vector<Vertex *> Algorithms::nearestNeighbor() {
    for(auto v: graph.getVertexSet()){
        v->setVisited(false);
    }
    vector<Vertex*> path;
    Vertex* firstVertex = graph.findVertex(0);
    firstVertex->setVisited(true);
    path.push_back(firstVertex);
    for(int i = 0; i < graph.getNumVertex()-2;i++){
        Vertex* next = findNearestVertex(path[i]);
        next->setVisited(true);
        path.push_back(next);
    }
    return path;
}


double Algorithms::calculatePathCost(vector<Vertex *> path) {
    double cost = 0;
    for(int  i = 0; i < path.size()-1;i++){
        cost += path[i]->getEdge(path[i+1])->getWeight();
    }
    return cost;
}

double Algorithms::calculateDistance(Vertex *source, Vertex *dest) {
    double lat1 = source->getLatitude();
    double lat2 = dest->getLatitude();
    double lon1 = source->getLongitude();
    double lon2 = dest->getLongitude();

    double dLat = (lat2 - lat1) *
                  M_PI / 180.0;
    double dLon = (lon2 - lon1) *
                  M_PI / 180.0;

    // convert to radians
    lat1 = (lat1) * M_PI / 180.0;
    lat2 = (lat2) * M_PI / 180.0;

    // apply formulae
    double a = pow(sin(dLat / 2), 2) +
               pow(sin(dLon / 2), 2) *
               cos(lat1) * cos(lat2);
    double rad = 6371;
    double c = 2 * asin(sqrt(a));
    return rad * c;
}

Vertex *Algorithms::findNearestVertex(Vertex *current, vector<Vertex *> cluster) {
    Vertex* nearest;
    double dist = numeric_limits<double>::max();
    for(auto e : current->getAdj()){
        if(e->getWeight() < dist && find(cluster.begin(),cluster.end(),e->getDest()) != cluster.end()){
            nearest = e->getDest();
            dist = e->getWeight();
        }
    }

    return nearest;
}

Vertex *Algorithms::findNearestVertex(Vertex *current) {
    Vertex* nearest;
    double dist = numeric_limits<double>::max();
    for(auto e : current->getAdj()){
        if(e->getWeight() < dist && !e->getDest()->isVisited()){
            nearest = e->getDest();
            dist = e->getWeight();
        }
    }

    return nearest;
}







