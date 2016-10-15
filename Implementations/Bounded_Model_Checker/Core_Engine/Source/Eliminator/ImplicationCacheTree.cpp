
/********************************************************************
  This code was developed at IIT Bombay, India partly using public-domain
  free code.  No part of this code may be used for commercial or other 
  purposes without explicit permission from supratik@cse.iitb.ac.in

  AUTHORS : Ajith John, Supratik Chakraborty

  BEGIN DATE : February, 2010
*********************************************************************/


#include "ImplicationCacheTree.h"

// Constructor: creates graph with the starting vertex
ImplicationCacheTree::ImplicationCacheTree() {
  first = NULL;
  AddVertex(NULL); // add a new vertex with data = NULL. Make it the root, context
  root = first;
  context = first;
}
 
// Destructor: to be coded properly. Should delete the entire tree 
ImplicationCacheTree::~ImplicationCacheTree() {
 }

// the method creats a new ImplicationCacheTree node and keeps a pointer to it in first
void ImplicationCacheTree::AddVertex(DAGNode* theData) { 
  // allocate memory for new vertex with data theData,
  // make it point to the previous first vertex
  ImplicationCacheVertex *newVertex = new ImplicationCacheVertex(theData, first);
  // make the new vertex the first one in the list of vertexes
  first = newVertex;
}

  
// print all vertexes and edges of the graph
void ImplicationCacheTree::print() {
  if (first == NULL) 
    cout << "ImplicationCacheTree has no vertexes " << endl;
  else
    first -> printTree();
}


// move the context pointer to the parent of the current node
void ImplicationCacheTree::moveUp(){
context = context->getParent();
}

// move the context pointer down based on the newData
void ImplicationCacheTree::moveDown(DAGNode* newData){
// check if a child exists for context with data = newData
ImplicationCacheVertex *child = context->getChild(newData);
if(child!=NULL) { // child exists
	context = child;
	}
else {// child does not exist
	// create a new child with data = newData
	AddVertex(newData); // new vertex with data = newData created. Pointer to it is in first
	child = first;
	context->connectTo(child); // make child, the new child of context
	context = child; // child becomes the new context
      }
}

// show the current context node
void ImplicationCacheTree::showContext(){
cout<<"\nCurrent context node is "<<context->getData()<<endl;
}

// reset the context to the root
void ImplicationCacheTree::reset() {
  context = root;
}

// get the context : set of LMEs and LMDs from the root to the context
void ImplicationCacheTree::getPath(set<DAGNode*> &Path) {
  ImplicationCacheVertex *temp = context; // start from the context
  while(temp!=root){// root is reached. exit from the loop
    Path.insert(temp->getData());
    temp = temp->getParent();
  }
    
}

// Let we are at a specific context. We need to check to see if context => ~ImpliedPred is already decided. i.e. check if context /\ ImpliedPred is unsat. This is done by this way. Check if context has a child with data = ImpliedPred. If no return 'x'. Else check if this child has a child with data = true/false. If true, context => ~ImpliedPred. else NOT(context => ~ImpliedPred)
char ImplicationCacheTree::decide(DAGNode *ImpliedPred){
// check if a child exists for context with data = ImpliedPred
ImplicationCacheVertex *child = context->getChild(ImpliedPred);
if(child==NULL) { // child does not exist
	return 'x';
	}
else {// child exists
      ImplicationCacheVertex *grand_child = child->getTerminalChild();
      if(grand_child==NULL) {// no terminal grand child
	return 'x';
	}
      else { // terminal grand child
	if((grand_child->getData())->Get_node_name() == "true")
		{
		return 'u';
		}
	else
		{
		return 's';
		}
	  }	      

      }
}


// ********************************************************************
// methods of class ImplicationCacheVertex

// Constructor: creates a vertex with data theData that points to the next 
// vertex nextVert and has no outcoming edges
ImplicationCacheVertex::ImplicationCacheVertex(DAGNode* theData, ImplicationCacheVertex *nextVert) {
  data = theData;
  next = nextVert;
  edges = NULL;     // no edges 
  incoming = NULL;  // no incoming edges
}

// Destructor: to be coded properly. Should delete the entire vertex
ImplicationCacheVertex:: ~ImplicationCacheVertex() {
}
  
// returns the data of the vertex
DAGNode* ImplicationCacheVertex::getData() {
  return data;
}
  

