//          File: graph.cpp
//        Author: Elena Machkasova
// Last Modified: April 25, 2001
//         Topic: Linked List Implementation of Graphs in C++

// The file contains implementation of class Graph


#include "graph-bb.h"

#define UNROLLING_COUNT 2

// Constructor: creates graph with no vertexes
Graph_BB::Graph_BB() {
  first = NULL;
}
 
// Destructor: deletes the first vertex of the graph, which calls the vertex 
// destructor to delete its edges and the other vertexes with their edges 
Graph_BB::~Graph_BB() {
  delete first;
}

// the method returns the pointer to the vertex with data theData 
// if such vertex occurs in the graph, otherwise returns NULL
Vertex_BB *Graph_BB::find(CBasicBlock* theData) {
  Vertex_BB *vPtr = first;

  while (vPtr != NULL) {
    // check if the data of the vertex is theData
    if ( vPtr -> getData() == theData)
      return vPtr;
    // go to the next vertex
    vPtr = vPtr -> getNext();
  }

  // there has been no match in the loop, so there is no such vertex
  return NULL;
}

// the method checks if there is a vertex with data theData in the graph
// if yes, the vertex cannot be added, the method returns 'false'
// otherwise the vertex is added, the method returns 'true'
bool Graph_BB::AddVertex(CBasicBlock* theData) { 
  // checking if the vertex is already in the graph
  // if it is, it cannot be added again 
  if ( find(theData) != NULL )
    return false;

  // allocate memory for new vertex with data theData,
  // make it point to the previous first vertex
  Vertex_BB *newVertex = new Vertex_BB(theData, first);
  // make the new vertex the first one in the list of vertexes
  first = newVertex;
  return true;
}

// check if the vertexes with data Begin and End are present in the graph. 
// if yes, create an edge from Begin to End, return 'true'
// otherwise return 'false'
bool Graph_BB::AddEdge(CBasicBlock* Begin, CBasicBlock* End) {
  // find the pointer to the end vertex
  Vertex_BB *vEnd = find(End);

  // if the vertex is not in the graph, cannot add the edge
  if (vEnd == NULL) return false;

  // find the pointer to the start vertex
  Vertex_BB *vBegin = find(Begin);

  // if the vertex is not in the graph, cannot add the edge
  if (vBegin == NULL) return false;

  // connect the start vertex to the end one
  vBegin -> connectTo(vEnd);
  return true;
}
  
// print all vertexes and edges of the graph
void Graph_BB::print() {
  if (first == NULL) 
    cout << "Graph has no vertexes " << endl;
  else
    first -> printGraph();
}


