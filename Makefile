
out_dir = out
src_dir = src

CC=gcc
CXX = g++
CFLAGS=-I. -Wall -Wextra -Werror -Wpedantic -Wno-unused-parameter -std=c++2a -g -lm -Iinclude
DEPS = $(wildcard $(src_dir)/*.h)
SOURCE = $(wildcard $(src_dir)/*.cpp)
OBJ_temp = $(SOURCE:.cpp=.o)
OBJ = $(subst $(src_dir),$(out_dir),$(OBJ_temp))
exec = main

$(out_dir)/%.o: $(src_dir)/%.cpp $(DEPS)
	if [ ! -d $(out_dir) ]; then mkdir $(out_dir); fi
	$(CXX) -c -o $@ $< $(CFLAGS)

$(exec): $(OBJ)
	if [ ! -d $(out_dir) ]; then mkdir $(out_dir); fi
	$(CXX) -o $@ $^ $(CFLAGS)

clean:
	if [ ! -d $(out_dir) ]; then mkdir $(out_dir); fi
	rm -f $(out_dir)/*
	rm -f $(exec)
	rm -f *.bc *.ll *.o *.s *.out *.exe *.output

.PHONY: clean

debug:
	if [ ! -d "DrMemory-Linux-2.5.0" ]; then \
		wget https://github.com/DynamoRIO/drmemory/releases/download/release_2.5.0/DrMemory-Linux-2.5.0.tar.gz ; \
	tar -xzf DrMemory-Linux-2.5.0.tar.gz ; \
		rm DrMemory-Linux-2.5.0.tar.gz ; \
	fi
	DrMemory-Linux-2.5.0/bin64/drmemory -- $(exec)
