CC = g++
SRCS = main.cpp
LIBS = -Ibuild/include -lglfw -ldl -lGLEW
FRAMEWORKS = -framework OpenGL
OBJS = $(SRCS:.cpp=.o)
PROG = main

$(PROG): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS) $(FRAMEWORKS)

clean:
	rm $(OBJS) $(PROG)