int Graph_BB::checkCycles()
{
  Vertex_BB *start;
  int stopping_cycle_present = 0;

  start = first;
  while(start)
    {
      start->setVisited(0);
      start->setStacked(0);
      start = start->getNext();
    }


  start = first;
  
  while(start)
    {
      //cout<<"\n\nStarting at Vertex "<<start->getData();
      if(start->getVisited() == 0)
	{
	  if(dfs(start) == 1) // Check if cycle with closing edge going to a vertex with count = 2 is present
	    {
	      stopping_cycle_present = 1;
	    }
	}
      start = start->getNext();
    }

  if(stopping_cycle_present == 1)
    {
      //cout<<"\n\nSTOPPING CYCLE PRESENT\n\n";
      return 1;
    }

  else
    {
      
      //cout<<"\n\nSTOPPING CYCLE NOT PRESENT\n\n";
      int cycle_present = 0;
      list<Vertex_BB*> cycle;
      int stop = 0;
      
  
      start = first;
      while(start)
	{
	  start->setVisited(0);
	  start->setStacked(0);
	  start = start->getNext();
	}


      start = first;
  
      while(start)
	{
	  //cout<<"\n\nStarting at Vertex "<<start->getData();
	  if(start->getVisited() == 0)
	    {
	      if(dfs_cycle_finding(start, cycle, stop) == 1)
		{
		  cycle_present = 1;
		}
	    }
	  start = start->getNext();
	}
  

      //cout<<"\n\nCYCLE PRESENT = "<<cycle_present<<endl<<endl;

      if(cycle_present == 1)

	{

	  //cout<<"CYCLE PRESENT\n\n";
	  
	  list<Vertex_BB*>::iterator cycle_it = cycle.begin();

	  for(; cycle_it != cycle.end(); cycle_it++)
	    cout<<(*cycle_it)->getData()<<endl;

	  // Updating the count field of vertexes in the cycle

	  cycle_it = cycle.begin();

	  if(cycle_it != cycle.end()) cycle_it++;

	  for(; cycle_it != cycle.end(); cycle_it++)
	    (*cycle_it)->setCount((*cycle_it)->getCount()+1);

	  // Deleting the edges in the cycle

	  cycle_it = cycle.begin();
	  list<Vertex_BB*>::iterator cycle_it2 = cycle.begin();

	  if(cycle_it2 != cycle.end()) cycle_it2++;

	  for(; cycle_it2 != cycle.end(); cycle_it++, cycle_it2++)

	    deleteEdge((*cycle_it)->getData(), (*cycle_it2)->getData());
	}

      else
	{
	  //cout<<"\n\nNO CYCLE PRESENT\n\n";
	}

	  return 0;
    }
}
  
int Graph_BB::dfs_cycle_finding(Vertex_BB* start, list<Vertex_BB*> &cycle, int &stop)
{
  Edge_BB *temp;
  Vertex_BB *neighbour;

  start->setVisited(1); // set the visited field of the vertex to 1 
  start->setStacked(1); // set the stacked field of the vertex to 1 
  
  temp = start->getFirstEdge();  // get the first edge of the vertex
  
  while(temp) // loop until all the neighbours are considered
    {
      neighbour = temp->getEnd(); // get the neighbour connected by this edge

      if(neighbour->getVisited() == 0)
	{
	  if(dfs_cycle_finding(neighbour, cycle, stop) == 1)
	    {
	    if(stop == 0)  
	      {
		if(presentInList(cycle, start))
		{
		  stop = 1;
		}
		cycle.push_back(start);  
	      }
	    
	    return 1;
	    }
	}

      else if(neighbour->getStacked() == 1)
	{
	  cycle.push_back(neighbour); 
	  cycle.push_back(start); 
	  return 1;
	 }

      temp = temp->getNext(); // get the next edge
    }

  start->setStacked(0); // set the stacked field of the vertex to 0
  return 0;
}

int Graph_BB::dfs(Vertex_BB* start)
{
  Edge_BB *temp;
  Vertex_BB *neighbour;

  start->setVisited(1); // set the visited field of the vertex to 1 
  start->setStacked(1); // set the stacked field of the vertex to 1 
  
  temp = start->getFirstEdge();  // get the first edge of the vertex
  
  while(temp) // loop until all the neighbours are considered
    {
      neighbour = temp->getEnd(); // get the neighbour connected by this edge

      if(neighbour->getVisited() == 0)
	{
	  if(dfs(neighbour) == 1)
	    {
	     return 1;
	    }
	}

      else if(neighbour->getStacked() == 1 && neighbour->getCount() >= UNROLLING_COUNT)
	{	  
	  return 1;
	}

      temp = temp->getNext(); // get the next edge
    }

  start->setStacked(0); // set the stacked field of the vertex to 0
  return 0;
}
      

// Delete a vertex from the graph

