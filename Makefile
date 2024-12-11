CXX = g++
CXXFLAGS = -Wall -Wextra
LIBS = -lmingw32 -lSDL2main -lSDL2 -mwindows
INCLUDES = -ID:/Compilers/msys64/ucrt64/include/SDL2
LDFLAGS = -LD:/Compilers/msys64/ucrt64/lib
SRCS = snake.cpp
OBJS = $(SRCS:.cpp=.o)
MAIN = snake.exe

.PHONY: all clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) $(LIBS) -o $(MAIN)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	del /Q *.o *.exe