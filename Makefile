SOURCES := watermarking.cpp
FLAGS   := `pkg-config --cflags --libs opencv`
OBJS    := $(SOURCES:.cpp=.o)


all : watermarking

watermarking : $(OBJS)
	g++ $(OBJS) -o watermarking $(FLAGS)

.cpp.o:
	g++ $(SOURCES) $(FLAGS) -c

clean: 
	rm *.o watermarking