void Graph_BB::deleteVertex(CBasicBlock* target)
{
  Edge_BB *temp, *parent;
  Vertex_BB *start, *neighbour, *father;

  // Remove the edges to the node with data = target

  start = first;
  
  while(start)
    {
    
      temp = start->getFirstEdge();  // get the first edge of the vertex
  
      if(temp) // check if it is the first edge
	{
	  neighbour = temp->getEnd(); // get the neighbour connected by this edge
	  
	  if(neighbour->getData() == target)
	    
	    {
	      start->setFirstEdge(temp->getNext());
	    }

	  parent = temp;
	  temp = temp->getNext();
	}

      while(temp)
	{
	   neighbour = temp->getEnd(); // get the neighbour connected by this edge
	  
	  if(neighbour->getData() == target)
	    
	    {
	      parent->setNext(temp->getNext());
	      temp = temp->getNext();
	    }
	  else
	    {
	      parent = temp;
	      temp = temp->getNext();
	    }
	}

      start = start->getNext();
    }

  // Remove the node with data = target

  start = first;

  if(start)
    {
      if(start->getData()==target) // Match found!!!
	{
	  first = start->getNext();
	  return; // We have deleted the matching vertex. Return.
	}
    }

  father = first;
  
  start = first->getNext();

   while(start)
    {
      if(start->getData()==target) // Match found!!!
	{
	   father->setNext(start->getNext());
	   start = start->getNext();
	   return; // We have deleted the matching vertex. Return.
	}

      father = start;
      start = start->getNext();

    }
}

// Function to check if an edge exists b/w vertices with data StartBlock and EndBlock
bool Graph_BB::checkEdge(CBasicBlock* StartBlock, CBasicBlock* EndBlock)
{
  // locate vertex with data start

  Vertex_BB *start = first;
  bool flag = false;
  Vertex_BB *neighbour;

  while(start)
    {
      if(start->getData()==StartBlock)
	break;
      start = start->getNext();
    }

  if(start == NULL)
    {
      cout<<"\nERROR! graph::checkEdge CALLED WITH NON EXISTING VERTEX "<<StartBlock<<endl;
      exit(1);
    }
	
  Edge_BB *temp = start->getFirstEdge();


  while(temp) 
	{
	  neighbour = temp->getEnd(); // get the neighbour connected by this edge
	  
	  if(neighbour->getData() == EndBlock)
	    
	    {
	      flag = true;
	    }

	  temp = temp->getNext();
	}

  return flag;
}


// Delete an edge  from the graph

void Graph_BB::deleteEdge(CBasicBlock *source, CBasicBlock *target)
{
  Edge_BB *temp, *parent;
  Vertex_BB *start, *neighbour;

  start = first;
  
  while(start)
    {
 
      if(start->getData() == source)
	{


      temp = start->getFirstEdge();  // get the first edge of the vertex
  
      if(temp) // check if it is the first edge
	{
	  neighbour = temp->getEnd(); // get the neighbour connected by this edge
	  
	  if(neighbour->getData() == target)
	    
	    {
	      start->setFirstEdge(temp->getNext());
	    }

	  parent = temp;
	  temp = temp->getNext();
	}

      while(temp)
	{
	   neighbour = temp->getEnd(); // get the neighbour connected by this edge
	  
	  if(neighbour->getData() == target)
	    
	    {
	      parent->setNext(temp->getNext());
	      temp = temp->getNext();
	    }
	  else
	    {
	      parent = temp;
	      temp = temp->getNext();
	    }
	}
	}

      start = start->getNext();
    }

 }
  

int Graph_BB::presentInList(list<Vertex_BB*> cycle, Vertex_BB* thisVertex)
{
  for(list<Vertex_BB*>::iterator cycle_it = cycle.begin(); cycle_it != cycle.end(); cycle_it++)
    {
      if ((*cycle_it) == thisVertex)
	return 1;
    }

  return 0;
}
  


// ********************************************************************
// methods of class Vertex_BB

// Constructor: creates a vertex with data theData that points to the next 
// vertex nextVert and has no outcoming edges
Vertex_BB::Vertex_BB(CBasicBlock* theData, Vertex_BB *nextVert) {
  data = theData;
  next = nextVert;
  edges = NULL;     // no edges 
  visited = 0;      // node is initially unvisited 
  stacked = 0;      // node is initially not in stack
  count = 0;       // initially count = 0 
}

