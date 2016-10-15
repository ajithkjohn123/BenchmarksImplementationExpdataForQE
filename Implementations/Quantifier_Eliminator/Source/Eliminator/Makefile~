#############################################################################
#  This code was developed at IIT Bombay, India partly using public-domain
#  free code.  No part of this code may be used for commercial or other 
#  purposes without explicit permission from supratik@cse.iitb.ac.in
#
#  AUTHORS : Ajith John, Supratik Chakraborty
#
#  BEGIN DATE : February, 2010
#############################################################################
CC=gcc
CPP=g++
CFLAGS=-g  
LDFLAGS=	
INCLUDES=-I. -I../include

YACC=bison
YFLAGS=-d -v -t -y

LEX=flex

DM_DIR = ../DAGManager

BDD_PATH = /home/ajith/CUDD

OBJS= lex.yy.o y.tab.o main.o Value.o BitBlaster.o DDNode.o Eliminator.o DDManager.o ILP_Eliminator.o ImplicationCacheTree.o DAGEliminator.o
Quantifier_Eliminator: y.tab.o lex.yy.o $(DM_DIR)/DagManager.o main.o Value.o BitBlaster.o DDNode.o Eliminator.o DDManager.o ILP_Eliminator.o ImplicationCacheTree.o DAGEliminator.o
	
	g++ -o Quantifier_Eliminator y.tab.o lex.yy.o $(DM_DIR)/DagManager.o  $(DM_DIR)/helping_functions.o main.o Value.o BitBlaster.o DDNode.o Eliminator.o DDManager.o ILP_Eliminator.o ImplicationCacheTree.o DAGEliminator.o $(BDD_PATH)/cudd-2.5.0/cudd/libcudd.a $(BDD_PATH)/cudd-2.5.0/st/libst.a $(BDD_PATH)/cudd-2.5.0/util/libutil.a $(BDD_PATH)/cudd-2.5.0/mtr/libmtr.a $(BDD_PATH)/cudd-2.5.0/dddmp/libdddmp.a $(BDD_PATH)/cudd-2.5.0/epd/libepd.a 


main.o : main.cc
	$(CPP) -c $(INCLUDES) -I $(DM_DIR) -I $(BDD_PATH)/cudd-2.5.0/include/ $(CFLAGS)   main.cc

y.tab.o: y.tab.c y.tab.h
	$(CPP) -c  -g $(INCLUDES) $(CFLAGS) y.tab.c -I $(DM_DIR)

y.tab.c y.tab.h: expr1.y
	$(YACC) $(YFLAGS) expr1.y

lex.yy.o: lex.yy.c y.tab.h
	$(CPP) -c  $(CFLAGS) $(INCLUDES) lex.yy.c

lex.yy.c: expr1.lex
	$(LEX) expr1.lex

Value.o : Value.cpp
	$(CPP) -c $(CFLAGS) Value.cpp $(INCLUDES) 

BitBlaster.o: BitBlaster.cc
	$(CPP) -c $(CFLAGS) BitBlaster.cc $(INCLUDES) -I $(DM_DIR) -I $(BDD_PATH)/cudd-2.5.0/include/

DDNode.o: DDNode.cpp
	$(CPP) -c $(CFLAGS) DDNode.cpp $(INCLUDES) -I $(DM_DIR) 

Eliminator.o: Eliminator.cpp
	$(CPP) -c $(CFLAGS)  Eliminator.cpp $(INCLUDES) -I $(DM_DIR)

DDManager.o: DDManager.cpp
	$(CPP) -c $(CFLAGS)  DDManager.cpp $(INCLUDES) -I $(DM_DIR) -I $(BDD_PATH)/cudd-2.5.0/include/

ILP_Eliminator.o: ILP_Eliminator.cpp
	$(CPP) -c $(CFLAGS) ILP_Eliminator.cpp $(INCLUDES) -I $(DM_DIR)

ImplicationCacheTree.o: ImplicationCacheTree.cpp
	$(CPP) -c $(CFLAGS) ImplicationCacheTree.cpp $(INCLUDES) -I $(DM_DIR)

DAGEliminator.o: DAGEliminator.cpp
	$(CPP) -c $(CFLAGS) DAGEliminator.cpp $(INCLUDES) -I $(DM_DIR) -I $(BDD_PATH)/cudd-2.5.0/include/




clean:
	rm -f $(OBJS) y.* lex.yy.? Quantifier_Eliminator

