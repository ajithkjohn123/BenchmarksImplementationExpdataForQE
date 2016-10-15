#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include <list>
#include <set>
#include <stack>


using namespace std;

//#ifndef _EDGE_H
//#define _EDGE_H

// forward declaration of the class Vertex
// needed to reslove cyclic dependency between the classes Vertex and Edge
class Vertex;

class Edge{
 public:
  Edge(Vertex *, Edge *);
  ~Edge();
  Vertex *getEnd();
  void print();
  Edge *getNext();
  void setNext(Edge *x);

 private:
  Vertex *end;   // pointer to the end of the edge
  Edge *next;    // pointer to the next edge in the list of edges associated 
                 // with the vertex
};

//#endif

//#ifndef _VERTEX_H
//#define _VERTEX_H

class Vertex {
 public:
  Vertex(string theData, Vertex *first);
  ~Vertex();
  string getData();
  int getVisited();
  int getStacked();
  void setVisited(int);
  void setStacked(int);
  void printEdges();
  void printGraph();
  void connectTo(Vertex *);
  Vertex *getNext();
  Edge *getFirstEdge();
  void setNumber(int);
  void setLlink(int);
  int getNumber();
  int getLlink();
  void setFirstEdge(Edge *x);
  void setNext(Vertex *x);

 private:
  string data;     // stores data of the vertex
  Edge *edges;   // pointer to the list of outcoming edges of the vertex
  Vertex *next;  // pointer to the next vertex in the graph
  int visited;  // flag to indicate whether the node is visited or not
  int stacked;  // flag to indicate whether the node is stacked or not
  int number; // flags for SCC finding
  int llink; 
};

//#endif


class Graph {
 public:
  Graph();
  ~Graph();
  Vertex *find(string theData);
  bool AddVertex(string theData);
  bool AddEdge(string start, string end);
  void print();
  Vertex* makeWeaklyConnected();

  // For SCC finding by Tarjan

  bool inStack(stack<Vertex*> mystack, Vertex *neighbour);
  void SCC(Vertex *start, set<string> &Components, int &scc_number, stack<Vertex*> &mystack);
  void Tarjan(set<string> &Components);
  void deleteVertex(string target);
  bool deleteVerticesNotPresent(set<string> SCC);
  bool presentInBlockSCC(string target, set<string> SCC);
  void splitSetIntoStrings(string target, set<string> &elements);
  bool stringPresentInSet(string target, set<string> elements);
  void removeTrivialSCCs(set<string> &Components);
  bool checkEdge(string start, string end);

 private: 
  Vertex *first;  // a pointer to the first vertex of the graph
                  // NULL if the graph does not have vertexes
};