// Destructor: deletes the outcoming edges (the edge destructor calls 
// destructor for the next edge) and calls destructor for the next vertex 
// of the graph 
Vertex_BB:: ~Vertex_BB() {
  delete next;
  delete edges;
}
  
// returns the data of the vertex
CBasicBlock* Vertex_BB::getData() {
  return data;
}
  
// returns the visited field of the vertex
int Vertex_BB::getVisited() {
  return visited;
}
  
// returns the stacked field of the vertex
int Vertex_BB::getStacked() {
  return stacked;
}

// sets the visited field of the vertex
void Vertex_BB::setVisited(int newvisited) {
  visited=newvisited;
}
  
// sets the stacked field of the vertex
void Vertex_BB::setStacked(int newstacked) {
  stacked=newstacked;
}

// sets the llink field of the vertex
void Vertex_BB::setLlink(int newllink) {
  llink=newllink;
}
  
// sets the number field of the vertex
void Vertex_BB::setNumber(int newnumber) {
  number=newnumber;
}

// returns the llink field of the vertex
int Vertex_BB::getLlink() {
  return llink;
}

// returns the number field of the vertex
int Vertex_BB::getNumber() {
  return number;
}
  

// prints all the edges of the vertex by calling the print method
// of the first edge
void Vertex_BB::printEdges() {
  if (edges == NULL)
    {
    cout << "vertex " << getData() << " has no edges" << endl;
    cout << "\tvisited = " << getVisited() << "\tstacked= " << getStacked() << endl;
    } 
  else {
    cout << "vertex " << getData() << " has edges to: " << endl;
    edges -> print();
    cout << "\tvisited = "  << getVisited() << "\tstacked= " << getStacked() << endl;
  }
}  

// prints the edges of the vertex, calls the printGraph method for 
// the next vertex
void Vertex_BB::printGraph() {
  printEdges();
  if (next != NULL)
    next -> printGraph();
}
  
// adds an edge to connect the vertex to the vertex pointed to by Vert
void Vertex_BB::connectTo(Vertex_BB *Vert) {
  // allocate memory for a new Edge, set its Vertex pointer to point 
  // to Vert, and its Edge pointer to point to the rest of edges 
  Edge_BB *newEdge = new Edge_BB(Vert, edges);

  // make the new edge the first edge of the vertex
  edges = newEdge; 
}

// returns the pointer to the next vertex in the graph
Vertex_BB *Vertex_BB::getNext() {
  return next;
}

// returns the pointer to the first outcoming edge of the vertex 
Edge_BB *Vertex_BB::getFirstEdge() {
  return edges;
}

int Vertex_BB::getCount()
{
  return count;
}

void Vertex_BB::setCount(int x)
{
  count = x;
}

// ********************************************************************
// methods of class Edge_BB

// Constructor: sets the two fields to the two given values
Edge_BB::Edge_BB(Vertex_BB *Vert, Edge_BB *nextEdge) {
  end = Vert;
  next = nextEdge;
}

// Destructor: calls the destructor for the next edge on the list
Edge_BB::~Edge_BB() {
  delete next;
}


// returns the pointer to the end vertex of the edge
Vertex_BB *Edge_BB::getEnd() {
  return end;
}


// prints out the data of the end of the edge, then calls the print method 
// of the next edge on the list (if there is one)
void Edge_BB::print() {
  cout << end -> getData() << " ";
  if (next == NULL)
    cout << endl;
  else {
    next -> print();
  }
}

// returns the pointer to the next edge on the list
Edge_BB *Edge_BB::getNext() {
  return next;
}

void Vertex_BB::setFirstEdge(Edge_BB *x)
{
  edges = x;
}

void Edge_BB::setNext(Edge_BB *x)
{
  next = x;
}

void Vertex_BB::setNext(Vertex_BB *x)
{
  next = x;
}



