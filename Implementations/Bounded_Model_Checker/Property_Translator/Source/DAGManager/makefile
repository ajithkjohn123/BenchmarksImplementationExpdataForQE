CC=gcc
CPP=g++
# CFLAGS=-gx -DBASE_REPRESENTATION
CFLAGS=-g 
LDFLAGS=

INCLUDES=-I. -I./Include/

YACC=yacc
YFLAGS=-d -v -t

LEX=lex

OBJS=test.o CHelper.o CDAGNode.o DAG_hash_table.o FG_hash_table.o Factor_graph_Node.o

all: test

test: $(OBJS)
	$(CPP) -o test $(CFLAGS) $(OBJS) $(LDFLAGS) 

test.o: test.cpp 
	$(CPP) -c $(CFLAGS) $(INCLUDES)  test.cpp

CHelper.o: CHelper.cpp CHelper.h
	$(CPP) -c $(CFLAGS) $(INCLUDES)  CHelper.cpp

CDAGNode.o: CDAGNode.cpp CDAGNode.h
	$(CPP) -c  $(CFLAGS) $(INCLUDES)  CDAGNode.cpp

DAG_hash_table.o: DAG_hash_table.cpp DAG_hash_table.h
	$(CPP) -c  $(CFLAGS) $(INCLUDES)  DAG_hash_table.cpp

FG_hash_table.o: FG_hash_table.cpp FG_hash_table.h
	$(CPP) -c  $(CFLAGS) $(INCLUDES)  FG_hash_table.cpp

Factor_graph_Node.o: Factor_graph_Node.cpp Factor_graph_Node.h
	$(CPP) -c  $(CFLAGS) $(INCLUDES)  Factor_graph_Node.cpp


clean:
	rm -f $(OBJS) test

