SOURCES := imagesignature.cpp
FLAGS   := `pkg-config --cflags --libs opencv`
OBJS    := $(SOURCES:.cpp=.o)


all : imagesignature

imagesignature : $(OBJS)
	g++ $(OBJS) -o imagesignature $(FLAGS)

.cpp.o:
	g++ $(SOURCES) $(FLAGS) -c

clean: 
	rm *.o imagesignature