// prints all the edges of the vertex by calling the print method
// of the first edge
void ImplicationCacheVertex::printEdges() {
  if (edges == NULL)
    {
    cout << "vertex " << getData() << " has no edges" << endl;
    } 
  else {
    cout << "vertex " << getData() << " has edges to: " << endl;
    edges -> print();
  }
}  

// prints the edges of the vertex, calls the printTree method for 
// the next vertex
void ImplicationCacheVertex::printTree() {
  printEdges();
  if (next != NULL)
    next -> printTree();
}
  
// adds an edge to connect the vertex to the vertex pointed to by Vert
void ImplicationCacheVertex::connectTo(ImplicationCacheVertex *Vert) {
  // allocate memory for a new Edge, set its Vertex pointer to point 
  // to Vert, and its Edge pointer to point to the rest of edges 
  ImplicationCacheEdge *newEdge = new ImplicationCacheEdge(this, Vert, edges); // edge this--->Vert
  // make newEdge the incoming edge of the Vertex Vert
  Vert->setIncoming(newEdge);  
  // make the new edge the first edge of the vertex this
  edges = newEdge; 
}

// returns the parent of the given vertex
ImplicationCacheVertex *ImplicationCacheVertex::getParent() {
  if(incoming==NULL) {
	cout<<"\nNo parent for node "<<this<<" with data "<<data<<endl;
	exit(1);
	}
  ImplicationCacheVertex *parent = incoming->getStart();
  if(parent==NULL) {
	cout<<"\nNo parent for node "<<this<<" with data "<<data<<endl;
	exit(1);}
  return parent;
}

// set the incoming field
void ImplicationCacheVertex::setIncoming(ImplicationCacheEdge *x){
  if(incoming==NULL) { // incoming should be NULL for setIncoming to work. No multiple parents
    incoming = x;
  }
  else
    {
      cout<<"\nImplicationCacheVertex::setIncoming called on a node whose incoming field is non-NULL\n";
      exit(1);
    }
}

// Function to return the pointer to the child of this node with data = ChildData. If no child with ChildData is found, return NULL
ImplicationCacheVertex *ImplicationCacheVertex::getChild(DAGNode* ChildData)
{
  ImplicationCacheEdge *temp = edges;
  while(temp) 
	{
	  ImplicationCacheVertex *child = temp->getEnd(); // get the child connected by this edge
	  
	  if(child->getData() == ChildData)
	    
	    {
	      return child;
	    }

	  temp = temp->getNext();
	}

  return NULL;
}

// Function to return the pointer to the child of this node with name(data) = true/false. If no child with name(data) is  found, return NULL
ImplicationCacheVertex *ImplicationCacheVertex::getTerminalChild()
{
  ImplicationCacheEdge *temp = edges;
  while(temp) 
	{
	  ImplicationCacheVertex *child = temp->getEnd(); // get the child connected by this edge
	  
	  if((child->getData())->Get_node_name() == "true" || (child->getData())->Get_node_name() == "false")
	      
	    {
	      return child;
	    }

	  temp = temp->getNext();
	}

  return NULL;
}

// ********************************************************************
// methods of class ImplicationCacheEdge

// Constructor: sets the two fields to the two given values
ImplicationCacheEdge::ImplicationCacheEdge(ImplicationCacheVertex *StartVert, ImplicationCacheVertex *EndVert, ImplicationCacheEdge *nextEdge) {
  start = StartVert;
  end = EndVert;
  next = nextEdge;
}

// Destructor: calls the destructor for the next edge on the list
ImplicationCacheEdge::~ImplicationCacheEdge() {
}


// returns the pointer to the end vertex of the edge
ImplicationCacheVertex *ImplicationCacheEdge::getEnd() {
  return end;
}

// returns the pointer to the start vertex of the edge
ImplicationCacheVertex *ImplicationCacheEdge::getStart() {
  return start;
}


// prints out the data of the end of the edge, then calls the print method 
// of the next edge on the list (if there is one)
void ImplicationCacheEdge::print() {
  cout << end -> getData() << " ";
  if (next == NULL)
    cout << endl;
  else {
    next -> print();
  }
}

// returns the pointer to the next edge on the list
ImplicationCacheEdge *ImplicationCacheEdge::getNext() {
  return next;
}


