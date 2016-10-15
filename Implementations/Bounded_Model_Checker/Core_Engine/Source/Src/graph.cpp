//          File: graph.cpp
//        Author: Elena Machkasova
// Last Modified: April 25, 2001
//         Topic: Linked List Implementation of Graphs in C++

// The file contains implementation of class Graph


#include "graph.h"

// Constructor: creates graph with no vertexes
Graph::Graph() {
  first = NULL;
}
 
// Destructor: deletes the first vertex of the graph, which calls the vertex 
// destructor to delete its edges and the other vertexes with their edges 
Graph::~Graph() {
  delete first;
}

// the method returns the pointer to the vertex with data theData 
// if such vertex occurs in the graph, otherwise returns NULL
Vertex *Graph::find(string theData) {
  Vertex *vPtr = first;

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
bool Graph::AddVertex(string theData) { 
  // checking if the vertex is already in the graph
  // if it is, it cannot be added again 
  if ( find(theData) != NULL )
    return false;

  // allocate memory for new vertex with data theData,
  // make it point to the previous first vertex
  Vertex *newVertex = new Vertex(theData, first);
  // make the new vertex the first one in the list of vertexes
  first = newVertex;
  return true;
}

// check if the vertexes with data Begin and End are present in the graph. 
// if yes, create an edge from Begin to End, return 'true'
// otherwise return 'false'
bool Graph::AddEdge(string Begin, string End) {
  // find the pointer to the end vertex
  Vertex *vEnd = find(End);

  // if the vertex is not in the graph, cannot add the edge
  if (vEnd == NULL) return false;

  // find the pointer to the start vertex
  Vertex *vBegin = find(Begin);

  // if the vertex is not in the graph, cannot add the edge
  if (vBegin == NULL) return false;

  // connect the start vertex to the end one
  vBegin -> connectTo(vEnd);
  return true;
}
  
// print all vertexes and edges of the graph
void Graph::print() {
  if (first == NULL) 
    {
    cout << "Graph has no vertexes " << endl;
    }
  else
    {
    first -> printGraph();
    }
}

// Add a hypothetical starting node, say 'Start' to the graph and add an edge from 'Start' to all vertices.
// This procedure returns pointer to a vertex from which dfs can be started.
// This solves the problem of selecting a starting node. Otherwise, if we select a starting node and do dfs
// starting from this node, it may reach a dead end. We will not be able to proceed further and will need to 
// select another starting node. Adding a vertex with edges to all vertices solves this problem. Note that 
 // this will not create new cycles in the graph.
Vertex*  Graph::makeWeaklyConnected()
{
  Vertex* temp;
// Get the current first vertex, and store it's value in temp 
  temp = first;
// allocate memory for new vertex with data field 'S' to indicate that it is the starting node
// make it point to the previous first vertex
 Vertex *newVertex = new Vertex("START", first);
// make the new vertex the first one in the list of vertexes
 first = newVertex;
// Connect the new vertex to all existing vertices
 while(temp)
   {
     // connect the first vertex to 
     first -> connectTo(temp);
     // make temp point to the next vertex
     temp = temp -> getNext();
   }
// return the newly added vertex
 return first;
}

/* // Function for SCC finding

procedure main()
{ // main
scc_number := 0
empty the stack
mark all nodes in G as 'not visited'
for each node in G that is not visited, do
scc( w )
end for
} // end main */


void Graph::Tarjan(set<string> &Components)
{
  int scc_number = 0;
  stack<Vertex*> mystack;
  Vertex* start;

  // clear the current values in Components

  Components.clear();

  //cout<<"\n\nTarjan Started\n\n";
  
  //cout<<"\n\nStack emptied\n\n";

  //set visited(v)=0 for all vertexes
  start = first;
  while(start)
    {
      start->setVisited(0);
      start = start->getNext();
    }

  

  //for each node in G that is not visited, do
  //  scc( w )
  start = first;
  while(start)
    {
      //cout<<"\n\nStarting at Vertex "<<start->getData();
      if(start->getVisited() == 0)
	{
	  SCC(start, Components, scc_number, mystack);
	}
      start = start->getNext();
    }

  // remove trivial SCCs

  removeTrivialSCCs(Components);

}


/*procedure scc( v )
{ // scc
visited(v) := true 
lowlink(v) := number(v) := ++scc_number
push( v )
for all successors w of v do
if w is not visited then -- v->w is a tree arc
scc( w )
lowlink(v) := min( lowlink(v), lowlink(w) )
elsif number(w) < number(v) then -- v->w is cross link
if in_stack(w) then
lowlink(v) := min( lowlink(v), number(w) )
end if
end if
end for
if lowlink(v) = number(v) then -- next scc found
while w := top_of_stack_node; number(w) >= number(v) do
pop(w)
end while
end if
} // end scc*/

void Graph::SCC(Vertex *start, set<string> &Components, int &scc_number, stack<Vertex*> &mystack)  
{
  
  //cout<<"\n\nInside SCC of "<<start->getData()<<" started";
  // visited(v) := true
  start->setVisited(1);

  // lowlink(v) := number(v) := ++scc_number
  scc_number++;
  start->setLlink(scc_number);
  start->setNumber(scc_number);

  //push( v )
  mystack.push(start);

  //for all successors w of v do
  Edge *temp = start->getFirstEdge();  // get the first edge of the vertex
  while(temp) // loop until all the neighbours are considered
    {
      Vertex *neighbour = temp->getEnd(); // get the neighbour connected by this edge

      //cout<<"\n\nProcessing neighbour "<<neighbour->getData();
      
      // if w is not visited then -- v->w is a tree arc
      //	 scc( w )
      //         lowlink(v) := min( lowlink(v), lowlink(w) )
      if(neighbour->getVisited()==0)
	{
	SCC(neighbour, Components, scc_number, mystack);
	if(start->getLlink() <= neighbour->getLlink())
	  start->setLlink(start->getLlink());
	else
	  start->setLlink(neighbour->getLlink());
	}
      
      //elsif number(w) < number(v) then -- v->w is cross link
      //	if in_stack(w) then
      //		     lowlink(v) := min( lowlink(v), number(w) )
      //	end if
      //end if
      else if(neighbour->getNumber() < start->getNumber())
	{
	  if(inStack(mystack, neighbour))
	    {
	      if(start->getLlink() <= neighbour->getNumber())
		start->setLlink(start->getLlink());
	      else
		start->setLlink(neighbour->getNumber());
	    }
	}
      temp = temp->getNext(); // get the next edge
    }

  //if lowlink(v) = number(v) then -- next scc found
  //while w := top_of_stack_node; number(w) >= number(v) do
  //pop(w)
  //end while
  ///end if

  //cout<<"\n\nInside SCC of "<<start->getData()<<" before SCC found";
  if(start->getLlink() == start->getNumber())
    {
      //cout<<"\nSCC FOUND !!!\n";
      string SCC_element="";

      // show_stack(stack);
      Vertex *last;

      if(!mystack.empty())

	last = mystack.top();

      while(last->getNumber() >= start->getNumber())
	{
	  if(SCC_element == "")
	    SCC_element = last->getData();
	  else
	    SCC_element = SCC_element + " " + last->getData();
	  //cout<<"\nElement = "<<last->getData();
	  mystack.pop();
	  //cout<<"\n\n"<<last->getData()<<" poped from the stack";
	  if(!mystack.empty())
	    last = mystack.top();
	  else
	    break;
	}
      
      Components.insert(SCC_element);
    }
}


// return true if neighbour is in stack; else return false
bool Graph::inStack(stack<Vertex*> mystack, Vertex *neighbour)
{
  Vertex *element;
  
  while(!mystack.empty())
    {
      element = mystack.top();
      mystack.pop();
      if(element->getData() == neighbour->getData())
	return true;
    }
  return false;
}


// Delete a vertex from the graph

void Graph::deleteVertex(string target)
{
  Edge *temp, *parent;
  Vertex *start, *neighbour, *father;

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


// Delete vertices of the graph which are not present in any SCC and return true if happens, else false

bool Graph::deleteVerticesNotPresent(set<string> SCC)
{
  bool flag = false;

  Vertex *start = first; // Take the first vertex

  while(start) // If it is not NULL
	{

	  //cout<<"\n\nChecking if "<<start->getData()<<"is present in SCC";

	  if(!presentInBlockSCC(start->getData(), SCC)) // check if data in vertex is to be removed
	    {
	      //cout<<"\n\nYes!"<<start->getData()<<"is present in SCC";
	      flag=true;
	      //cout<<"\n\nDelete Vertex "<<start->getData();
	      deleteVertex(start->getData());
	      //cout<<"\n\nGraph after deleting Vertex "<<start->getData();
	      //print();
	      start = first;
	    }
	  else
	    {
	      //cout<<"\n\nNo!"<<start->getData()<<"is not present in SCC";
	      start = start->getNext();
	    }
	}

  return flag;
}

bool Graph::presentInBlockSCC(string target, set<string> SCC)
{
  set<string>::iterator SCC_it;
  bool flag;

  for(SCC_it=SCC.begin(); SCC_it != SCC.end(); SCC_it++)
    {
      string SCC_element = (*SCC_it);
      set<string> elements;
      splitSetIntoStrings(SCC_element, elements);
      flag = stringPresentInSet(target, elements);
      if (flag == true)
	return true;
    }
  return false;
}

bool Graph::stringPresentInSet(string target, set<string> elements)
{
  set<string>::iterator elements_it;
  
  for(elements_it=elements.begin(); elements_it != elements.end(); elements_it++)
    {
      if(target==(*elements_it))
	
	return true;
    }
  return false;
}
      

void Graph::splitSetIntoStrings(string target, set<string> &elements)
{
  string element;
  while(target.find(" ")!=string::npos)
    {
      int index = target.find(" ");
      element = target.substr(0, index);
      elements.insert(element);
      target = target.substr(index+1);
    }

  element = target;
  elements.insert(element);
  
}


// ********************************************************************
// methods of class Vertex

// Constructor: creates a vertex with data theData that points to the next 
// vertex nextVert and has no outcoming edges
Vertex::Vertex(string theData, Vertex *nextVert) {
  data = theData;
  next = nextVert;
  edges = NULL;     // no edges 
  visited = 0;      // node is initially unvisited 
  stacked = 0;      // node is initially not in stack
}

// Destructor: deletes the outcoming edges (the edge destructor calls 
// destructor for the next edge) and calls destructor for the next vertex 
// of the graph 
Vertex:: ~Vertex() {
  delete next;
  delete edges;
}
  
// returns the data of the vertex
string Vertex::getData() {
  return data;
}
  
// returns the visited field of the vertex
int Vertex::getVisited() {
  return visited;
}
  
// returns the stacked field of the vertex
int Vertex::getStacked() {
  return stacked;
}

// sets the visited field of the vertex
void Vertex::setVisited(int newvisited) {
  visited=newvisited;
}
  
// sets the stacked field of the vertex
void Vertex::setStacked(int newstacked) {
  stacked=newstacked;
}

// sets the llink field of the vertex
void Vertex::setLlink(int newllink) {
  llink=newllink;
}
  
// sets the number field of the vertex
void Vertex::setNumber(int newnumber) {
  number=newnumber;
}

// returns the llink field of the vertex
int Vertex::getLlink() {
  return llink;
}

// returns the number field of the vertex
int Vertex::getNumber() {
  return number;
}
  

// prints all the edges of the vertex by calling the print method
// of the first edge
void Vertex::printEdges() {
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
void Vertex::printGraph() {
  printEdges();
  if (next != NULL)
    next -> printGraph();
}
  
// adds an edge to connect the vertex to the vertex pointed to by Vert
void Vertex::connectTo(Vertex *Vert) {
  // allocate memory for a new Edge, set its Vertex pointer to point 
  // to Vert, and its Edge pointer to point to the rest of edges 
  Edge *newEdge = new Edge(Vert, edges);

  // make the new edge the first edge of the vertex
  edges = newEdge; 
}

// returns the pointer to the next vertex in the graph
Vertex *Vertex::getNext() {
  return next;
}

// returns the pointer to the first outcoming edge of the vertex 
Edge *Vertex::getFirstEdge() {
  return edges;
}

// ********************************************************************
// methods of class Edge

// Constructor: sets the two fields to the two given values
Edge::Edge(Vertex *Vert, Edge *nextEdge) {
  end = Vert;
  next = nextEdge;
}

// Destructor: calls the destructor for the next edge on the list
Edge::~Edge() {
  delete next;
}


// returns the pointer to the end vertex of the edge
Vertex *Edge::getEnd() {
  return end;
}


// prints out the data of the end of the edge, then calls the print method 
// of the next edge on the list (if there is one)
void Edge::print() {
  cout << end -> getData() << " ";
  if (next == NULL)
    cout << endl;
  else {
    next -> print();
  }
}

// returns the pointer to the next edge on the list
Edge *Edge::getNext() {
  return next;
}

void Vertex::setFirstEdge(Edge *x)
{
  edges = x;
}

void Edge::setNext(Edge *x)
{
  next = x;
}

void Vertex::setNext(Vertex *x)
{
  next = x;
}


void Graph::removeTrivialSCCs(set<string> &Components)
{
 set<string>::iterator Components_it;
  bool flag;

  for(Components_it=Components.begin(); Components_it != Components.end(); Components_it++)
    {
      string SCC_element = (*Components_it);
      set<string> elements;
      splitSetIntoStrings(SCC_element, elements);
      if(elements.size()==1)
	{
	  if(!checkEdge(SCC_element, SCC_element))
	    {
	    Components.erase(SCC_element);
	    }
	}
   }
}

// Function to check if an edge exists b/w vertices with data StartString and EndString
bool Graph::checkEdge(string StartString, string EndString)
{
  // locate vertex with data start

  Vertex *start = first;
  bool flag = false;
  Vertex *neighbour;

  while(start)
    {
      if(start->getData()==StartString)
	break;
      start = start->getNext();
    }

  if(start == NULL)
    {
      cout<<"\nERROR! graph::checkEdge CALLED WITH NON EXISTING VERTEX "<<StartString<<endl;
      exit(1);
    }
	
  Edge *temp = start->getFirstEdge();


  while(temp) 
	{
	  neighbour = temp->getEnd(); // get the neighbour connected by this edge
	  
	  if(neighbour->getData() == EndString)
	    
	    {
	      flag = true;
	    }

	  temp = temp->getNext();
	}

  return flag;
}
