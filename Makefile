DEBUG = false
CPP = g++
CFLAGS = -std=c++17

ifeq ($(DEBUG), true)
CFLAGS += -g3 -O0
else
CFLAGS += -O2
endif

all : temu
.PHONY : all

temu : main.o debugger.o express.o mem.o processor.o
	$(CPP) -o temu main.o debugger.o express.o mem.o processor.o $(CFLAGS) -lreadline
main.o : main.cpp debugger.h
	$(CPP) -c -o main.o main.cpp $(CFLAGS)
debugger.o : debugger.cpp debugger.h
	$(CPP) -c -o debugger.o debugger.cpp $(CFLAGS)
express.o : express.cpp express.h
	$(CPP) -c -o express.o express.cpp $(CFLAGS)
mem.o : mem.cpp mem.h
	$(CPP) -c -o mem.o mem.cpp $(CFLAGS)
processor.o : processor.cpp defs.h processor.h
	$(CPP) -c -o processor.o processor.cpp $(CFLAGS)

.PHONY : clean
clean :
	-rm temu *.o
