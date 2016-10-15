
/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/


#ifndef _IMPLICATIONCACHETREE_H
#define _IMPLICATIONCACHETREE_H

#include <string>
#include <iostream>

#include <list>
#include <set>
#include <stack>

#include "DagManager.h" // For the DAGNode class

using namespace std;

// forward declaration of the class ImplicationCacheVertex
// needed to reslove cyclic dependency between the classes ImplicationCacheVertex and ImplicationCacheEdge
class ImplicationCacheVertex;

class ImplicationCacheEdge{
 public:
  ImplicationCacheEdge(ImplicationCacheVertex *, ImplicationCacheVertex *, ImplicationCacheEdge *);
  ~ImplicationCacheEdge();
  ImplicationCacheVertex *getEnd();
  ImplicationCacheVertex *getStart();
  void print();
  ImplicationCacheEdge *getNext();

 private:
  ImplicationCacheVertex *start;  // pointer to the start of the edge
  ImplicationCacheVertex *end;   // pointer to the end of the edge
  ImplicationCacheEdge *next;    // pointer to the next edge in the list of edges associated 
                 // with the vertex
};

//#endif

//#ifndef _VERTEX_H
//#define _VERTEX_H

class ImplicationCacheVertex {
 public:
  ImplicationCacheVertex(DAGNode* theData, ImplicationCacheVertex *first);
  ~ImplicationCacheVertex();
  DAGNode* getData();
  void printEdges();
  void printTree();
  void connectTo(ImplicationCacheVertex *);
  ImplicationCacheVertex *getParent();
  void setIncoming(ImplicationCacheEdge *x);
  ImplicationCacheVertex *getChild(DAGNode* ChildData);
  ImplicationCacheVertex *getTerminalChild();

 private:
  DAGNode* data;     // stores data of the vertex
  ImplicationCacheEdge *incoming;  // pointer to the incoming edge of the vertex
  ImplicationCacheEdge *edges;   // pointer to the list of outcoming edges of the vertex
  ImplicationCacheVertex *next;  // pointer to the next vertex in the graph
};

//#endif


class ImplicationCacheTree {
 public:
  ImplicationCacheTree();
  ~ImplicationCacheTree();
  void print();
  void moveDown(DAGNode* newData);
  void moveUp();
  void AddVertex(DAGNode* theData);
  void showContext(); 
  void reset();
  void getPath(set<DAGNode*> &Path) ;
  char decide(DAGNode *ImpliedPred);

 private: 
  ImplicationCacheVertex *first;  // a pointer to the first vertex of the graph initialized to starting vertex
  ImplicationCacheVertex *root; // the starting vertex
  ImplicationCacheVertex *context; // keeps the current context
};

#endif	/* _IMPLICATIONCACHETREE_H */
