CC=g++
CFLAGS= -o2 -c -std=c++14
LDFLAGS=  -lsimlib -lm 
SOURCES=main.cpp globals.cpp Train.cpp Timer.cpp Commuter.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=trains

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


clean:
	rm -f *.o $(EXECUTABLE)

run:
	./trains