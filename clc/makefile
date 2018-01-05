#For MacOS & Linux & UNIXs

CC = g++ -std=c++11
OBJS = kc_aux.o KMatrix.o stdutil.o string_manip.o CLIK.o KVar.o KInterp.o KIFunctions.o KInterpAux.o interpret_keywords.o settings_menu.o
LIBS = -lreadline

all: main.cpp $(OBJS)
	$(CC) -o clc main.cpp $(OBJS) $(LIBS)

kc_aux.o: kc_aux.cpp kc_aux.hpp
	$(CC) -c kc_aux.cpp

KMatrix.o: KMatrix.cpp KMatrix.hpp
	$(CC) -c KMatrix.cpp

stdutil.o: stdutil.cpp stdutil.hpp
	$(CC) -c stdutil.cpp

string_manip.o: string_manip.cpp string_manip.hpp
	$(CC) -c string_manip.cpp

CLIK.o: CLIK.cpp CLIK.hpp
	$(CC) -c CLIK.cpp

KVar.o: KVar.cpp KVar.hpp
	$(CC) -c KVar.cpp

KInterp.o: KInterp.cpp KInterp.hpp
	$(CC) -c KInterp.cpp

KIFunctions.o: KIFunctions.cpp KIFunctions.hpp
	$(CC) -c KIFunctions.cpp

KInterpAux.o: KInterpAux.cpp KInterpAux.hpp
	$(CC) -c KInterpAux.cpp

interpret_keywords.o: interpret_keywords.cpp interpret_keywords.hpp
	$(CC) -c interpret_keywords.cpp

settings_menu.o: settings_menu.cpp settings_menu.hpp
	$(CC) -c settings_menu.cpp

clean:
	rm $(OBJS)
	rm clc

fresh: clean all

install: installer
	./installer

uninstall: installer
	./installer UNINSTALL

installer: installer.cpp
	$(CC) -o installer installer.cpp

#Development specific targets
test: test.cpp $(OBJS)
	$(CC) -o test test.cpp $(OBJS)

test2: test2.cpp $(OBJS)
	$(CC) -o test2 test2.cpp $(OBJS)

test3: test3.cpp $(OBJS)
	$(CC) -o test3 test3.cpp $(OBJS)
