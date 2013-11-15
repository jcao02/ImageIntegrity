SOURCES := channels.cpp
FLAGS   := `pkg-config --cflags --libs opencv`
OBJS    := $(SOURCES:.cpp=.o)


all : channels

channels : $(OBJS)
	g++ $(OBJS) -o channels $(FLAGS)

.cpp.o:
	g++ $(SOURCES) $(FLAGS) -c

clean: 
	rm *.o channels
