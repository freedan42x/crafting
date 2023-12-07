CXX = clang++

PKGS = sdl2 SDL2_ttf SDL2_image
FLAGS = -Wall -Wextra -pedantic -std=c++20 -Wno-reorder-ctor -g $(shell pkg-config --cflags $(PKGS))
LIBS = $(shell pkg-config --libs $(PKGS))

OBJDIR = .obj

SRCS = $(wildcard *.cpp)
OBJS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SRCS))

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(FLAGS) -c $< -o $@

main: $(OBJS)
	$(CXX) $(FLAGS) $^ -o $@ $(LIBS)
