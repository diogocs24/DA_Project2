#include <utility>
#include <algorithm>

#include "../../include/graph/VertexEdge.h"

/************************* Vertex  **************************/

Vertex::Vertex(int id) : id(id), visited(false) {}

Vertex::Vertex(int id, std::string label) : id(id), label(std::move(label)), visited(false) {}

Vertex::Vertex(int id, double longitude, double latitude) : id(id), longitude(longitude), latitude(latitude), visited(false) {}

bool Vertex::operator<(const Vertex &vertex) const {
    return this->dist < vertex.dist;
}


int Vertex::getId() const {
    return this->id;
}

double Vertex::getDist() const {
    return this->dist;
}

std::vector<Edge *> Vertex::getAdj() const {
    return this->adj;
}

std::vector<Edge *> Vertex::getIncoming() const {
    return this->incoming;
}


double Vertex::getLongitude() const {
    return longitude;
}

double Vertex::getLatitude() const {
    return latitude;
}

std::string Vertex::getLabel() const {
    return label;
}

bool Vertex::isVisited() const {
    return visited;
}

Edge *Vertex::getPath() const{
    return this->path;
}

void Vertex::setId(int info) {
    this->id = info;
}

void Vertex::setDist(double dist_) {
    this->dist = dist_;
}

void Vertex::setVisited(bool info) {
    visited = info;
}

void Vertex::setPath(Edge *path){
    this->path = path;
}

Edge *Vertex::addEdge(Vertex *dest, double w) {
    Edge *newEdge = new Edge(this, dest, w);
    adj.push_back(newEdge);
    dest->incoming.push_back(newEdge);
    return newEdge;
}

bool Vertex::removeEdge(int destID) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        Vertex *dest = edge->getDest();
        if (dest->getId() == destID) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        } else {
            it++;
        }
    }
    return removedEdge;
}

void Vertex::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}

void Vertex::deleteEdge(Edge* edge) const {
    Vertex* dest = edge->getDest();
    Edge* edgeToRemove = nullptr;

    // Procura a aresta correspondente na lista incoming
    auto it = std::find(dest->incoming.begin(), dest->incoming.end(), edge);
    if (it != dest->incoming.end()) {
        dest->incoming.erase(it);
    }

    // Deleta a aresta fornecida
    delete edge;
}

Edge *Vertex::getEdge(Vertex *dest) {
    for(auto e : adj){
        if(e->getDest() == dest){
            return e;
        }
    }
    return nullptr;
}

/************************* Edge  ****************************/

Edge::Edge(Vertex *orig, Vertex *dest, double w) : orig(orig), dest(dest), weight(w), reverse(nullptr) {}

Vertex *Edge::getDest() const {
    return dest;
}

Vertex *Edge::getOrig() const {
    return orig;
}

double Edge::getWeight() const {
    return weight;
}

Edge *Edge::getReverse() const {
    return reverse;
}

void Edge::setReverse(Edge *reverse_) {
    this->reverse = reverse_;
}
