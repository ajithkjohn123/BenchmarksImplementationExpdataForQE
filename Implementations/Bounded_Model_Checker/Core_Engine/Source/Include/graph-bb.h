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

// forward declaration of the class Vertex_BB
// needed to reslove cyclic dependency between the classes Vertex_BB and Edge_BB

class CBasicBlock;
class Vertex_BB;

class Edge_BB{
 public:
  Edge_BB(Vertex_BB *, Edge_BB *);
  ~Edge_BB();
  Vertex_BB *getEnd();
  void print();
  Edge_BB *getNext();
  void setNext(Edge_BB *x);

 private:
  Vertex_BB *end;   // pointer to the end of the edge
  Edge_BB *next;    // pointer to the next edge in the list of edges associated 
                 // with the vertex
};

//#endif

//#ifndef _VERTEX_H
//#define _VERTEX_H

class Vertex_BB {
 public:
  Vertex_BB(CBasicBlock* theData, Vertex_BB *first);
  ~Vertex_BB();
  CBasicBlock* getData();
  int getVisited();
  int getStacked();
  void setVisited(int);
  void setStacked(int);
  void printEdges();
  void printGraph();
  void connectTo(Vertex_BB *);
  Vertex_BB *getNext();
  Edge_BB *getFirstEdge();
  void setNumber(int);
  void setLlink(int);
  int getNumber();
  int getLlink();
  void setFirstEdge(Edge_BB *x);
  void setNext(Vertex_BB *x);
  int getCount();
  void setCount(int);

 private:
  CBasicBlock* data;     // stores data of the vertex
  Edge_BB *edges;   // pointer to the list of outcoming edges of the vertex
  Vertex_BB *next;  // pointer to the next vertex in the graph
  int visited;  // flag to indicate whether the node is visited or not
  int stacked;  // flag to indicate whether the node is stacked or not
  int number; // flags for SCC finding
  int llink;
  int count; // cycle count of the vertex
};

//#endif


class Graph_BB {
 public:
  Graph_BB();
  ~Graph_BB();
  Vertex_BB *find(CBasicBlock* theData);
  bool AddVertex(CBasicBlock* theData);
  bool AddEdge(CBasicBlock* start, CBasicBlock* end);
  void print();

  // For Cycle Detection
  int checkCycles();
  int dfs_cycle_finding(Vertex_BB* start, list<Vertex_BB*> &cycle, int &stop);
  int dfs(Vertex_BB* start);
  void deleteVertex(CBasicBlock* target);
  bool checkEdge(CBasicBlock* StartBlock, CBasicBlock* EndBlock);
  void deleteEdge(CBasicBlock *source, CBasicBlock *target);
  int presentInList(list<Vertex_BB*> cycle, Vertex_BB* thisVertex);

 private: 
  Vertex_BB *first;  // a pointer to the first vertex of the graph
                  // NULL if the graph does not have vertexes
};

