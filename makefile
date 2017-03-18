#For windows

CC = g++ -std=c++11
OBJS = kc_aux.o KMatrix.o stdutil.o string_manip.o CLIK.o KVar.o KInterp.o KIFunctions.o KInterpAux.o interpret_keywords.o

all: main.cpp $(OBJS)
	$(CC) -o clc main.cpp $(OBJS)

kc_aux.o: kc_aux.cpp
	$(CC) -c kc_aux.cpp

KMatrix.o: KMatrix.cpp
	$(CC) -c KMatrix.cpp

stdutil.o: stdutil.cpp
	$(CC) -c stdutil.cpp

string_manip.o: string_manip.cpp
	$(CC) -c string_manip.cpp

CLIK.o: CLIK.cpp
	$(CC) -c CLIK.cpp

KVar.o: KVar.cpp
	$(CC) -c KVar.cpp

KInterp.o: KInterp.cpp
	$(CC) -c KInterp.cpp

KIFunctions.o: KIFunctions.cpp
	$(CC) -c KIFunctions.cpp

KInterpAux.o: KInterpAux.cpp
	$(CC) -c KInterpAux.cpp

interpret_keywords.o: interpret_keywords.cpp
	$(CC) -c interpret_keywords.cpp

clean:
	@del $(OBJS) > nul 2> nul
	@del kc.exe > nul 2> nul

fresh: clean all

#Development specific targets
test: test.cpp $(OBJS)
	$(CC) -o test test.cpp $(OBJS)

test2: test2.cpp $(OBJS)
	$(CC) -o test2 test2.cpp $(OBJS)

test3: test3.cpp $(OBJS)
	$(CC) -o test3 test3.cpp $(OBJS)
