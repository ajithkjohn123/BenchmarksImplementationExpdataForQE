CC = g++
CFLAGS = -g -c 
YFLAGS = -d -v
LEX = lex
YACC = yacc



DAG_LEXER = dag_lexer
DAG_PARSER = dag_parser
YACC_UTILS = yacc_utils
HELPING_FNS = helping_functions

DAG_MANAGER = DagManager

build : $(DAG_MANAGER).o y.tab.o lex.yy.o $(YACC_UTILS).o $(HELPING_FNS).o

$(DAG_MANAGER) : $(DAG_MANAGER).o
	$(CC) -o $(DAG_MANAGER) $(DAG_MANAGER).o

$(DAG_MANAGER).o : $(DAG_MANAGER).cc $(DAG_MANAGER).h
	$(CC) $(CFLAGS)  $(DAG_MANAGER).cc $(DAG_MANAGER).h -I .

$(HELPING_FNS).o : $(HELPING_FNS).cc 
	$(CC) $(CFLAGS)  $(HELPING_FNS).cc -I .


lex.yy.o : lex.yy.c
	g++ -c -g lex.yy.c

y.tab.o : y.tab.c
	g++ -c -g y.tab.c -I .

lex.yy.c : $(DAG_LEXER).l
	$(LEX) $(DAG_LEXER).l

y.tab.c : $(DAG_PARSER).y
	$(YACC) $(YFLAGS) $(DAG_PARSER).y  

$(YACC_UTILS).o : $(YACC_UTILS).cc $(YACC_UTILS).h
	g++ $(CFLAGS) $(YACC_UTILS).cc -I .

clean:
	rm -f *.o *.gch *.*.gch lex.yy.c y.tab.c *.output y.tab.h
